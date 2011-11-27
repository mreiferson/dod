/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodgfx.h
 * Description: the include file for graphical routines, loading and deleting gfx surfaces
 *
 */

#ifndef DOD_GFXH
#define DOD_GFXH

/*
 * Defines
 */
#define DOD_PANELWIDTH  160
#define DOD_NUMCURSORS  18
#define DOD_NUMBUTTONS  5

/*
 * Typedefs
 */
// RGB (red, green, blue) triplet
typedef struct _RGB {
    int r; // red component
    int g; // green component
    int b; // blue component
} RGB;

/*
 * Variables
 */
extern LPDIRECTDRAWSURFACE          lpDDSDOD;                       // Do Or Die Intro Surface
extern LPDIRECTDRAWSURFACE          lpDDSXGames;                    // Xtreme Games LLC Intro Surface
extern LPDIRECTDRAWSURFACE          lpDDSDSoft;                     // DSoft Graphic
extern LPDIRECTDRAWSURFACE          lpDDSMenu;                      // Menu Graphic
extern LPDIRECTDRAWSURFACE          lpDDSButtons[DOD_NUMBUTTONS];   // The menu button graphics
extern LPDIRECTDRAWSURFACE          lpDDSCursor;                    // Cursor
extern HBITMAP                          Cursors[DOD_NUMCURSORS];   // The bitmaps holding the various cursors
extern LPDIRECTDRAWSURFACE          lpDDSView;                      // The viewport during game
extern LPDIRECTDRAWSURFACE          lpDDSPanel;                     // Left panel during game
extern LPDIRECTDRAWSURFACE          lpDDSMinimap;                   // The surface for the mini map view
extern LPDIRECTDRAWSURFACE          lpDDSMMArea;                    // The Currently Viewed Area
extern LPDIRECTDRAWSURFACE          lpDDSTileset[64];               // The tileset for BASE layer
extern WORD                             *backbuf;                       // the system memory back buffer, everything is blitted here

/*
 * Function Declarations
 */
void RestoreGraphics(void);
void InitPixelShade(void);
void AlphaTransition(LPDIRECTDRAWSURFACE Src, LPDIRECTDRAWSURFACE Des);
void FadeToBlack(void);
void FadeToSurface(LPDIRECTDRAWSURFACE lpDDS);
void DrawCursor(int x, int y);
void BlitString(char *txtstring, int x, int y, RGB *rgb);
void DrawLine(WORD *vb_start, int lpitch, int xo, int yo, int x1, int y1, WORD color);
void DrawRect(WORD *vb_start, int lpitch, int x, int y, int w, int h, WORD color);
void LoadButtonGFX(void);
void KillButtonGFX(void);
void LoadIntroGFX(void);
void KillIntroGFX(void);
void InitCursorGFX(void);
void ChangeCursor(int cursor_id);
int  CursorID(void);
void LoadGameGFX(void);
void KillGameGFX(void);
void LoadMenuGFX(void);
void KillMenuGFX(void);
void InitBackBuf(void);
void KillBackBuf(void);

#endif