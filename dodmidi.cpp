/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodmidi.cpp
 * Description: MCI MM musical routines, playing, resuming, loading, stoping midi
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "dodmidi.h"

extern HWND main_window_handle; // external window handle needed for midi support

BOOL MidiLoop = FALSE; // Variable which holds whether a midi playing is supposed to loop

/*
 * MidiPlay:
 *    Plays a given midi file and sets the flag on whether or not to loop it
 */
BOOL MidiPlay(const char *Filename, BOOL loop)
{
    char buffer[256]; // buffer used to pass MCI strings
    
    // make sure a filename was passed
    if (Filename == NULL) {
        return FALSE;
    }
    
    // put the MCI string to open a midi into the buffer
    sprintf(buffer, "open %s type sequencer alias MUSIC", Filename);
    
    // send the MCI string to stop all midis
    if (mciSendString("close all", NULL, 0, NULL) != 0) {
        return(FALSE);
    }
    
    // send the MCI string to open the midi
    if (mciSendString(buffer, NULL, 0, NULL) != 0) {
        return(FALSE);
    }
    
    // send the MCI string to play the midi and notify us of events
    if (mciSendString("play MUSIC from 0 notify", NULL, 0, main_window_handle) != 0) {
        return(FALSE);
    }
    
    // set the global MidiLoop variable to whatever we want for this midi
    MidiLoop = loop;
    
    // return to caller
    return TRUE;
}

/*
 * MidiStop:
 *    Stops any currently playing midi
 */
BOOL MidiStop(void)
{
    // send the MCI string to stop all midis
    if (mciSendString("close all", NULL, 0, NULL) != 0) {
        return(FALSE);
    }
    
    // Set the looping variable to false so it doesn't just magically restart
    MidiLoop = FALSE;
    
    // return to caller
    return TRUE;
}

/*
 * MidiPause:
 *    Pauses any currently playing midi
 */
BOOL MidiPause(void)
{
    // send the MCI String to Pause midi currently playing
    if (mciSendString("stop MUSIC", NULL, 0, NULL) != 0) {
        return(FALSE);
    }
    
    // return to caller
    return TRUE;
}

/*
 * MidiResume:
 *    Resumes a midi that was previously paused
 */
BOOL MidiResume(void)
{
    // send the MCI String to Resume midi that is currently paused
    if (mciSendString("play MUSIC notify", NULL, 0, main_window_handle) != 0) {
        return(FALSE);
    }
    
    // return to caller
    return TRUE;
}

/*
 * MidiRestart:
 *    Restarts a midi that is loaded from the beginning
 */
BOOL MidiRestart(void)
{
    // send the MCI String to restart a midi that is loaded
    if (mciSendString("play MUSIC from 0 notify", NULL, 0, main_window_handle) != 0) {
        return(FALSE);
    }
    
    // return to caller
    return TRUE;
}

/*
 * IsMidiLooping:
 *    Returns TRUE or FALSE depending on the state of MidiLoop
 */
BOOL IsMidiLooping(void)
{
    return MidiLoop;
}