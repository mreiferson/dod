/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dod.h
 * Description: main include file, contains declarations, macros, etc.
 *
 */

#ifndef DOD_DODH
#define DOD_DODH

/*
 * Typedefs
 */
// The various things the game can be doing
typedef enum _GAMESTATE {
    DOD_NOTHING, DOD_XGAMESINTRO, DOD_DODINTRO, DOD_DSOFTINTRO, DOD_MENU, DOD_CREDITS, DOD_PLAYING
} GAMESTATE;

/*
 * Variables
 */
extern BOOL      bActive;                      // is the application currently active
extern GAMESTATE Game_State;                   // state of the game
extern HWND main_window_handle; // save the window handle
extern HINSTANCE main_instance; // save the instance
extern RECT rcWindow;

/*
 * Function Declarations
 */
void FiniApp(void);

#endif