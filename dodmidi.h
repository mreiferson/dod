/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodmidi.h
 * Description: the include file for MCI MM musical routines, playing, resuming, loading, stoping midi
 *
 */

#ifndef DOD_MIDIH
#define DOD_MIDIH

/*
 * Function Declarations
 */
BOOL MidiPlay(const char *Filename, BOOL loop);
BOOL MidiStop(void);
BOOL MidiPause(void);
BOOL MidiResume(void);
BOOL MidiRestart(void);
BOOL IsMidiLooping(void);

#endif