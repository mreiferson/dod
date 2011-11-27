/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodworld.cpp
 * Description: world functions, loading world files, initialization & deletion
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
#include "dodddraw.h"
#include "dodgfx.h"
#include "dodworld.h"
#include "dodutil.h"

WORLD      world;                       // The world structure
int        world_offset_x = 0,          // The x offset for the current viewport
           world_offset_y = 0;          // the y offset for the current viewport

/*
 * InitWorld:
 *    The function that initializes a world
 */
void InitWorld(void)
{
    int i, j, x, y;  // counting variables
    int y_tables[14][5] = {
        /* 0*/4, 1, 2, 5, 0,
        /* 1*/3, 3, 8, 10, -1,
        /* 2*/3, 4, 9, 11, -1,
        /* 3*/4, 0, 1, 2, 5,
        /* 4*/4, 0, 1, 2, 5,
        /* 5*/4, 6, 7, 12, 13,
        /* 6*/4, 0, 1, 2, 5,
        /* 7*/4, 6, 12, 13, 7,
        /* 8*/3, 8, 3, 10, -1,
        /* 9*/3, 9, 11, 4, -1,
        /*10*/4, 12, 13, 7, 6,
        /*11*/4, 12, 13, 7, 6,
        /*12*/3, 9, 4, 11, -1,
        /*13*/3, 10, 8, 3, -1
    };
    int x_tables[14][5] = {
        /* 0*/4, 1, 3, 8, 0,
        /* 1*/3, 5, 2, 10, -1,
        /* 2*/4, 3, 0, 8, 1,
        /* 3*/3, 4, 6, 13, -1,
        /* 4*/4, 3, 1, 0, 8,
        /* 5*/3, 2, 5, 10, -1,
        /* 6*/3, 6, 4, 13, -1,
        /* 7*/4, 7, 12, 11, 9,
        /* 8*/4, 9, 11, 12, 7,
        /* 9*/4, 8, 0, 1, 3,
        /*10*/4, 11, 9, 12, 7,
        /*11*/3, 10, 5, 2, -1,
        /*12*/3, 13, 6, 4, -1,
        /*13*/4, 9, 12, 11, 7
    };
    
    // kill old world_offset coordinates
    world_offset_x = 0;
    world_offset_y = 0;
    
    // seed the randomize number generator
    srand(Get_Clock());
    
    for (y = 0; y < WORLD_SIZE_Y; y++) {
        for (x = 0; x < WORLD_SIZE_X; x++) {
            if (x == 0 && y == 0) {
                world.base[y][x].tileindex = 0;
            } else {
                if (y != 0) {
                    for (i = 0; i < 14; i++) {
                        if (world.base[y - 1][x].tileindex == i) {
                            world.base[y][x].tileindex = y_tables[i][1 + rand() % y_tables[i][0]];
                        }
                    }
                } else {
                    for (i = 0; i < 14; i++) {
                        if (world.base[y][x - 1].tileindex == i) {
                            world.base[y][x].tileindex = x_tables[i][1 + rand() % x_tables[i][0]];
                        }
                    }
                }
            }
        }
    }
    
    int matches[2] = { -1, -1 };
    int mcount = 0;
    
    for (y = 1; y < WORLD_SIZE_Y; y++) {
        for (x = 1; x < WORLD_SIZE_X; x++) {
            for (i = 0; i < y_tables[world.base[y - 1][x].tileindex][0]; i++) {
                for (j = 0; j < x_tables[world.base[y][x - 1].tileindex][0]; j++) {
                    if (x_tables[world.base[y][x - 1].tileindex][1 + j] == y_tables[world.base[y - 1][x].tileindex][1 + i]) {
                        matches[mcount] = x_tables[world.base[y][x - 1].tileindex][1 + j];
                        mcount++;
                    }
                }
            }
            world.base[y][x].tileindex = matches[rand() % mcount];
            mcount = 0;
            matches[0] = -1;
            matches[1] = -1;
        }
    }
    world.base[0][0].tileindex = 30;
    world.base[127][0].tileindex = 30;
    world.base[0][127].tileindex = 30;
    world.base[127][127].tileindex = 30;
}

/*
 * KillWorld:
 *    The function that releases all resources used by the world
 */
void KillWorld(void)
{

}

#define myRGB16(r, g, b) ( ((int)((float)r/(float)255*(float)31) << pRed) | ((int)((float)g/(float)255*(float)((2<<(pRed-pGreen))-1)) << pGreen) | (int)((float)b/(float)255*(float)31))

/*
 * FillInMM:
 *    This fills in the surface data for the MiniMap using the world database
 */
void FillInMM(void)
{
    int x, y;
    int mmPitch;
    WORD mmcolor[14] = {        myRGB16(0, 96, 0),
                                myRGB16(0, 64, 0),
                                myRGB16(0, 64, 0),
                                myRGB16(0, 64, 0),
                                myRGB16(0, 64, 0),
                                myRGB16(60, 90, 30),
                                myRGB16(60, 90, 30),
                                myRGB16(120, 180, 55),
                                myRGB16(60, 90, 30),
                                myRGB16(60, 90, 30),
                                myRGB16(120, 140, 55),
                                myRGB16(120, 140, 55),
                                myRGB16(120, 140, 55),
                                myRGB16(120, 140, 55)
                       };
                       
    WORD *mm = DDLockSurface(lpDDSMinimap, &mmPitch);
    
    for (y = 0; y < WORLD_SIZE_Y; y++) {
        for (x = 0; x < WORLD_SIZE_X; x++) {
            mm[(y * mmPitch) + x] = mmcolor[world.base[y][x].tileindex];
        }
    }
    
    DDUnlockSurface(lpDDSMinimap);
}