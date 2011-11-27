/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodddraw.cpp
 * Description: DirectDraw utility functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <ddraw.h>
#include "doddefines.h"
#include "dod.h"
#include "dodddraw.h"

LPDIRECTDRAW         lpDD         = NULL;  // dd object
LPDIRECTDRAWSURFACE  lpDDSPrimary = NULL;  // dd primary surface
LPDIRECTDRAWSURFACE  lpDDSBack    = NULL;  // dd back surface
LPDIRECTDRAWCLIPPER  lpDDClipper     = NULL;  // for windowed modes
DDSURFACEDESC        ddsd;                 // a direct draw surface description struct
DDSCAPS              ddscaps;              // a direct draw surface capabilities struct
HRESULT              ddrval;               // result back from dd calls

// these are overwritten globally by DD_Init()
int screen_width,            // width of screen
    screen_height,           // height of screen
    screen_bpp;              // bits per pixel

HFONT Font; // the default global font

RGB16                   rgb16;             // Video Card RGB Information Structure
int                     mRed, mGreen,      // Faster values of above structure
                        mBlue, pRed,       // Faster values of above structure
                        pGreen, pBlue;     // Faster values of above structure

int DDInitFullscreen(int width, int height, int bpp, HWND hwnd)
{
    HRESULT ret;
    
    // create object and test for error
    if (DirectDrawCreate(NULL, &lpDD, NULL) != DD_OK) {
        return(0);
    }
    
    // set cooperation level to windowed mode normal
    if (lpDD->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE) != DD_OK) {
        return(0);
    }
    
    // set the display mode
    if (lpDD->SetDisplayMode(width, height, bpp) != DD_OK) {
        return(0);
    }
    
    // set globals
    screen_height = height;
    screen_width = width;
    screen_bpp = bpp;
    
    // Create the primary surface
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    
    // we need to let dd know that we want a complex
    // flippable surface structure, set flags for that
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    
    // set the backbuffer count to 1
    ddsd.dwBackBufferCount = 1;
    
    // create the primary surface
    ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
    
    // query for the backbuffer i.e the secondary surface
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
    
    // clear out both primary and secondary surfaces
    DDFillSurface(lpDDSPrimary, 0);
    DDFillSurface(lpDDSBack, 0);
    
    DDGetRGB16();
    
    return 1;
}

int DDInitWindowed(int width, int height, int bpp, HWND hwnd)
{
    HRESULT ret;
    
    // create object and test for error
    if (DirectDrawCreate(NULL, &lpDD, NULL) != DD_OK) {
        return(0);
    }
    
    // set cooperation level to windowed mode normal
    if (lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL) != DD_OK) {
        return(0);
    }
    
    // set globals
    screen_height = height;
    screen_width = width;
    screen_bpp = bpp;
    
    // Create the primary surface
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    
    // all we need for windowed mode is access to the primary surface
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    
    // create the primary surface
    ret = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
    
    // create an offscreen and system mem back surface
    lpDDSBack = DDCreateSurface(width, height, NULL);
    
    lpDD->CreateClipper(0, &lpDDClipper, NULL);
    lpDDClipper->SetHWnd(0, hwnd);
    lpDDSPrimary->SetClipper(lpDDClipper);
    
    // clear out both primary and secondary surfaces
    DDFillSurface(lpDDSPrimary, 0);
    DDFillSurface(lpDDSBack, 0);
    
    DDGetRGB16();
    
    return 1;
}

void DDShutdown(void)
{
    // release clipper
    if (lpDDClipper) {
        lpDDClipper->Release();
        lpDDClipper = NULL;
    }
    
    // release the secondary surface
    if (lpDDSBack) {
        lpDDSBack->Release();
        lpDDSBack = NULL;
    }
    
    // release the primary surface
    if (lpDDSPrimary) {
        lpDDSPrimary->Release();
        lpDDSPrimary = NULL;
    }
    
    // finally, the main dd object
    if (lpDD) {
        lpDD->Release();
        lpDD = NULL;
    }
}

LPDIRECTDRAWSURFACE DDCreateSurface(int width, int height, int mem_flags)
{
    DDSURFACEDESC ddsd;         // working description
    LPDIRECTDRAWSURFACE lpdds;  // temporary surface
    
    // set to access caps, width, and height
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize  = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    
    // set dimensions of the new bitmap surface
    ddsd.dwWidth  =  width;
    ddsd.dwHeight =  height;
    
    // set surface to offscreen plain
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;
    
    // create the surface
    if (lpDD->CreateSurface(&ddsd, &lpdds, NULL) != DD_OK) {
        return(NULL);
    }
    
    // return surface
    return(lpdds);
}

