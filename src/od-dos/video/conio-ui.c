 /*
  * UAE - The Un*x Amiga Emulator
  *
  * conio frontend for a text-based user interface.
  *
  * Copyright 1996 Tim Gunn, Gustavo Goedert
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <conio.h>
#include <ctype.h>
#include <crt0.h>

#include "config.h"
#include "options.h"
#include "uae.h"
#include "tui.h"

void save_state(void);
void drawbox(int,int,int,int);
int alphasort(const void *a, const void *b);
int scandir(const char *dir, struct dirent ***namelist);

#define TB textbackground
#define TC textcolor
#define CURSON _setcursortype(_NORMALCURSOR);
#define CURSOFF _setcursortype(_NOCURSOR);
#define SCRON _wscroll = 1;
#define SCROFF _wscroll = 0;

int SETTC=15; //standard text colour.
int SETTB=1;  //standard background color.
int SETHL=14; //text hilight.

typedef struct {
	int left;
	int top;
	int right;
	int bottom;
	char *memory;
} WINDOW;

static int currwin;

static WINDOW winstack[10]; /* more than enough */
static int winnr = 0;

void save_state(void) {
    int x1, y1, x2, y2;

    if (winstack[currwin].memory != NULL)
	free(winstack[currwin].memory);
    x1 = winstack[currwin].left;
    y1 = winstack[currwin].top;
    x2 = winstack[currwin].right;
    y2 = winstack[currwin].bottom;
    winstack[currwin].memory = malloc((x2-x1+1)*(y2-y1+1)*2);
    gettext(x1, y1, x2, y2, winstack[currwin].memory);
}

void tui_setup(void)
{
    int i;

    /* enable case on LFN systems (eg. win95) */
    if (_USE_LFN)
	_crt0_startup_flags = _CRT0_FLAG_PRESERVE_FILENAME_CASE;
    _set_screen_lines(50);
    SCROFF
    CURSOFF
    for (i = 0; i < 10; i++)
	winstack[i].memory = NULL;
    highvideo();
    TC(SETTC); TB(SETTB); clrscr();

    winstack[0].left = 1;
    winstack[0].top = 1;
    winstack[0].right = tui_cols();
    winstack[0].bottom = tui_lines();
    winstack[0].memory = malloc(tui_cols()*tui_lines()*2);
    gettext(1, 1, tui_cols(), tui_lines(), winstack[0].memory);
    window(1, 1, tui_cols(), tui_lines());
    currwin = 0;
    winnr = 1;
}

int tui_lines(void)
{
    return 50;
}

int tui_cols(void)
{
    return 80;
}

void tui_shutdown(void)
{
    int i;

    for (i = 0; i < 10; i++) {
	if (winstack[i].memory != NULL) {
	    free(winstack[i].memory);
	    winstack[i].memory = NULL;
	}
    }
    winnr = 0;
    normvideo();
    SCRON
    CURSON
    window(1, 1, tui_cols(), tui_lines());
    TC(LIGHTGRAY); TB(BLACK); clrscr();
    _set_screen_lines(25);
}

void tui_refresh(void)
{
    int i, x1, y1, x2, y2;

    for (i=0; i<=currwin; i++) {
	if (winstack[i].memory != NULL) {
	    x1 = winstack[i].left;
	    y1 = winstack[i].top;
	    x2 = winstack[i].right;
	    y2 = winstack[i].bottom;
	    puttext(x1, y1, x2, y2, winstack[i].memory);
	}
    }
}

void tui_puts(const char *s)
{
    cputs(s);
    save_state();
}

void tui_cursoff(void)
{
}

void tui_curson(void)
{
}

void tui_putc(char c)
{
    putch(c);
    save_state();
}

void tui_cr(void)
{
    tui_puts("\r\n");
    save_state();
}

char tui_getc(void)
{
    return getch();
//    save_state();
}

void tui_gotoxy(int x, int y)
{
    gotoxy(x, y);
//    save_state();
}

void tui_selwin(int w)
{
    window(winstack[w].left, winstack[w].top, winstack[w].right, winstack[w].bottom);
    currwin = w;
    tui_refresh();
}

void tui_clrwin(int w)
{
    tui_selwin(w);
    clrscr();
    save_state();
}

void drawbox(int x, int y, int x2, int y2) {
    int i;
    clrscr();
    gotoxy(x,y); cprintf("É");
    for(i=0;i<x2-x-1;i++){ cprintf("Í"); }
    cprintf("»");
    gotoxy(x,y2); cprintf("È");
    for(i=0;i<x2-x-1;i++){ cprintf("Í"); }
    cprintf("¼");
    for(i=y+1;i<y2;i++) {
	gotoxy(x,i); cprintf("º");
	gotoxy(x2,i); cprintf("º");
    }
}

