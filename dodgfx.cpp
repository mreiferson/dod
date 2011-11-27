/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodgfx.cpp
 * Description: graphical routines, loading and deleting gfx surfaces
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "doddefines.h"
#include "dod.h"
#include "dodddraw.h"
#include "dodengine.h"
#include "dodgfx.h"
#include "dodinput.h"
#include "dodmemcpy.h"
#include "dodsound.h"
#include "dodworld.h"

// Global Game Surfaces
LPDIRECTDRAWSURFACE     lpDDSDOD;                           // Do Or Die Intro Surface
LPDIRECTDRAWSURFACE     lpDDSXGames;                        // Xtreme Games LLC Intro Surface
LPDIRECTDRAWSURFACE     lpDDSDSoft;                         // DSoft Intro Surface
LPDIRECTDRAWSURFACE     lpDDSMenu;                          // Menu graphic
LPDIRECTDRAWSURFACE     lpDDSButtons[DOD_NUMBUTTONS];   // The menu button graphics
LPDIRECTDRAWSURFACE     lpDDSCursor;                        // Mouse Cursor Graphic
LPDIRECTDRAWSURFACE     lpDDSView;                          // The viewport during game
LPDIRECTDRAWSURFACE     lpDDSPanel;                         // Left Panel During Game
LPDIRECTDRAWSURFACE     lpDDSMinimap;                       // the mini map view
LPDIRECTDRAWSURFACE     lpDDSMMArea;                        // the mini map area bitmap
LPDIRECTDRAWSURFACE     lpDDSTileset[64];                   // The tileset graphics

// the cursor HBITMAP's (copied into lpDDSCursor at runtime)
HBITMAP                     Cursors[DOD_NUMCURSORS];        // The bitmaps holding the various cursors

// Temporary Surfaces Only Needed In DODGFX.CPP
LPDIRECTDRAWSURFACE     lpDDSRef;        // Reference Surface
LPDIRECTDRAWSURFACE     lpDDSTmp;       // Temporary Transition Surface
LPDIRECTDRAWSURFACE     lpDDSSrc;       // Source Surface For A Transition
LPDIRECTDRAWSURFACE     lpDDSDes;       // Destination Surface For A Transition

WORD                            PixelShade[32][65536]; // Precomputed shaded pixels

int                         Current_Cursor_ID = -1; // for the cursor changing system

WORD                            *backbuf; // the system memory back buffer, everything is blitted here

/*
 * RestoreGraphics:
 *    Restores the GFX if the surface was lost
 */
void RestoreGraphics(void)
{
    switch (Game_State) {
        case DOD_DODINTRO:
        case DOD_DSOFTINTRO:
        case DOD_XGAMESINTRO:
            if (lpDDSPrimary) {
                lpDDSPrimary->Restore();
            }
            if (lpDDSBack) {
                lpDDSBack->Restore();
            }
            if (lpDDSDOD) {
                lpDDSDOD->Restore();
                DDReLoadBitmap(lpDDSDOD, ".\\Gfx\\dod.bmp");
            }
            if (lpDDSDSoft) {
                lpDDSDSoft->Restore();
                DDReLoadBitmap(lpDDSDSoft, ".\\Gfx\\dsoft.bmp");
            }
            if (lpDDSXGames) {
                lpDDSXGames->Restore();
                DDReLoadBitmap(lpDDSXGames, ".\\Gfx\\xlogo.bmp");
            }
            if (lpDDSCursor) {
                lpDDSCursor->Restore();
                ChangeCursor(DOD_CURSORNORM);
            }
            break;
        case DOD_MENU:
        case DOD_CREDITS:
            if (lpDDSPrimary) {
                lpDDSPrimary->Restore();
            }
            if (lpDDSBack) {
                lpDDSBack->Restore();
            }
            if (lpDDSMenu) {
                KillMenuGFX();
                LoadMenuGFX();
            }
            if (lpDDSCursor) {
                lpDDSCursor->Restore();
                ChangeCursor(DOD_CURSORNORM);
            }
            break;
        case DOD_PLAYING:
            if (lpDDSPrimary) {
                lpDDSPrimary->Restore();
            }
            if (lpDDSBack) {
                lpDDSBack->Restore();
            }
            if (lpDDSCursor) {
                lpDDSCursor->Restore();
                ChangeCursor(Current_Cursor_ID);
            }
            if (lpDDSView) {
                KillGameGFX();
                LoadGameGFX();
            }
            break;
    }
}

