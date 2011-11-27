/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodutil.cpp
 * Description: general utility functions, like error reporting
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "dodutil.h"

FILE    *fp_error = NULL;  // general error file
DWORD   start_clock_count = 0; // used for timing

DWORD Get_Clock(void)
{
    // return time
    return(GetTickCount());
}

DWORD Start_Clock(void)
{
    // init the count
    return(start_clock_count = Get_Clock());
}

DWORD Wait_Clock(DWORD count)
{
    // wait specified count
    while ((Get_Clock() - start_clock_count) < count);
    return(Get_Clock());
}

int Open_Error_File(char *filename)
{
    FILE *fp_temp; // temporary file
    
    // test if this file is valid
    if ((fp_temp = fopen(filename, "w")) == NULL) {
        return(0);
    }
    
    // close old file if there was one
    if (fp_error) {
        Close_Error_File();
    }
    
    // assign new file
    fp_error = fp_temp;
    
    // write out header
    Write_Error("\nOpening Error Output File (%s):\n", filename);
    
    // return success
    return(1);
}

int Close_Error_File(void)
{
    if (fp_error) {
        // write close file string
        Write_Error("\nClosing Error Output File.");
        
        // close the file handle
        fclose(fp_error);
        fp_error = NULL;
        
        // return success
        return(1);
    } else {
        return(0);
    }
}

int Write_Error(char *string, ...)
{
    char buffer[80]; // working buffer
    
    va_list arglist; // variable argument list
    
    // make sure both the error file and string are valid
    if (!string || !fp_error) {
        return(0);
    }
    
    // print out the string using the variable number of arguments on stack
    va_start(arglist, string);
    vsprintf(buffer, string, arglist);
    va_end(arglist);
    
    // write string to file
    fprintf(fp_error, buffer);
    
    // return success
    return(1);
}

int Collision_Test(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    // get the radi of each rect
    int width1  = (w1 >> 1) - (w1 >> 3);
    int height1 = (h1 >> 1) - (h1 >> 3);
    
    int width2  = (w2 >> 1) - (w2 >> 3);
    int height2 = (h2 >> 1) - (h2 >> 3);
    
    // compute center of each rect
    int cx1 = x1 + width1;
    int cy1 = y1 + height1;
    
    int cx2 = x2 + width2;
    int cy2 = y2 + height2;
    
    // compute deltas
    int dx = abs(cx2 - cx1);
    int dy = abs(cy2 - cy1);
    
    // test if rects overlap
    if (dx < (width1 + width2) && dy < (height1 + height2)) {
        return(1);
    } else
        // else no collision
    {
        return(0);
    }
}