void tui_drawbox(int w)
{
    tui_selwin(w);
    drawbox(1, 1, winstack[w].right-winstack[w].left+1, winstack[w].bottom-winstack[w].top+1);
    save_state();
}

void tui_hline(int x1, int y1, int x2)
{
    int i;
    gotoxy(x1,y1);
    for (i=x1; i<=x2; i++)
	putch('Ä');
    save_state();
}

int tui_dlog(int x1, int y1, int x2, int y2)
{
    winstack[winnr].left = x1;
    winstack[winnr].top = y1;
    winstack[winnr].right = x2;
    winstack[winnr].bottom = y2;
    window(x1, y1, x2, y2);
    currwin = winnr;
    return winnr++;
}

void tui_dlogdie(int w)
{
    tui_selwin(w);
    if (winstack[w].memory != NULL) {
	free(winstack[w].memory);
	winstack[w].memory = NULL;
    }
    winnr--;
    currwin = w-1;
}

int tui_gets(char *buf, int x, int y, int n)
{
    int i = 0;
    tui_gotoxy(x+1, y+1);
    CURSON
    for (;;) {
	int c = getch();
	int j;
	if (c == 13) {
	    {
		char *tmp;

		while ((tmp = strchr(buf, '\\')))
			*tmp = '/';
	    }
	    CURSOFF
	    return 1;
	} else if (c == 27) {
	    CURSOFF
	    return 0;
	} else if (c == 8) {
	    if (i>0)
		i--;
	} else if (i + 1 < n)
	    buf[i++] = c;
	tui_gotoxy(x+1, y+1);
	for (j = 0; j < i; j++)
	    tui_putc(buf[j]);
	for (; j < n; j++)
	    tui_putc(' ');
	tui_gotoxy(x+i+1, y+1);
	buf[i] = 0;
    }
}

int tui_wgets(char *buf, const char *title, int n)
{
    int result;
    int l = strlen(title);
    int ww = l > n ? l : n;
//    int w = tui_dlog((tui_cols()-ww-2)/2, tui_lines()/2-1, (tui_cols()+ww+2)/2, tui_lines()/2+1);
    int w = tui_dlog((tui_cols()-ww-2)/2+1, tui_lines()/2-1, (tui_cols()+ww+2)/2, tui_lines()/2+1);
    tui_drawbox(w);
    tui_gotoxy((ww-l)/2+2, 1);
    tui_puts(title);
    result = tui_gets(buf, 1, 1, n);
    tui_dlogdie(w);
    return result;
}


int tui_menubrowse(struct bstring *menu, int xoff, int yoff, int selected, int XXX) //FIXME!!!
{
    int count = 0, maxsel = 0, maxw = 0;
    int i, j, w;

    const char *mtitle = NULL;

    for (i = 0; menu[i].val != -3; i++) {
	int tmp;
	if (menu[i].val != 0) {
	    count++;
	    if (menu[i].val != -2)
		maxsel++;
	} else
	    mtitle = menu[i].data;
	if ((tmp = strlen(menu[i].data)) > maxw)
	    maxw = tmp;
    }
    maxw += 3;
    w = tui_dlog(xoff, yoff, xoff+maxw, yoff+count+1);
    tui_selwin(w);
    tui_drawbox(w);
    if (mtitle != NULL) {
	tui_gotoxy(2, 1);
	tui_puts(mtitle);
    }

    for (;;) {
	int c;
	int s2;

	for (i = j = s2 = 0; i < count; i++, j++) {
	    int k, x;
	    int a = s2 == selected ? 1 : 0;

	    while (menu[j].val == 0)
		j++;
	    tui_gotoxy(2, 2+i);
	    if (a) {
		TC(SETTB);
		TB(SETTC);
	    }
	    tui_putc(' ');
	    for (k = x = 0; menu[j].data[k]; k++, x++) {
		int a2 = 0;
		c = menu[j].data[k];
		if (c == '_')
		    c = menu[j].data[++k], a2 = 1;
		tui_gotoxy(3+x, 2+i);
		if (a2)
		    TC(SETHL);
		tui_putc(c);
		if (a2) {
		    if (a)
			TC(SETTB);
		    else
			TC(SETTC);
		}
	    }
	    for (; x < maxw-2; x++) {
		tui_gotoxy(3+x, 2+i);
		tui_putc(' ');
	    }
	    if (a) {
		TC(SETTC);
		TB(SETTB);
	    }
	    if (menu[j].val != -1)
		s2++;
	}

	tui_refresh();
	c = getch();
	if (c == 27) {
	    tui_dlogdie(w);
	    return -1;
	} else if (c == 13 || c == ' ') {
	    tui_dlogdie(w);
	    return selected;
	} else if (c == 0) {
	    c = getch();
	    switch (c) {
		case 72:
		   if (selected > 0)
		       selected--;
		   break;
		case 80:
		   if (selected + 1 < count)
		       selected++;
		   break;
		case 73:
		   selected = 0;
		   break;
		case 81:
		   selected = count - 1;
		   break;
		default:
		   for (j = i = 0; menu[i].val != -3; i++)
		       if (menu[i].val == toupper(c)) {
			   tui_dlogdie(w);
			   return j;
		       } else if (menu[i].val == -1 || menu[i].val > 0) {
			   j++;
		   }

		   break;
	    }
	} else {
	    for (j = i = 0; menu[i].val != -3; i++)
		if (menu[i].val == toupper(c)) {
		    tui_dlogdie(w);
		    return j;
		} else if (menu[i].val == -1 || menu[i].val > 0) {
		    j++;
	    }

	}
    }
    return -1; /* Can't get here */
}

