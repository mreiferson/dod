/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      doddinput.h
 * Description: include file for DirectInput utility functions
 *
 */

#ifndef DOD_DODDINPUTH
#define DOD_DODDINPUTH

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// input
int DInput_Init(void);
void DInput_Shutdown(void);
int DI_Init_Mouse(void);
int DI_Init_Keyboard(void);
int DI_Read_Mouse(void);
int DI_Read_Keyboard(void);
void DI_Release_Mouse(void);
void DI_Release_Keyboard(void);

// directinput globals
extern LPDIRECTINPUT        lpdi;       // dinput object
extern LPDIRECTINPUTDEVICE  lpdikey;    // dinput keyboard
extern LPDIRECTINPUTDEVICE  lpdimouse;  // dinput mouse
// these contain the target records for all di input packets
extern UCHAR keyboard_state[256]; // contains keyboard state table
extern DIMOUSESTATE mouse_state;  // contains state of mouse

#endif