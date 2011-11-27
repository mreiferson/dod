/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodddraw.h
 * Description: include file for DirectDraw utility functions
 *
 */

#ifndef DOD_DODDDRAWH
#define DOD_DODDDRAWH

#define RED(p) (p >> pRed)                                             // Extracts Red Component
#define GREEN(p) ((p & mGreen) >> pGreen)                              // Extracts Green Component
#define BLUE(p) (p & mBlue)                                            // Extracts Blue Component
#define RGB16(r, g, b) ((r << pRed) | (g << pGreen) | b)               // Creates RGB Pixel Value
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct); }

// the structure which holds pixel RGB masks
typedef struct _RGBMASK {
    unsigned long rgbRed; // red component
    unsigned long rgbGreen; // green component
    unsigned long rgbBlue; // blue component
} RGBMASK;

// the structure which holds screen surface info (5,6,5 or 5,5,5 and masking)
typedef struct _RGB16 {
    RGBQUAD depth;
    RGBQUAD Amount;
    RGBQUAD Position;
    RGBMASK Mask;
} RGB16;

// DirectDraw functions
int                     DDInitFullscreen(int width, int height, int bpp, HWND hwnd);
int                     DDInitWindowed(int width, int height, int bpp, HWND hwnd);
void                        DDShutdown(void);
LPDIRECTDRAWSURFACE DDCreateSurface(int width, int height, int mem_flags);
HRESULT                 DDFlip(void);
void                        DDWaitForVsync(void);
void                        DDFillSurface(LPDIRECTDRAWSURFACE lpdds, WORD color);
WORD                       *DDLockSurface(LPDIRECTDRAWSURFACE lpdds, int *lpitch);
void                        DDUnlockSurface(LPDIRECTDRAWSURFACE lpdds);
LPDIRECTDRAWSURFACE DDLoadBitmap(LPCSTR szBitmap, int dx, int dy);
HRESULT                 DDReLoadBitmap(LPDIRECTDRAWSURFACE pdds, LPCSTR szBitmap);
HRESULT                 DDCopyBitmap(LPDIRECTDRAWSURFACE pdds, HBITMAP hbm, int x, int y, int dx, int dy);
DWORD                       DDColorMatch(LPDIRECTDRAWSURFACE pdds, COLORREF rgb);
HRESULT                 DDSetColorKey(LPDIRECTDRAWSURFACE pdds, COLORREF rgb);
void                        DDGetRGB16();

extern LPDIRECTDRAW         lpDD;                 // dd object
extern LPDIRECTDRAWSURFACE  lpDDSPrimary;         // dd primary surface
extern LPDIRECTDRAWSURFACE  lpDDSBack;            // dd back surface
extern DDSURFACEDESC        ddsd;                 // a direct draw surface description struct
extern DDBLTFX              ddbltfx;              // used to fill
extern DDSCAPS              ddscaps;              // a direct draw surface capabilities struct
extern HRESULT              ddrval;               // result back from dd calls

// these are overwritten globally by DD_Init()
extern int screen_width,                            // width of screen
       screen_height,                           // height of screen
       screen_bpp;                              // bits per pixel

extern RGB16    rgb16;                          // Video Card RGB Information Structure
extern int      mRed, mGreen,                   // Faster values of above structure
       mBlue, pRed,                    // Faster values of above structure
       pGreen, pBlue;                  // Faster values of above structure

#endif