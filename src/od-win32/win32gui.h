#ifndef WIN32GUI_H
#define WIN32GUI_H

#define MAXJOYSTICKS 2

#define MAX_BUFFER_SIZE    256
#define NUM_DRIVES         16

typedef struct
{
    char path[256];
    char name[256];
    uae_u16 rw;
    uae_u8  sectors;
    uae_u8  surfaces;
    uae_u8  reserved;
    uae_u8  hardfile;
    uae_u16 spare;
} drive_specs;

extern drive_specs drives[NUM_DRIVES];

#define CFG_DESCRIPTION_LENGTH 128
#define CFG_SER_LENGTH 256
#define CFG_ROM_LENGTH 256
#define CFG_PAR_LENGTH 256
#define CFG_KEY_LENGTH 256

#define CONFIG_VERSION_MAJOR 1
#define CONFIG_VERSION_MINOR 2

#define DIRECT_SOUND_ENABLED 0x01
#define AHI_ENABLED          0x02
#define SOUND_HAS_PRIORITY   0x03

typedef struct
{
    // Configuration Version Information
    DWORD Size;
    WORD VersionMajor;
    WORD VersionMinor;

    // Configuration Description
    char  Name[ MAX_PATH ];
    char  Description[ CFG_DESCRIPTION_LENGTH ];

    // Actual configuration information
    char  CustomSize;             // flag that the user has selected custom xy dimensions
    char  SerialPort;           // serial-port flag
    char  InvalidAddresses;     // log invalid-address stuff
    char  NoAutoConfig;         // do not add uae-devices and memory if set

    char  AddressingIs24Bit;    // 24-bit addressing
    char  JulianMode;
    char  CPULevel;             // 0 = 68000, 1 = 68010, 2 = 68020, 3 = 68020/68881
    char  CPUCompatible;        // 0 = no, 1 = yes

    int   FakeJoystick;         // Joystick settings

    char  CyrixCPU;             // related to QueryPerformanceCounter or RTSC register
    char  GfxLibReplacement;    // use the EXPERIMENTAL gfx-lib replacement stuff
    char  CenterX;              // center the image horizontally
    char  CenterY;              // center the image vertically

    char  Blit32;               // 32-bit blits
    char  BlitImmediate;        // blit immediately
    char  FullScreen;           // full-screen display mode
    char  LineDouble;           // double lines

    char  CorrectAspect;        // dunno
    char  Lores;                // lores pixels
    char  MoreOptions;          // 8-bit mask of other options:
                                //   0x01 = direct-sound
    char  DebugLogging;

    char  ColorMode;            // dunno
    char  M68KSpeed;            // -w option, for tuning CPU vs. custom-chip performance

    char  SerialName[CFG_SER_LENGTH];      // name of port to use
    char  KickstartName[CFG_ROM_LENGTH];   // Name of ROM file
    char  KeyFileName[CFG_KEY_LENGTH];     // Name of Key-File
    char  PrinterName[CFG_PAR_LENGTH-4];     // Name of printer

    int   FrameRate;            // refresh-rate for Amiga screens

    // Memory sizes
    uae_u32 ChipMem;
    uae_u32 FastMem;
    uae_u32 P96Mem;
    uae_u32 BogoMem;              // slow-mem
    uae_u32 Z3Mem;                // Zorro-III memory

    int   ScreenWidth;          // width of display for Amiga screens
    int   ScreenHeight;         // height of display for Amiga screens

    // Sound settings
    char  SoundSupport;
    char  SoundStereo;
    int   SoundBits;
    int   SoundFreq;

    // Hard-Drive Settings
    drive_specs drives[ NUM_DRIVES ];

    // Floppy Settings
    char  df[4][256];

} ConfigStruct, *ConfigStructPtr;

extern int GetSettings (int all_options);

#endif