HRESULT DDFlip(void)
{
#ifndef DOD_WINDOWED
    HRESULT ddrval;
    ddrval = lpDDSPrimary->Flip(NULL, DDFLIP_WAIT); // flip the surfaces
    return ddrval;
#else
    RECT rect = { 0, 0, 640, 480 };
    lpDDSPrimary->Blt(&rcWindow, lpDDSBack, &rect, DDBLT_WAIT, NULL);
    return 1;
#endif
}

void DDWaitForVsync(void)
{
    lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
}

void DDFillSurface(LPDIRECTDRAWSURFACE lpdds, WORD color)
{
    DDBLTFX ddbltfx; // this contains the DDBLTFX structure
    
    // clear out the structure and set the size field
    DD_INIT_STRUCT(ddbltfx);
    
    // set the dwfillcolor field to the desired color
    ddbltfx.dwFillColor = color;
    
    // ready to blt to surface
    lpdds->Blt(NULL,                           // ptr to dest rectangle
               NULL,                           // ptr to source surface, NA
               NULL,                           // ptr to source rectangle, NA
               DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait
               &ddbltfx);                      // ptr to DDBLTFX structure
}

/*
 * DD_Lock_Surface:
 *      Locks a specified surface
 */
WORD *DDLockSurface(LPDIRECTDRAWSURFACE lpdds, int *lpitch)
{
    // lock the surface
    DD_INIT_STRUCT(ddsd);
    lpdds->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
    
    // set the memory pitch
    *lpitch = ddsd.lPitch >> 1;
    
    // return pointer to surface
    return((WORD *)ddsd.lpSurface);
}

/*
 * DD_Unlock_Surface:
 *      Unlocks a specified surface
 */
void DDUnlockSurface(LPDIRECTDRAWSURFACE lpdds)
{
    // unlock the surface memory
    lpdds->Unlock(NULL);
}

/*
 * DDLoadBitmap:
 *      create a DirectDrawSurface from a bitmap resource.
 */
LPDIRECTDRAWSURFACE DDLoadBitmap(LPCSTR szBitmap, int dx, int dy)
{
    HBITMAP             hbm;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    IDirectDrawSurface  *pdds;
    
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    
    if (hbm == NULL) {
        return NULL;
    }
    
    // get size of the bitmap
    GetObject(hbm, sizeof(bm), &bm);      // get size of bitmap
    
    // create a DirectDrawSurface for this bitmap
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = bm.bmWidth;
    ddsd.dwHeight = bm.bmHeight;
    
    if (lpDD->CreateSurface(&ddsd, &pdds, NULL) != DD_OK) {
        return NULL;
    }
    
    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    
    DeleteObject(hbm);
    
    return pdds;
}

/*
 * DDReLoadBitmap:
 *      load a bitmap from a file or resource into a directdraw surface.
 *      normaly used to re-load a surface after a restore.
 */
HRESULT DDReLoadBitmap(LPDIRECTDRAWSURFACE pdds, LPCSTR szBitmap)
{
    HBITMAP             hbm;
    HRESULT             hr;
    
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    
    if (hbm == NULL) {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }
    
    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);
    if (hr != DD_OK) {
        OutputDebugString("ddcopybitmap failed\n");
    }
    
    DeleteObject(hbm);
    return hr;
}

/*
 * DDCopyBitmap:
 *     draw a bitmap into a DirectDrawSurface
 */
HRESULT DDCopyBitmap(LPDIRECTDRAWSURFACE pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
    HDC                 hdcImage;
    HDC                 hdc;
    BITMAP              bm;
    DDSURFACEDESC       ddsd;
    HRESULT             hr;
    
    if (hbm == NULL || pdds == NULL) {
        return E_FAIL;
    }
    
    // make sure this surface is restored.
    pdds->Restore();
    
    //  select bitmap into a memoryDC so we can use it.
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage) {
        OutputDebugString("createcompatible dc failed\n");
    }
    SelectObject(hdcImage, hbm);
    
    // get size of the bitmap
    GetObject(hbm, sizeof(bm), &bm);    // get size of bitmap
    dx = dx == 0 ? bm.bmWidth  : dx;    // use the passed size, unless zero
    dy = dy == 0 ? bm.bmHeight : dy;
    
    // get size of surface.
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);
    
    if ((hr = pdds->GetDC(&hdc)) == DD_OK) {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }
    
    DeleteDC(hdcImage);
    
    return hr;
}

/*
 * DDColorMatch:
 *    Matches a color (RGB) with a surface
 */