/*
 * InitPixelShade:
 *      Fills the PixelShade array with precomputed shades of every possible pixel (32 shades)
 */
void InitPixelShade(void)
{
    int i, j;
    int r, g, b;
    int dr, dg, db;
    const double alpha[32] = { 0.0, 0.03, 0.06, 0.09,
                               0.13, 0.17, 0.21, 0.24,
                               0.27, 0.31, 0.34, 0.37,
                               0.41, 0.44, 0.47, 0.49,
                               0.51, 0.53, 0.56, 0.59,
                               0.63, 0.66, 0.69, 0.73,
                               0.76, 0.79, 0.83, 0.87,
                               0.91, 0.94, 0.97, 1.0
                             };
                             
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 65536; j++) {
            r = RED(j);
            g = GREEN(j);
            b = BLUE(j);
            dr = (int)(r * alpha[i]);
            dg = (int)(g * alpha[i]);
            db = (int)(b * alpha[i]);
            PixelShade[i][j] = RGB16(dr, dg, db);
        }
    }
}

/*
 * AlphaTransition:
 *    Does an alpha transition from Src -> Des
 */
void AlphaTransition(LPDIRECTDRAWSURFACE Src, LPDIRECTDRAWSURFACE Des)
{
    long i;                          // index into surfaces
    int alpha;                  // Holds current alpha value
    int dpitch, spitch, tpitch, ppitch; // surface pitch for destination, source, temp surfaces
    WORD *AlphaPTR;              // pointer to the current AlphaMap level (Source)
    WORD *InvAlphaPTR;           // the inverted pointer to the current AlphaMap level (Destination)
    WORD *src, *des, *tmp, *prm;
    WORD *fastsrc, *fastdes, *fasttmp;      // Surface memory pointer for source, destination, and temporary surfaces
    RECT SrcRect, DesRect;      // Source and destination rectangles
    
    // Set source and destination rectangles to the screen size
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    
    // Create the three surface we are going to use
    lpDDSTmp = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // The temporary surface
    lpDDSSrc = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // The source surface
    lpDDSDes = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // The destination surface
    
    // Blit the transition surfaces into out newly created source/destination surfaces
    lpDDSSrc->Blt(&DesRect, Src, &SrcRect, DDBLT_WAIT, NULL); // Blit Src->lpDDSSrc
    lpDDSDes->Blt(&DesRect, Des, &SrcRect, DDBLT_WAIT, NULL); // Blit Des->lpDDSDes
    
    // lock all three surfaces temporary, source, and destination
    des = DDLockSurface(lpDDSDes, &dpitch);
    src = DDLockSurface(lpDDSSrc, &spitch);
    tmp = DDLockSurface(lpDDSTmp, &tpitch);
    prm = DDLockSurface(lpDDSPrimary, &ppitch);
    
    // for each alpha level
    for (alpha = 31; alpha >= 0; alpha--) {
        // set AlphaMap pointers to appropriate levels
        AlphaPTR = PixelShade[alpha];
        InvAlphaPTR = PixelShade[31 - alpha];
        
        // "reset" the *fast* pointers to the locked surfaces
        fastsrc = src;
        fastdes = des;
        fasttmp = tmp;
        
        // loop through every pixel
        for (i = 0; i < 307200; i++, fasttmp++, fastsrc++, fastdes++) {
            // Set the new pixel value in temporary surface
            *fasttmp = AlphaPTR[*fastsrc] + InvAlphaPTR[*fastdes];
        }
        
        // copy the temp surface to the primary surface
#ifdef DOD_WINDOWED
        WORD *fastprm = prm + (rcWindow.top * ppitch) + rcWindow.left;
        fasttmp = tmp;
        for (i = 0; i < 480; i++, fastprm += ppitch, fasttmp += 640) {
            systovid_memcpy(fastprm, fasttmp, 1280);
        }
#else
        systovid_memcpy(prm, tmp, 614400);
#endif
    }
    // Unlock our temporary, source, and destination surfaces
    DDUnlockSurface(lpDDSPrimary);
    DDUnlockSurface(lpDDSTmp);
    DDUnlockSurface(lpDDSDes);
    DDUnlockSurface(lpDDSSrc);
    
    // Release or temporary, source, and destination surfaces
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
    lpDDSSrc->Release();
    lpDDSSrc = NULL;
    lpDDSDes->Release();
    lpDDSDes = NULL;
}

