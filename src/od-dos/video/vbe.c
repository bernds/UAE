 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS VESA BIOS / VGA / Mode X graphics interface.
  *
  * (c) 1997 Gustavo Goedert
  */

#include "sysconfig.h"
#include "sysdeps.h"

#include <stdlib.h>
#include <dos.h>
#include <go32.h>
#include <dpmi.h>
#include <sys/farptr.h>

#include "vbe.h"

#define ALIGN(x) ((((int)(x)>>12)<<12)+4096)
#define PACKED __attribute__ ((packed))

/* externs in vbe.h */
int NumberOfModes = 0;
T_ModeInfo *ModeList = NULL;
T_ModeInfo CurrentMode;
int CanMapBuffer = 0;
int CurrentBankNumber = 0;
/* end from vbe.h */

enum VBE_MemoryModels {
    TextMode,    CGA_Graphics, HerculesGraphics, Planar,
    PackedPixel, ModeX,        DirectColor,      YUV
};

typedef struct {
    char VbeSignature[4] PACKED; // = VBE2
    short VbeVersion PACKED;
    char *OemStringPtr PACKED;
    char Capabilities[4] PACKED;
    unsigned int VideoModePtr PACKED;
    short TotalMemory PACKED;
    short OemSoftwareRev PACKED;
    char *OemVendorNamePtr PACKED;
    char *OemProductNamePtr PACKED;
    char *OemProductRevPtr PACKED;
    char Reserved[222] PACKED;
    char OemData[256] PACKED;
} T_VBE_VbeInfoBlock;

typedef struct {
    // Mandatory information for all VBE revisions
    short ModeAttributes PACKED;
    char WinAAttributes PACKED;
    char WinBAttributes PACKED;
    short WinGranularity PACKED;
    unsigned short WinSize PACKED;
    unsigned short WinASegment PACKED;
    unsigned short WinBSegment PACKED;
    unsigned int WinFuncPtr PACKED;
    short BytesPerScanLine PACKED;
    // Mandatory information for VBE 1.2 and above
    short XResolution PACKED;
    short YResolution PACKED;
    char XCharSize PACKED;
    char YCharSize PACKED;
    char NumberOfPlanes PACKED;
    char BitsPerPixel PACKED;
    char NumberOfBanks PACKED;
    char MemoryModel PACKED;
    char BankSize PACKED;
    char NumberOfImagePages PACKED;
    char ReservedA PACKED; // = 1
    // Direct Color fields (required for direct/6 and YUV/7 memory models)
    char RedMaskSize PACKED;
    char RedFieldPosition PACKED;
    char GreenMaskSize PACKED;
    char GreenFieldPosition PACKED;
    char BlueMaskSize PACKED;
    char BlueFieldPosition PACKED;
    char RsvdMaskSize PACKED;
    char RsvdFieldPosition PACKED;
    char DirectColorModeInfo PACKED;
    // Mandatory information for VBE 2.0 and above
    unsigned int PhysBasePtr PACKED;
    unsigned int OffScreenMemOffset PACKED;
    short OffScreenMemSize PACKED;
    char ReservedB[206] PACKED;
} T_VBE_ModeInfoBlock;

typedef struct {
    short SetWindowOffset PACKED;
    short SetDisplayStartOffset PACKED;
    short SetPrimaryPaletteDataOffset PACKED;
    short PortsMemoryOffset PACKED;
} T_VBE_ProtectedModeInterface;

typedef enum {TVBE, TVGA, TModeX} T_ControlType;

typedef struct {
    T_ModeType ModeType;
    int OriginalModeNumber;
    int ModeWidth, ModeHeight;
    int BitsPerPixel, BitsPerColor;
    int RedSize, RedPosition;
    int GreenSize, GreenPosition;
    int BlueSize, BluePosition;
} T_VBE_Table;

typedef struct {
    T_ControlType ControlType;
    int OriginalModeNumber;
    int WriteWindow;
    int FastWinCalc;
    int GranularityFix;
    unsigned int WinMask1, WinMask2;
    unsigned int WinFuncPtrSegment, WinFuncPtrOffset;
    unsigned int WindowPosition, LinearPosition;
} T_ModeInternalInfo;

T_ModeInfo VGA_InfoTable[7] = {
    {T16, 320, 200, 8, 4, 0, 0, 0, 0, 0, 0, 40, 40, 0, 0, 0x10000, 0xa0000, 0, 0, 0, 0, NULL, NULL, NULL},
    {T16, 640, 200, 8, 4, 0, 0, 0, 0, 0, 0, 80, 80, 0, 0, 0x10000, 0xa0000, 0, 0, 0, 0, NULL, NULL, NULL},
    {T16, 640, 350, 8, 4, 0, 0, 0, 0, 0, 0, 80, 80, 0, 0, 0x10000, 0xa0000, 0, 0, 0, 0, NULL, NULL, NULL},
    {T16, 640, 480, 8, 4, 0, 0, 0, 0, 0, 0, 80, 80, 0, 0, 0x10000, 0xa0000, 0, 0, 0, 0, NULL, NULL, NULL},
    {T256, 320, 200, 8, 8, 0, 0, 0, 0, 0, 0, 320, 320, 1, 0x10000, 0x12000, 0xa0000, 1, 0x10000, 0x12000, 0xa0000, NULL, NULL, NULL},
    {T256, 320, 240, 8, 8, 0, 0, 0, 0, 0, 0, 80, 80, 0, 0, 0x10000, 0xa0000, 0, 0, 0, 0, NULL, NULL, NULL},
    {T256, 320, 400, 8, 8, 0, 0, 0, 0, 0, 0, 80, 80, 0, 0, 0x10000, 0xa0000, 0, 0, 0, 0, NULL, NULL, NULL}
/*  {T256, 320, 480
    {T256, 360, 200
    {T256, 360, 400
    {T256, 360, 480*/
};

