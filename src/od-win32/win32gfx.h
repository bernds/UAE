#ifndef __WIN32GFX_H__
#define __WIN32GFX_H__

#ifndef _WIN32_WCE
#include <ddraw.h>
#endif

#ifndef _WIN32_WCE
HRESULT CALLBACK modesCallback( LPDDSURFACEDESC2 modeDesc, LPVOID context );
#endif

int WIN32GFX_IsPicassoScreen( void );
int WIN32GFX_IsFullScreen( void );
int WIN32GFX_GetWidth( void );
int WIN32GFX_GetHeight( void );
void WIN32GFX_DisplayChangeRequested( void );
void WIN32GFX_ToggleFullScreen( void );
void WIN32GFX_DisablePicasso( void );
void WIN32GFX_EnablePicasso( void );
void WIN32GFX_PaletteChange( void );
void WIN32GFX_ClearPalette( void );
void WIN32GFX_SetPalette( void );

#ifndef _WIN32_WCE
RGBFTYPE WIN32GFX_FigurePixelFormats( RGBFTYPE colortype );
int WIN32GFX_AdjustScreenmode( uae_u32 *pwidth, uae_u32 *pheight, uae_u32 *ppixbits );
#endif

extern HWND hStatusWnd;
extern HINSTANCE hDDraw;
extern char *start_path;
extern uae_u32 default_freq;

#endif