/*
 * FadeToBlack:
 *      Fades a screen to black
 */
void FadeToBlack(void)
{
    RECT SrcRect, DesRect; // Source and Destination Rectangles
    WORD *tmp;             // temporary surface memory pointer
    WORD *ref;
    WORD *prm;
    WORD *fastref, *fasttmp;
    int c, tpitch, rpitch, ppitch;         // incrementing variable, temporary surface pitch
    long i;                // another incrementing variable
    WORD *shade;
    
    // Set source and destination rectangles to size of screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    
    // Create our temporary surface
    lpDDSTmp = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY);
    lpDDSRef = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY);
    
    // Blit our primary surface into our temporary SYSTEM MEMORY surface
#ifdef DOD_WINDOWED
    lpDDSRef->Blt(&DesRect, lpDDSPrimary, &rcWindow, DDBLT_WAIT, NULL);
#else
    lpDDSRef->Blt(&DesRect, lpDDSPrimary, &SrcRect, DDBLT_WAIT, NULL);
#endif
    
    // Lock our temporary surface
    tmp = DDLockSurface(lpDDSTmp, &tpitch);
    ref = DDLockSurface(lpDDSRef, &rpitch);
    prm = DDLockSurface(lpDDSPrimary, &ppitch);
    
    for (c = 30; c >= 1; c--) {
        // get a pointer indexed to the start of the current shade level
        shade = PixelShade[c];
        
        // "reset" our *fast* surface pointers
        fastref = ref;
        fasttmp = tmp;
        
        // for every pixel on the screen (640*480=307200)
        for (i = 0; i < 307200; i++, fasttmp++, fastref++) {
            // new pixel please....
            *fasttmp = shade[*fastref];
        }
        
        // copy the temp surface to the primary surface
#ifdef DOD_WINDOWED
        WORD *fastprm = prm + (rcWindow.top * ppitch) + rcWindow.left;
        fasttmp = tmp;
        for (i = 0; i < 480; i++, fastprm += ppitch, fasttmp += 640) {
            systovid_memcpy(fastprm, fasttmp, 1280);
        }
#else
        systovid_memcpy(prm, tmp, 614400);
#endif
    }
    
    // unlock our temporary surface
    DDUnlockSurface(lpDDSTmp);
    DDUnlockSurface(lpDDSRef);
    DDUnlockSurface(lpDDSPrimary);
    
    // just to make sure the screen is black when this routine is over, fill it with 0
    DDFillSurface(lpDDSPrimary, 0);
    
    // release our temporary surface
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
    lpDDSRef->Release();
    lpDDSRef = NULL;
}

/*
 * FadeToSurface:
 *    Fades into a surface from black
 */
