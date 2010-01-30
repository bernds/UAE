 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS Sound Blaster interface.
  *
  * (c) 1996 Peter Remmers, Gustavo Goedert
  *
  * Bugfixes by Michael Sontheimer
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <pc.h>
#include <sys/farptr.h>
#include <dpmi.h>
#include <go32.h>

#include "sound/sb.h"
#include "sound/dma.h"

/* some useful macros */
#define LOBYTE(x) ((UBYTE)(((UWORD)(x)) &  0xFF))
#define HIBYTE(x) ((UBYTE)(((UWORD)(x)) >> 8))
#define LOWORD(x) ((UWORD)(((ULONG)(x)) & 0xFFFF))
#define HIWORD(x) ((UWORD)(((ULONG)(x)) >> 16))
#define NUMPARAS(bytesize) (((bytesize)+15) >> 4)

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Offsets relative to base I/O address. */
#define SB_MIXER_ADDRESS        0x04
#define SB_MIXER_DATA           0x05
#define SB_DSP_RESET            0x06
#define SB_DSP_READ_DATA        0x0A
#define SB_DSP_WRITE_DATA       0x0C
#define SB_DSP_WRITE_STATUS     0x0C
#define SB_DSP_DATA_AVAIL       0x0E
#define SB_DSP_INT_CLEAR        0x0F

/* DSP Commands */
#define CMD_PLAY_8BIT                0x14
#define CMD_SET_TIME_CONSTANT        0x40
#define CMD_DEFINE_SILENCE_BLOCK     0x80
#define CMD_PAUSE_8BIT_DMA           0xD0
#define CMD_CONTINUE_8BIT_DMA        0xD4
#define CMD_SPEAKER_ON               0xD1
#define CMD_SPEAKER_OFF              0xD3
#define CMD_GET_SPEAKER_SETTING      0xD8
#define CMD_DSP_VER                  0xE1

/* SbPro-only commands */
#define CMD_SET_BLOCK_SIZE           0x48
#define CMD_PLAY_8BIT_HISPEED        0x91

/* SB16-only commands */
#define CMD_SET_OUTPUT_RATE          0x41
#define CMD_PLAY_16BIT_SINGLE        0xB0
#define CMD_PLAY_16BIT_AUTOINIT      0xB6
#define CMD_PLAY_8BIT_SINGLE         0xC0
#define CMD_PLAY_8BIT_AUTOINIT       0xC6
#define CMD_PAUSE_16BIT_DMA          0xD5
#define CMD_CONTINUE_16BIT_DMA       0xD6

typedef void (*tPlayProc)(UWORD bytes);

typedef struct
{
  ULONG Data;
  _go32_dpmi_seginfo dma_mem; /* info for DMA memory */
}
tDMABuffer;

tSBType SB_Type;

UWORD SB_Base;      /* the Base Address  e.g. 220h                         */
UWORD SB_IRQ;       /* The IRQ           e.g. 5                            */
UWORD SB_DMAlo;     /* Low DMA channel   e.g. 1                            */
UWORD SB_DMAhi;     /* high DMA channel  e.g. 5                            */
UWORD SB_INT;       /* The Interrupt vector corresponding to IRQ e.g. 0Dh  */
UWORD SB_DMA;       /* The current DMA channel                             */
		    /* (depends on if we're playing 8Bit or 16Bit sound)   */
UBYTE SB_VersMaj;   /* Major DSP Version byte                              */
UBYTE SB_VersMin;   /* Minor DSP Version byte                              */
UBYTE SB_ModeByte;  /* Mode Byte for SB16 (Mono/Stereo, Signed/Unsigned )  */
UWORD SB_IntClearPort; /* SB Port to read from to acknoledge interrupt     */

tPlayProc SB_Play_Proc;        /* function */
tDMABuffer dma_buf[2];         /* the two DMA buffers */

BOOL  SB_16Bit;       /* we're playing in 16 Bits                          */
BOOL  SB_Stereo;      /* we're playing in stereo                           */
UWORD SB_Active_Rate; /* the active sampling rate                          */

