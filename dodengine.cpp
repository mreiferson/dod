/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodengine.cpp
 * Description: the game engine and it's associated routines
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
#include "dodengine.h"
#include "dodgfx.h"
#include "dodinput.h"
#include "dodintro.h"
#include "dodmenu.h"
#include "dodmidi.h"
#include "dodsound.h"
#include "dodworld.h"

RECT            SrcMMRect       = { 0, 0, 128, 128 };                   // Preset rectangles for the MiniMap Blit (Source)
RECT            SrcMMAreaRect   = { 0, 0, 15, 15 };
RECT            SrcPanelRect    = { 0, 0, DOD_PANELWIDTH, 480 };        // Preset rectangle for the Panel Blit (Source & Destination)
RECT        SrcBaseRect     = { 0, 0, BASE_TILE_W, BASE_TILE_H }; // Preset rectangle for the Base Tile Blit (Source)
RECT            SrcViewRect     = { 0, 0, 640 - DOD_PANELWIDTH, 480 }; // Preset rectangles for the Viewport Blit (Source)

/*
 * UpdateGame:
 *    Updates the game processes.
 */
void GameMain(void)
{
    BOOL ttl = TRUE; // time to live?
    
    UpdateInput(); // Update all input related variables for the game
    
    // What game state are we in?
    switch (Game_State) {
        case DOD_NOTHING:
            break;
            // We are currently in the XGAMESINTRO phase
        case DOD_XGAMESINTRO:
            // Update the XGAMESINTRO screen
            UpdateXGamesIntro(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                DODDeleteXGamesSound(); // delete the XGames sound
                Game_State = DOD_DSOFTINTRO; // set Game_State to next phase
                AlphaTransition(lpDDSXGames, lpDDSDSoft); // do an alpha transition into next screen
            }
            break;
            // We are currently in the DSOFTINTRO phase
        case DOD_DSOFTINTRO:
            // Update the DSOFTINTRO screen
            UpdateDSoftIntro(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                Game_State = DOD_DODINTRO; // set the Game_State to the next phase
                AlphaTransition(lpDDSDSoft, lpDDSDOD); // do another alphatransition to the next screen
            }
            break;
            // We are currently in the DODINTRO phase
        case DOD_DODINTRO:
            // Update the DODINTRO screen
            UpdateDODIntro(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                Game_State = DOD_MENU; // set the Game_State to the next phase
                FadeToBlack(); // fade the screen to black
                KillIntroGFX(); // release the intro surfaces
                LoadMenuGFX(); // load the menu gfx
                MidiStop(); // stop the midi form playing
                MidiPlay(".\\Music\\menu.mid", TRUE); // play the menu midi
                FadeToSurface(lpDDSMenu); // fade into the menu graphic from black
            }
            break;
            // We are currently in the MENU phase
        case DOD_MENU:
            // Update the menu screen
            UpdateMenu();
            break;
            // We are currently in the CREDITS phase
        case DOD_CREDITS:
            // Update the credits screen
            UpdateCredits(&ttl);
            // if the player hits enter go on to next phase
            if (keyboard_state[DIK_RETURN] || !ttl) {
                ClearCreditsCount(); // clear out the credits counter array
                Game_State = DOD_MENU; // set the Game_State to the next phase
                FadeToBlack(); // fade to black
                MidiStop(); // stop the music
                MidiPlay(".\\Music\\menu.mid", TRUE); // play the menu midi
                FadeToSurface(lpDDSMenu); // Fade into the menu pic
            }
            break;
            // We are currently in the PLAYING phase
        case DOD_PLAYING:
            // Update the game engine
            UpdateGame();
            if (keyboard_state[DIK_RETURN]) {
                Game_State = DOD_MENU; // send the game back to the menu
                FadeToBlack(); // fade the screen to black
                KillGameGFX(); // release all the game Gfx
                LoadMenuGFX(); // load the menu gfx
                ChangeCursor(DOD_CURSORNORM);
                MidiPlay(".\\Music\\menu.mid", TRUE); // play the menu music
                FadeToSurface(lpDDSMenu); // fade to the menu pic
            }
            break;
    }
}

/*
 * UpdateGame:
 *    Updates The Game
 */