void FadeToSurface(LPDIRECTDRAWSURFACE lpDDS)
{
    int c;                 // counter variable
    long i;                // incrementing variable
    WORD *tmp, *ref, *prm;
    WORD *fasttmp, *fastref;       // temporary and destination surface mem pointers
    RECT SrcRect, DesRect; // Source and destination rectangles
    int tpitch, rpitch, ppitch;    // temporary and destination surface pitch
    WORD *shade;
    
    // Set the source and destination rectangles to the size of the screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    
    // Create the surfaces
    lpDDSTmp = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // the temporary surface
    lpDDSRef = DDCreateSurface(640, 480, DDSCAPS_SYSTEMMEMORY); // the temporary surface
    lpDDSRef->Blt(&DesRect, lpDDS, &SrcRect, DDBLT_WAIT, NULL); // blit the desired surface into our destination surface
    
    // Lock our surfaces temporary, and destination
    tmp = DDLockSurface(lpDDSTmp, &tpitch);
    prm = DDLockSurface(lpDDSPrimary, &ppitch);
    ref = DDLockSurface(lpDDSRef, &rpitch);
    
    // This can be changed, but it worx out nice to do 10 iterations
    for (c = 1; c <= 30; c++) {
        // get pointer indexed to the start of the current shade level
        shade = PixelShade[c];
        
        // "reset" our *fast* surface pointers
        fasttmp = tmp;
        fastref = ref;
        
        // for every pixel on the screen (640*480=307200)
        for (i = 0; i < 307200; i++, fasttmp++, fastref++) {
            // new pixel please.....
            *fasttmp = shade[*fastref];
        }
        
        // copy the temp surface to the primary surface
#ifdef DOD_WINDOWED
        WORD *fastprm = prm + (rcWindow.top * ppitch) + rcWindow.left;
        fasttmp = tmp;
        for (i = 0; i < 480; i++, fastprm += ppitch, fasttmp += 640) {
            systovid_memcpy(fastprm, fasttmp, 1280);
        }
#else
        systovid_memcpy(prm, tmp, 614400);
#endif
    }
    
    // unlock the temporary surface and destination surface
    DDUnlockSurface(lpDDSTmp);
    DDUnlockSurface(lpDDSPrimary);
    DDUnlockSurface(lpDDSRef);
    
    // blit the actual destination surface to the primary surface so we're sure
    // the screen is where it should be
#ifdef DOD_WINDOWED
    lpDDSPrimary->Blt(&rcWindow, lpDDS, &SrcRect, DDBLT_WAIT, NULL);
#else
    lpDDSPrimary->Blt(&DesRect, lpDDS, &SrcRect, DDBLT_WAIT, NULL);
#endif
    
    // release the temporary and destination surfaces
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
    lpDDSRef->Release();
    lpDDSRef = NULL;
}

/*
 * DrawCursor:
 *    Draws the cursor at the given X,Y location
 */
