 /*
  * UAE - The Un*x Amiga Emulator
  *
  * bsdsocket.library emulation - Win32 OS-dependent part
  *
  * Copyright 1997,98 Mathias Ortmann
  *
  */

#include <windows.h>
#include <winsock.h>

#include "sysconfig.h"
#include "sysdeps.h"

#include <stddef.h>

#include "config.h"   
#include "options.h"
#include "include/memory.h"
#include "custom.h"
#include "readcpu.h"  
#include "newcpu.h"
#include "autoconf.h"
#include "bsdsocket.h"

CRITICAL_SECTION csSigQueueLock;

DWORD threadid;

#define THREAD(func,arg) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)func,(LPVOID)arg,0,&threadid)

#define SETERRNO seterrno(sb,WSAGetLastError()-WSABASEERR)
#define SETHERRNO setherrno(sb,WSAGetLastError()-WSABASEERR)
#define WAITSIGNAL waitsig(sb)

#define SETSIGNAL addtosigqueue(sb,0)
#define CANCELSIGNAL cancelsig(sb)

#define BEGINBLOCKING if (sb->ftable[sd-1] & SF_BLOCKING) sb->ftable[sd-1] |= SF_BLOCKINGINPROGRESS
#define ENDBLOCKING sb->ftable[sd-1] &= ~SF_BLOCKINGINPROGRESS

static WSADATA wsbData;

int PASCAL WSAEventSelect(SOCKET,HANDLE,long);

#define MAX_SELECT_THREADS 64
static HANDLE hThreads[MAX_SELECT_THREADS];
uae_u32 *threadargs[MAX_SELECT_THREADS];
static HANDLE hEvents[MAX_SELECT_THREADS];

static HANDLE hConnectThread;
static HANDLE hConnectEvent, hConnectEventReply;
static void thread_connect(void);

