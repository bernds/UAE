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

#define CONFIG_SAVE   0
#define CONFIG_LOAD   1
#define CONFIG_SAVE_FULL 2
#define CONFIG_LOAD_FULL 3
#define CONFIG_DELETE 4

typedef struct
{
    char  Name[ MAX_PATH ];
    char  Description[ CFG_DESCRIPTION_LENGTH ];
} ConfigStruct, *ConfigStructPtr;

void WIN32GUI_LoadUIString( DWORD id, char *string, DWORD dwStringLen );
void WIN32GUI_DisplayGUI( int drive );
extern int GetSettings (int all_options);
extern void DiskSelection( HWND hDlg, WPARAM wParam, int flag, struct uae_prefs *prefs );
ConfigStructPtr AllocateConfigStruct( void );
void FreeConfigStruct( ConfigStructPtr cfgptr );
ConfigStructPtr GetFirstConfigEntry( HANDLE *file_handle, LPWIN32_FIND_DATA find_data );
ConfigStructPtr GetNextConfigEntry( HANDLE *file_handle, LPWIN32_FIND_DATA find_data );
void HandleConfiguration( HWND hDlg, int flag );
void InitializeListView( HWND hDlg );
#endif