void DrawCursor(int x, int y)
{
    RECT SrcRect;           // Source rectangle
    int rX = 32, rY = 32;   // rectangle X,Y
    
    if (x > 608) {
        rX = 32 - (x - 608);
    }
    if (y > 448) {
        rY = 32 - (y - 448);
    }
    
    SetRect(&SrcRect, 0, 0, rX, rY);
    lpDDSBack->BltFast(x, y, lpDDSCursor, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

/*
 * BlitString:
 *    Blits a string to the surface passed in color passed by RGB structure
 */
void BlitString(char *txtstring, int x, int y, RGB *rgb)
{
    HDC hdc; // Device Context
    
    // Get the Device Context of the back surface
    if (lpDDSBack->GetDC(&hdc) == DD_OK) {
        // Set the Backgroung color to transparent
        SetBkColor(hdc, TRANSPARENT);
        // Set the text color to whatever is specified in the RGB triplet structure
        SetTextColor(hdc, RGB(rgb->r, rgb->g, rgb->b));
        // Print out the text we passed to the function
        TextOut(hdc, x, y, txtstring, lstrlen(txtstring));
        // Release the device context of the back surface
        lpDDSBack->ReleaseDC(hdc);
    }
}

/*
 * DrawLine:
 *    Draws a line using bresenhams algorithm
 */
void DrawLine(WORD *vb_start, int lpitch, int xo, int yo, int x1, int y1, WORD color)
{
    int dx,             // difference in x's
        dy,             // difference in y's
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error = 0,      // the discriminant i.e. error i.e. decision variable
        index;          // used for looping
        
    // pre-compute first pixel address in video buffer
    vb_start = vb_start + xo + yo * lpitch;
    
    // compute horizontal and vertical deltas
    dx = x1 - xo;
    dy = y1 - yo;
    
    // test which direction the line is going in i.e. slope angle
    if (dx >= 0) {
        x_inc = 1;
    } else {
        x_inc = -1;
        dx    = -dx;  // need absolute value
    }
    
    // test y component of slope
    if (dy >= 0) {
        y_inc = lpitch;
    } else {
        y_inc = -lpitch;
        dy    = -dy;  // need absolute value
    }
    
    // now based on which delta is greater we can draw the line
    if (dx > dy) {
        // draw the line
        for (index = 0; index <= dx; index++) {
            // set the pixel
            *vb_start = color;
            
            // adjust the error term
            error += dy;
            
            // test if error has overflowed
            if (error > dx) {
                error -= dx;
                
                // move to next line
                vb_start += y_inc;
            }
            // move to the next pixel
            vb_start += x_inc;
        }
    } else {
        // draw the line
        for (index = 0; index <= dy; index++) {
            // set the pixel
            *vb_start = color;
            
            // adjust the error term
            error += dx;
            
            // test if error overflowed
            if (error > 0) {
                error -= dy;
                
                // move to next line
                vb_start += x_inc;
            }
            // move to the next pixel
            vb_start += y_inc;
        }
    }
}

/*
 * DrawRect:
 *    Draws a rectangle on a given surface memory pointer
 */
void DrawRect(WORD *surface, int lpitch, int x, int y, int w, int h, WORD color)
{
    // Draw the top side of the rectangle
    DrawLine(surface, lpitch, x, y, x + w, y, color);
    // Draw the right side of the rectangle
    DrawLine(surface, lpitch, x + w, y, x + w, y + h, color);
    // Draw the bottom side of the rectangle
    DrawLine(surface, lpitch, x + w, y + h, x, y + h, color);
    // Draw the left side of the rectangle
    DrawLine(surface, lpitch, x, y + h, x, y, color);
}

/*
 * LoadIntroGFX:
 *    This function loads the graphics needed for the intros
 */
void LoadIntroGFX(void)
{
    // Load Intro GFX
    lpDDSDOD = DDLoadBitmap(".\\Gfx\\dod.bmp", 0, 0);
    lpDDSXGames = DDLoadBitmap(".\\Gfx\\xlogo.bmp", 0, 0);
    lpDDSDSoft = DDLoadBitmap(".\\Gfx\\dsoft.bmp", 0, 0);
    
    // Initializes the cursor, put here out of convienence
    InitCursorGFX();
}

/*
 * KillIntroGFX:
 *    releases the surfaces for the intros
 */
void KillIntroGFX(void)
{
    lpDDSDOD->Release(); // release the intro DOD graphic
    lpDDSDOD = NULL;
    lpDDSXGames->Release(); // release the intro XGames graphic
    lpDDSXGames = NULL;
    lpDDSDSoft->Release(); // release the intro DSoft graphic
    lpDDSDSoft = NULL;
}

/*
 * InitCursorGFX:
 *    This function loads the mouse graphics needed for the menu, and menu only
 */
void InitCursorGFX(void)
{
    // Load Cursor GFX
    lpDDSCursor = DDCreateSurface(32, 32, DDSCAPS_VIDEOMEMORY);
    DDSetColorKey(lpDDSCursor, RGB(255, 0, 255));
    Cursors[DOD_CURSORNORM] = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursornorm.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    ChangeCursor(DOD_CURSORNORM);
}

/*
 * ChangeCursor:
 *    This function changes the cursor to the given bitmap
 */
void ChangeCursor(int cursor_id)
{
    if (cursor_id != Current_Cursor_ID) {
        // Copy the bitmap to the surface
        DDCopyBitmap(lpDDSCursor, Cursors[cursor_id], 0, 0, 0, 0);
        // set the global current cursor id #
        Current_Cursor_ID = cursor_id;
    }
}

/*
 * CursorID:
 *    Simply returns the cursor ID #
 */
int CursorID(void)
{
    int halfcount = (DOD_NUMCURSORS >> 1);
    
    if (Current_Cursor_ID > halfcount) {
        return (Current_Cursor_ID - halfcount);
    } else {
        return Current_Cursor_ID;
    }
}

/*
 * LoadGameGFX:
 *    This function loads the graphics needed for the game
 */
void LoadGameGFX(void)
{
    RECT    SrcRect, DesRect; // source and destination rectangles
    int     i, x, y;         // counting variables
    
    // load in the cursor bitmaps
    Cursors[DOD_CURSORLEFT]             = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursorleft.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORRIGHT]            = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursorright.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORUP]               = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursorup.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORDOWN]             = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursordown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORUPLEFT]           = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursorupleft.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORUPRIGHT]          = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursorupright.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORDOWNLEFT]     = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursordownleft.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSORDOWNRIGHT]        = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursordownright.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFNORM]          = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffnorm.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFLEFT]          = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffleft.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFRIGHT]     = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffright.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFUP]            = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffup.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFDOWN]          = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffdown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFUPLEFT]        = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffupleft.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFUPRIGHT]       = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffupright.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFDOWNLEFT]  = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffdownleft.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    Cursors[DOD_CURSOROFFDOWNRIGHT] = (HBITMAP)LoadImage(NULL, ".\\Gfx\\cursoroffdownright.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    
    
    // create our viewport surface
    lpDDSView = DDCreateSurface(640 - DOD_PANELWIDTH, 480, DDSCAPS_SYSTEMMEMORY);
    // clear out the viewport surface
    DDFillSurface(lpDDSView, RGB16(31, 0, 31));
    // set the source color key
    DDSetColorKey(lpDDSView, RGB(255, 0, 255));
    
    // load the left panel graphic
    lpDDSPanel = DDLoadBitmap(".\\Gfx\\leftpanel.bmp", 0, 0);
    
    // create the minimap surface, it contains nothing cuz it's filled at runtime
    lpDDSMinimap = DDCreateSurface(128, 128, DDSCAPS_SYSTEMMEMORY);
    // clear out the minimap surface
    DDFillSurface(lpDDSMinimap, 0);
    // fill in the minimap
    FillInMM();
    
    // load the minimap currently viewed area surface
    lpDDSMMArea = DDCreateSurface(15, 15, DDSCAPS_VIDEOMEMORY);
    // load in the bitmap
    DDReLoadBitmap(lpDDSMMArea, ".\\Gfx\\mmarea.bmp");
    // set the color key
    DDSetColorKey(lpDDSMMArea, RGB(255, 0, 255));
    
    // create the tileset surfaces
    for (i = 0; i < 64; i++) {
        // create a surface for this index
        lpDDSTileset[i] = DDCreateSurface(BASE_TILE_W, BASE_TILE_H, DDSCAPS_VIDEOMEMORY);
        // clear out the surface
        DDFillSurface(lpDDSTileset[i], 0);
    }
    
    // load the tileset bmp into a temporary surface
    lpDDSTmp = DDLoadBitmap(".\\Gfx\\tileset.bmp", 0, 0);
    // for every y (row)
    for (y = 0; y < 8; y++) {
        // for every pic in row
        for (x = 0; x < 8; x++) {
            // set source and destination rectangles
            SetRect(&SrcRect, x * BASE_TILE_W, y * BASE_TILE_H, (x * BASE_TILE_W) + BASE_TILE_W, (y * BASE_TILE_H) + BASE_TILE_H);
            SetRect(&DesRect, 0, 0, BASE_TILE_W, BASE_TILE_H);
            // blit the tile from the Temp source pic to the actual tileset surface
            lpDDSTileset[(y * 8) + x]->Blt(&DesRect, lpDDSTmp, &SrcRect, DDBLT_WAIT, NULL);
        }
    }
    // release the temporary surface
    lpDDSTmp->Release();
    lpDDSTmp = NULL;
}

