/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodengine.h
 * Description: the include file for the game engine and it's associated routines
 *
 */

#ifndef DOD_ENGINEH
#define DOD_ENGINEH

/*
 * Defines
 */
#define VIEWABLE_W                  480
#define VIEWABLE_H                  480
#define BASE_TILE_W                 32
#define BASE_TILE_H                 32
#define SCREEN_NUMTILES_X           (VIEWABLE_W/BASE_TILE_W)
#define SCREEN_NUMTILES_Y           (VIEWABLE_H/BASE_TILE_H)
#define WORLD_SIZE_X                    128
#define WORLD_SIZE_Y                    128
#define WORLD_PIXEL_SIZE_X          (WORLD_SIZE_X*BASE_TILE_W)
#define WORLD_PIXEL_SIZE_Y          (WORLD_SIZE_Y*BASE_TILE_H)
#define MAX_WORLDOFFSETX            (WORLD_PIXEL_SIZE_X-VIEWABLE_W)
#define MAX_WORLDOFFSETY            (WORLD_PIXEL_SIZE_Y-VIEWABLE_H)

/*
 * Function Declarations
 */
void GameMain(void);
void UpdateGame(void);
void RenderMM(void);
void RenderBase(void);

#endif