volatile UBYTE IRQ_Detected;    /* the detected IRQ number                 */

_go32_dpmi_seginfo   old_pmint;  /* the old interrupt handler         */
_go32_dpmi_seginfo   pm_wrapper; /* the new interrupt handler         */

/* Reset SB at SB_Base, returns TRUE if it worked */
BOOL SB_Reset (void)
{
  outportb (SB_Base+SB_DSP_RESET, 1);
  delay(100);
  outportb (SB_Base+SB_DSP_RESET, 0);
  delay(100);
  return (inportb(SB_Base+SB_DSP_READ_DATA) == 0xAA);
}

/* Write a command to DSP chip */
inline void SB_WriteDSP (UBYTE value)
{
  while (inportb(SB_Base + SB_DSP_WRITE_STATUS) & 0x80);
  outportb (SB_Base + SB_DSP_WRITE_DATA, value);
}

/* read a byte from DSP chip */
inline UBYTE SB_ReadDSP (void)
{
  while (!(inportb (SB_Base + SB_DSP_DATA_AVAIL) & 0x80));
  return  (inportb (SB_Base + SB_DSP_READ_DATA));
}

/* write a value to the mixer chip */
inline void SB_WriteMixer (UBYTE reg, UBYTE data)
{
  outportb (SB_Base + SB_MIXER_ADDRESS, reg);
  outportb (SB_Base + SB_MIXER_DATA,    data);
}

/* read a value from the mixer chip */
inline UBYTE SB_ReadMixer (UBYTE reg)
{
  outportb (SB_Base + SB_MIXER_ADDRESS, reg);
  return (inportb (SB_Base + SB_MIXER_DATA));
}

/* try to detect, if SB has a mixer chip, returns TRUE if found */
BOOL SB_DetectMixer (void)
{
  UBYTE orgval;

  orgval = SB_ReadMixer (0x22);

  // Some SB-Pro-clones always set the bit 0 and 4, if reading
  // the volume (the bit 0 and 4 aren't used for setting the volume)

  SB_WriteMixer (0x22, 0);
  delay(200);
  if ((SB_ReadMixer (0x22)&(0xEE)) != 0)
    return (FALSE);

  SB_WriteMixer (0x22, 0xEE);
  delay(200);
  if ((SB_ReadMixer (0x22)&(0xEE)) != 0xEE)
    return (FALSE);

//  SB_WriteMixer (0x22, orgval);   // Why not setting the volume to maximum ?

  return (TRUE);
}

/* play a block in 8 Bits on a Standard SB */
void SB_Play_8Bit_Std (UWORD bytes)
{
  SB_WriteDSP (CMD_PLAY_8BIT);
  SB_WriteDSP (LOBYTE (bytes-1));
  SB_WriteDSP (HIBYTE (bytes-1));
}

/* Play a block in 8 Bits on an SB PRO */
void SB_Play_8Bit_SbPro (UWORD bytes)
{
  SB_WriteDSP (CMD_SET_BLOCK_SIZE);
  SB_WriteDSP (LOBYTE (bytes-1));
  SB_WriteDSP (HIBYTE (bytes-1));
  SB_WriteDSP (CMD_PLAY_8BIT_HISPEED);
}

/* play a block in 8 Bits on an SB 16 */
void SB_Play_8Bit_SB16 (UWORD bytes)
{
  SB_WriteDSP (CMD_PLAY_8BIT_SINGLE);
  SB_WriteDSP (SB_ModeByte);
  SB_WriteDSP (LOBYTE (bytes-1));
  SB_WriteDSP (HIBYTE (bytes-1));
}

/* play a block in 16 Bits on an SB 16 */
void SB_Play_16Bit (UWORD bytes)
{
  SB_WriteDSP (CMD_PLAY_16BIT_SINGLE);
  SB_WriteDSP (SB_ModeByte);
  SB_WriteDSP (LOBYTE ((bytes>>1) -1));
  SB_WriteDSP (HIBYTE ((bytes>>1) -1));
}

