/*
 * Do Or Die� PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      doddsound.cpp
 * Description: DirectSound utility functions
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <dsound.h>
#include "doddsound.h"

// directsound stuff
LPDIRECTSOUND           lpDS;                           // directsound interface pointer
LPDIRECTSOUNDBUFFER lpDSBPrimary;               // the primary mixing buffer
DSBUFFERDESC            dsbd;                           // directsound description
DSCAPS                  dscaps;                     // directsound caps
HRESULT                 dsresult;                   // general directsound result
DSBCAPS                 dsbcaps;                        // directsound buffer caps
pcm_sound               sound_fx[MAX_SOUNDS];   // the array of secondary sound buffers
WAVEFORMATEX            pcmwf;                      // generic waveformat structure

int Load_VOC(char *filename)
{
    int   sound_id = -1,       // id of sound to be loaded
          index,               // looping variable
          data_offset,         // offset to data part of file
          playback_rate,       // playback rate as encoded in file
          data_length;         // length of data
          
    ULONG bytesread = 0,    // actual number of bytes read during file read
          filelength;       // length of file
          
    int   file_handle;      // general file handle
    
    UCHAR *snd_buffer,       // temporary sound buffer to hold voc data
          *audio_ptr_1 = NULL, // data ptr to first write buffer
           *audio_ptr_2 = NULL; // data ptr to second write buffer
           
    DWORD audio_length_1 = 0, // length of first write buffer
          audio_length_2 = 0; // length of second write buffer
          
    // step one: are there any open id's ?
    for (index = 0; index < MAX_SOUNDS; index++) {
        // make sure this sound is unused
        if (sound_fx[index].state == SOUND_NULL) {
            sound_id = index;
            break;
        }
    }
    
    // did we get a free id?
    if (sound_id == -1) {
        return(-1);
    }
    
    // step two: load the voc file off disk
    if ((file_handle = _open(filename, _O_BINARY | _O_RDONLY)) == -1) {
        return(0);
    }
    
    // get size of file so we can allocate temporary read buffer
    filelength = _filelength(file_handle);
    
    // allocate a large enough temporary buffer
    snd_buffer = (UCHAR *)malloc(filelength);
    
    // now read in the data
    bytesread = _read(file_handle, snd_buffer, filelength);
    
    // access all values and decode VOC encoding for data fields
    data_offset    = snd_buffer[20];
    playback_rate = (-1000000 / (snd_buffer[data_offset + 4] - 256));
    data_length   = ((*(int *)(snd_buffer + data_offset)) >> 8);
    
    // set rate and size in data structure
    sound_fx[sound_id].rate  = playback_rate;
    sound_fx[sound_id].size  = data_length;
    sound_fx[sound_id].state = SOUND_LOADED;
    
    // close the file
    _close(file_handle);
    
    // step three: create the sound buffer and copy voc data into buffer
    
    // set up the format data structure
    memset(&pcmwf, 0, sizeof(WAVEFORMATEX));
    
    pcmwf.wFormatTag      = WAVE_FORMAT_PCM;  // pulse code modulation
    pcmwf.nChannels        = 1;                // mono
    pcmwf.nSamplesPerSec   = 11025;            // always this rate
    pcmwf.nBlockAlign         = 1;
    pcmwf.nAvgBytesPerSec  = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
    pcmwf.wBitsPerSample   = 8;
    pcmwf.cbSize          = 0;
    
    // prepare to create sounds buffer
    dsbd.dwSize           = sizeof(DSBUFFERDESC);
    dsbd.dwFlags      = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes = data_length - NVB_SIZE;
    dsbd.lpwfxFormat  = &pcmwf;
    
    // create the sound buffer
    if (lpDS->CreateSoundBuffer(&dsbd, &sound_fx[sound_id].dsbuffer, NULL) != DS_OK) {
        // release memory
        free(snd_buffer);
        // return error
        return(-1);
    }
    
    // copy data into sound buffer
    if (sound_fx[sound_id].dsbuffer->Lock(0,
                                          data_length - NVB_SIZE,
                                          (void **) &audio_ptr_1,
                                          &audio_length_1,
                                          (void **)&audio_ptr_2,
                                          &audio_length_2,
                                          DSBLOCK_FROMWRITECURSOR) != DS_OK) {
        return(0);
    }
    
    // copy first section of circular buffer
    memcpy(audio_ptr_1, snd_buffer + data_offset + NVB_SIZE, audio_length_1);
    
    // copy last section of circular buffer
    memcpy(audio_ptr_2, (snd_buffer + data_offset + NVB_SIZE + audio_length_1), audio_length_2);
    
    // unlock the buffer
    if (sound_fx[sound_id].dsbuffer->Unlock(audio_ptr_1,
                                            audio_length_1,
                                            audio_ptr_2,
                                            audio_length_2) != DS_OK) {
        return(0);
    }
    
    // release the temp buffer
    free(snd_buffer);
    
    // return id
    return(sound_id);
}

int Load_WAV(char *filename, int control_flags)
{
    HMMIO        hwav;                // handle to wave file
    MMCKINFO      parent,              // parent chunk
                  child;               // child chunk
    WAVEFORMATEX wfmtx;               // wave format structure
    int         sound_id = -1,       // id of sound to be loaded
                index;               // looping variable
    UCHAR        *snd_buffer,         // temporary sound buffer to hold voc data
                 *audio_ptr_1 = NULL, // data ptr to first write buffer
                  *audio_ptr_2 = NULL; // data ptr to second write buffer
    DWORD        audio_length_1 = 0,  // length of first write buffer
                 audio_length_2 = 0;  // length of second write buffer
                 
    // step one: are there any open id's ?
    for (index = 0; index < MAX_SOUNDS; index++) {
        // make sure this sound is unused
        if (sound_fx[index].state == SOUND_NULL) {
            sound_id = index;
            break;
        }
    }
    
    // did we get a free id?
    if (sound_id == -1) {
        return(-1);
    }
    
    // set up chunk info structure
    parent.ckid      = (FOURCC)0;
    parent.cksize        = 0;
    parent.fccType       = (FOURCC)0;
    parent.dwDataOffset = 0;
    parent.dwFlags       = 0;
    
    // copy data
    child = parent;
    
    // open the WAV file
    if ((hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF)) == NULL) {
        return(-1);
    }
    
    // descend into the RIFF
    parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF)) {
        // close the file
        mmioClose(hwav, 0);
        // return error, no wave section
        return(-1);
    }
    
    // descend to the WAVEfmt
    child.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hwav, &child, &parent, 0)) {
        // close the file
        mmioClose(hwav, 0);
        // return error, no format section
        return(-1);
    }
    
    // now read the wave format information from file
    if (mmioRead(hwav, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)) {
        // close file
        mmioClose(hwav, 0);
        // return error, no wave format data
        return(-1);
    }
    
    // make sure that the data format is PCM
    if (wfmtx.wFormatTag != WAVE_FORMAT_PCM) {
        // close the file
        mmioClose(hwav, 0);
        // return error, not the right data format
        return(-1);
    }
    
    // now ascend up one level, so we can access data chunk
    if (mmioAscend(hwav, &child, 0)) {
        // close file
        mmioClose(hwav, 0);
        // return error, couldn't ascend
        return(-1);
    }
    
    // descend to the data chunk
    child.ckid = mmioFOURCC('d', 'a', 't', 'a');
    
    if (mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK)) {
        // close file
        mmioClose(hwav, 0);
        // return error, no data
        return(-1);
    }
    
    // finally!!!! now all we have to do is read the data in and
    // set up the directsound buffer
    
    // allocate the memory to load sound data
    snd_buffer = (UCHAR *)malloc(child.cksize);
    
    // read the wave data
    mmioRead(hwav, (char *)snd_buffer, child.cksize);
    
    // close the file
    mmioClose(hwav, 0);
    
    // set rate and size in data structure
    sound_fx[sound_id].rate  = wfmtx.nSamplesPerSec;
    sound_fx[sound_id].size  = child.cksize;
    sound_fx[sound_id].state = SOUND_LOADED;
    
    // set up the format data structure
    memset(&pcmwf, 0, sizeof(WAVEFORMATEX));
    
    pcmwf.wFormatTag      = WAVE_FORMAT_PCM;  // pulse code modulation
    pcmwf.nChannels        = 1;                // mono
    pcmwf.nSamplesPerSec   = 11025;            // always this rate
    pcmwf.nBlockAlign         = 1;
    pcmwf.nAvgBytesPerSec  = pcmwf.nSamplesPerSec * pcmwf.nBlockAlign;
    pcmwf.wBitsPerSample   = 8;
    pcmwf.cbSize          = 0;
    
    // prepare to create sounds buffer
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags     = control_flags | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
    dsbd.dwBufferBytes   = child.cksize;
    dsbd.lpwfxFormat = &pcmwf;
    
    // create the sound buffer
    if (lpDS->CreateSoundBuffer(&dsbd, &sound_fx[sound_id].dsbuffer, NULL) != DS_OK) {
        // release memory
        free(snd_buffer);
        // return error
        return(-1);
    }
    
    // copy data into sound buffer
    if (sound_fx[sound_id].dsbuffer->Lock(0,
                                          child.cksize,
                                          (void **) &audio_ptr_1,
                                          &audio_length_1,
                                          (void **)&audio_ptr_2,
                                          &audio_length_2,
                                          DSBLOCK_FROMWRITECURSOR) != DS_OK) {
        return(0);
    }
    
    // copy first section of circular buffer
    memcpy(audio_ptr_1, snd_buffer, audio_length_1);
    
    // copy last section of circular buffer
    memcpy(audio_ptr_2, (snd_buffer + audio_length_1), audio_length_2);
    
    // unlock the buffer
    if (sound_fx[sound_id].dsbuffer->Unlock(audio_ptr_1,
                                            audio_length_1,
                                            audio_ptr_2,
                                            audio_length_2) != DS_OK) {
        return(0);
    }
    
    // release the temp buffer
    free(snd_buffer);
    
    // return id
    return(sound_id);
}

int Replicate_Sound(int source_id)
{
    if (source_id != -1) {
        // duplicate the sound buffer
        // first hunt for an open id
        for (int id = 0; id < MAX_SOUNDS; id++) {
            // is this sound open?
            if (sound_fx[id].state == SOUND_NULL) {
                // first make an identical copy
                sound_fx[id] = sound_fx[source_id];
                
                // now actually replicate the directsound buffer
                if (lpDS->DuplicateSoundBuffer(sound_fx[source_id].dsbuffer, &sound_fx[id].dsbuffer) != DS_OK) {
                    // reset sound to NULL
                    sound_fx[id].dsbuffer = NULL;
                    sound_fx[id].state    = SOUND_NULL;
                    // return error
                    return(-1);
                }
                
                // now fix up id
                sound_fx[id].id = id;
                // return replicated sound
                return(id);
            }
        }
    } else {
        return(-1);
    }
    
    // else failure
    return(-1);
}

int DSInit(HWND hwnd)
{
    static int first_time = 1; // used to track the first time the function is entered
    int index;
    
    // test for very first time
    if (first_time) {
        // clear everything out
        memset(sound_fx, 0, sizeof(pcm_sound) * MAX_SOUNDS);
        
        // reset first time
        first_time = 0;
        
        // create a directsound object
        if (DirectSoundCreate(NULL, &lpDS, NULL) != DS_OK) {
            return(0);
        }
        
        // set cooperation level
        if (lpDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL) != DS_OK) {
            return(0);
        }
    }
    
    // initialize the sound fx array
    for (index = 0; index < MAX_SOUNDS; index++) {
        // test if this sound has been loaded
        if (sound_fx[index].dsbuffer) {
            // stop the sound
            sound_fx[index].dsbuffer->Stop();
            // release the buffer
            sound_fx[index].dsbuffer->Release();
        }
        // clear the record out
        memset(&sound_fx[index], 0, sizeof(pcm_sound));
        
        // now set up the fields
        sound_fx[index].state = SOUND_NULL;
        sound_fx[index].id    = index;
    }
    
    // return sucess
    return(1);
}

int DSShutdown(void)
{
    // first turn all sounds off
    Stop_All_Sounds();
    
    // now release all sound buffers
    for (int index = 0; index < MAX_SOUNDS; index++)
        if (sound_fx[index].dsbuffer) {
            sound_fx[index].dsbuffer->Release();
        }
        
    // now release the directsound interface itself
    lpDS->Release();
    lpDS = NULL;
    
    // return success
    return(1);
}

int Play_Sound(int id, int flags, int volume, int rate, int pan)
{
    if (sound_fx[id].dsbuffer) {
        // reset position to start
        if (sound_fx[id].dsbuffer->SetCurrentPosition(0) != DS_OK) {
            return(0);
        }
        // play sound
        if (sound_fx[id].dsbuffer->Play(0, 0, flags) != DS_OK) {
            return(0);
        }
    }
    
    // return success
    return(1);
}

int Set_Sound_Volume(int id, int vol)
{
    if (sound_fx[id].dsbuffer->SetVolume(DSVOLUME_TO_DB(vol)) != DS_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

int Set_Sound_Freq(int id, int freq)
{
    if (sound_fx[id].dsbuffer->SetFrequency(freq) != DS_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

int Set_Sound_Pan(int id, int pan)
{
    if (sound_fx[id].dsbuffer->SetPan(pan) != DS_OK) {
        return(0);
    }
    
    // return success
    return(1);
}

int Stop_Sound(int id)
{
    if (sound_fx[id].dsbuffer) {
        sound_fx[id].dsbuffer->Stop();
        sound_fx[id].dsbuffer->SetCurrentPosition(0);
    }
    
    // return success
    return(1);
}

int Delete_All_Sounds(void)
{
    for (int index = 0; index < MAX_SOUNDS; index++) {
        Delete_Sound(index);
    }
    
    // return success always
    return(1);
}

int Delete_Sound(int id)
{
    // first stop it
    if (!Stop_Sound(id)) {
        return(0);
    }
    
    // now delete it
    if (sound_fx[id].dsbuffer) {
        // release the com object
        sound_fx[id].dsbuffer->Release();
        sound_fx[id].dsbuffer = NULL;
    }
    
    // return success
    return(1);
}

int Stop_All_Sounds(void)
{
    for (int index = 0; index < MAX_SOUNDS; index++) {
        Stop_Sound(index);
    }
    
    // return success
    return(1);
}

int Status_Sound(int id)
{
    ULONG status;
    
    if (sound_fx[id].dsbuffer) {
        // get the status
        sound_fx[id].dsbuffer->GetStatus(&status);
        
        // return the status
        return(status);
    } else { // total failure
        return(-1);
    }
}