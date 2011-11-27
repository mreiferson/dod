/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodutil.h
 * Description: include file for general utility functions
 *
 */

#ifndef DOD_DODUTILH
#define DOD_DODUTILH

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// general utility functions
DWORD Get_Clock(void);
DWORD Start_Clock(void);
DWORD Wait_Clock(DWORD count);

// error functions
int Open_Error_File(char *filename);
int Close_Error_File(void);
int Write_Error(char *string, ...);
// general util
int Collision_Test(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void ChangeFont(const char *FontName, int Width, int Height, int Attributes);
void SetFont(HDC sdc);

extern FILE                 *fp_error;            // general error file
extern DWORD                start_clock_count;    // used for timing

#endif