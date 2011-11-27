/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodsound.cpp
 * Description: this file is so the sound ID's can be declared here, and no external access is needed
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
#include "doddsound.h"
#include "dodsound.h"

int ClickSndID; // the click sound id #
int XGamesSndID; // the xgames lightning sound id #

/*
 * DODPlayClickSound:
 *    This function plays the clicking sound
 */
void DODPlayClickSound(void)
{
    Play_Sound(ClickSndID);
}

/*
 * DODLoadClickSound:
 *    This function loads the clicking sound
 */
void DODLoadClickSound(void)
{
    ClickSndID = Load_WAV(".\\Sound\\click.wav", NULL);
}

/*
 * DODDeleteClickSound:
 *    This function deletes the clicking sound
 */
void DODDeleteClickSound(void)
{
    Delete_Sound(ClickSndID);
}

/*
 * DODPlayXGamesSound:
 *    This function plays the XGames sound
 */
void DODPlayXGamesSound(void)
{
    Play_Sound(XGamesSndID);
}

/*
 * DODLoadXGamesSound:
 *    This function loads the XGames sound
 */
void DODLoadXGamesSound(void)
{
    XGamesSndID = Load_WAV(".\\Sound\\xlogo.wav", NULL);
}

/*
 * DODDeleteXGamesSound:
 *    This functions deleted the XGames Sound
 */
void DODDeleteXGamesSound(void)
{
    Delete_Sound(XGamesSndID);
}