/* the main interrupt handler */
void SB_IntHandler (void)
{
  IsPlaying = FALSE;

  inportb (SB_IntClearPort);
  if (SB_IRQ > 7) outportb (0xA0, 0x20);

  outportb (0x20, 0x20);
  enable();
}

/* set sampling rate on a standard SB */
void SB_SetRate_LoSpeed (UWORD rate)
{
  SB_WriteDSP (CMD_SET_TIME_CONSTANT);
  SB_WriteDSP (0x100 - 1000000 / rate);
}

/* set sampling rate on a SB PRO */
void SB_SetRate_HiSpeed (UWORD rate)
{
  SB_WriteDSP (CMD_SET_TIME_CONSTANT);
  SB_WriteDSP (HIBYTE (0x10000 - 256000000 / rate));
}

/* set sampling rate on an SB 16 */
void SB_SetRate_SB16 (UWORD rate)
{
  SB_WriteDSP (CMD_SET_OUTPUT_RATE);
  SB_WriteDSP (HIBYTE (rate));
  SB_WriteDSP (LOBYTE (rate));
}

/* set the sampling rate */
int SB_SetRate (unsigned short rate)
{
  if (rate > 23000 && SB_Type < SB_Type_SbPro) return(FALSE);

  switch (SB_Type)
  {
  case SB_Type_StdSB: SB_SetRate_LoSpeed (rate); break;
  case SB_Type_SbPro: SB_SetRate_HiSpeed (rate); break;
  case SB_Type_SB16:  SB_SetRate_SB16    (rate); break;
  }

  SB_Active_Rate = rate;

  return (TRUE);
}

/* set the number of bits, i.e. 8 Bit / 16 Bit mode */
int SB_SetBits (unsigned char bits)
{
  if (bits==16 && SB_Type < SB_Type_SB16) return (FALSE);

  if (SB_Type == SB_Type_SB16)
  {
    if (bits==16)
    {
      SB_Play_Proc        = SB_Play_16Bit;
      SB_IntClearPort     = SB_Base + SB_DSP_INT_CLEAR;
      SB_DMA              = SB_DMAhi;
      SB_16Bit            = TRUE;
    }
    else
    {
      SB_Play_Proc        = SB_Play_8Bit_SB16;
      SB_IntClearPort     = SB_Base + SB_DSP_DATA_AVAIL;
      SB_DMA              = SB_DMAlo;
      SB_16Bit            = FALSE;
    }
  }

  return (TRUE);
}

/* set the number of channels, i.e. Mono / Stereo */
int SB_SetChannels (unsigned char channels)
{
  if (channels==2 && SB_Type < SB_Type_SbPro) return (FALSE);

  switch (SB_Type)
  {
  case SB_Type_StdSB: break;

  case SB_Type_SbPro:
    if (channels==2) SB_WriteMixer (0x0E, SB_ReadMixer (0x0E) | 0x02);
		else SB_WriteMixer (0x0E, SB_ReadMixer (0x0E) & 0xFD);
    break;

  case SB_Type_SB16:
    if (channels==2) SB_ModeByte |= 0x20;
		else SB_ModeByte &= 0xDF;
    break;
  }
  SB_Stereo = (channels==2);

  return (TRUE);
}

/* set Signed / Unsigned mode (only on SB 16) */
int SB_SetSigned (int sign)
{
  if (sign && SB_Type < SB_Type_SB16) return (FALSE);

  if (SB_Type == SB_Type_SB16)
  {
    if (sign) SB_ModeByte |= 0x10;
	 else SB_ModeByte &= 0xEF;
  }
  return (TRUE);
}

/* turn speaker off (doesn't seem to work on SB 16) */
void SB_SpeakerOff(void)
{
  SB_WriteDSP(CMD_SPEAKER_OFF);
}

/* turn speaker on (doesn't seem to work on SB 16) */
void SB_SpeakerOn(void)
{
  SB_WriteDSP(CMD_SPEAKER_ON);
}