void tui_errorbox(const char *err)
{
    const char *hak = "Hit any key";
    int n = strlen(hak);
    int l = strlen(err);
    int ww = (l > n ? l : n) + 2;
    int w = tui_dlog((tui_cols()-ww)/2, tui_lines()/2-1, (tui_cols()+ww)/2, tui_lines()/2+1);
    tui_selwin(w); tui_drawbox(w);

    tui_gotoxy((ww-6)/2+2, 1);
    tui_puts("Error!");
    tui_gotoxy((ww-l)/2+2, 2);
    tui_puts(err);
    tui_gotoxy((ww-n)/2+2, 3);
    tui_puts(hak);

    tui_refresh();
    for (;;) {
	int c = getch();
	if (c == 13)
	    break;
    }
    tui_dlogdie(w);
}

static char *pattern;
static int maxlen;

static void put_filename(char *s, int x, int y, int HL)
{
    char buf[256];
    int i;

    tui_gotoxy(x,y);
    if (strcmp(s, ".") == 0)
	strcpy(buf, "(none)");
    else
	strcpy(buf, s);
    buf[maxlen] = 0;
    TC(SETHL);
    if (HL)
	TB(SETTC);
    for (i = 0; i < (int) strlen(buf); i++)
	tui_putc(buf[i]);
    for (; i < maxlen; i++)
	tui_putc(' ');
    TC(SETTC);
    if (HL)
	TB(SETTB);
}

static char fsbuf[256];

static int selectfn(const struct dirent *de)
{
    int l1, l2;

/*    l1 = strlen(pattern + 1);*/
    l2 = strlen(de->d_name);

    if (l2 >= tui_cols()-10) /* Restrict length of filenames so we won't mess up the display */
	return 0;

    /* No pattern matching for now. But we don't show hidden files. */
    if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0
	&& de->d_name[0] == '.')
	return 0;
    if (l2 > maxlen)
	maxlen = l2;
    return 1;
}

