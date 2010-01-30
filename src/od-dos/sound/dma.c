 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS DMA interface.
  *
  * (c) 1996 Peter Remmers
  */

#include <stdlib.h>
#include <pc.h>
#include <dpmi.h>

#include "sound/dma.h"

/* some useful macros */
#define LOBYTE(x) ((unsigned char)(((unsigned short)(x)) &  0xFF))
#define HIBYTE(x) ((unsigned char)(((unsigned short)(x)) >> 8))
#define LOWORD(x) ((unsigned short)(((unsigned long)(x)) & 0xFFFF))
#define HIWORD(x) ((unsigned short)(((unsigned long)(x)) >> 16))

const unsigned short DMA_Address [8] = {0x00,0x02,0x04,0x06,0xC0,0xC4,0xC8,0xCC};
const unsigned short DMA_Count   [8] = {0x01,0x03,0x05,0x07,0xC2,0xC6,0xCA,0xCE};

const unsigned short DMA_PageLo[8]= { 0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A};
const unsigned short DMA_PageHi[8]= {0x487,0x483,0x481,0x482,0x48F,0x48B,0x489,0x48A};

#define DMA_Status(ch)        ((ch)<4 ? 0x08 : 0xD0)  /* Read  */
#define DMA_Command(ch)       ((ch)<4 ? 0x08 : 0xD0)  /* Write */
#define DMA_Request(ch)       ((ch)<4 ? 0x09 : 0xD2)
#define DMA_MaskSingle(ch)    ((ch)<4 ? 0x0A : 0xD4)
#define DMA_Mode(ch)          ((ch)<4 ? 0x0B : 0xD6)
#define DMA_ClearFF(ch)       ((ch)<4 ? 0x0C : 0xD8)
#define DMA_Latch(ch)         ((ch)<4 ? 0x0D : 0xDA)  /* Read  */
#define DMA_MasterClear(ch)   ((ch)<4 ? 0x0D : 0xDA)  /* Write */
#define DMA_ClearMask(ch)     ((ch)<4 ? 0x0E : 0xDC)
#define DMA_MaskAll(ch)       ((ch)<4 ? 0x0F : 0xDE)


/* allocate DOS memory that does not cross a physical 64k boundary */
unsigned long DMA_AllocDMABuf  (_go32_dpmi_seginfo *info)
{
  unsigned long Phys;
  unsigned long bytes;

  /* first allocate requested size */
  if (_go32_dpmi_allocate_dos_memory (info)) return (NULL);

  Phys  = (info->rm_segment) << 4;
  bytes = (info->size)       << 4;
  
  /* if memory does not cross boundary we are lucky and that's it */
  if (HIWORD(Phys + bytes) == HIWORD (Phys)) return (Phys);

  /* if not, free memory */
  _go32_dpmi_free_dos_memory (info);

  /* and allocate twice the requested size */
  bytes      <<= 1;
  info->size <<= 1;
  if (_go32_dpmi_allocate_dos_memory (info)) return (NULL);

  /* and take the half that does not cross a boundary */
  Phys = (info->rm_segment) << 4;
  if (HIWORD(Phys + bytes) == HIWORD (Phys)) return (Phys);
  
  return (Phys + (bytes>>1));
}

void DMA_Mask (unsigned char channel)
{
  outportb(DMA_MaskSingle(channel), 4 + (channel&3));
}

void DMA_UnMask (unsigned char channel)
{
  outportb(DMA_MaskSingle(channel), channel&3);
}

void DMA_InitTransfer (unsigned char channel, unsigned char Mode,
                       unsigned long PhysAddr, unsigned short Count)
{
  unsigned short Page;
  unsigned short Addr;

  if (channel & 4) Count >>= 1;
  Count--;
  
  Page = HIWORD(PhysAddr);
  if (channel & 4) PhysAddr >>= 1;
  Addr = LOWORD (PhysAddr);

  outportb (DMA_MaskSingle(channel), (channel&3) + 0x04);
  outportb (DMA_Mode      (channel), (channel&3) | (Mode&0xFC));
  outportb (DMA_ClearFF   (channel), 0);
  outportb (DMA_Count     [channel], LOBYTE(Count));
  outportb (DMA_Count     [channel], HIBYTE(Count));
  outportb (DMA_Address   [channel], LOBYTE(Addr));
  outportb (DMA_Address   [channel], HIBYTE(Addr));
  outportb (DMA_PageLo    [channel], LOBYTE(Page));
  outportb (DMA_PageHi    [channel], HIBYTE(Page));
  outportb (DMA_MaskSingle(channel), channel&3);
}