void UpdateGame(void)
{
    int cid;  // current cursor ID
    HRESULT ret;
    
    // Check to see where that mouse is moving
    CheckMouseExtent();
    
    // Blit the left panel to the back buffer
    lpDDSBack->BltFast(0, 0, lpDDSPanel, &SrcPanelRect, DDBLTFAST_WAIT);
    
    // draw the mini map
    RenderMM();
    // draw the base layer
    RenderBase();
    
    // blit the viewport surface to the lpDDSBack
    lpDDSBack->BltFast(DOD_PANELWIDTH, 0, lpDDSView, &SrcViewRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
    
    // Set the destination rectangle to the area where the mouse is (MouseX, MouseY) (MouseX+32, MouseY+32)
    // If it's scrolling on the right or botton, adjust position so we see the cursor
    cid = CursorID(); // get the cursor ID #
    if ((cid != DOD_CURSORRIGHT) && (cid != DOD_CURSORDOWN) && (cid != DOD_CURSORUPRIGHT) && (cid != DOD_CURSORDOWNRIGHT) && (cid != DOD_CURSORDOWNLEFT)) {
        DrawCursor(MouseX, MouseY);
    } else if ((cid == DOD_CURSORRIGHT) || (cid == DOD_CURSORUPRIGHT)) {
        DrawCursor(MouseX - 32, MouseY);
    } else if ((cid == DOD_CURSORDOWN) || (cid == DOD_CURSORDOWNLEFT)) {
        DrawCursor(MouseX, MouseY - 32);
    } else if (cid == DOD_CURSORDOWNRIGHT) {
        DrawCursor(MouseX - 32, MouseY - 32);
    }
    
    // Flip To Primary Surface
    ret = DDFlip();
    if (ret == DDERR_SURFACELOST) {
        RestoreGraphics();
    }
}

/*
 * RenderMM:
 *    This function renders the mini map view
 */
void RenderMM(void)
{
    // blit the MiniMap to the back buffer
    lpDDSBack->BltFast(16, 16, lpDDSMinimap, &SrcMMRect, DDBLTFAST_WAIT);
    
    // blit the MiniMap Veiwable Area Bitmap
    lpDDSBack->BltFast(16 + (world_offset_x >> 5), 16 + (world_offset_y >> 5), lpDDSMMArea, &SrcMMAreaRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

/*
 * RenderBase:
 *    Renders the base layer of the world
 */
void RenderBase(void)
{
    RECT rcClip;                                                // clipping rectangle
    int x, y;                                                    // counting variables
    int ClipLeftSave, ClipTopSave;                      // clipping values, saved
    int ClipRight, ClipBottom;                              // right and bottom clip values
    int screen_x = DOD_PANELWIDTH,                          // world to screen x
        screen_y = 0;                                           // world to screen y
    int y_start = (world_offset_y / BASE_TILE_H),       // precomputed y starting value
        x_start = (world_offset_x / BASE_TILE_W);       // precomputed x starting value
    int world_index_y = y_start,
        world_index_x = x_start;
    int numtiles_y = SCREEN_NUMTILES_Y + ((world_offset_y % BASE_TILE_W) > 0 ? 1 : 0),
        numtiles_x = SCREEN_NUMTILES_X + ((world_offset_x % BASE_TILE_H) > 0 ? 1 : 0);
    BASE *row;                                                  // BASE Tile pointer for faster access
    
    ClipLeftSave = rcClip.left = world_offset_x % BASE_TILE_W;
    ClipTopSave = rcClip.top = world_offset_y % BASE_TILE_H;
    ClipRight = ClipLeftSave;
    ClipBottom = ClipTopSave;
    rcClip.right = BASE_TILE_W;
    rcClip.bottom = BASE_TILE_H;
    
    // for every y (row)
    for (y = 0; y < numtiles_y; y++) {
        // set our fast BASE tile access pointer
        row = world.base[world_index_y];
        
        // set the screen X coordinate back to start of row
        screen_x = DOD_PANELWIDTH;
        // set the clipping rectangle to our saved values
        rcClip.left = ClipLeftSave;
        rcClip.right = BASE_TILE_W;
        
        if ((y == numtiles_y - 1) && (numtiles_y > SCREEN_NUMTILES_Y)) {
            rcClip.bottom = ClipBottom;
        }
        
        // for every tile in row
        for (x = 0; x < numtiles_x; x++) {
            if ((x == numtiles_x - 1) && (numtiles_x > SCREEN_NUMTILES_X)) {
                rcClip.right = ClipRight;
            }
            // render the base tile to our viewport surface
            lpDDSBack->BltFast(screen_x, screen_y, lpDDSTileset[row[world_index_x].tileindex], &rcClip, DDBLTFAST_WAIT);
            screen_x += BASE_TILE_W - rcClip.left;
            rcClip.left = 0;
            world_index_x++;
        }
        
        screen_y += BASE_TILE_H - rcClip.top;
        rcClip.top = 0;
        world_index_y++;
        world_index_x = x_start;
    }
}