char *tui_filereq(char *s, char *oldfile, const char *title) //FIXME!!!
{
    char cwd[256];
    char *retval = fsbuf;
    char *tmp;
    int fin = 0;
//    chtype moresave[6][2];

    /* Save wd */
    if (getcwd(cwd, 256) == NULL)
	return NULL;

    /* Change into directory of old file */
    strcpy(fsbuf, oldfile);
    tmp = strrchr(fsbuf, '/');
    if (tmp != NULL) {
	*tmp = 0;
	if (strlen(fsbuf) > 0)
	    chdir(fsbuf);
    }

    pattern = s;
    if (s[0] != '*')
	fprintf(stderr, "Can't handle wildcard %s\n", s);
    if (s[1] != 0 && strchr(s+1, '*') != NULL)
	fprintf(stderr, "Can't handle wildcard %s\n", s);
    for (;!fin;) {
	struct dirent **names;
	int i, w, n, l, yp, oldyp, s;

	maxlen = 0;
	n = scandir(".", &names);

	if (n <= 0)
	    return NULL;
	if (title != NULL && strlen(title) + 6 > maxlen)
	    maxlen = strlen(title) + 6;
	l = n;
	if (l > 40)
	    l = 40;
	yp = s = 0; oldyp = -1;
	w = tui_dlog(tui_cols() - maxlen - 8, 5, tui_cols() - 5, 5 + l + 1);
	tui_selwin(w); tui_drawbox(w);
/*	if (title)
	    mvwaddstr(currwin, 0, 2, title);
	for (i = 0; i < 6; i++) {
	    moresave[i][0] = mvwinch(currwin, 0, maxlen-3+i);
	    moresave[i][1] = mvwinch(currwin, l+1, maxlen-3+i);
	}*/
	for (;;) {
	    int c;
	    char tmp[256];
	    while (s < yp)
		yp--;
	    while (s >= yp + l)
		yp++;
	    if (oldyp != yp) {
		oldyp = yp;
		for (i = 0; i < l; i++) {
		    put_filename(names[i + yp]->d_name, 3, 2 + i, 0);
		}
	    }
	    put_filename(names[s]->d_name, 3, 2 + s - yp, 1);

/*	    if (yp == 0)
		for (i = 0; i < 6; i++)
		    mvwaddch(currwin, 0, maxlen-3+i, moresave[i][0]);
	    else
		mvwaddstr(currwin, 0, maxlen-3, "(more)");
	    if (yp + l == n)
		for (i = 0; i < 6; i++)
		    mvwaddch(currwin, l+1, maxlen-3+i, moresave[i][1]);
	    else
		mvwaddstr(currwin, l+1, maxlen-3, "(more)");
	    tui_refresh();*/
	    c = getch();
	    put_filename(names[s]->d_name, 3, 2 + s - yp, 0);
	    if (c == 27) {
		retval = NULL; fin = 1;
		break;
	    } else if (c == 13 || c == ' ') {
		int err;

		if (strcmp(names[s]->d_name, ".") == 0) {
		    fin = 1;
		    strcpy(fsbuf, "");
		    break;
		}
		err = chdir(names[s]->d_name);

		if (err == 0)
		    break;
		else /*if (errno == ENOTDIR)*/ {
		    fin = 1;
		    if (getcwd(fsbuf, 256) == NULL)
			retval = NULL;
		    if (strlen(fsbuf) + strlen(names[s]->d_name) + 2 >= 256)
			retval = NULL;
		    else {
			strcat(fsbuf, "/");
			strcat(fsbuf, names[s]->d_name);
		    }
		    break;
		} /* else what? */
	    }
	    if (c==0) {
		c = getch();
		switch (c) {
		 case 72:
		    if (s > 0)
			s--;
		    break;
		 case 80:
		    if (s + 1 < n)
			s++;
		    break;
		 case 73:
		    if (s > l)
			s -= l;
		    else
			s = 0;
		    break;
		 case 81:
		    if (s + l < n)
			s += l;
		    else
			s = n - 1;
		    break;
		}
	    } else {
		for (i = 0; i < n; i++)
		    if (names[i]->d_name[0] == c) {
			s = i;
			break;
		    }
	    }
	}
#if 0
	/* @@@ is this right? */
	for (i = 0; i < n; i++)
	    free(names[i]->d_name);
	free(names);
#endif
	tui_dlogdie(w);
    }
    chdir(cwd);
    return retval;
}

int tui_backup_optionsfile(void)
{
    char tmp[257];
    char *ptr;
    if (access(optionsfile, 00) == 0)
	return 0;
    strcpy(tmp, optionsfile);
    ptr = strrchr(tmp, '.');
    if (ptr != NULL)
	*ptr = 0;
    strcat(tmp, ".bak");
    return rename(optionsfile, tmp);
}

int alphasort(const void *a, const void *b)
{
  return strcmp ((*((struct dirent **)a))->d_name, (*((struct dirent **)b))->d_name);
}

int scandir(const char *dir, struct dirent ***namelist)
{
  DIR *dp = opendir (dir);
  struct dirent **v = NULL;
  size_t vsize = 0, i;
  struct dirent *d;
  int save;

  if (dp == NULL)
    return -1;

  save = errno;
  errno = 0;

  i = 0;
  while ((d = readdir (dp)) != NULL)
    if (selectfn(d))
      {
	if (i == vsize)
	  {
	    struct dirent **new;
	    if (vsize == 0)
	      vsize = 10;
	    else
	      vsize *= 2;
	    new = (struct dirent **) realloc (v, vsize * sizeof (*v));
	    if (new == NULL)
	      {
	      lose:
		(void) closedir (dp);
		while (i > 0)
		  free (v[--i]);
		free (v);
		errno = ENOMEM;
		return -1;
	      }
	    v = new;
	  }

	v[i] = (struct dirent *) malloc (sizeof (**v));
	if (v[i] == NULL)
	  goto lose;

	*v[i++] = *d;
      }

  if (errno != 0)
    {
      save = errno;

      (void) closedir (dp);

      while (i > 0) free(v[--i]);
      free(v);

      errno = save;
      return -1;
    }

  (void) closedir (dp);
  errno = save;

  qsort (v, i, sizeof (*v), alphasort);
  *namelist = v;
  return i;
}