/*
 * KillGameGFX:
 *    This function releases all GFX used during the game
 */
void KillGameGFX(void)
{
    int i; // counting variable
    
    // release the cursor bitmaps
    for (i = 1; i < DOD_NUMCURSORS; i++) {
        DeleteObject(Cursors[i]);
    }
    
    // release the viewport surface
    lpDDSView->Release();
    lpDDSView = NULL;
    
    // release the panel gfx
    lpDDSPanel->Release();
    lpDDSPanel = NULL;
    
    // release the minimap surface
    lpDDSMinimap->Release();
    lpDDSMinimap = NULL;
    
    // release the minimap current area bitmatp
    lpDDSMMArea->Release();
    lpDDSMMArea = NULL;
    
    // release all the tileset surfaces
    for (i = 0; i < 64; i++) {
        lpDDSTileset[i]->Release();
        lpDDSTileset[i] = NULL;
    }
}

/*
 * LoadMenuGFX:
 *    Loads the menu graphic, and sets MenuReady to TRUE
 */
void LoadMenuGFX(void)
{
    // Load the menu graphic into the lpDDSMenu surface
    lpDDSMenu = DDLoadBitmap(".\\Gfx\\menu.bmp", 0, 0);
    // load the button gfx
    LoadButtonGFX();
    // Load the clicking sound
    DODLoadClickSound();
}

