/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodworld.h
 * Description: the include file for world functions, loading world files, initialization & deletion
 *
 */

#ifndef DOD_WORLDH
#define DOD_WORLDH

#define WORLD_SIZE_X                    128
#define WORLD_SIZE_Y                    128

/*
 * Typedefs
 */
// enumeration to signify where a base tile is covered with fog of war, if anywhere
typedef enum _TILEFOG {
    NONE, TOP, BOTTOM, LEFT, RIGHT, U_LEFT, U_RIGHT, B_LEFT, B_RIGHT
} TILEFOG;

// the structure representing the BASE layer in a world
typedef struct _BASE {
    RECT    walkable;     // where in this tile can an entity walk?
    int     discovered;   // has this tile being discovered?
    int     covered;      // is this tile covered? IE has an object on it
    TILEFOG fogged;       // enumeration above, signifies where the tile is fogged
    int     tileindex;    // the index into the tileset to show what tile pic it is
} BASE;

// the structure representing an object in the world
typedef struct _OBJECT {
    // nutin yet
} OBJECT;

// the structure representing a being in the world
typedef struct _BEING {
    // nutin yet
} BEING;

// the structure representing the entire world
typedef struct _WORLD {
    BASE base[WORLD_SIZE_Y][WORLD_SIZE_X];  // the base layer
    OBJECT object;        // the object layer
    BEING being;          // the being layer
} WORLD;

/*
 * Variables
 */
extern int       world_offset_x,               // The x offset for the current viewport
       world_offset_y;               // the y offset for the current viewport
extern WORLD     world;                        // the world the game lives in

/*
 * Function Declarations
 */
void InitWorld(void);
void KillWorld(void);
void FillInMM(void);

#endif