/* set sound blaster master volume, range from 0 to 7 */
void SB_SetVolume(UBYTE vol)
{

  /* no mixer on standard sb */
  if (SB_Type < SB_Type_SbPro)
      return;

  if (vol>7)
      vol = 7;
  vol = (vol<<1);
  if (SB_Type == SB_Type_SbPro)
      vol = vol | 1;
  vol = vol | (vol<<4);

  SB_WriteMixer (0x22, vol);
}

/* interrupt handler functions used for IRQ/DMA detection */
void irq2 (void)
{
  IRQ_Detected = 2;
  inportb (SB_IntClearPort);
  outportb (0x20, 0x20);
}
void irq5 (void)
{
  IRQ_Detected = 5;
  inportb (SB_IntClearPort);
  outportb (0x20, 0x20);
}
void irq7 (void)
{
  IRQ_Detected = 7;
  inportb (SB_IntClearPort);
  outportb (0x20, 0x20);
}
void irq10 (void)
{
  IRQ_Detected = 10;
  inportb (SB_IntClearPort);
  outportb (0xA0, 0x20);
  outportb (0x20, 0x20);
}

/* detect the sound blaster Base/IRQ/DMA settings (may hang computer) */
int SB_Detect (unsigned short *base, unsigned short *irq,
	       unsigned short *dmalo, unsigned short *dmahi)
{
#define TESTBUFSIZE 0x80
  static const UBYTE lowdma[3]  = { 1,3,0 };
  static const UBYTE highdma[3] = { 5,6,7 };

  UBYTE old21,oldA1,old0F,oldDE;
  _go32_dpmi_seginfo oldirq2,oldirq5,oldirq7,oldirq10;
  _go32_dpmi_seginfo newirq2,newirq5,newirq7,newirq10;
  _go32_dpmi_seginfo testbuf_info;
  ULONG testbuf;
  int   ret1,ret2,ret3,ret4;
  UBYTE i;
  BOOL  found;

  found = FALSE;

  *base  = 0;
  *irq   = 0;
  *dmalo = 0;
  *dmahi = 0;

  SB_Base = 0x210;
  while (SB_Base != 0x290)
  {
    if (SB_Reset()) break;
    SB_Base += 0x10;
  }
  if (SB_Base == 0x290) return (FALSE);
  *base = SB_Base;

  /* install detection interrupt handlers */
  newirq2.pm_offset  = (int) irq2;
  newirq5.pm_offset  = (int) irq5;
  newirq7.pm_offset  = (int) irq7;
  newirq10.pm_offset = (int) irq10;

  ret1 = _go32_dpmi_allocate_iret_wrapper (&newirq2);
  ret2 = _go32_dpmi_allocate_iret_wrapper (&newirq5);
  ret3 = _go32_dpmi_allocate_iret_wrapper (&newirq7);
  ret4 = _go32_dpmi_allocate_iret_wrapper (&newirq10);

  if (ret1|ret2|ret3|ret4)
  {
    if (!ret1) _go32_dpmi_free_iret_wrapper (&newirq2);
    if (!ret2) _go32_dpmi_free_iret_wrapper (&newirq5);
    if (!ret3) _go32_dpmi_free_iret_wrapper (&newirq7);
    if (!ret4) _go32_dpmi_free_iret_wrapper (&newirq10);
    return (FALSE);
  }
  _go32_dpmi_lock_code (irq2, (int)SB_Detect - (int)irq2);
  _go32_dpmi_lock_data ((void*)&IRQ_Detected,    sizeof(IRQ_Detected));
  _go32_dpmi_lock_data (&SB_IntClearPort, sizeof(SB_IntClearPort));

  _go32_dpmi_get_protected_mode_interrupt_vector (0x0A, &oldirq2);
  _go32_dpmi_get_protected_mode_interrupt_vector (0x0D, &oldirq5);
  _go32_dpmi_get_protected_mode_interrupt_vector (0x0F, &oldirq7);
  _go32_dpmi_get_protected_mode_interrupt_vector (0x72, &oldirq10);

  _go32_dpmi_set_protected_mode_interrupt_vector (0x0A, &newirq2);
  _go32_dpmi_set_protected_mode_interrupt_vector (0x0D, &newirq5);
  _go32_dpmi_set_protected_mode_interrupt_vector (0x0F, &newirq7);
  _go32_dpmi_set_protected_mode_interrupt_vector (0x72, &newirq10);

  testbuf_info.size = NUMPARAS(TESTBUFSIZE);
  testbuf = DMA_AllocDMABuf (&testbuf_info);

  /* turn on interrupts 2,5,7 and 10 */
  old21 = inportb (0x21);
  oldA1 = inportb (0xA1);
  outportb (0x21, old21 & 0x5B);
  outportb (0xA1, oldA1 & 0xFB);

  /* turn off DMA 0,1,3,5,6 and 7 */
  old0F = inportb (0x0F);
  oldDE = inportb (0xDE);
  outportb (0x0F, old0F | 0x0B);
  outportb (0xDE, oldDE | 0x0E);

  /* fill testbuffer with silence */
  for (i=0; i<TESTBUFSIZE; i++) _farnspokeb (testbuf+i, 128);

  /* detect low DMA and IRQ */
  SB_IntClearPort = SB_Base + SB_DSP_DATA_AVAIL;
  SB_SetRate_LoSpeed (22050);
  /* program SB to play testbuffer, will start playing */
  /* when we hit the right DMA channel                 */
  SB_Play_8Bit_Std (TESTBUFSIZE);

  /* test all DMA channels */
  IRQ_Detected = 0;
  for (i=0; i<3; i++)
  {
    DMA_InitTransfer (lowdma[i], DMA_READ, testbuf, TESTBUFSIZE);
    delay (TESTBUFSIZE*2000 / 22050);
    if (IRQ_Detected)
    {
      *dmalo = lowdma[i];
      *irq   = IRQ_Detected;
      break;
    }
  }

  if (IRQ_Detected)
  {
    found = TRUE;

    SB_WriteDSP (CMD_DSP_VER);
    SB_VersMaj = SB_ReadDSP();
    SB_VersMin = SB_ReadDSP();

    /* if SoundBlaster is an SB16 then test high DMA channel */
    if (SB_VersMaj >= 4)
    {
      outportb (0x0F, old0F | 0x0B);
      outportb (0xDE, oldDE | 0x0E);

      for (i=0; i<TESTBUFSIZE; i+=2) _farnspokew (testbuf+i, 0x8000);

      SB_ModeByte = 0x00;
      SB_IntClearPort = SB_Base + SB_DSP_INT_CLEAR;
      SB_SetRate_SB16(44100);
      SB_Play_16Bit(TESTBUFSIZE);

      IRQ_Detected = 0;
      for (i=0; i<3; i++)
      {
	DMA_InitTransfer (highdma[i], DMA_READ, testbuf, TESTBUFSIZE);
	delay (TESTBUFSIZE*1000 / 44100);
	if (IRQ_Detected)
	{
	  *dmahi = highdma[i];
	  break;
	}
      }
      if (!IRQ_Detected) *dmahi = *dmalo;
    }
  }

  /* clean up */
  _go32_dpmi_free_dos_memory (&testbuf_info);

  outportb (0x0F, old0F);
  outportb (0xDE, oldDE);
  outportb (0x21, old21);
  outportb (0xA1, oldA1);

  _go32_dpmi_set_protected_mode_interrupt_vector (0x0A, &oldirq2);
  _go32_dpmi_set_protected_mode_interrupt_vector (0x0D, &oldirq5);
  _go32_dpmi_set_protected_mode_interrupt_vector (0x0F, &oldirq7);
  _go32_dpmi_set_protected_mode_interrupt_vector (0x72, &oldirq10);

  _go32_dpmi_free_iret_wrapper (&newirq2);
  _go32_dpmi_free_iret_wrapper (&newirq5);
  _go32_dpmi_free_iret_wrapper (&newirq7);
  _go32_dpmi_free_iret_wrapper (&newirq10);

  return (found);
}

