/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      doddsound.h
 * Description: include file for DirectSound utility functions
 *
 */

#ifndef DOD_DODDSOUNDH
#define DOD_DODDSOUNDH

#define MAX_SOUNDS     64 // max number of sounds in system at once 

#define SOUND_NULL     0
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

// voc file defines
#define NVB_SIZE       6  // size of new voice block in bytes

// MACROS /////////////////////////////////////////////////

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

// this holds a single sound
typedef struct pcm_sound_typ {
    LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
    int state;                      // state of the sound
    int rate;                       // playback rate
    int size;                       // size of sound
    int id;                         // id number of the sound
} pcm_sound, *pcm_sound_ptr;

// sound
int Load_VOC(char *filename);
int Load_WAV(char *filename, int control_flags);
int Replicate_Sound(int source_id);
int Play_Sound(int id, int flags = 0, int volume = 0, int rate = 0, int pan = 0);
int Stop_Sound(int id);
int Stop_All_Sounds(void);
int DSInit(HWND hwnd);
int DSShutdown(void);
int Delete_Sound(int id);
int Delete_All_Sounds(void);
int Status_Sound(int id);
int Set_Sound_Volume(int id, int vol);
int Set_Sound_Freq(int id, int freq);
int Set_Sound_Pan(int id, int pan);

extern LPDIRECTSOUND            lpDS;                           // directsound interface pointer
extern LPDIRECTSOUNDBUFFER  lpDSBPrimary;               // the primary mixing buffer
extern DSBUFFERDESC         dsbd;                           // directsound description
extern DSCAPS                   dscaps;                     // directsound caps
extern HRESULT                  dsresult;                   // general directsound result
extern DSBCAPS                  dsbcaps;                        // directsound buffer caps
extern pcm_sound                sound_fx[MAX_SOUNDS];   // the array of secondary sound buffers
extern WAVEFORMATEX         pcmwf;                      // generic waveformat structure

#endif