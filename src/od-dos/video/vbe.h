 /*
  * UAE - The Un*x Amiga Emulator
  *
  * DOS VESA BIOS / VGA / Mode X graphics interface.
  *
  * (c) 1997 Gustavo Goedert
  */

typedef enum {T16, T256, T32K, T64K, T16M} T_ModeType;

typedef struct {
    T_ModeType ModeType;
    int ModeWidth, ModeHeight;
    int BitsPerPixel, BitsPerColor;
    int RedSize, RedPosition;
    int GreenSize, GreenPosition;
    int BlueSize, BluePosition;
    int LineLength, LogicalLineLength;
    int HasWindow;
    unsigned int WindowSize, WindowMappingSize, WindowAddress;
    int HasLinear;
    unsigned int LinearSize, LinearMappingSize, LinearAddress;
    char *MappedAddress;
    void (*ChangeBank)(int BankNumber);
    void (*PutLine)(char *Address, int y);
} T_ModeInfo;

extern int NumberOfModes;
extern T_ModeInfo *ModeList;
extern T_ModeInfo CurrentMode;
extern int CanMapBuffer;
extern int CurrentBankNumber;

void SetPalette(int Color, int Red, int Green, int Blue);
void DelayedSetPalette(int Color, int Red, int Green, int Blue);
void ViewPalette(int Color, int *Red, int *Green, int *Blue);
void LoadPalette(void);
int InitGfxLib(void);
void CloseGfxLib(void);
int SetMode(int ModeNumber, int Linear, int Debug);
void SaveMode(void);
void RestoreMode(void);
