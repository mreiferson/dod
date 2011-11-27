/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodmenu.h
 * Description: the include file for menu routines, for the main DOD Menu screen
 *
 */

#ifndef DOD_MENUH
#define DOD_MENUH

/*
 * Function Declarations
 */
int XYInRect(int x, int y, RECT *rect);
void DrawButtonDown(int button_num);
void UpdateMenu(void);

#endif