/*
 * KillMenuGFX:
 *    Releases the menu graphic, and sets MenuReady to FALSE
 */
void KillMenuGFX(void)
{
    // Release the lpDDSMenu surface
    lpDDSMenu->Release();
    lpDDSMenu = NULL;
    // release the button graphics
    KillButtonGFX();
    // delete the clicking sound
    DODDeleteClickSound();
}

/*
 * LoadButtonGFX:
 *    loads the graphics for the menu buttons
 */
void LoadButtonGFX(void)
{
    lpDDSButtons[0] = DDLoadBitmap(".\\Gfx\\buttonstartgamedown.bmp", 0, 0);
    lpDDSButtons[1] = DDLoadBitmap(".\\Gfx\\buttonloadgamedown.bmp", 0, 0);
    lpDDSButtons[2] = DDLoadBitmap(".\\Gfx\\buttonoptionsdown.bmp", 0, 0);
    lpDDSButtons[3] = DDLoadBitmap(".\\Gfx\\buttoncreditsdown.bmp", 0, 0);
    lpDDSButtons[4] = DDLoadBitmap(".\\Gfx\\buttonexitdown.bmp", 0, 0);
}

/*
 * KillButtonGFX:
 *    releases the graphics for the menu buttons
 */
void KillButtonGFX(void)
{
    for (int i = 0; i < DOD_NUMBUTTONS; i++) {
        lpDDSButtons[i]->Release();
        lpDDSButtons[i] = NULL;
    }
}

/*
 * InitBackBuf:
 *      Initializes the back buffer in system memory
 */
void InitBackBuf(void)
{
    backbuf = (WORD *)malloc(640 * 480 * sizeof(WORD));
}

/*
 * KillBackBuf:
 *      Releases the memory of the back buffer
 */
void KillBackBuf(void)
{
    free(backbuf);
}
