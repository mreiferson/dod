/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dod.cpp
 * Description: main file, contains WinMain, initialization routines.
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dsound.h>
#include <dinput.h>
#include <stdlib.h>
#include <stdio.h>
#include "doddefines.h"
#include "dod.h"
#include "dodddraw.h"
#include "doddsound.h"
#include "doddinput.h"
#include "dodengine.h"
#include "dodgfx.h"
#include "dodinput.h"
#include "dodmemcpy.h"
#include "dodmidi.h"
#include "dodsound.h"
#include "dodutil.h"

#ifndef DOD_NOINTRO
GAMESTATE Game_State = DOD_XGAMESINTRO; // State of the game
#else
#include "dodworld.h"
GAMESTATE Game_State = DOD_PLAYING;
#endif

BOOL      bActive;                      // is the application active
HWND      main_window_handle;           // Global Main Window Handle
HINSTANCE main_instance;                // Global Window Instance Handle
RECT         rcWindow;

/*
 * WinProc:
 *    Processes windows messages.
 */
long PASCAL WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // determine which message was sent in order to handle it
    switch (message) {
            // the app is being activated
        case WM_ACTIVATEAPP:
            bActive = wParam; // is it active, TRUE or FALSE?
            break;
            // we've returned, kill the windows cursor
        case WM_SETCURSOR:
            SetCursor(NULL); // kill the windows cursor
            break;
            // it's telling us it's time to go, quit.....
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            // wow, time to restart the midi, if the looping variable is true.....
        case MM_MCINOTIFY:
            if ((wParam == MCI_NOTIFY_SUCCESSFUL) && IsMidiLooping())
                // restart the midi playing
            {
                MidiRestart();
            }
            break;
            // update the client area rectangle
        case WM_MOVE:
            GetClientRect(hWnd, &rcWindow);
            ClientToScreen(hWnd, (LPPOINT)&rcWindow);
            ClientToScreen(hWnd, (LPPOINT)&rcWindow + 1);
            break;
    }
    
    // return to caller
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
 * FiniApp:
 *    Cleans up the application, frees memory.
 */
void FiniApp(void)
{
    // Free cursor, it's bitmaps, and the clicking sound
    lpDDSCursor->Release();
    lpDDSCursor = NULL;
    DeleteObject(Cursors[DOD_CURSORNORM]);
    
    // shutdown directdraw
    DDShutdown();
    
    // release the back buffer
    KillBackBuf();
    
    // shutdown directsound
    DSShutdown();
    
    // release all input devices
    DIReleaseKeyboard();
    DIReleaseMouse();
    
    // shutdown directinput
    DIShutdown();
    
    // close the error file
    Close_Error_File();
}

/*
 * InitGame:
 *    Initializes core game components
 */
void InitGame()
{
    // open the error file, useful for debugging
    Open_Error_File("doderror.dat");
    
    // Initialize directdraw
#ifdef DOD_WINDOWED
    DDInitWindowed(640, 480, 16, main_window_handle);
#else
    DDInitFullscreen(640, 480, 16, main_window_handle);
#endif
    
    // initialize directsound
    DSInit(main_window_handle);
    
    // initialize Directinput
    DIInit(main_instance);
    
    // initialize all input devices
    DIInitKeyboard(main_window_handle);
    DIInitMouse(main_window_handle);
    
    // initialize the back buffer
    InitBackBuf();
    
    // pick the memcpy to use throughout the game
    Pickmemcpy();
    
    // Load in the Intro GFX
#ifdef DOD_NOINTRO
    InitCursorGFX();
#else
    LoadIntroGFX();
#endif
    
    // Fill Alpha Precomputed Values Array
    InitPixelShade();
    
    // Hide the mouse cursor
    ShowCursor(FALSE);
    
#ifdef DOD_NOINTRO
    MouseX = 320;
    MouseY = 240;
    InitWorld();
    LoadGameGFX();
#endif
}

/*
 * InitApp:
 *    Initializes the application window.
 */
BOOL InitApp(HINSTANCE hInst, int nCmdShow)
{
    WNDCLASS WndClass;
    HWND hWnd;
    char *name = "Do Or Die";
    
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WinProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInst;
    WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(0, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH__ *)GetStockObject(BLACK_BRUSH);
    WndClass.lpszMenuName = 0;
    WndClass.lpszClassName = name;
    RegisterClass(&WndClass);
    
#ifndef DOD_WINDOWED
    // create the window, it's fullscreen mode
    hWnd = CreateWindowEx(
               WS_EX_TOPMOST,
               name,
               name,
               WS_POPUP,
               0, 0,
               64,
               48,
               NULL,
               NULL,
               hInst,
               NULL);
#else
    // Get Screen Metrics
    RECT rcScreen;
    SystemParametersInfo(SPI_GETWORKAREA, NULL, (RECT *)&rcScreen, NULL);
    // Determine how big we should make our window in order to have a 640x480 Client area
    RECT rcNewWindow = { 0, 0, 640, 480 };
    AdjustWindowRect((RECT *)&rcNewWindow, WS_VISIBLE | WS_OVERLAPPEDWINDOW, FALSE);
    // create the window using our data above, it's windowed mode
    hWnd = CreateWindow(
               name,
               name,
               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
               (rcScreen.right / 2) - 320,
               (rcScreen.bottom / 2) - 240,
               rcNewWindow.right - rcNewWindow.left,
               rcNewWindow.bottom - rcNewWindow.top,
               NULL,
               NULL,
               hInst,
               NULL);
#endif
               
    if (!hWnd) {
        return FALSE;
    }
    
    // initialize global window variables
    main_window_handle = hWnd;
    main_instance      = hInst;
    
    // Show the Window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    // Initialize the game
    InitGame();
    
    return TRUE;
}

/*
 * WinMain:
 *    Contains the message loop.
 */
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    
    // initialize the app
    if (!InitApp(hInst, nCmdShow)) {
        return FALSE;
    }
    
    // infinite loop
    while (1) {
        // peekmessage so as not to interrupt processes
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // if the message said to quit, get the hell outta here
            if (msg.message == WM_QUIT) {
                break;
            }
            
            // translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // if there are no messages, update the game
        if (bActive) {
            GameMain();
        }
    }
    
    // shutdown game and release all resources
    FiniApp();
    
    // return to Windows like this
    return(msg.wParam);
}