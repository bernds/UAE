 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Joystick, mouse and keyboard emulation prototypes and definitions
  *
  * Copyright 1995 Bernd Schmidt
  * Copyright 2001-2002 Toni Wilen
  */

enum mousestate { unknown_mouse, normal_mouse, dont_care_mouse, follow_mouse };
extern enum mousestate mousestate;

extern void do_mouse_hack (void);
extern void mousehack_setfollow (void);
extern void mousehack_setdontcare (void);
extern uae_u32 mousehack_helper (void);
extern void mousehack_handle (int sprctl, int sprpos);
extern int needmousehack (void);
extern void togglemouse (void);
extern int mousehack_alive (void);

extern uae_u16 potgo_value;
extern uae_u16 POTGOR (void);
extern void POTGO (uae_u16 v);
extern uae_u16 POT0DAT (void);
extern uae_u16 POT1DAT (void);
extern void JOYTEST (uae_u16 v);
extern uae_u16 JOY0DAT (void);
extern uae_u16 JOY1DAT (void);