int SB_InitBuffers(int bufsize)
{
  dma_buf[0].dma_mem.size = NUMPARAS(bufsize);
  dma_buf[0].Data = DMA_AllocDMABuf (&dma_buf[0].dma_mem);

  dma_buf[1].dma_mem.size = NUMPARAS(bufsize);
  dma_buf[1].Data = DMA_AllocDMABuf (&dma_buf[1].dma_mem);

  if ((dma_buf[0].Data == NULL) || (dma_buf[1].Data == NULL))
    return (FALSE);

  return (TRUE);
}


/* init Sound blaster using the given settings */
int SB_Init (unsigned short base, unsigned short irq,
	     unsigned short dmalo, unsigned short dmahi)
{
  SB_Base  = base;
  SB_IRQ   = irq;
  SB_DMAlo = dmalo;
  SB_DMAhi = dmahi;

  if (SB_IRQ > 7) SB_INT = 0x68 + SB_IRQ;
	     else SB_INT = 0x08 + SB_IRQ;

  if (!SB_Reset()) return (FALSE);

  pm_wrapper.pm_offset = (int) SB_IntHandler;
  if (_go32_dpmi_allocate_iret_wrapper (&pm_wrapper))
  {
    printf ("Cannot allocate protected mode wrapper for SB interrupt!\n");
    _go32_dpmi_free_dos_memory (&dma_buf[0].dma_mem);
    _go32_dpmi_free_dos_memory (&dma_buf[1].dma_mem);
    return (FALSE);
  }

  _go32_dpmi_lock_code (SB_WriteDSP, (ULONG)SB_WriteMixer - (ULONG)SB_WriteDSP);
  _go32_dpmi_lock_data (&SB_Base, (ULONG)&old_pmint - (ULONG)&SB_Base);

  _go32_dpmi_get_protected_mode_interrupt_vector (SB_INT, &old_pmint);
  _go32_dpmi_set_protected_mode_interrupt_vector (SB_INT, &pm_wrapper);

  if (SB_IRQ>7)
    outportb(0xA1, inportb(0xA1) & ~(1 << (SB_IRQ-8)));
  else
    outportb(0x21, inportb(0x21) & ~(1 << SB_IRQ));

  SB_Type = SB_Type_StdSB;

  if( SB_DetectMixer() != FALSE )
    SB_Type = SB_Type_SbPro;

  SB_WriteDSP (CMD_DSP_VER);
  SB_VersMaj = SB_ReadDSP ();
  SB_VersMin = SB_ReadDSP ();

  if (SB_VersMaj >= 4) SB_Type = SB_Type_SB16;

//  SB_Type = SB_Type_SbPro;  /* for testing */

  /* initialize SB to default 8Bit Mono 22050Hz */
  /* (available on all SoundBlaster Versions)   */
  switch (SB_Type)
  {
  case SB_Type_StdSB:
    SB_Play_Proc = SB_Play_8Bit_Std;
    SB_SetRate_LoSpeed(22050);
    break;
  case SB_Type_SbPro:
    SB_Play_Proc = SB_Play_8Bit_SbPro;
    SB_SetRate_HiSpeed(22050);
    break;
  case SB_Type_SB16:
    SB_Play_Proc = SB_Play_8Bit_SB16;
    SB_SetRate_SB16(22050);
    break;
  }
  SB_IntClearPort = SB_Base + SB_DSP_DATA_AVAIL;
  SB_DMA          = SB_DMAlo;
  IsPlaying       = FALSE;
  SB_16Bit        = FALSE;
  SB_Stereo       = FALSE;
  SB_Active_Rate  = 22050;
  SB_ModeByte     = 0x00;

  SB_SpeakerOn();
  SB_SetVolume(6);

  return (TRUE);
}