T_ModeInternalInfo VGA_InternalInfoTable[7] = {
    {TVGA, 0x0d, 0, 0, 0, 0, 0, 0, 0, 0xa0000, 0},
    {TVGA, 0x0e, 0, 0, 0, 0, 0, 0, 0, 0xa0000, 0},
    {TVGA, 0x10, 0, 0, 0, 0, 0, 0, 0, 0xa0000, 0},
    {TVGA, 0x12, 0, 0, 0, 0, 0, 0, 0, 0xa0000, 0},
    {TVGA, 0x13, 0, 0, 0, 0, 0, 0, 0, 0xa0000, 0xa0000},
    {TModeX, 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {TModeX, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0}
/*  {TModeX, 0x03
    {TModeX, 0x04
    {TModeX, 0x05
    {TModeX, 0x06*/
};

T_VBE_Table VBE_Table[24] = {
    {T16, 0x6a, 800, 600, 8, 4, 0, 0, 0, 0, 0, 0},
    {T256, 0x100, 640, 400, 8, 8, 0, 0, 0, 0, 0, 0},
    {T256, 0x101, 640, 480, 8, 8, 0, 0, 0, 0, 0, 0},
    {T16, 0x102, 800, 600, 8, 4, 0, 0, 0, 0, 0, 0},
    {T256, 0x103, 800, 600, 8, 8, 0, 0, 0, 0, 0, 0},
    {T16, 0x104, 1024, 768, 8, 4, 0, 0, 0, 0, 0, 0},
    {T256, 0x105, 1024, 768, 8, 8, 0, 0, 0, 0, 0, 0},
    {T16, 0x106, 1280, 1024, 8, 4, 0, 0, 0, 0, 0, 0},
    {T256, 0x107, 1280, 1024, 8, 8, 0, 0, 0, 0, 0, 0},
    {T32K, 0x10d, 320, 200, 16, 15, 5, 10, 5, 5, 5, 0},
    {T64K, 0x10e, 320, 200, 16, 16, 5, 11, 6, 5, 5, 0},
    {T16M, 0x10f, 320, 200, 24, 24, 8, 16, 8, 8, 8, 0},
    {T32K, 0x110, 640, 480, 16, 15, 5, 10, 5, 5, 5, 0},
    {T64K, 0x111, 640, 480, 16, 16, 5, 11, 6, 5, 5, 0},
    {T16M, 0x112, 640, 480, 24, 24, 8, 16, 8, 8, 8, 0},
    {T32K, 0x113, 800, 600, 16, 15, 5, 10, 5, 5, 5, 0},
    {T64K, 0x114, 800, 600, 16, 16, 5, 11, 6, 5, 5, 0},
    {T16M, 0x115, 800, 600, 24, 24, 8, 16, 8, 8, 8, 0},
    {T32K, 0x116, 1024, 768, 16, 15, 5, 10, 5, 5, 5, 0},
    {T64K, 0x117, 1024, 768, 16, 16, 5, 11, 6, 5, 5, 0},
    {T16M, 0x118, 1024, 768, 24, 24, 8, 16, 8, 8, 8, 0},
    {T32K, 0x119, 1280, 1024, 16, 15, 5, 10, 5, 5, 5, 0},
    {T64K, 0x11a, 1280, 1024, 16, 16, 5, 11, 6, 5, 5, 0},
    {T16M, 0x11b, 1280, 1024, 24, 24, 8, 16, 8, 8, 8, 0}
};

int HasVbe = 0;
int HasProtectedModeInterface = 0;
int RestoreOldVGAMode = 0, OldVGAMode;

T_VBE_VbeInfoBlock VBE_VbeInfoBlock;
T_VBE_ModeInfoBlock VBE_ModeInfoBlock;
T_VBE_ProtectedModeInterface *VBE_ProtectedModeInterface;
short VBE_ModeList[512];
unsigned int WinFuncPtrPM;
short *PortsMemory;

T_ModeInternalInfo *InternalModeList = NULL;
T_ModeInternalInfo CurrentInternalMode;
char *LinearVgaBuffer, *AlignedLinearVgaBuffer;

unsigned char PaletteData[768];
_go32_dpmi_seginfo PaletteMem;

int VBE_GetVbeInfoBlock(void);
int VBE_GetModeInfoBlock(int mode);
int VBE_GetProtectedModeInterface(void);
void VBE_ChangeBankRealInterrupt(int BankNumber);
void VBE_ChangeBankRealMode(int BankNumber);
void VBE_ChangeBankProtectMode(int BankNumber);
void NULL_ChangeBank(int BankNumber);
void PutLineLinear(char *addr, int target_y);
void PutLineBanked(char *addr, int target_y);
void PutLineVGA16(char *addr, int target_y);
void PutLineVGAX(char *addr, int target_y);
void InsertMode(T_ModeInfo *ModeInfo, T_ModeInternalInfo *ModeInternalInfo);
void SetModeX(int ModeNumber);
void MapVideoMemory(void);

int VBE_GetVbeInfoBlock(void) {
    _go32_dpmi_registers IntRegs;

    strncpy(VBE_VbeInfoBlock.VbeSignature, "VBE2", 4);
    dosmemput(&VBE_VbeInfoBlock, sizeof(VBE_VbeInfoBlock), __tb);
    IntRegs.x.ax = 0x4F00;
    IntRegs.x.di = __tb & 0x0F;
    IntRegs.x.es = (__tb >> 4) & 0xFFFF;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    dosmemget(__tb, sizeof(VBE_VbeInfoBlock), &VBE_VbeInfoBlock);
    return((IntRegs.x.ax == 0x004F) && (VBE_VbeInfoBlock.VbeVersion >= 0x0100));
}

int VBE_GetModeInfoBlock(int mode) {
    _go32_dpmi_registers IntRegs;

    VBE_ModeInfoBlock.ReservedA = 1;
    IntRegs.x.ax = 0x4F01;
    IntRegs.x.cx = mode;
    IntRegs.x.di = __tb & 0x0F;
    IntRegs.x.es = (__tb >> 4) & 0xFFFF;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    dosmemget(__tb, sizeof(VBE_ModeInfoBlock), &VBE_ModeInfoBlock);
    return(IntRegs.x.ax == 0x004F);
}

int VBE_GetProtectedModeInterface(void) {
    _go32_dpmi_registers IntRegs;

    IntRegs.x.ax = 0x4F0A;
    IntRegs.x.bx = 0x0000;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    VBE_ProtectedModeInterface = (T_VBE_ProtectedModeInterface *) malloc(IntRegs.x.cx);
    if (VBE_ProtectedModeInterface == NULL)
	return(0);
    dosmemget(IntRegs.x.es * 16 + IntRegs.x.di, IntRegs.x.cx, VBE_ProtectedModeInterface);
    WinFuncPtrPM = (unsigned int) VBE_ProtectedModeInterface + VBE_ProtectedModeInterface->SetWindowOffset;
    PortsMemory = (short *) VBE_ProtectedModeInterface + VBE_ProtectedModeInterface->PortsMemoryOffset;
    return(IntRegs.x.ax == 0x004F);
}

void VBE_ChangeBankRealInterrupt(int BankNumber) {
    _go32_dpmi_registers IntRegs;

    IntRegs.x.ax = 0x4F05;
    IntRegs.x.bx = CurrentInternalMode.WriteWindow;
    IntRegs.x.dx = CurrentInternalMode.GranularityFix ? BankNumber * CurrentInternalMode.GranularityFix : BankNumber;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    CurrentBankNumber = BankNumber;
}

void VBE_ChangeBankRealMode(int BankNumber) {
    _go32_dpmi_registers IntRegs;

    IntRegs.x.ax = 0x4F05;
    IntRegs.x.bx = CurrentInternalMode.WriteWindow;
    IntRegs.x.dx = CurrentInternalMode.GranularityFix ? BankNumber * CurrentInternalMode.GranularityFix : BankNumber;
    IntRegs.x.cs = CurrentInternalMode.WinFuncPtrSegment;
    IntRegs.x.ip = CurrentInternalMode.WinFuncPtrOffset;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_fcall(&IntRegs);
    CurrentBankNumber = BankNumber;
}

void VBE_ChangeBankProtectMode(int BankNumber) {
    int BNumber = CurrentInternalMode.GranularityFix ? BankNumber * CurrentInternalMode.GranularityFix : BankNumber;

    asm __volatile__ ("
	 push %%es
	 movw %0, %%es
	 call %1
	 pop %%es
	 "
	 :
	 : "g" (_dos_ds), "r" (WinFuncPtrPM), "a" (0x4F05), "b" (CurrentInternalMode.WriteWindow), "d" (BNumber)
	 : "%ax", "%bx", "%dx", "cc", "memory"
    );
    CurrentBankNumber = BankNumber;
    _farsetsel(_dos_ds);
}

void NULL_ChangeBank(int BankNumber) {
    CurrentBankNumber = BankNumber;
}

void PutLineLinear(char *addr, int target_y) {
    unsigned int ptr;

    ptr = CurrentMode.LinearAddress + target_y * CurrentMode.LogicalLineLength;
    asm __volatile__ ("
	 push %%es
	 cld
	 movw %0, %%es
	 shrl $2, %%ecx
	 rep
	 movsl
	 pop %%es
	 "
	 :
	 : "g" (_dos_ds), "S" (addr), "D" (ptr), "c" (CurrentMode.LineLength)
	 : "%esi", "%edi", "%ecx", "cc"
    );
}

void PutLineBanked(char *addr, int target_y) {
    unsigned int offs;
    int page, pos, left;
    char *ptr;

    offs = target_y * CurrentMode.LogicalLineLength;
    if (CurrentInternalMode.FastWinCalc) {
	page = offs >> CurrentInternalMode.WinMask1;
	pos = offs & CurrentInternalMode.WinMask2;
    } else {
	page = offs / CurrentMode.WindowSize;
	pos = offs % CurrentMode.WindowSize;
    }
    left = CurrentMode.WindowSize - pos;
    ptr = (char *) CurrentMode.WindowAddress + pos;
    if (page != CurrentBankNumber)
	CurrentMode.ChangeBank(page);
    if (left >= CurrentMode.LineLength)
	asm __volatile__ ("
	     push %%es
	     cld
	     movw %0, %%es
	     shrl $2, %%ecx
	     rep
	     movsl
	     pop %%es
	     "
	     :
	     : "g" (_dos_ds), "S" (addr), "D" (ptr), "c" (CurrentMode.LineLength)
	     : "%esi", "%edi", "%ecx", "cc"
	);
    else {
	asm __volatile__ ("
	     push %%es
	     cld
	     movw %0, %%es
	     shrl $2, %%ecx
	     rep
	     movsl
	     pop %%es
	     "
	     :
	     : "g" (_dos_ds), "S" (addr), "D" (ptr), "c" (left)
	     : "%esi", "%edi", "%ecx", "cc"
	);
	CurrentMode.ChangeBank(page+1);
	asm __volatile__ ("
	     push %%es
	     cld
	     movw %0, %%es
	     addl %1, %%esi
	     subl %1, %%ecx
	     shrl $2, %%ecx
	     rep
	     movsl
	     pop %%es
	     "
	     :
	     : "g" (_dos_ds), "g" (left), "S" (addr), "D" (CurrentMode.WindowAddress), "c" (CurrentMode.LineLength)
	     : "%esi", "%edi", "%ecx", "cc"
	);
    }
}

void PutLineVGA16(char *addr, int target_y) {
    unsigned int start, ptr;
    int i;
    UBYTE c;

#if 0
    ptr = CurrentMode.WindowAddress + target_y * CurrentMode.LogicalLineLength;
    outportw(0x3ce, 0x205);
    outportw(0x3ce, 3);
    for (i=0; i<CurrentMode.ModeWidth; i++) {
	outportw(0x3ce, 1<<(((i&7)^7)+8)|8);
	_farnspeekb(ptr+(i>>3));
	_farnspokeb(ptr+(i>>3), addr[i]);
    }
#else
    start = CurrentMode.WindowAddress + target_y * CurrentMode.LogicalLineLength;
    ptr = start;
    outportw(0x3c4, 0x802);
    for (i=0; i<CurrentMode.ModeWidth; i+=8, ptr++) {
	c = ((addr[i  ]<<4)&128)|((addr[i+1]<<3)&64)|((addr[i+2]<<2)&32)|((addr[i+3]<<1)&16)|
	    ((addr[i+4]   )&8  )|((addr[i+5]>>1)&4 )|((addr[i+6]>>2)&2 )|((addr[i+7]>>3)&1);
	_farnspokeb(ptr, c);
    }
    ptr = start;
    outportw(0x3c4, 0x402);
    for (i=0; i<CurrentMode.ModeWidth; i+=8, ptr++) {
	c = ((addr[i  ]<<5)&128)|((addr[i+1]<<4)&64)|((addr[i+2]<<3)&32)|((addr[i+3]<<2)&16)|
	    ((addr[i+4]<<1)&8  )|((addr[i+5]   )&4 )|((addr[i+6]>>1)&2 )|((addr[i+7]>>2)&1 );
	_farnspokeb(ptr, c);
    }
    ptr = start;
    outportw(0x3c4, 0x202);
    for (i=0; i<CurrentMode.ModeWidth; i+=8, ptr++) {
	c = ((addr[i  ]<<6)&128)|((addr[i+1]<<5)&64)|((addr[i+2]<<4)&32)|((addr[i+3]<<3)&16)|
	    ((addr[i+4]<<2)&8  )|((addr[i+5]<<1)&4 )|((addr[i+6]   )&2 )|((addr[i+7]>>1)&1 );
	_farnspokeb(ptr, c);
    }
    ptr = start;
    outportw(0x3c4, 0x102);
    for (i=0; i<CurrentMode.ModeWidth; i+=8, ptr++) {
	c = ((addr[i  ]<<7)&128)|((addr[i+1]<<6)&64)|((addr[i+2]<<5)&32)|((addr[i+3]<<4)&16)|
	    ((addr[i+4]<<3)&8  )|((addr[i+5]<<2)&4 )|((addr[i+6]<<1)&2 )|((addr[i+7]   )&1 );
	_farnspokeb(ptr, c);
    }
#endif
}

void PutLineVGAX(char *addr, int target_y) {
    unsigned int start, ptr;
    int i;
    UBYTE c;

    start = CurrentMode.WindowAddress + target_y * CurrentMode.LogicalLineLength;
    ptr = start;
    outportw(0x3c4, 0x102);
    for (i=0; i<CurrentMode.ModeWidth; i+=4, ptr++)
	_farnspokeb(ptr, addr[i]);
    ptr = start;
    outportw(0x3c4, 0x202);
    for (i=1; i<CurrentMode.ModeWidth; i+=4, ptr++)
	_farnspokeb(ptr, addr[i]);
    ptr = start;
    outportw(0x3c4, 0x402);
    for (i=2; i<CurrentMode.ModeWidth; i+=4, ptr++)
	_farnspokeb(ptr, addr[i]);
    ptr = start;
    outportw(0x3c4, 0x802);
    for (i=3; i<CurrentMode.ModeWidth; i+=4, ptr++)
	_farnspokeb(ptr, addr[i]);
}

void SetPalette(int Color, int Red, int Green, int Blue) {
    _go32_dpmi_registers IntRegs;

    if (CurrentMode.ModeType == T16) {
	IntRegs.h.ah = 0x10;
	IntRegs.h.al = 0x00;
	IntRegs.h.bh = Color;
	IntRegs.h.bl = Color;
	IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
	_go32_dpmi_simulate_int(0x10, &IntRegs);
    }
    IntRegs.h.ah = 0x10;
    IntRegs.h.al = 0x10;
    IntRegs.x.bx = Color;
    IntRegs.h.ch = Green;
    IntRegs.h.cl = Blue;
    IntRegs.h.dh = Red;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    DelayedSetPalette(Color, Red, Green, Blue);
}

void DelayedSetPalette(int Color, int Red, int Green, int Blue) {
    PaletteData[Color * 3 + 0] = Red;
    PaletteData[Color * 3 + 1] = Green;
    PaletteData[Color * 3 + 2] = Blue;
}

void ViewPalette(int Color, int *Red, int *Green, int *Blue) {
    *Red = PaletteData[Color * 3 + 0] << 2;
    *Green = PaletteData[Color * 3 + 1] << 2;
    *Blue = PaletteData[Color * 3 + 2] << 2;
}

void LoadPalette(void) {
    _go32_dpmi_registers IntRegs;
    int i;

    dosmemput(PaletteData, 768, PaletteMem.rm_segment*16);
    if (CurrentMode.ModeType == T16) {
	for(i=0; i<16; i++) {
	    IntRegs.h.ah = 0x10;
	    IntRegs.h.al = 0x00;
	    IntRegs.h.bh = i;
	    IntRegs.h.bl = i;
	    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
	    _go32_dpmi_simulate_int(0x10, &IntRegs);
	}
    }
    IntRegs.h.ah = 0x10;
    IntRegs.h.al = 0x12;
    IntRegs.x.bx = 0;
    if (CurrentMode.ModeType == T16)
	IntRegs.x.cx = 16;
    else
	IntRegs.x.cx = 256;
    IntRegs.x.es = PaletteMem.rm_segment;
    IntRegs.x.dx = 0x0000;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
}

int InitGfxLib(void) {
    static int GfxInicilized = 0;
    _go32_dpmi_registers IntRegs;
    __dpmi_meminfo MemoryInfo;
    unsigned int DosMemPos;
    int DiscartMode;
    int i, j, TryList=1, CurrentList=0;
    short ModeNumber;
    T_ModeInfo ModeInfo;
    T_ModeInternalInfo ModeInternalInfo;

    if (GfxInicilized)
	return(1);

    /* Save Old VGA Mode */
    IntRegs.x.ax = 0x1C;
    IntRegs.h.ah = 0x0F;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    OldVGAMode = IntRegs.h.al;

    /* Allocate Palette Memory */
    PaletteMem.size = 48;
    if (_go32_dpmi_allocate_dos_memory(&PaletteMem)) {
	printf ("Can't allocate real mode memory for palette data.\n");
	return(0);
    }

    /* Check for VBE */
    HasVbe = VBE_GetVbeInfoBlock();

    /* Build ModeInfo Tables */
    if (HasVbe) {
	printf ("VESA BIOS Extension version %d.%d found.\n", VBE_VbeInfoBlock.VbeVersion >> 8, VBE_VbeInfoBlock.VbeVersion & 0xFF);
	if (VBE_VbeInfoBlock.VbeVersion >= 0x0200)
	    HasProtectedModeInterface = VBE_GetProtectedModeInterface();
	DosMemPos = (VBE_VbeInfoBlock.VideoModePtr >> 16) * 16 +
		    (VBE_VbeInfoBlock.VideoModePtr & 0xffff);
	dosmemget(DosMemPos, 1024, VBE_ModeList);
	VBE_ModeList[511] = -1;
	i = 0;
	while(1) {
	    if (TryList) {
		ModeNumber = VBE_Table[CurrentList].OriginalModeNumber;
		CurrentList++;
		if (CurrentList == 25)
		    TryList = 0;
	    } else {
		ModeNumber = VBE_ModeList[i];
		if (ModeNumber == -1)
		    break;
		i++;
		if ((ModeNumber == 0x6a) || ((ModeNumber >= 0x100) && (ModeNumber <= 0x107)) || ((ModeNumber >= 0x10d) && (ModeNumber <= 0x11b)))
		    continue;
	    }
	    if ((ModeNumber != (short)0x81ff) && VBE_GetModeInfoBlock(ModeNumber)) {
		DiscartMode = 0;
		if (!(VBE_ModeInfoBlock.ModeAttributes & 0x1))
		    DiscartMode = 1;
		ModeInternalInfo.OriginalModeNumber = ModeNumber;
		ModeInternalInfo.ControlType = TVBE;
		ModeInfo.RedSize = 0;
		ModeInfo.RedPosition = 0;
		ModeInfo.GreenSize = 0;
		ModeInfo.GreenPosition = 0;
		ModeInfo.BlueSize = 0;
		ModeInfo.BluePosition = 0;
		if (VBE_VbeInfoBlock.VbeVersion < 0x0102) {
		    if (VBE_ModeInfoBlock.ModeAttributes & 0x2) {
			ModeInfo.ModeWidth = VBE_ModeInfoBlock.XResolution;
			ModeInfo.ModeHeight = VBE_ModeInfoBlock.YResolution;
			ModeInfo.BitsPerPixel = VBE_ModeInfoBlock.BitsPerPixel;
			if (ModeInfo.BitsPerPixel == 15)
			    ModeInfo.BitsPerPixel = 16;
			if (ModeInfo.BitsPerPixel == 4) {
			    ModeInfo.ModeType = T16;
			    ModeInfo.BitsPerPixel = 8;
			    ModeInfo.BitsPerColor = 4;
			    if (VBE_ModeInfoBlock.MemoryModel != Planar)
				DiscartMode = 1;
			} else if (ModeInfo.BitsPerPixel == 8) {
			    ModeInfo.ModeType = T256;
			    ModeInfo.BitsPerColor = 8;
			    if (VBE_ModeInfoBlock.MemoryModel != PackedPixel)
				DiscartMode = 1;
			} else if (ModeInfo.BitsPerPixel == 16) {
			    ModeInfo.ModeType = T32K;
			    ModeInfo.BitsPerColor = 15;
			    ModeInfo.RedSize = 5;
			    ModeInfo.RedPosition = 10;
			    ModeInfo.GreenSize = 5;
			    ModeInfo.GreenPosition = 5;
			    ModeInfo.BlueSize = 5;
			    ModeInfo.BluePosition = 0;
			    if (VBE_ModeInfoBlock.MemoryModel != PackedPixel)
				DiscartMode = 1;
			} else if ((ModeInfo.BitsPerPixel == 24) || (ModeInfo.BitsPerPixel == 32)) {
			    ModeInfo.ModeType = T16M;
			    ModeInfo.BitsPerColor = 24;
			    ModeInfo.RedSize = 8;
			    ModeInfo.RedPosition = 16;
			    ModeInfo.GreenSize = 8;
			    ModeInfo.GreenPosition = 8;
			    ModeInfo.BlueSize = 8;
			    ModeInfo.BluePosition = 0;
			    if (VBE_ModeInfoBlock.MemoryModel != PackedPixel)
				DiscartMode = 1;
			} else
			    DiscartMode = 1;
		    } else {
			for (j = 0; j < 24; j++) {
			    if (VBE_Table[j].OriginalModeNumber == ModeInternalInfo.OriginalModeNumber) {
				ModeInfo.ModeType = VBE_Table[j].ModeType;
				ModeInfo.ModeWidth = VBE_Table[j].ModeWidth;
				ModeInfo.ModeHeight = VBE_Table[j].ModeHeight;
				ModeInfo.BitsPerPixel = VBE_Table[j].BitsPerPixel;
				ModeInfo.BitsPerColor = VBE_Table[j].BitsPerColor;
				ModeInfo.RedSize = VBE_Table[j].RedSize;
				ModeInfo.RedPosition = VBE_Table[j].RedPosition;
				ModeInfo.GreenSize = VBE_Table[j].GreenSize;
				ModeInfo.GreenPosition = VBE_Table[j].GreenPosition;
				ModeInfo.BlueSize = VBE_Table[j].BlueSize;
				ModeInfo.BluePosition = VBE_Table[j].BluePosition;
				break;
			    }
			}
			if (j == 24)
			    DiscartMode = 1;
		    }
		} else {
		    ModeInfo.ModeWidth = VBE_ModeInfoBlock.XResolution;
		    ModeInfo.ModeHeight = VBE_ModeInfoBlock.YResolution;
		    ModeInfo.BitsPerPixel = VBE_ModeInfoBlock.BitsPerPixel;
		    if (ModeInfo.BitsPerPixel == 15)
			ModeInfo.BitsPerPixel = 16;
		    if (ModeInfo.BitsPerPixel == 4) {
			ModeInfo.ModeType = T16;
			ModeInfo.BitsPerPixel = 8;
			ModeInfo.BitsPerColor = 4;
			if (VBE_ModeInfoBlock.MemoryModel != Planar)
			    DiscartMode = 1;
		    } else if (ModeInfo.BitsPerPixel == 8) {
			ModeInfo.ModeType = T256;
			ModeInfo.BitsPerColor = 8;
			if (VBE_ModeInfoBlock.MemoryModel != PackedPixel)
			    DiscartMode = 1;
		    } else {
			ModeInfo.RedSize = VBE_ModeInfoBlock.RedMaskSize;
			ModeInfo.RedPosition = VBE_ModeInfoBlock.RedFieldPosition;
			ModeInfo.GreenSize = VBE_ModeInfoBlock.GreenMaskSize;
			ModeInfo.GreenPosition = VBE_ModeInfoBlock.GreenFieldPosition;
			ModeInfo.BlueSize = VBE_ModeInfoBlock.BlueMaskSize;
			ModeInfo.BluePosition = VBE_ModeInfoBlock.BlueFieldPosition;
			ModeInfo.BitsPerColor = ModeInfo.RedSize + ModeInfo.GreenSize + ModeInfo.BlueSize;
			if (ModeInfo.BitsPerColor == 15) {
			    ModeInfo.ModeType = T32K;
			    if (ModeInfo.BitsPerPixel != 16)
				DiscartMode = 1;
			} else if (ModeInfo.BitsPerColor == 16) {
			    ModeInfo.ModeType = T64K;
			    if (ModeInfo.BitsPerPixel != 16)
				DiscartMode = 1;
			} else if (ModeInfo.BitsPerColor == 24) {
			    ModeInfo.ModeType = T16M;
			    if ((ModeInfo.BitsPerPixel != 24) && (ModeInfo.BitsPerPixel != 32))
				DiscartMode = 1;
			} else
			    DiscartMode = 1;
			if (VBE_ModeInfoBlock.MemoryModel != DirectColor)
			    DiscartMode = 1;
		    }
		}
		ModeInfo.LineLength = (ModeInfo.BitsPerPixel >> 3) * ModeInfo.ModeWidth;
		ModeInfo.LogicalLineLength = VBE_ModeInfoBlock.BytesPerScanLine;
		ModeInfo.HasWindow = 0;
		ModeInfo.HasLinear = 0;
		if ((VBE_ModeInfoBlock.MemoryModel == PackedPixel) || (VBE_ModeInfoBlock.MemoryModel == DirectColor)) {
		    if (!(VBE_ModeInfoBlock.ModeAttributes & 0x40)) {
			ModeInfo.HasWindow = 1;
			if (VBE_ModeInfoBlock.WinFuncPtr != 0) {
			    ModeInternalInfo.WinFuncPtrSegment = VBE_ModeInfoBlock.WinFuncPtr >> 16;
			    ModeInternalInfo.WinFuncPtrOffset = VBE_ModeInfoBlock.WinFuncPtr & 0xffff;
			} else {
			    ModeInternalInfo.WinFuncPtrSegment = 0;
			    ModeInternalInfo.WinFuncPtrOffset = 0;
			}
			ModeInfo.WindowSize = VBE_ModeInfoBlock.WinSize * 1024;
			if ((VBE_ModeInfoBlock.WinAAttributes & 1) || (VBE_ModeInfoBlock.WinBAttributes & 1)) {
			    if ((!(VBE_ModeInfoBlock.WinBAttributes & 4)) || (VBE_ModeInfoBlock.WinAAttributes & 4)) {
				ModeInternalInfo.WriteWindow = 0;
				ModeInternalInfo.WindowPosition = ModeInfo.WindowAddress = VBE_ModeInfoBlock.WinASegment * 16;
			    } else {
				ModeInternalInfo.WriteWindow = 1;
				ModeInternalInfo.WindowPosition = ModeInfo.WindowAddress = VBE_ModeInfoBlock.WinBSegment * 16;
			    }
			    if (VBE_ModeInfoBlock.WinGranularity) {
				ModeInternalInfo.GranularityFix = VBE_ModeInfoBlock.WinSize / VBE_ModeInfoBlock.WinGranularity;
				if (ModeInternalInfo.GranularityFix == 1)
				    ModeInternalInfo.GranularityFix = 0;
			    } else
				ModeInternalInfo.GranularityFix = 0;
			} else
			    ModeInfo.WindowSize = ModeInfo.WindowAddress = ModeInternalInfo.GranularityFix = 0;
			/* You never know... */
			if (ModeInfo.WindowSize == 0)
			    ModeInfo.WindowSize = 0x10000;
			if (ModeInfo.WindowAddress == 0)
			    ModeInternalInfo.WindowPosition = ModeInfo.WindowAddress = 0xa0000;
			ModeInternalInfo.FastWinCalc = 0;
			for (j=1; j<32; j++) {
			    if (((unsigned int) 1<<j) == ModeInfo.WindowSize) {
				ModeInternalInfo.FastWinCalc = 1;
				ModeInternalInfo.WinMask1 = j;
				ModeInternalInfo.WinMask2 = ModeInfo.WindowSize-1;
				break;
			    }
			}
			ModeInfo.WindowMappingSize = ModeInfo.WindowSize + 0x2000;
		    }
		    if (VBE_ModeInfoBlock.ModeAttributes & 0x80) {
			ModeInfo.HasLinear = 1;
			ModeInternalInfo.LinearPosition = VBE_ModeInfoBlock.PhysBasePtr;
			ModeInfo.LinearSize = ModeInfo.ModeHeight * ModeInfo.LogicalLineLength;
			MemoryInfo.size = ModeInfo.LinearSize;
			MemoryInfo.address = ModeInternalInfo.LinearPosition;
			if (!__dpmi_physical_address_mapping(&MemoryInfo))
			    ModeInfo.LinearAddress = MemoryInfo.address;
			else
			    ModeInfo.LinearAddress = 0;
			ModeInfo.LinearMappingSize = ModeInfo.LinearSize + 0x2000;
		    }
		}
		ModeInfo.ChangeBank = NULL;
		ModeInfo.PutLine = NULL;
		ModeInfo.MappedAddress = NULL;
		if ((ModeInfo.ModeType == T16) && (ModeInfo.ModeHeight > 600)) //FIXME! not supported yeat
		    DiscartMode = 1;
		if (!DiscartMode)
		    InsertMode(&ModeInfo, &ModeInternalInfo);
	    }
	}
    }
    for (i = 0; i < 7; i++)
	InsertMode(&VGA_InfoTable[i], &VGA_InternalInfoTable[i]);

    /* See if we can map */
    LinearVgaBuffer = malloc(0x11000);
    if (LinearVgaBuffer != NULL) {
	AlignedLinearVgaBuffer = (char *) ALIGN(LinearVgaBuffer);
	if (!__djgpp_map_physical_memory(AlignedLinearVgaBuffer, 0x10000, 0xa0000))
	    CanMapBuffer = 1;
    }

    GfxInicilized = 1;
    return(1);
}

void CloseGfxLib(void) {
    _go32_dpmi_registers IntRegs;

    if (RestoreOldVGAMode) {
	IntRegs.x.ax = 0x1C;
	IntRegs.h.ah = 0x00;
	IntRegs.h.al = OldVGAMode;
	IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
	_go32_dpmi_simulate_int(0x10, &IntRegs);
    }
    _go32_dpmi_free_dos_memory(&PaletteMem);
}

void InsertMode(T_ModeInfo *ModeInfo, T_ModeInternalInfo *ModeInternalInfo) {
    if (NumberOfModes == 0) {
	ModeList = malloc(sizeof(T_ModeInfo));
	InternalModeList = malloc(sizeof(T_ModeInternalInfo));
    } else {
	ModeList = realloc(ModeList, sizeof(T_ModeInfo) * (NumberOfModes + 1));
	InternalModeList = realloc(InternalModeList, sizeof(T_ModeInternalInfo) * (NumberOfModes + 1));
    }
    ModeList[NumberOfModes] = *ModeInfo;
    InternalModeList[NumberOfModes] = *ModeInternalInfo;
    NumberOfModes++;
}

int SetMode(int ModeNumber, int Linear, int Debug) {
    _go32_dpmi_registers IntRegs;

    switch(InternalModeList[ModeNumber].ControlType) {
	case TVBE:
	    if (Debug) printf ("Using VESA BIOS.\n");
	    IntRegs.x.ax = 0x4F02;
	    if (ModeList[ModeNumber].ModeType == T16) {
		if (Linear)
		    return(0);
		IntRegs.x.bx = InternalModeList[ModeNumber].OriginalModeNumber;
		ModeList[ModeNumber].ChangeBank = NULL_ChangeBank;
		ModeList[ModeNumber].PutLine = PutLineVGA16;
	    } else if (Linear) {
		if (!ModeList[ModeNumber].HasLinear)
		    return(0);
		IntRegs.x.bx = InternalModeList[ModeNumber].OriginalModeNumber | (1<<14);
		ModeList[ModeNumber].ChangeBank = NULL_ChangeBank;
		ModeList[ModeNumber].PutLine = PutLineLinear;
	    } else {
		if (!ModeList[ModeNumber].HasWindow)
		    return(0);
		IntRegs.x.bx = InternalModeList[ModeNumber].OriginalModeNumber;
		if (HasProtectedModeInterface) {
		    if (Debug) printf ("Using Protected Mode Bank Switch Function.\n");
		    ModeList[ModeNumber].ChangeBank = VBE_ChangeBankProtectMode;
		} else if ((InternalModeList[ModeNumber].WinFuncPtrSegment != 0) && (InternalModeList[ModeNumber].WinFuncPtrOffset != 0)) {
		    if (Debug) printf ("Using Real Mode Bank Switch Function.\n");
		    ModeList[ModeNumber].ChangeBank = VBE_ChangeBankRealMode;
		} else {
		    if (Debug) printf ("Using Real Mode Bank Switch Interrupt Function.\n");
		    ModeList[ModeNumber].ChangeBank = VBE_ChangeBankRealInterrupt;
		}
		ModeList[ModeNumber].PutLine = PutLineBanked;
	    }
	    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
	    _go32_dpmi_simulate_int(0x10, &IntRegs);
	    break;
	case TVGA:
	    if (Debug) printf ("Using normal VGA.\n");
	    IntRegs.h.ah = 0x00;
	    IntRegs.h.al = InternalModeList[ModeNumber].OriginalModeNumber;
	    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
	    _go32_dpmi_simulate_int(0x10, &IntRegs);
	    ModeList[ModeNumber].ChangeBank = NULL_ChangeBank;
	    if (ModeList[ModeNumber].ModeType == T16)
		ModeList[ModeNumber].PutLine = PutLineVGA16;
	    else
		ModeList[ModeNumber].PutLine = PutLineLinear;
	    break;
	case TModeX:
	    if (Debug) printf ("Using VGA Mode X.\n");
	    SetModeX(InternalModeList[ModeNumber].OriginalModeNumber);
	    ModeList[ModeNumber].ChangeBank = NULL_ChangeBank;
	    ModeList[ModeNumber].PutLine = PutLineVGAX;
	    break;
    }
    CurrentMode = ModeList[ModeNumber];
    CurrentInternalMode = InternalModeList[ModeNumber];
    RestoreOldVGAMode = 1;
    MapVideoMemory();
    return(1);
}

void SetModeX(int ModeNumber) {
    _go32_dpmi_registers IntRegs;
    int i;
    char cleared[320];

    IntRegs.h.ah = 0x00;
    IntRegs.h.al = 0x13;
    IntRegs.x.ss = IntRegs.x.sp = IntRegs.x.flags = 0;
    _go32_dpmi_simulate_int(0x10, &IntRegs);
    for (i=0; i<320; i++)
	cleared[i] = 0;
    outportw(0x3c4, 0x604);
    if (ModeNumber == 1) {
	outportw(0x3c4, 0x0100);
	outportb(0x3c2, 0x00e3);
	outportw(0x3c4, 0x0300);
	outportw(0x3d4, 0x2c11);
	outportw(0x3d4, 0x5f00);
	outportw(0x3d4, 0x4f01);
	outportw(0x3d4, 0x5002);
	outportw(0x3d4, 0x8203);
	outportw(0x3d4, 0x5404);
	outportw(0x3d4, 0x8005);
	outportw(0x3d4, 0x0d06);
	outportw(0x3d4, 0x3e07);
	outportw(0x3d4, 0x0008);
	outportw(0x3d4, 0x4109);
	outportw(0x3d4, 0x000a);
	outportw(0x3d4, 0x000b);
	outportw(0x3d4, 0x000c);
	outportw(0x3d4, 0x000d);
	outportw(0x3d4, 0x000e);
	outportw(0x3d4, 0x000f);
	outportw(0x3d4, 0xea10);
	outportw(0x3d4, 0xac11);
	outportw(0x3d4, 0xdf12);
	outportw(0x3d4, 0x2813);
	outportw(0x3d4, 0x0014);
	outportw(0x3d4, 0xe715);
	outportw(0x3d4, 0x0616);
	outportw(0x3d4, 0xe317);
	for (i=0; i<240; i++)
	    PutLineVGAX(cleared, i);
    } else {
	outportw(0x3d4, 0x0e11);
	outportw(0x3d4, 0x5f00);
	outportw(0x3d4, 0x4f01);
	outportw(0x3d4, 0x5002);
	outportw(0x3d4, 0x8203);
	outportw(0x3d4, 0x5404);
	outportw(0x3d4, 0x8005);
	outportw(0x3d4, 0xbf06);
	outportw(0x3d4, 0x1f07);
	outportw(0x3d4, 0x0008);
	outportw(0x3d4, 0x4009);
	outportw(0x3d4, 0x000a);
	outportw(0x3d4, 0x000b);
	outportw(0x3d4, 0x000c);
	outportw(0x3d4, 0x000d);
	outportw(0x3d4, 0x000e);
	outportw(0x3d4, 0x000f);
	outportw(0x3d4, 0x9c10);
	outportw(0x3d4, 0x8e11);
	outportw(0x3d4, 0x8f12);
	outportw(0x3d4, 0x2813);
	outportw(0x3d4, 0x0014);
	outportw(0x3d4, 0x9615);
	outportw(0x3d4, 0xb916);
	outportw(0x3d4, 0xe317);
	for (i=0; i<400; i++)
	    PutLineVGAX(cleared, i);
    }
}

void MapVideoMemory(void) {
    int Amont, AlignedAmont;
    unsigned int PhysicalPosition;
    char *NearPosition, *AlignedBufferPtr;

    if ((!CanMapBuffer) || (CurrentMode.MappedAddress != NULL))
	return;
    if ((CurrentMode.HasLinear) && (CurrentMode.ChangeBank == NULL_ChangeBank)) {
	Amont = CurrentMode.LinearSize;
	PhysicalPosition = CurrentInternalMode.LinearPosition;
	NearPosition = malloc(CurrentMode.LinearMappingSize);
    } else if (CurrentMode.HasWindow) {
	Amont = CurrentMode.WindowSize;
	PhysicalPosition = CurrentInternalMode.WindowPosition;
	NearPosition = malloc(CurrentMode.WindowMappingSize);
    } else
	return;
    if (NearPosition == NULL)
	return;
    AlignedBufferPtr = (char *) ALIGN(NearPosition);
    AlignedAmont = ALIGN(Amont);
    if (__djgpp_map_physical_memory(AlignedBufferPtr, AlignedAmont, PhysicalPosition)) {
	free(NearPosition);
	CurrentMode.MappedAddress = NULL;
    } else
	CurrentMode.MappedAddress = AlignedBufferPtr;
}
