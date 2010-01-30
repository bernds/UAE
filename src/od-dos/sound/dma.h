 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS DMA interface.
  *
  * (c) 1996 Peter Remmers
  */

/* Transfer modes */
#define DMA_MODE_WRITE     0x04
#define DMA_MODE_READ      0x08
/* autoinitialization */
#define DMA_MODE_AUTOINIT  0x10
/* address increment/decrement */
#define DMA_MODE_INCREMENT 0x00
#define DMA_MODE_DECREMENT 0x20
/* DMA Modes */
#define DMA_MODE_DEMAND    0x00
#define DMA_MODE_SINGLE    0x40
#define DMA_MODE_BLOCK     0x80
#define DMA_MODE_CASCADE   0xC0

/* convenience shortcuts */
#define DMA_READ  ( DMA_MODE_READ  | DMA_MODE_INCREMENT | DMA_MODE_SINGLE )
#define DMA_WRITE ( DMA_MODE_WRITE | DMA_MODE_INCREMENT | DMA_MODE_SINGLE )
#define DMA_READ_AUTOINIT  ( DMA_READ  | DMA_MODE_AUTOINIT )
#define DMA_WRITE_AUTOINIT ( DMA_WRITE | DMA_MODE_AUTOINIT )

extern unsigned long DMA_AllocDMABuf (_go32_dpmi_seginfo *info);
extern void  DMA_Mask         (unsigned char channel);
extern void  DMA_UnMask       (unsigned char channel);
extern void  DMA_InitTransfer (unsigned char channel, unsigned char Mode,
			       unsigned long PhysAddr, unsigned short Count);