DWORD DDColorMatch(LPDIRECTDRAWSURFACE pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC ddsd;
    HRESULT hres;
    
    //  use GDI SetPixel to color match for us
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK) {
        rgbT = GetPixel(hdc, 0, 0);             // save current pixel value
        SetPixel(hdc, 0, 0, rgb);               // set our value
        pdds->ReleaseDC(hdc);
    }
    
    // now lock the surface so we can read back the converted color
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);
    
    if (hres == DD_OK) {
        dw = *(DWORD *)ddsd.lpSurface; // get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32) {
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // mask it to bpp
        }
        pdds->Unlock(NULL);
    }
    
    //  now put the color that was there back.
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK) {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    
    return dw;
}

/*
 * DDSetColorKey
 *      set a color key for a surface, given a RGB.
 *      if you pass CLR_INVALID as the color key, the pixel
 *      in the upper-left corner will be used.
 */
HRESULT DDSetColorKey(LPDIRECTDRAWSURFACE pdds, COLORREF rgb)
{
    DDCOLORKEY          ddck;
    
    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

/*
 * ChangeFont:
 *    Changes the default global font
 */
void ChangeFont(const char *FontName, int Width, int Height, int Attributes)
{
    Font = CreateFont(Height, Width, 0, 0, Attributes, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, FontName);
}

/*
 * SetFont:
 *    Sets the font output to current global font
 */
void SetFont(HDC sdc)
{
    SelectObject(sdc, Font);
}

/*
 * GetRGB16:
 *    Must run this function to fill the RGB16 struct with the information needed to plot a pixel
 *      To call this, you must have rgb16 defined as a global (unless you want to modify this) variable
 *      RGB16 rgb16;
 */
void DDGetRGB16(void)
{
    DDSURFACEDESC   ddsd;       // DirectDraw Surface Description
    BYTE            shiftcount; // Shift Counter
    
    // get a surface despriction
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_PIXELFORMAT;
    lpDDSPrimary->GetSurfaceDesc(&ddsd);
    
    // Fill in the masking values for extracting colors
    rgb16.Mask.rgbRed = ddsd.ddpfPixelFormat.dwRBitMask;
    rgb16.Mask.rgbGreen = ddsd.ddpfPixelFormat.dwGBitMask;
    rgb16.Mask.rgbBlue = ddsd.ddpfPixelFormat.dwBBitMask;
    
    // get red surface information
    shiftcount = 0;
    while (!(ddsd.ddpfPixelFormat.dwRBitMask & 1)) {
        ddsd.ddpfPixelFormat.dwRBitMask >>= 1;
        shiftcount++;
    }
    rgb16.depth.rgbRed = (BYTE)ddsd.ddpfPixelFormat.dwRBitMask;
    rgb16.Position.rgbRed = shiftcount;
    rgb16.Amount.rgbRed = (ddsd.ddpfPixelFormat.dwRBitMask == 0x1f) ? 3 : 2;
    
    // get green surface information
    shiftcount = 0;
    while (!(ddsd.ddpfPixelFormat.dwGBitMask & 1)) {
        ddsd.ddpfPixelFormat.dwGBitMask >>= 1;
        shiftcount++;
    }
    rgb16.depth.rgbGreen = (BYTE)ddsd.ddpfPixelFormat.dwGBitMask;
    rgb16.Position.rgbGreen = shiftcount;
    rgb16.Amount.rgbGreen = (ddsd.ddpfPixelFormat.dwGBitMask == 0x1f) ? 3 : 2;
    
    // get Blue surface information
    shiftcount = 0;
    while (!(ddsd.ddpfPixelFormat.dwBBitMask & 1)) {
        ddsd.ddpfPixelFormat.dwBBitMask >>= 1;
        shiftcount++;
    }
    rgb16.depth.rgbBlue = (BYTE)ddsd.ddpfPixelFormat.dwBBitMask;
    rgb16.Position.rgbBlue = shiftcount;
    rgb16.Amount.rgbBlue = (ddsd.ddpfPixelFormat.dwBBitMask == 0x1f) ? 3 : 2;
    
    // fill in variables so we dont' have to access the structure anymore
    mRed = rgb16.Mask.rgbRed;         // Red Mask
    mGreen = rgb16.Mask.rgbGreen;     // Green Mask
    mBlue = rgb16.Mask.rgbBlue;       // Blue Mask
    pRed = rgb16.Position.rgbRed;     // Red Position
    pGreen = rgb16.Position.rgbGreen; // Green Position
    pBlue = rgb16.Position.rgbBlue;   // Blue Position
}