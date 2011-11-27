/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodinput.cpp
 * Description: input routines, includes updating of variables
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
#include "doddinput.h"
#include "dodengine.h"
#include "dodgfx.h"
#include "dodinput.h"
#include "dodworld.h"

int MouseX, MouseY;     // Mouse X,Y Coordinates
BYTE MouseLB, MouseRB;  // Variables Which Tell If The Left/Right Mouse Buttons Are Pressed

/*
 * UpdateInput:
 *    This updates the various input related variables of the game
 */
void UpdateInput(void)
{
    // Read the data from the mouse and keyboard
    DIReadMouse();
    DIReadKeyboard();
    
    // Update the mouse position since it's relative the info we are given
    // is just the delta from the last time we called DI_Read_Mouse, so
    // we need to add this value to the X,Y mouse coordinates
    MouseX += (mouse_state.lX << 1); // scale the delta by multiplying by 2 (<< 1)
    MouseY += (mouse_state.lY << 1); // scale the delta by multiplying by 2 (<< 1)
    // Update the state of the mouse's buttons
    MouseLB = mouse_state.rgbButtons[0];
    MouseRB = mouse_state.rgbButtons[1];
    
    // crunch mouse coordinates to the extents of the screen
    if (MouseX < 0) {
        MouseX = 0;
    } else if (MouseX > 639) {
        MouseX = 639;
    }
    if (MouseY < 0) {
        MouseY = 0;
    } else if (MouseY > 479) {
        MouseY = 479;
    }
}

/*
 * CheckMouseExtent:
 *    Checks to see if the mouse is telling us to scroll!
 */
void CheckMouseExtent(void)
{
    int cursor_where_x = 1; // 0 = left edge || 1 = no edge || 2 = right edge
    int cursor_where_y = 1; // 0 = top edge || 1 = no edge || 2 = bottom edge
    BOOL CursorChanged = FALSE;
    BOOL CursorInvalidX = FALSE;
    BOOL CursorInvalidY = FALSE;
    
    if (MouseX > 630) {
        cursor_where_x = 2;
        if ((world_offset_x += ((MouseX - 630) << 1)) > MAX_WORLDOFFSETX) {
            CursorInvalidX = TRUE;
            world_offset_x = MAX_WORLDOFFSETX;
        }
    } else if (MouseX < 10) {
        cursor_where_x = 0;
        if ((world_offset_x -= ((10 - MouseX) << 1)) < 0) {
            CursorInvalidX = TRUE;
            world_offset_x = 0;
        }
    }
    
    if (MouseY > 470) {
        cursor_where_y = 2;
        if ((world_offset_y += ((MouseY - 470) << 1)) > MAX_WORLDOFFSETY) {
            CursorInvalidY = TRUE;
            world_offset_y = MAX_WORLDOFFSETY;
        }
    } else if (MouseY < 10) {
        cursor_where_y = 0;
        if ((world_offset_y -= ((10 - MouseY) << 1)) < 0) {
            CursorInvalidY = TRUE;
            world_offset_y = 0;
        }
    }
    
    if ((cursor_where_x != 1) || (cursor_where_y != 1)) {
        CursorChanged = TRUE;
        switch (cursor_where_x) {
            case 0:
                switch (cursor_where_y) {
                    case 0:
                        if (CursorInvalidX && CursorInvalidY) {
                            ChangeCursor(DOD_CURSOROFFUPLEFT);
                        } else {
                            ChangeCursor(DOD_CURSORUPLEFT);
                        }
                        break;
                    case 1:
                        if (CursorInvalidX) {
                            ChangeCursor(DOD_CURSOROFFLEFT);
                        } else {
                            ChangeCursor(DOD_CURSORLEFT);
                        }
                        break;
                    case 2:
                        if (CursorInvalidX && CursorInvalidY) {
                            ChangeCursor(DOD_CURSOROFFDOWNLEFT);
                        } else {
                            ChangeCursor(DOD_CURSORDOWNLEFT);
                        }
                        break;
                }
                break;
            case 1:
                switch (cursor_where_y) {
                    case 0:
                        if (CursorInvalidY) {
                            ChangeCursor(DOD_CURSOROFFUP);
                        } else {
                            ChangeCursor(DOD_CURSORUP);
                        }
                        break;
                    case 2:
                        if (CursorInvalidY) {
                            ChangeCursor(DOD_CURSOROFFDOWN);
                        } else {
                            ChangeCursor(DOD_CURSORDOWN);
                        }
                        break;
                }
                break;
            case 2:
                switch (cursor_where_y) {
                    case 0:
                        if (CursorInvalidX && CursorInvalidY) {
                            ChangeCursor(DOD_CURSOROFFUPRIGHT);
                        } else {
                            ChangeCursor(DOD_CURSORUPRIGHT);
                        }
                        break;
                    case 1:
                        if (CursorInvalidX) {
                            ChangeCursor(DOD_CURSOROFFRIGHT);
                        } else {
                            ChangeCursor(DOD_CURSORRIGHT);
                        }
                        break;
                    case 2:
                        if (CursorInvalidX && CursorInvalidY) {
                            ChangeCursor(DOD_CURSOROFFDOWNRIGHT);
                        } else {
                            ChangeCursor(DOD_CURSORDOWNRIGHT);
                        }
                        break;
                }
                break;
        }
    }
    
    
    if (!CursorChanged && (CursorID() != DOD_CURSORNORM)) {
        ChangeCursor(DOD_CURSORNORM);
    }
}