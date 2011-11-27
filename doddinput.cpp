/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      doddinput.cpp
 * Description: DirectInput utility functions
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
#include <dinput.h>
#include "doddinput.h"
#include "dodutil.h"

// directinput globals
LPDIRECTINPUT        lpDI      = NULL;    // dinput object
LPDIRECTINPUTDEVICE  lpDIKey   = NULL;    // dinput keyboard
LPDIRECTINPUTDEVICE  lpDIMouse = NULL;    // dinput mouse

// these contain the target records for all di input packets
UCHAR keyboard_state[256]; // contains keyboard state table
DIMOUSESTATE mouse_state;  // contains state of mouse

int DIInit(HINSTANCE inst)
{
    if (DirectInputCreate(inst, DIRECTINPUT_VERSION, &lpDI, NULL) != DI_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

void DIShutdown(void)
{
    if (lpDI) {
        lpDI->Release();
        lpDI = NULL;
    }
}

int DIInitMouse(HWND hwnd)
{
    // create a mouse device
    if (lpDI->CreateDevice(GUID_SysMouse, &lpDIMouse, NULL) != DI_OK) {
        return(0);
    }
    
    // set cooperation level
    if (lpDIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND) != DI_OK) {
        return(0);
    }
    
    // set data format
    if (lpDIMouse->SetDataFormat(&c_dfDIMouse) != DI_OK) {
        return(0);
    }
    
    // acquire the mouse
    if (lpDIMouse->Acquire() != DI_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

int DIInitKeyboard(HWND hwnd)
{
    // create the keyboard device
    if (lpDI->CreateDevice(GUID_SysKeyboard, &lpDIKey, NULL) != DI_OK) {
        return(0);
    }
    
    // set cooperation level
    if (lpDIKey->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK) {
        return(0);
    }
    
    // set data format
    if (lpDIKey->SetDataFormat(&c_dfDIKeyboard) != DI_OK) {
        return(0);
    }
    
    // acquire the keyboard
    if (lpDIKey->Acquire() != DI_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

void DIReadMouse(void)
{
    if (lpDIMouse) {
        if (lpDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state) != DI_OK) {
            lpDIMouse->Acquire();
        }
    } else {
        memset(&mouse_state, 0, sizeof(mouse_state));
    }
}

void DIReadKeyboard(void)
{
    if (lpDIKey) {
        if (lpDIKey->GetDeviceState(256, (LPVOID)keyboard_state) != DI_OK) {
            lpDIKey->Acquire();
        }
    } else {
        memset(keyboard_state, 0, sizeof(keyboard_state));
    }
}

void DIReleaseMouse(void)
{
    if (lpDIMouse) {
        lpDIMouse->Unacquire();
        lpDIMouse->Release();
        lpDIMouse = NULL;
    }
}

void DIReleaseKeyboard(void)
{
    if (lpDIKey) {
        lpDIKey->Unacquire();
        lpDIKey->Release();
        lpDIKey = NULL;
    }
}