 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS handlers.
  *
  * (c) 1997 Gustavo Goedert
  *
  * Led routines by Michael Sontheimer
  */

int InstallHandlers(void);
void UninstallHandlers(void);
static void KeyboardHandler(void);
int InstallKeyboardHandler(void);
void UninstallKeyboardHandler(void);
static void MouseHandler(_go32_dpmi_registers *MouseRegs);
int InstallMouseHandler(void);
void UninstallMouseHandler(void);
void NumLockLED(int on);
void CapsLockLED(int on);
void ScrollLockLED(int on);
UBYTE InitLED(void);
void RestoreLEDStatus(UBYTE ubSavedLED);
void PicassoRefresh(void);

extern int Original_produce_sound;
