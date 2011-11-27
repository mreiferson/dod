/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodinput.h
 * Description: the include file for input routines, includes updating of variables
 *
 */

#ifndef DOD_INPUTH
#define DOD_INPUTH

/*
 * Defines
 */
#define DOD_CURSORNORM              0
#define DOD_CURSORLEFT              1
#define DOD_CURSORRIGHT             2
#define DOD_CURSORUP                    3
#define DOD_CURSORDOWN              4
#define DOD_CURSORUPLEFT            5
#define DOD_CURSORUPRIGHT           6
#define DOD_CURSORDOWNLEFT          7
#define DOD_CURSORDOWNRIGHT     8
#define DOD_CURSOROFFNORM           9
#define DOD_CURSOROFFLEFT           10
#define DOD_CURSOROFFRIGHT          11
#define DOD_CURSOROFFUP             12
#define DOD_CURSOROFFDOWN           13
#define DOD_CURSOROFFUPLEFT     14
#define DOD_CURSOROFFUPRIGHT        15
#define DOD_CURSOROFFDOWNLEFT       16
#define DOD_CURSOROFFDOWNRIGHT  17

/*
 * Variables
 */
extern int MouseX, MouseY;
extern BYTE MouseLB, MouseRB;

/*
 * Function Declarations
 */
void UpdateInput(void);
void CheckMouseExtent(void);

#endif