/* clean up Sound Blaster */
void SB_Done (void)
{
  SB_Reset ();
  if (SB_IRQ > 7)
    outportb (0xA1, inportb(0xA1) | (1 << (SB_IRQ-8)));
  else
    outportb (0x21, inportb(0x21) | (1 << SB_IRQ));

  _go32_dpmi_set_protected_mode_interrupt_vector (SB_INT, &old_pmint);
  _go32_dpmi_free_iret_wrapper (&pm_wrapper);
  _go32_dpmi_free_dos_memory (&dma_buf[0].dma_mem);
  _go32_dpmi_free_dos_memory (&dma_buf[1].dma_mem);
}

/* write sound data to be played to the fifo buffer */
void SB_DirectWrite(unsigned int size, int freq)
{
    DMA_InitTransfer (SB_DMA, DMA_READ, dma_buf[CurrentBuffer].Data, size);
    (*SB_Play_Proc) (size);
    IsPlaying  = TRUE;
    if (freq != SB_Active_Rate)
	SB_SetRate(freq);
}

/* return thee DSP chip version */
void SB_GetVersion (unsigned char *Maj, unsigned char *Min)
{
  *Maj = SB_VersMaj;
  *Min = SB_VersMin;
}

/* Detection Routine */
int SB_DetectInitSound(int *dspbits, int *dsprate, int *sndbufsize, unsigned int *direct_buffers, int *stereo_sound)
{
  UWORD base,irq,dmalo,dmahi;
  char *blaster;
  char *tok;
  int have_sound;
  extern void (*SND_DirectWrite)(unsigned int size, int freq);

  if ((*sndbufsize) > 65536)
      *sndbufsize = 65536;
  if ((*sndbufsize) < 4)
      *sndbufsize = 4;
  *sndbufsize = ((*sndbufsize)>>2)<<2;

  blaster = getenv ("BLASTER");
  if (blaster == NULL)
  {
    printf ("No BLASTER variable found!\n"
	    "Detecting SoundBlaster Settings...."); fflush (stdout);
    have_sound = SB_Detect (&base, &irq, &dmalo, &dmahi);
    if (!have_sound)
    {
      printf ("Not found!!\n");
      return (0);
    }
    printf ("Found!  Base: %hx  IRQ: %hd  DMA: %hd,%hd\n",
			   base,     irq,    dmalo,dmahi);
  }
  else
  {
    base = 0; irq = 0; dmalo = 0; dmahi = 0;

    tok = strtok(blaster, " \t");
    while (tok != NULL)
    {
      switch (toupper(*tok))
      {
      case 'A': sscanf(tok+1, "%hx", &base);  break;
      case 'I': sscanf(tok+1, "%hd", &irq);   break;
      case 'D': sscanf(tok+1, "%hd", &dmalo); break;
      case 'H': sscanf(tok+1, "%hd", &dmahi); break;
      default: break;
      }
      tok = strtok (NULL, " \t");
    }
    printf ("SoundBlaster Settings taken from BLASTER variable:\n"
	    "  Base: %hx  IRQ: %hd  DMA: %hd,%hd\n", base,irq,dmalo,dmahi);
  }

  if (!SB_InitBuffers(*sndbufsize))
  {
      fprintf(stderr, "Can't set sound buffers to %d!\n", *sndbufsize);
      return(0);
  }

  have_sound = SB_Init (base, irq, dmalo, dmahi);
  if (!have_sound)
  {
    printf ("Cannot Initialize Soundblaster!\n");
    return (0);
  }

  if (((*dspbits) == 16) && (SB_SetBits (16)))
    SB_SetSigned (1);
  else
    *dspbits = 8;

  if (*stereo_sound) {
     *stereo_sound = SB_SetChannels(2);
     if (!(*stereo_sound))
	 fprintf(stderr, "Warning, stereo not suporeted!\n");
  }

  if (!SB_SetRate(*dsprate))
  {
      fprintf(stderr, "SoundBlaster%s can't use sample rate %d!\n",
	       SB_Type==SB_Type_StdSB ? "" :
	      (SB_Type==SB_Type_SbPro ? " PRO" : " 16"),
	      *dsprate);
      return(0);
  }

  printf ("SoundBlaster%s found.\n",
	   SB_Type==SB_Type_StdSB ? "" :
	  (SB_Type==SB_Type_SbPro ? " PRO" : " 16"));

  SND_DirectWrite = SB_DirectWrite;
  direct_buffers[0] = dma_buf[0].Data;
  direct_buffers[1] = dma_buf[1].Data;

  atexit (SB_Done);

  return 1;
}

/* that's it... */
