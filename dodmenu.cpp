/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodmenu.cpp
 * Description: menu routines, for the main DOD Menu screen
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

#include "dod.h"
#include "dodddraw.h"
#include "doddsound.h"
#include "doddinput.h"
#include "dodgfx.h"
#include "dodinput.h"
#include "dodmenu.h"
#include "dodmidi.h"
#include "dodsound.h"
#include "dodworld.h"

// The rectangles which define the various clickable areas of the menu
RECT StartGameRect  = { 195, 102, 334, 125 }; // The Start Game Button RECT
RECT LoadGameRect       = { 199, 169, 326, 189 }; // The Load Game Button RECT
RECT OptionsRect        = { 215, 233, 307, 254 }; // The Options Button RECT
RECT CreditsRect        = { 214, 297, 311, 316 }; // The Credits Button RECT
RECT ExitRect           = { 228, 362, 291, 385 }; // The Exit Button RECT

// this 2D array holds the X,Y coordinates of each button
int ButtonLocation[DOD_NUMBUTTONS][2] = { 169, 83, // The start game location
                                        169, 147, // The load game location
                                        170, 211, // The options location
                                        169, 278, // The credits location
                                        169, 344
                                        }; // The exit location

/*
 * XYInRect:
 *    Convienence function to do the repetitive task of determining if a X,Y
 * coordinate is in a given RECT structure
 */
int XYInRect(int x, int y, RECT *rect)
{
    // determine if the X,Y coordinate is inside the rectangle
    if ( ((x >= rect->left) && (x <= rect->right))
            && ((y >= rect->top) && (y <= rect->bottom)) ) {
        return 1;    // return 1 saying it was
    } else { // oh well, maybe next time
        return 0;    // return 0 saying it wasn't
    }
}

/*
 * DrawButtonDown:
 *    Draws a menu button in the down state
 */
void DrawButtonDown(int button_num)
{
    RECT SrcRect, DesRect; // Source and destination rectangles
    
    // Set the source and destination rectangles, using our 2D location array
    SetRect(&SrcRect, 0, 0, 184, 62);
    SetRect(&DesRect, ButtonLocation[button_num][0], ButtonLocation[button_num][1],
            ButtonLocation[button_num][0] + 184, ButtonLocation[button_num][1] + 62);
    // Blit the button to the back surface
    lpDDSBack->Blt(&DesRect, lpDDSButtons[button_num], &SrcRect, DDBLT_WAIT, NULL);
}

/*
 * UpdateMenu:
 *    The routine which determines which menu buttons are being pressed and does
 * other tidbits of crap
 */
void UpdateMenu(void)
{
    RECT SrcRect, DesRect; // Source and Destination rectangles
    HRESULT ret;
    
    // Set the source and destination rectangles
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    // Blit the lpDDSMenu surface (Menu Graphic) to the back buffer
    lpDDSBack->Blt(&DesRect, lpDDSMenu, &SrcRect, DDBLT_WAIT, NULL);
    
    // If the left mouse button is pressed
    if (MouseLB) {
        // Determine what was clicked on
        if (XYInRect(MouseX, MouseY, &StartGameRect)) {
            // The start game button was pressed
            // Game Start Code Here
            DrawButtonDown(0); // draw the button in it's down (highlighted) position
            DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
            ret = DDFlip(); // flip to the primary surface
            if (ret == DDERR_SURFACELOST) {
                RestoreGraphics();
            }
            DODPlayClickSound(); // play the clicking sound
            FadeToBlack(); // Fade the screen to black
            MidiStop(); // Stop the midi
            KillMenuGFX(); // release the menu graphic surface
            InitWorld();   // initialize the world
            LoadGameGFX(); // Load the games needed graphics
            Game_State = DOD_PLAYING; // set the game state to DOD_PLAYING
        } else if (XYInRect(MouseX, MouseY, &LoadGameRect)) {
            // The load game button was pressed
            // Load Game Code Here
            DrawButtonDown(1); // draw the button in it's down (highlighted) position
            DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
            ret = DDFlip(); // flip to the primary surface
            if (ret == DDERR_SURFACELOST) {
                RestoreGraphics();
            }
            DODPlayClickSound(); // play the clicking sound
        } else if (XYInRect(MouseX, MouseY, &OptionsRect)) {
            // The options button was pressed
            // Options Code Here
            DrawButtonDown(2); // draw the button in it's down (highlighted) position
            DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
            ret = DDFlip(); // flip to the primary surface
            if (ret == DDERR_SURFACELOST) {
                RestoreGraphics();
            }
            DODPlayClickSound(); // play the clicking sound
        } else if (XYInRect(MouseX, MouseY, &CreditsRect)) {
            // The credits button was pressed
            // Credits Code Here
            DrawButtonDown(3); // draw the button in it's down (highlighted) position
            DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
            ret = DDFlip(); // flip to the primary surface
            if (ret == DDERR_SURFACELOST) {
                RestoreGraphics();
            }
            DODPlayClickSound(); // play the clicking sound
            FadeToBlack(); // fade the menu to black
            MidiStop(); // stop the midi
            MidiPlay(".\\Music\\credits.mid", TRUE); // play the credits music
            Game_State = DOD_CREDITS; // set the Game_State into Credits mode
        } else if (XYInRect(MouseX, MouseY, &ExitRect)) {
            // The exit button was pressed
            // Exit Code Here
            DrawButtonDown(4); // draw the button in it's down (highlighted) position
            DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
            ret = DDFlip(); // flip to the primary surface
            if (ret == DDERR_SURFACELOST) {
                RestoreGraphics();
            }
            DODPlayClickSound(); // play the clicking sound
            FadeToBlack(); // Fade to a black screen
            DDFillSurface(lpDDSPrimary, 0); // Fill the primary surface with black
            DDFillSurface(lpDDSBack, 0);    // Fill the back surface with black
            KillMenuGFX(); // release the menu graphic surface
            PostMessage(main_window_handle, WM_DESTROY, 0, 0); // Post the destroy window message
            Game_State = DOD_NOTHING; // set the game state to nothing
        } else {
            // No buttons were pressed
            // Just draw the cursor, and flip
            DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
            ret = DDFlip(); // flip to the primary surface
            if (ret == DDERR_SURFACELOST) {
                RestoreGraphics();
            }
        }
    } else {
        // the mouse button wasn't pressed
        // just draw the cursor and flip
        DrawCursor(MouseX, MouseY); // Draw the mouse Cursor
        ret = DDFlip(); // flip to the primary surface
        if (ret == DDERR_SURFACELOST) {
            RestoreGraphics();
        }
    }
}