int init_socket_layer(void)
{
	SOCKET dummy;
	DWORD lasterror;

	if (WSAStartup(2, &wsbData))
	{
		lasterror = WSAGetLastError();
		
		if (lasterror == WSAVERNOTSUPPORTED) write_log("Please upgrade to WinSock2.\n");
		else write_log("bsdsocket: ERROR: Unable to initialize Windows socket layer! Error code: %d\n",lasterror);
		return 0;
	}

	InitializeCriticalSection(&csSigQueueLock);

	// make sure WSP/NSPStartup gets called from within the regular stack
	// (Windows 95/98 need this)
	if ((dummy = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) != INVALID_SOCKET) closesocket(dummy);
	else write_log("bsdsocket: ERROR: WSPStartup/NSPStartup failed! Error code: %d\n",WSAGetLastError());

	if (hConnectThread == NULL)
	{
		hConnectThread = THREAD(thread_connect,NULL);
		hConnectEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
		hConnectEventReply = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	
	return 1;
}

void deinit_socket_layer(void)
{
	WSACleanup();
}

void locksigqueue(void)
{
	EnterCriticalSection(&csSigQueueLock);
}

void unlocksigqueue(void)
{
	LeaveCriticalSection(&csSigQueueLock);
}

// Asynchronous completion notification

// We use window messages posted to hAmigaWnd in the range from 0xb000 to 0xb000+MAXPENDINGASYNC*2
// Socket events cause even-numbered messages, task events odd-numbered messages
// Message IDs are allocated on a round-robin basis and deallocated by the main thread.

// WinSock tends to choke on WSAAsyncCancelMessage(s,w,m,0,0) called too often with an event pending

// @@@ Enabling all socket event messages for every socket by default and basing things on that would
// be cleaner (and allow us to write a select() emulation that doesn't need to be kludge-aborted).
// However, the latency of the message queue is too high for that at the moment (setting up a dummy
// window from a separate thread would fix that).

// Blocking sockets with asynchronous event notification are currently not safe to use.

struct socketbase *asyncsb[MAXPENDINGASYNC];
SOCKET asyncsock[MAXPENDINGASYNC];
uae_u32 asyncsd[MAXPENDINGASYNC];
int asyncindex;

int host_sbinit(SB)
{
	sb->sockAbort = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if (sb->sockAbort == INVALID_SOCKET) return 0;
	if ((sb->hEvent = CreateEvent(NULL,FALSE,FALSE,NULL)) == NULL) return 0;

	sb->mtable = calloc(sb->dtablesize,sizeof(*sb->mtable));
	
	return 1;
}

void host_closesocketquick(int s)
{
	BOOL true = 1;

	setsockopt((SOCKET)s,SOL_SOCKET,SO_DONTLINGER,(char *)&true,sizeof(true));
	closesocket((SOCKET)s);
}

void host_sbcleanup(SB)
{
	int i;

	for (i = 0; i < MAXPENDINGASYNC; i++) if (asyncsb[i] == sb) asyncsb[i] = NULL;

	if (sb->hEvent != NULL) CloseHandle(sb->hEvent);
	if (sb->hAsyncTask) WSACancelAsyncRequest(sb->hAsyncTask);
	
	for (i = sb->dtablesize; i--; )
	{
		if (sb->dtable[i] != (int)INVALID_SOCKET) host_closesocketquick(sb->dtable[i]);
		
		if (sb->mtable[i]) asyncsb[(sb->mtable[i]-0xb000)/2] = NULL;
	}

	closesocket(sb->sockAbort);

	free(sb->mtable);
}

void host_sbreset(void)
{
	memset(asyncsb,0,sizeof asyncsb);
	memset(asyncsock,0,sizeof asyncsock);
	memset(asyncsd,0,sizeof asyncsd);
	memset(threadargs,0,sizeof threadargs);
}

void sockmsg(unsigned int msg, unsigned long wParam, unsigned long lParam)
{
	SB;
	unsigned int index;
	int sdi;

	index = (msg-0xb000)/2;
	sb = asyncsb[index];

	if (!(msg & 1))
	{
		// is this one really for us?
		if ((SOCKET)wParam != asyncsock[index])
		{
			// cancel socket event
			WSAAsyncSelect((SOCKET)wParam,hAmigaWnd,0,0);
			return;
		}

		sdi = asyncsd[index]-1;

		// asynchronous socket event?
		if (sb && !(sb->ftable[sdi] & SF_BLOCKINGINPROGRESS) && sb->mtable[sdi])
		{
			long wsbevents = WSAGETSELECTEVENT(lParam);
			int fmask = 0;

			// regular socket event?
			if (wsbevents & FD_READ) fmask = REP_READ;
			else if (wsbevents & FD_WRITE) fmask = REP_WRITE;
			else if (wsbevents & FD_OOB) fmask = REP_OOB;
			else if (wsbevents & FD_ACCEPT) fmask = REP_ACCEPT;
			else if (wsbevents & FD_CONNECT) fmask = REP_CONNECT;
			else if (wsbevents & FD_CLOSE) fmask = REP_CLOSE;

			// error?
			if (WSAGETSELECTERROR(lParam)) fmask |= REP_ERROR;

			// notify
			if (sb->ftable[sdi] & fmask) sb->ftable[sdi] |= fmask<<8;

			addtosigqueue(sb,1);
			return;
		}
	}

	locksigqueue();

	if (sb != NULL)
	{
		if (msg & 1)
		{
			// is this one really for us?
			if (sb->hAsyncTask != (void *)~0L && (void *)wParam != sb->hAsyncTask)
			{
				unlocksigqueue();
				return;
			}
		}

		asyncsb[index] = NULL;

		if (WSAGETASYNCERROR(lParam))
		{
			seterrno(sb,WSAGETASYNCERROR(lParam)-WSABASEERR);
			if (sb->sb_errno >= 1001 && sb->sb_errno <= 1005) setherrno(sb,sb->sb_errno-1000);
			else if (sb->sb_errno == 55)	// ENOBUFS
				write_log("bsdsocket: ERROR: Buffer overflow - %d bytes requested\n",WSAGETASYNCBUFLEN(lParam));
		}
		else seterrno(sb,0);

		if (msg & 1) sb->hAsyncTask = 0;

		SETSIGNAL;
	}
	
	unlocksigqueue();
}

static unsigned int allocasyncmsg(SB,uae_u32 sd,SOCKET s)
{
	int i;
	
	locksigqueue();
	
	for (i = asyncindex+1; i != asyncindex; i++)
	{
		if (i == MAXPENDINGASYNC) i = 0;
		
		if (!asyncsb[i])
		{
			asyncsb[i] = sb;
			if (++asyncindex == MAXPENDINGASYNC) asyncindex = 0;
			unlocksigqueue();
			
			if (s == INVALID_SOCKET)
			{
				sb->hAsyncTask = (void *)~0L;	// set wildcard value to prevent race condition
				return i*2+0xb001;
			}
			else
			{
				asyncsd[i] = sd;
				asyncsock[i] = s;
				return i*2+0xb000;
			}
		}
	}
	
	unlocksigqueue();

	seterrno(sb,12); // ENOMEM
	write_log("bsdsocket: ERROR: Async operation completion table overflow\n");
	
	return 0;
}

static void cancelasyncmsg(unsigned int wMsg)
{
	SB;
	
	wMsg = (wMsg-0xb000)/2;

	sb = asyncsb[wMsg];

	if (sb != NULL)
	{
		asyncsb[wMsg] = NULL;
		CANCELSIGNAL;
	}
}

void sockabort(SB)
{	
	locksigqueue();

	if (sb->hAsyncTask)
	{
		if (WSACancelAsyncRequest(sb->hAsyncTask)) write_log("bsdsocket: ERROR: WSACancelAsyncRequest() failed with error code %d\n",WSAGetLastError());
		sb->hAsyncTask = 0;
	}

	unlocksigqueue();
}

// address cleaning
static void prephostaddr(SOCKADDR_IN *addr)
{
	addr->sin_family = AF_INET;
}

static void prepamigaaddr(struct sockaddr *realpt, int len)
{
	// little endian address family value to the byte sin_family member
	((char *)realpt)[1] = *((char *)realpt);
	
	// set size of address
	*((char *)realpt) = len;
}

int host_socket(SB, int af, int type, int protocol)
{
	int sd;
	SOCKET s;
	unsigned long nonblocking = 1;

	TRACE(("socket(%d,%d,%d) -> ",af,type,protocol));

	if ((s = socket(af,type,protocol)) == INVALID_SOCKET)
	{
		SETERRNO;
		TRACE(("failed (%d)\n",sb->sb_errno));
		return -1;
	}
	else sd = getsd(sb,(int)s);

	sb->ftable[sd-1] = SF_BLOCKING;
	ioctlsocket(s,FIONBIO,&nonblocking);
	TRACE(("%d\n",sd));

	return sd;
}

uae_u32 host_bind(SB, uae_u32 sd, uae_u32 name, uae_u32 namelen)
{
	char buf[MAXADDRLEN];
	uae_u32 success = 0;
	SOCKET s;

	TRACE(("bind(%d,0x%lx,%d) -> ",sd,name,namelen));
	s = getsock(sb, sd);

	if (s != INVALID_SOCKET)
	{
		if (namelen <= sizeof buf)
		{
			memcpy(buf,get_real_address(name),namelen);
			
			// some Amiga programs set this field to bogus values
			prephostaddr((SOCKADDR_IN *)buf);

			if ((success = bind(s,(struct sockaddr *)buf,namelen)) != 0)
			{
				SETERRNO;
				TRACE(("failed (%d)\n",sb->sb_errno));
			}
			else TRACE(("OK\n"));
		}
		else write_log("bsdsocket: ERROR: Excessive namelen (%d) in bind()!\n",namelen);
	}

	return success;
}

uae_u32 host_listen(SB, uae_u32 sd, uae_u32 backlog)
{
	SOCKET s;
	uae_u32 success = -1;

	TRACE(("listen(%d,%d) -> ",sd,backlog));
	s = getsock(sb, sd);

	if (s != INVALID_SOCKET)
	{
		if ((success = listen(s,backlog)) != 0)
		{
			SETERRNO;
			TRACE(("failed (%d)\n",sb->sb_errno));
		}
		else TRACE(("OK\n"));
	}
			
	return success;
}

void host_accept(SB, uae_u32 sd, uae_u32 name, uae_u32 namelen)
{
	struct sockaddr *rp_name;
	int hlen;
	SOCKET s, s2;
	int success = 0;
	unsigned int wMsg;
	
	rp_name = (struct sockaddr *)get_real_address(name);
	hlen = get_long(namelen);
	TRACE(("accept(%d,%d,%d) -> ",sd,name,hlen));

	s = (SOCKET)getsock(sb,(int)sd);
	
	if (s != INVALID_SOCKET)
	{
		BEGINBLOCKING;
		
		s2 = accept(s,rp_name,&hlen);

		if (s2 == INVALID_SOCKET)
		{
			SETERRNO;

			if (sb->ftable[sd-1] & SF_BLOCKING && sb->sb_errno == WSAEWOULDBLOCK-WSABASEERR)
			{
				if ((wMsg = allocasyncmsg(sb,sd,s)) != 0)
				{
					WSAAsyncSelect(s,hAmigaWnd,wMsg,FD_ACCEPT);

					WAITSIGNAL;

					cancelasyncmsg(wMsg);
					
					if (sb->eintr)
					{
						TRACE(("[interrupted]\n"));
						ENDBLOCKING;
						return;
					}

					s2 = accept(s,rp_name,&hlen);

					if (s2 == INVALID_SOCKET)
					{
						SETERRNO;

						if (sb->sb_errno == WSAEWOULDBLOCK-WSABASEERR) write_log("bsdsocket: ERRRO: accept() would block despite FD_ACCEPT message\n");
					}
				}
			}
		}
			
		if (s2 == INVALID_SOCKET)
		{
			sb->resultval = -1;
			TRACE(("failed (%d)\n",sb->sb_errno));
		}
		else
		{
			sb->resultval = getsd(sb, s2);
			sb->ftable[sb->resultval-1] = sb->ftable[sd-1];	// new socket inherits the old socket's properties

			prepamigaaddr(rp_name,hlen);
			put_long(namelen,hlen);
			TRACE(("%d/%d\n",sb->resultval,hlen));
		}

		ENDBLOCKING;
	}
}

// Windows 9x workaround... Microsoft blows chunks.
volatile SOCKET connect_s;
volatile uae_u32 connect_name, connect_namelen;
struct socketbase *connect_sb;

static void thread_connect(void)
{
	char buf[MAXADDRLEN];
	SB;
	
	for (;;)
	{
		WaitForSingleObject(hConnectEvent,INFINITE);
		
		if ((sb = connect_sb) != NULL)
		{
			memcpy(buf,get_real_address(connect_name),connect_namelen);
			prephostaddr((SOCKADDR_IN *)buf);

			if ((sb->resultval = connect(connect_s,(struct sockaddr *)buf,connect_namelen)) != 0) SETERRNO;

			connect_sb = NULL;
			SetEvent(hConnectEventReply);
		}
	}
}

void host_connect(SB, uae_u32 sd, uae_u32 name, uae_u32 namelen)
{
	SOCKET s;
	int success = 0;
	unsigned int wMsg;
	
	TRACE(("connect(%d,0x%lx,%d) -> ",sd,name,namelen));

	s = (SOCKET)getsock(sb,(int)sd);
	
	if (s != INVALID_SOCKET)
	{
		if (namelen <= MAXADDRLEN)
		{
			if ((wMsg = allocasyncmsg(sb,sd,s)) != 0)
			{
				WSAAsyncSelect(s,hAmigaWnd,wMsg,FD_CONNECT);

				BEGINBLOCKING;

				connect_s = s;
				connect_name = name;
				connect_namelen = namelen;
				connect_sb = sb;
				SetEvent(hConnectEvent);
				WaitForSingleObject(hConnectEventReply,INFINITE);

				if (sb->resultval)
				{
					if (sb->sb_errno == WSAEWOULDBLOCK-WSABASEERR)
					{
						if (sb->ftable[sd-1] & SF_BLOCKING)
						{
							seterrno(sb,0);
	
							WAITSIGNAL;

							if (sb->eintr)
							{
								// Destroy socket to cancel abort, replace it with fake socket to enable proper closing.
								// This is in accordance with BSD behaviour.
								closesocket(s);
								sb->dtable[sd-1] = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
							}
						}
						else seterrno(sb,36);	// EINPROGRESS
					}
				}
				
				ENDBLOCKING;

				cancelasyncmsg(wMsg);
			}
		}
		else write_log("bsdsock: WARNING: Excessive namelen (%d) in connect()!\n",namelen);
	}

	TRACE(("%d\n",sb->sb_errno));
}

// @@@ move sendto() to an extra thread to make UDP-based apps work under Windows 9x
void host_sendto(SB, uae_u32 sd, uae_u32 msg, uae_u32 len, uae_u32 flags, uae_u32 to, uae_u32 tolen)
{
	SOCKET s;
	char *realpt;
	unsigned int wMsg;
	char buf[MAXADDRLEN];

#ifdef TRACING_ENABLED
	if (to) TRACE(("sendto(%d,0x%lx,%d,0x%lx,0x%lx,%d) -> ",sd,msg,len,flags,to,tolen))
	else TRACE(("send(%d,0x%lx,%d,%d) -> ",sd,msg,len,flags));
#endif

	s = getsock(sb,sd);

	if (s != INVALID_SOCKET)
	{
		realpt = get_real_address(msg);
		
		if (to)
		{
			if (tolen > sizeof buf) write_log("bsdsocket: WARNING: Target address in sendto() too large (%d)!\n",tolen);
			else
			{
				memcpy(buf,get_real_address(to),tolen);
				// some Amiga software sets this field to bogus values
				prephostaddr((SOCKADDR_IN *)buf);
			}
		}
		
		BEGINBLOCKING;

		for (;;)
		{
			sb->resultval = to ? sendto(s,realpt,len,flags,(struct sockaddr *)buf,tolen) : send(s,realpt,len,flags);
		
			if (sb->resultval == -1)
			{
				SETERRNO;
				if (sb->sb_errno != WSAEWOULDBLOCK-WSABASEERR || !(sb->ftable[sd-1] & SF_BLOCKING)) break;
			}
			else
			{
				realpt += sb->resultval;
				len -= sb->resultval;
				
				if (len <= 0) break;
				else continue;
			}

			if ((wMsg = allocasyncmsg(sb,sd,s)) != 0)
			{
				WSAAsyncSelect(s,hAmigaWnd,wMsg,FD_WRITE);
			
				WAITSIGNAL;
				
				cancelasyncmsg(wMsg);
				
				if (sb->eintr)
				{
					TRACE(("[interrupted]\n"));
					return;
				}
			}
			else break;
		}

		ENDBLOCKING;
	}
	else sb->resultval = -1;

#ifdef TRACING_ENABLED
	if (sb->resultval == -1) TRACE(("failed (%d)\n",sb->sb_errno))
	else TRACE(("%d\n",sb->resultval));
#endif
}

void host_recvfrom(SB, uae_u32 sd, uae_u32 msg, uae_u32 len, uae_u32 flags, uae_u32 addr, uae_u32 addrlen)
{
	SOCKET s;
	char *realpt;
	struct sockaddr *rp_addr = NULL;
	int hlen;
	char buf[MAXADDRLEN];
	unsigned int wMsg;

#ifdef TRACING_ENABLED
	if (addr) TRACE(("recvfrom(%d,0x%lx,%d,0x%lx,0x%lx,0x%lx) -> ",sd,msg,len,flags,addr,addrlen))
	else TRACE(("recv(%d,0x%lx,%d,0x%lx) -> ",sd,msg,len,flags));
#endif

	s = getsock(sb,sd);

	if (s != INVALID_SOCKET)
	{
		realpt = get_real_address(msg);

		if (addr)
		{
			hlen = get_long(addrlen);
			rp_addr = (struct sockaddr *)get_real_address(addr);
		}

		BEGINBLOCKING;

		for (;;)
		{
			sb->resultval = addr ? recvfrom(s,realpt,len,flags,rp_addr,&hlen) : recv(s,realpt,len,flags);

			if (sb->resultval == -1)
			{
				SETERRNO;
			
				if (sb->sb_errno == WSAEWOULDBLOCK-WSABASEERR && sb->ftable[sd-1] & SF_BLOCKING)
				{
					if ((wMsg = allocasyncmsg(sb,sd,s)) != 0)
					{
						WSAAsyncSelect(s,hAmigaWnd,wMsg,FD_READ);

						WAITSIGNAL;
					
						cancelasyncmsg(wMsg);
					
						if (sb->eintr)
						{
							TRACE(("[interrupted]\n"));
							return;
						}
					}
					else break;
				}
				else break;
			}
			else break;
		}
		
		ENDBLOCKING;

		if (addr)
		{
			prepamigaaddr(rp_addr,hlen);
			put_long(addrlen,hlen);
		}
	}
	else sb->resultval = -1;

#ifdef TRACING_ENABLED
	if (sb->resultval == -1) TRACE(("failed (%d)\n",sb->sb_errno))
	else TRACE(("%d\n",sb->resultval));
#endif
}

uae_u32 host_shutdown(SB, uae_u32 sd, uae_u32 how)
{
	SOCKET s;
	
	TRACE(("shutdown(%d,%d) -> ",sd,how));
	
	s = getsock(sb,sd);

	if (s != INVALID_SOCKET)
	{
		if (shutdown(s,how))
		{
			SETERRNO;
			TRACE(("failed (%d)\n",sb->sb_errno));
		}
		else
		{
			TRACE(("OK\n"));
			return 0;
		}
	}
	
	return -1;
}

void host_setsockopt(SB, uae_u32 sd, uae_u32 level, uae_u32 optname, uae_u32 optval, uae_u32 len)
{
	SOCKET s;
	char buf[MAXADDRLEN];

	TRACE(("setsockopt(%d,%d,0x%lx,0x%lx,%d) -> ",sd,(short)level,optname,optval,len));

	s = getsock(sb,sd);

	if (s != INVALID_SOCKET)
	{
		if (len > sizeof buf)
		{
			write_log("bsdsocket: WARNING: Excessive optlen in setsockopt() (%d)\n",len);
			len = sizeof buf;
		}	
		
		if (level == SOL_SOCKET && optname == SO_LINGER)
		{
			((LINGER *)buf)->l_onoff = get_long(optval);
			((LINGER *)buf)->l_linger = get_long(optval+4);
		}
		else
		{
			if (len == 4) *(long *)buf = get_long(optval);
			else if (len == 2) *(short *)buf = get_word(optval);
			else write_log("bsdsocket: ERROR: Unknown optlen (%d) in setsockopt(%d,%d)\n",level,optname);
		}

		// handle SO_EVENTMASK
		if (level == 0xffff && optname == 0x2001)
		{
			long wsbevents = 0;
			uae_u32 eventflags = get_long(optval);

			sb->ftable[sd-1] = (sb->ftable[sd-1] & ~REP_ALL) | (eventflags & REP_ALL);

			if (eventflags & REP_ACCEPT) wsbevents |= FD_ACCEPT;
			if (eventflags & REP_CONNECT) wsbevents |= FD_CONNECT;
			if (eventflags & REP_OOB) wsbevents |= FD_OOB;
			if (eventflags & REP_READ) wsbevents |= FD_READ;
			if (eventflags & REP_WRITE) wsbevents |= FD_WRITE;
			if (eventflags & REP_CLOSE) wsbevents |= FD_CLOSE;
			
			if (sb->mtable[sd-1] || (sb->mtable[sd-1] = allocasyncmsg(sb,sd,s)))
			{
				WSAAsyncSelect(s,hAmigaWnd,sb->mtable[sd-1],wsbevents);
				sb->resultval = 0;
			}
			else sb->resultval = -1;
		}
		else sb->resultval = setsockopt(s,level,optname,buf,len);
		
		if (!sb->resultval)
		{
			TRACE(("OK\n"));
			return;
		}
		else SETERRNO;
		
		TRACE(("failed (%d)\n",sb->sb_errno));
	}
}

uae_u32 host_getsockopt(SB, uae_u32 sd, uae_u32 level, uae_u32 optname, uae_u32 optval, uae_u32 optlen)
{
	SOCKET s;
	char buf[MAXADDRLEN];
	int len = sizeof(buf);

	TRACE(("getsockopt(%d,%d,0x%lx,0x%lx,0x%lx) -> ",sd,(short)level,optname,optval,optlen));

	s = getsock(sb,sd);
	
	if (s != INVALID_SOCKET)
	{
		if (!getsockopt(s,level,optname,buf,&len))
		{
			if (level == SOL_SOCKET && optname == SO_LINGER)
			{
				put_long(optval,((LINGER *)buf)->l_onoff);
				put_long(optval+4,((LINGER *)buf)->l_linger);
			}
			else
			{
				if (len == 4) put_long(optval,*(long *)buf);
				else if (len == 2) put_word(optval,*(short *)buf);
				else write_log("bsdsocket: ERROR: Unknown optlen (%d) in setsockopt(%d,%d)\n",level,optname);
			}

//			put_long(optlen,len); // some programs pass the actual ength instead of a pointer to the length, so...
			TRACE(("OK (%d,%d)\n",len,*(long *)buf));
			return 0;
		}
		else
		{
			SETERRNO;
			TRACE(("failed (%d)\n",sb->sb_errno));
		}
	}

	return -1;
}

uae_u32 host_getsockname(SB, uae_u32 sd, uae_u32 name, uae_u32 namelen)
{
	SOCKET s;
	int len;
	struct sockaddr *rp_name;
	
	len = get_long(namelen);
	
	TRACE(("getsockname(%d,0x%lx,%d) -> ",sd,name,len));
	
	s = getsock(sb,sd);
	
	if (s != INVALID_SOCKET)
	{
		rp_name = (struct sockaddr *)get_real_address(name);
		
		if (getsockname(s,rp_name,&len))
		{
			SETERRNO;
			TRACE(("failed (%d)\n",sb->sb_errno));
		}
		else
		{
			TRACE(("%d\n",len));
			prepamigaaddr(rp_name,len);
			put_long(namelen,len);
			return 0;
		}
	}	

	return -1;
}

uae_u32 host_getpeername(SB, uae_u32 sd, uae_u32 name, uae_u32 namelen)
{
	SOCKET s;
	int len;
	struct sockaddr *rp_name;
	
	len = get_long(namelen);
	
	TRACE(("getpeername(%d,0x%lx,%d) -> ",sd,name,len));
	
	s = getsock(sb,sd);
	
	if (s != INVALID_SOCKET)
	{
		rp_name = (struct sockaddr *)get_real_address(name);
		
		if (getpeername(s,rp_name,&len))
		{
			SETERRNO;
			TRACE(("failed (%d)\n",sb->sb_errno));
		}
		else
		{
			TRACE(("%d\n",len));
			prepamigaaddr(rp_name,len);
			put_long(namelen,len);
			return 0;
		}
	}	

	return -1;
}

uae_u32 host_IoctlSocket(SB, uae_u32 sd, uae_u32 request, uae_u32 arg)
{
	SOCKET s;
	uae_u32 data;
	int success = SOCKET_ERROR;

	TRACE(("IoctlSocket(%d,0x%lx,0x%lx) ",sd,request,arg));

	s = getsock(sb,sd);

	if (s != INVALID_SOCKET)
	{
		switch (request)
		{
			case FIONBIO:
				TRACE(("[FIONBIO] -> "));
				if (get_long(arg))
				{
					TRACE(("nonblocking\n"));
					sb->ftable[sd-1] &= ~SF_BLOCKING;
				}
				else
				{
					TRACE(("blocking\n"));
					sb->ftable[sd-1] |= SF_BLOCKING;
				}
				success = 0;
				break;
			case FIONREAD:
				ioctlsocket(s,request,(u_long *)&data);
				TRACE(("[FIONREAD] -> %d\n",data));
				put_long(arg,data);
				success = 0;
				break;
			case FIOASYNC:
				if (get_long(arg))
				{
					sb->ftable[sd-1] |= REP_ALL;

					TRACE(("[FIOASYNC] -> enabled\n"));
					if (sb->mtable[sd-1] || (sb->mtable[sd-1] = allocasyncmsg(sb,sd,s)))
					{
						WSAAsyncSelect(s,hAmigaWnd,sb->mtable[sd-1],FD_ACCEPT | FD_CONNECT | FD_OOB | FD_READ | FD_WRITE | FD_CLOSE);
						success = 0;
						break;
					}
				}
				else write_log(("bsdsocket: WARNING: FIOASYNC disabling unsupported.\n"));

				success = -1;
				break;
			default:
				write_log("bsdsocket: WARNING: Unknown IoctlSocket request: 0x%08lx\n",request);
				seterrno(sb,22);	// EINVAL
		}
	}
	
	return success;
}

int host_CloseSocket(SB, int sd)
{
	unsigned int wMsg;
	SOCKET s;

	TRACE(("CloseSocket(%d) -> ",sd));

	s = getsock(sb,sd);

	if (s != INVALID_SOCKET)
	{
		if (sb->mtable[sd-1])
		{
			asyncsb[(sb->mtable[sd-1]-0xb000)/2] = NULL;
			sb->mtable[sd-1] = 0;
		}

		BEGINBLOCKING;

		for (;;)
		{
			if (!closesocket(s))
			{
				releasesock(sb,sd);
				TRACE(("OK\n"));
				return 0;
			}

			SETERRNO;
	
			if (sb->sb_errno != WSAEWOULDBLOCK-WSABASEERR || !(sb->ftable[sd-1] & SF_BLOCKING)) break;
	
			if ((wMsg = allocasyncmsg(sb,sd,s)) != 0)
			{
				WSAAsyncSelect(s,hAmigaWnd,wMsg,FD_CLOSE);
			
				WAITSIGNAL;
				
				cancelasyncmsg(wMsg);
				
				if (sb->eintr)
				{
					TRACE(("[interrupted]\n"));
					break;
				}
			}
			else break;
		}

		ENDBLOCKING;
	}
	
	TRACE(("failed (%d)\n",sb->sb_errno));
	
	return -1;
}

// For the sake of efficiency, we do not malloc() the fd_sets here.
// 64 sockets should be enough for everyone.
static void makesocktable(SB, uae_u32 fd_set_amiga, struct fd_set *fd_set_win, int nfds, SOCKET addthis)
{
	int i, j, maxbit;
	uae_u32 currlong, mask;
	SOCKET s;

	if (addthis != INVALID_SOCKET)
	{
		*fd_set_win->fd_array = addthis;
		fd_set_win->fd_count = 1;
	}
	else fd_set_win->fd_count = 0;

	if (!fd_set_amiga)
	{
		fd_set_win->fd_array[fd_set_win->fd_count] = INVALID_SOCKET;
		return;
	}

	if (nfds > sb->dtablesize)
	{
		write_log("bsdsock: ERROR: select()ing more sockets (%d) than socket descriptors available (%d)!\n",nfds,sb->dtablesize);
		nfds = sb->dtablesize;
	}

	for (j = 0; ; j += 32, fd_set_amiga += 4)
	{
		currlong = get_long(fd_set_amiga);

		mask = 1;

		for (i = 0; i < 32; i++, mask <<= 1)
		{
			if (i+j > nfds)
			{
				fd_set_win->fd_array[fd_set_win->fd_count] = INVALID_SOCKET;
				return;
			}
			
			if (currlong & mask)
			{
				s = getsock(sb,j+i);
				
				if (s != INVALID_SOCKET)
				{
					fd_set_win->fd_array[fd_set_win->fd_count++] = s;

					if (fd_set_win->fd_count >= FD_SETSIZE)
					{
						write_log("bsdsock: ERROR: select()ing more sockets (%d) than the hard-coded fd_set limit (%d) - please report\n",nfds,FD_SETSIZE);
						return;
					}
				}
			}
		}
	}

	fd_set_win->fd_array[fd_set_win->fd_count] = INVALID_SOCKET;
}

static void makesockbitfield(SB, uae_u32 fd_set_amiga, struct fd_set *fd_set_win, int nfds)
{
	int n, i, j, val, mask;
	SOCKET currsock;

	for (n = 0; n < nfds; n += 32)
	{
		val = 0;
		mask = 1;
		
		for (i = 0; i < 32; i++, mask <<= 1)
		{
			if ((n || i) && (currsock = sb->dtable[n+i-1]) != INVALID_SOCKET)
			{
				for (j = fd_set_win->fd_count; j--; )
				{
					if (fd_set_win->fd_array[j] == currsock)
					{
						val |= mask;
						break;
					}
				}
			}
		}

		put_long(fd_set_amiga,val);
	}
}

static void fd_zero(uae_u32 fdset, uae_u32 nfds)
{
	unsigned int i;
	
	for (i = 0; i < nfds; i += 32, fdset += 4) put_long(fdset,0);
}

// This seems to be the only way of implementing a cancelable WinSock2 select() call... sigh.
static void thread_WaitSelect(uae_u32 index)
{
	long nfds;
	uae_u32 readfds, writefds, exceptfds;
	uae_u32 timeout;
	uae_u32 sigmp;
	struct fd_set readsocks, writesocks, exceptsocks;
	struct timeval tv;
	uae_u32 *args;

	SB;

	for (;;)
	{
		WaitForSingleObject(hEvents[index],INFINITE);

		if ((args = threadargs[index]) != NULL)
		{
			sb = (struct socketbase *)*args;
			nfds = args[1];
			readfds = args[2];
			writefds = args[3];
			exceptfds = args[4];
			timeout = args[5];
		
			// construct descriptor tables
			makesocktable(sb,readfds,&readsocks,nfds,sb->sockAbort);
			if (writefds) makesocktable(sb,writefds,&writesocks,nfds,INVALID_SOCKET);
			if (exceptfds) makesocktable(sb,exceptfds,&exceptsocks,nfds,INVALID_SOCKET);
		
			if (timeout)
			{
				tv.tv_sec = get_long(timeout);
				tv.tv_usec = get_long(timeout+4);
				TRACE(("(timeout: %d.%06d) ",tv.tv_sec,tv.tv_usec))
			}
		
			TRACE(("-> "));
		
			sb->resultval = select(nfds+1,&readsocks,writefds ? &writesocks : NULL,exceptfds ? &exceptsocks : NULL,timeout ? &tv : 0);
			sb->needAbort = 0;

			if (sb->resultval == SOCKET_ERROR)
			{
				SETERRNO;
				TRACE(("failed (%d) - ",sb->sb_errno));
				if (readfds) fd_zero(readfds,nfds);
				if (writefds) fd_zero(writefds,nfds);
				if (exceptfds) fd_zero(exceptfds,nfds);
			}
			else
			{
				if (readfds) makesockbitfield(sb,readfds,&readsocks,nfds);
				if (writefds) makesockbitfield(sb,writefds,&writesocks,nfds);
				if (exceptfds) makesockbitfield(sb,exceptfds,&exceptsocks,nfds);
			}
		
			SETSIGNAL;

			threadargs[index] = NULL;
			SetEvent(sb->hEvent);
		}
	}
}

void __stdcall host_WaitSelect(SB, uae_u32 nfds, uae_u32 readfds, uae_u32 writefds, uae_u32 exceptfds, uae_u32 timeout, uae_u32 sigmp)
{
	uae_u32 sigs, wssigs;
	int i;

	wssigs = sigmp ? get_long(sigmp) : 0;

	TRACE(("WaitSelect(%d,0x%lx,0x%lx,0x%lx,0x%lx,0x%lx) ",nfds,readfds,writefds,exceptfds,timeout,wssigs));

	if (!readfds && !writefds && !exceptfds && !timeout && !wssigs)
	{
		sb->resultval = 0;
		TRACE(("-> [ignored]\n"))
		return;
	}

	if (wssigs)
	{
		m68k_dreg(regs,0) = 0;
		m68k_dreg(regs,1) = wssigs;
		sigs = CallLib(get_long(4),-0x132) & wssigs;	// SetSignal()
		
		if (sigs)
		{
			TRACE(("-> [preempted by signals 0x%08lx]\n",sigs & wssigs));
			put_long(sigmp,sigs & wssigs);
			fd_zero(readfds,nfds);
			fd_zero(writefds,nfds);
			fd_zero(exceptfds,nfds);
			sb->resultval = 0;
			seterrno(sb,0);
			return;
		}
	}

	ResetEvent(sb->hEvent);

	sb->needAbort = 1;

	for (i = 0; i < MAX_SELECT_THREADS; i++) if (hThreads[i] && !threadargs[i]) break;

	if (i >= MAX_SELECT_THREADS)
	{
		for (i = 0; i < MAX_SELECT_THREADS; i++) if (!hThreads[i])
		{
			if ((hEvents[i] = CreateEvent(NULL,FALSE,FALSE,NULL)) == NULL || (hThreads[i] = THREAD(thread_WaitSelect,i)) == NULL)
			{
				hThreads[i] = 0;
				write_log("bsdsocket: ERROR: Thread/Event creation failed - error code: %d\n",GetLastError());
				seterrno(sb,12);	// ENOMEM
				sb->resultval = -1;
				return;
			}

			// this should improve responsiveness
			SetThreadPriority(hThreads[i],THREAD_PRIORITY_TIME_CRITICAL);
			break;
		}
	}
	
	if (i >= MAX_SELECT_THREADS) write_log("bsdsocket: ERROR: Too many select()s\n");
	else
	{
		SOCKET newsock = INVALID_SOCKET;

		threadargs[i] = (uae_u32 *)&sb;

		SetEvent(hEvents[i]);

		m68k_dreg(regs,0) = (((uae_u32)1)<<sb->signal)|sb->eintrsigs|wssigs;
		sigs = CallLib(get_long(4),-0x13e);	// Wait()

		if (sb->needAbort)
		{
			if ((newsock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == INVALID_SOCKET)
				write_log("bsdsocket: ERROR: Cannot create socket: %d\n",WSAGetLastError());

			closesocket(sb->sockAbort);
		}

		WaitForSingleObject(sb->hEvent,INFINITE);

		CANCELSIGNAL;

		if (newsock != INVALID_SOCKET) sb->sockAbort = newsock;

		put_long(sigmp,sigs & wssigs);

		if (sigs & sb->eintrsigs)
		{
			TRACE(("[interrupted]\n"));
			sb->resultval = -1;
			seterrno(sb,4);	// EINTR
		}
		else if (sigs & wssigs)
		{
			TRACE(("[interrupted by signals 0x%08lx]\n",sigs & wssigs));
			sb->resultval = 0;
			seterrno(sb,0);
		}
		else TRACE(("%d\n",sb->resultval));
	}
}

uae_u32 host_Inet_NtoA(SB, uae_u32 in)
{
	char *addr;
	struct in_addr ina;
	uae_u32 scratchbuf;

	*(uae_u32 *)&ina = htonl(in);

	TRACE(("Inet_NtoA(%lx) -> ",in));

	if ((addr = inet_ntoa(ina)) != NULL)
	{
		scratchbuf = m68k_areg(regs,6)+offsetof(struct UAEBSDBase,scratchbuf);
		strncpyha(scratchbuf,addr,SCRATCHBUFSIZE);
		TRACE(("%s\n",addr));
		return scratchbuf;
	}
	else SETERRNO;

	TRACE(("failed (%d)\n",sb->sb_errno));

	return 0;
}

uae_u32 host_inet_addr(uae_u32 cp)
{
	uae_u32 addr;
	char *cp_rp;

	cp_rp = get_real_address(cp);

	addr = htonl(inet_addr(cp_rp));

	TRACE(("inet_addr(%s) -> 0x%08lx\n",cp_rp,addr));

	return addr;
}

void host_gethostbynameaddr(SB, uae_u32 name, uae_u32 namelen, long addrtype)
{
	HOSTENT *h;
	int size, numaliases = 0, numaddr = 0;
	uae_u32 aptr, asize;
	char *name_rp;
	int i;
	uae_u32 addr;
	uae_u32 *addr_list[2];

	char buf[MAXGETHOSTSTRUCT];
	unsigned int wMsg = 0;

	name_rp = get_real_address(name);

	if (addrtype == -1)
	{
		TRACE(("gethostbyname(%s) -> ",name_rp));
		
		// workaround for numeric host "names"
		if ((addr = inet_addr(name_rp)) != INADDR_NONE)
		{
			seterrno(sb,0);
			((HOSTENT *)buf)->h_name = name_rp;
			((HOSTENT *)buf)->h_aliases = NULL;
			((HOSTENT *)buf)->h_addrtype = AF_INET;
			((HOSTENT *)buf)->h_length = 4;
			((HOSTENT *)buf)->h_addr_list = (char **)&addr_list;
			addr_list[0] = &addr;
			addr_list[1] = NULL;
	
			goto kludge;
		}
	}
	else
	{
		TRACE(("gethostbyaddr(0x%lx,0x%lx,%ld) -> ",name,namelen,addrtype));
	}
	
	if ((wMsg = allocasyncmsg(sb,0,INVALID_SOCKET)) != 0)
	{
		if ((sb->hAsyncTask = addrtype == -1 ? WSAAsyncGetHostByName(hAmigaWnd,wMsg,name_rp,buf,sizeof buf) : WSAAsyncGetHostByAddr(hAmigaWnd,wMsg,name_rp,namelen,addrtype,buf,sizeof buf)) != NULL)
		{
			while (sb->hAsyncTask) WAITSIGNAL;

			sockabort(sb);

			if (!sb->sb_errno)
			{
kludge:
				h = (HOSTENT *)buf;
				
				// compute total size of hostent
				size = 28;
				if (h->h_name != NULL) size += strlen(h->h_name)+1;
		
				if (h->h_aliases != NULL)
					while (h->h_aliases[numaliases]) size += strlen(h->h_aliases[numaliases++])+5;
		
				if (h->h_addr_list != NULL)
				{
					while (h->h_addr_list[numaddr]) numaddr++;
					size += numaddr*(h->h_length+4);
				}
		
				if (sb->hostent)
				{
					m68k_areg(regs,1) = sb->hostent;
					m68k_dreg(regs,0) = sb->hostentsize;
					CallLib(get_long(4),-0xd2);	// FreeMem()
				}
		
				m68k_dreg(regs,0) = size;
				m68k_dreg(regs,1) = 0;
				sb->hostent = CallLib(get_long(4),-0xc6); // AllocMem()
		
				if (!sb->hostent)
				{
					write_log("bsdsocket: WARNING: gethostby%s() ran out of Amiga memory (couldn't allocate %ld bytes) while returning result of lookup for '%s'\n",addrtype == -1 ? "name" : "addr",size,(char *)name);
					seterrno(sb,12); // ENOMEM
					return;
				}
				
				sb->hostentsize = size;
				
				aptr = sb->hostent+28+numaliases*4+numaddr*4;
			
				// transfer hostent to Amiga memory
				put_long(sb->hostent+4,sb->hostent+20);
				put_long(sb->hostent+8,h->h_addrtype);
				put_long(sb->hostent+12,h->h_length);
				put_long(sb->hostent+16,sb->hostent+24+numaliases*4);
				
				for (i = 0; i < numaliases; i++) put_long(sb->hostent+20+i*4,addstr(&aptr,h->h_aliases[i]));
				put_long(sb->hostent+20+numaliases*4,0);
				for (i = 0; i < numaddr; i++) put_long(sb->hostent+24+(numaliases+i)*4,addmem(&aptr,h->h_addr_list[i],h->h_length));
				put_long(sb->hostent+24+numaliases*4+numaddr*4,0);
				put_long(sb->hostent,aptr);
				addstr(&aptr,h->h_name);

				TRACE(("OK (%s)\n",h->h_name));
				seterrno(sb,0);
			}
			else
			{
				TRACE(("failed (%d/%d)\n",sb->sb_errno,sb->sb_herrno))
			}
		}
		else
		{
			seterrno(sb,12); // ENOMEM - well...
			write_log("bsdsocket: ERROR: WSAAsyncGetHostBy%s() failed - error code: %d\n",addrtype == -1 ? "Name" : "Addr",WSAGetLastError());
		}

		if (wMsg) cancelasyncmsg(wMsg);
	}
}

void host_getservbynameport(SB, uae_u32 nameport, uae_u32 proto, uae_u32 type)
{
	SERVENT *s;
	int size, numaliases = 0;
	uae_u32 aptr, asize;
	char *name_rp = NULL, *proto_rp = NULL;
	int i;

	char buf[MAXGETHOSTSTRUCT];
	unsigned int wMsg;

	if (proto) proto_rp = get_real_address(proto);

	if (type)
	{
		TRACE(("getservbyport(%d,%s) -> ",nameport,proto_rp ? proto_rp : "NULL"));
	}
	else
	{
		name_rp = get_real_address(nameport);
		TRACE(("getservbyname(%s,%s) -> ",name_rp,proto_rp ? proto_rp : "NULL"));
	}

	if ((wMsg = allocasyncmsg(sb,0,INVALID_SOCKET)) != 0)
	{
		if ((sb->hAsyncTask = type ? WSAAsyncGetServByPort(hAmigaWnd,wMsg,nameport,proto_rp,buf,sizeof buf) : WSAAsyncGetServByName(hAmigaWnd,wMsg,name_rp,proto_rp,buf,sizeof buf)) != NULL)
		{
			while (sb->hAsyncTask) WAITSIGNAL;

			sockabort(sb);

			if (!sb->sb_errno)
			{
				s = (SERVENT *)buf;

				// compute total size of servent
				size = 20;
				if (s->s_name != NULL) size += strlen(s->s_name)+1;
				if (s->s_proto != NULL) size += strlen(s->s_proto)+1;

				if (s->s_aliases != NULL)
					while (s->s_aliases[numaliases]) size += strlen(s->s_aliases[numaliases++])+5;

				if (sb->servent)
				{
					m68k_areg(regs,1) = sb->servent;
					m68k_dreg(regs,0) = sb->serventsize;
					CallLib(get_long(4),-0xd2);	// FreeMem()
				}

				m68k_dreg(regs,0) = size;
				m68k_dreg(regs,1) = 0;
				sb->servent = CallLib(get_long(4),-0xc6); // AllocMem()

				if (!sb->servent)
				{
					write_log("bsdsocket: WARNING: getservby%s() ran out of Amiga memory (couldn't allocate %ld bytes)\n",type ? "port" : "name",size);
					seterrno(sb,12); // ENOMEM
					return;
				}

				sb->serventsize = size;
				
				aptr = sb->servent+20+numaliases*4;
			
				// transfer servent to Amiga memory
				put_long(sb->servent+4,sb->servent+16);
				put_long(sb->servent+8,(unsigned short)htons(s->s_port));
				
				for (i = 0; i < numaliases; i++) put_long(sb->servent+16+i*4,addstr(&aptr,s->s_aliases[i]));
				put_long(sb->servent+16+numaliases*4,0);
				put_long(sb->servent,aptr);
				addstr(&aptr,s->s_name);
				put_long(sb->servent+12,aptr);
				addstr(&aptr,s->s_proto);

				TRACE(("OK (%s, %d)\n",s->s_name,(unsigned short)htons(s->s_port)));
				seterrno(sb,0);
			}
			else
			{
				TRACE(("failed (%d)\n",sb->sb_errno))
			}
		}
		else
		{
			seterrno(sb,12); // ENOMEM - well...
			write_log("bsdsocket: ERROR: WSAAsyncGetServBy%s() failed - error code: %d\n",type ? "Port" : "Name",WSAGetLastError());
		}
		
		cancelasyncmsg(wMsg);
	}
}

void host_getprotobyname(SB, uae_u32 name)
{
	PROTOENT *p;
	int size, numaliases = 0;
	uae_u32 aptr, asize;
	char *name_rp;
	int i;

	char buf[MAXGETHOSTSTRUCT];
	unsigned int wMsg;

	name_rp = get_real_address(name);

	TRACE(("getprotobyname(%s) -> ",name_rp));
	
	if ((wMsg = allocasyncmsg(sb,0,INVALID_SOCKET)) != 0)
	{
		if ((sb->hAsyncTask = WSAAsyncGetProtoByName(hAmigaWnd,wMsg,name_rp,buf,sizeof buf)) != NULL)
		{
			while (sb->hAsyncTask) WAITSIGNAL;

			sockabort(sb);

			if (!sb->sb_errno)
			{
				p = (PROTOENT *)buf;

				// compute total size of protoent
				size = 16;
				if (p->p_name != NULL) size += strlen(p->p_name)+1;

				if (p->p_aliases != NULL)
					while (p->p_aliases[numaliases]) size += strlen(p->p_aliases[numaliases++])+5;

				if (sb->protoent)
				{
					m68k_areg(regs,1) = sb->protoent;
					m68k_dreg(regs,0) = sb->protoentsize;
					CallLib(get_long(4),-0xd2);	// FreeMem()
				}

				m68k_dreg(regs,0) = size;
				m68k_dreg(regs,1) = 0;
				sb->protoent = CallLib(get_long(4),-0xc6); // AllocMem()

				if (!sb->protoent)
				{
					write_log("bsdsocket: WARNING: getprotobyname() ran out of Amiga memory (couldn't allocate %ld bytes) while returning result of lookup for '%s'\n",size,(char *)name);
					seterrno(sb,12); // ENOMEM
					return;
				}

				sb->protoentsize = size;
				
				aptr = sb->protoent+16+numaliases*4;
			
				// transfer protoent to Amiga memory
				put_long(sb->protoent+4,sb->protoent+12);
				put_long(sb->protoent+8,p->p_proto);
				
				for (i = 0; i < numaliases; i++) put_long(sb->protoent+12+i*4,addstr(&aptr,p->p_aliases[i]));
				put_long(sb->protoent+12+numaliases*4,0);
				put_long(sb->protoent,aptr);
				addstr(&aptr,p->p_name);
				TRACE(("OK (%s, %d)\n",p->p_name,p->p_proto));
				seterrno(sb,0);
			}
			else
			{
				TRACE(("failed (%d)\n",sb->sb_errno))
			}
		}
		else
		{
			seterrno(sb,12); // ENOMEM - well...
			write_log("bsdsocket: ERROR: WSAAsyncGetProtoByName() failed - error code: %d\n",WSAGetLastError());
		}
		
		cancelasyncmsg(wMsg);
	}
}

uae_u32 host_gethostname(uae_u32 name, uae_u32 namelen)
{
	return gethostname(get_real_address(name),namelen);
}
