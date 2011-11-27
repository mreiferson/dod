/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodintro.cpp
 * Description: introduction routines, and updating of them
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
#include "doddsound.h"
#include "doddinput.h"
#include "dodgfx.h"
#include "dodintro.h"
#include "dodmidi.h"
#include "dodsound.h"
#include "dodutil.h"

/*
 * UpdateXGamesIntro()
 *    Does the processing for XGames LLC Intro
 */
void UpdateXGamesIntro(BOOL *ttl)
{
    RECT SrcRect, DesRect; // Source And Destination Rectangles For Blit
    static int loaded = 0; // Variable To Determine If Function Is Being Run For
    // The First Time
    static int count = 0;  // used for timing purposes
    HRESULT ret;
    
    // Initialize the clock for this iteration
    Start_Clock();
    
    // Determine If This Is The First Run
    if (!loaded) {
        DODLoadXGamesSound(); // load the xgames sound
    }
    
    // Set Source And Destinationg Rectangles The Size Of The Screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    // Blit The XGames PIC
    lpDDSBack->Blt(&DesRect, lpDDSXGames, &SrcRect, DDBLT_WAIT, NULL);
    // Flip To The Primary Surface
    ret = DDFlip();
    if (ret == DDERR_SURFACELOST) {
        RestoreGraphics();
    }
    
    // play the sound right before the flip....good timing
    if (!loaded) {
        DODPlayXGamesSound(); // Play The XGames Lightning Sound
        MidiPlay(".\\Music\\intro.mid", TRUE); // play the intro midi
        loaded = 1;           // Stop This Code From Happening Again
    }
    
    // Wait for 1/10 of a second
    Wait_Clock(100);
    
    // see if it's time to kill this intro phase
    if (++count > 49) {
        *ttl = FALSE;
    } else {
        *ttl = TRUE;
    }
}

// The Intro Scrolling Text Scrolled During DOD_DODINTRO
// Pointer Of Strings So It Can Have A Variable Amount Of Lines
char *dodintro_text[128] = {
    // Limit
    /* 1*/"Years Of Fighting, Death, And Evil Have Overcome The Good People Of Goshia.",
    /* 2*/"They Have Fought With Bravery And Honor, But Have Been Forced To The End.",
    /* 3*/"In Their Last Hope For Survival They Have Called Upon You To Lead Them To",
    /* 4*/"Victory And Return Them To Their Glory.  With A Band Of Rebel Soldiers",
    /* 5*/"And Little Amounts Of Resources You Must Rise From Ashes And Overcome",
    /* 6*/"A Devistatingly Strong Enemy, The Kintos.  They Breed Well Trained Blood",
    /* 7*/"Thirsty Warriors Not Afraid To Die For Their Evil Cause.  But There Is",
    /* 8*/"Hope, Many Rebel Groups Are Sprouting All Over The Mainland Striving For",
    /* 9*/"Victory As Well.  It Will Be Difficult To Unite These Clans.  But Creating",
    /*10*/"A Monstrous Human Resitance, And Using The Various Abilities Of The",
    /*11*/"Groups Will Be Crucial To The Success Of Your Mission.  There Is No",
    /*12*/"Turning Back Now, It's Do Or Die!"
};

// Number Of Lines Of Text For The Intro
#define NUM_INTRO_LINES 12

/*
 * UpdateDODIntro:
 *    Updates the DOD Intro Music And GFX
 */
void UpdateDODIntro(BOOL *ttl)
{
    RECT SrcRect, DesRect; // Source And Destination Rectangles
    RGB rgb;               // RGB Triplet For BlitString
    static int count[NUM_INTRO_LINES] = { 480, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // Counters For Each Line Of Intro Text
    int i;                 // Variable For Looping
    HRESULT ret;
    
    // Set Source And Destination Rectangles To Size Of Screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    // Blit the DOD PIC to the back surface
    lpDDSBack->Blt(&DesRect, lpDDSDOD, &SrcRect, DDBLT_WAIT, NULL);
    
    // Init The Clock (Used For Wait_Clock)
    Start_Clock();
    
    // Loop through each line of text
    for (i = 0; i < NUM_INTRO_LINES; i++) {
        // If the line of text has been initialized
        if (count[i]) {
            // set the red, green, blue color components based on counter
            rgb.r = count[i] % 256 + 31;
            rgb.g = 0;
            rgb.b = 0;
            // blit the string to the back surface
            BlitString(dodintro_text[i], 15, count[i], &rgb);
            // decrement the counter by one (moves upward)
            count[i] -= 1;
            // determine if it's time to kill this line
            if ((rgb.r - 31) == 0) {
                count[i] = 0;
            }
            // determing if it's time to start the next line going
            if (i != (NUM_INTRO_LINES - 1))
                if (count[i] < 460 && !count[i + 1]) {
                    count[i + 1] = 480;
                }
        }
    }
    
    // hold this intro to about 10 frames per second (100 ms)
    Wait_Clock(100);
    
    // flip to the primary surface
    ret = DDFlip();
    if (ret == DDERR_SURFACELOST) {
        RestoreGraphics();
    }
    
    // see if it's time to kill this intro phase
    *ttl = FALSE;
    for (i = 0; i < NUM_INTRO_LINES; i++) {
        if (count[i] > 0) {
            *ttl = TRUE;
        }
    }
}

/*
 * UpdateDSoftIntro:
 *    Updates the DSoft Graphic Intro
 */
void UpdateDSoftIntro(BOOL *ttl)
{
    static int count = 0; // used for timing purposes
    HRESULT ret;
    
    // init the clock for timing
    Start_Clock();
    
    // Source and Destination rectangles
    RECT SrcRect, DesRect;
    
    // Set the source and destination rectangles to the size of the screen
    SetRect(&SrcRect, 0, 0, 640, 480);
    SetRect(&DesRect, 0, 0, 640, 480);
    // blit the DSoft pic to the back surface
    lpDDSBack->Blt(&DesRect, lpDDSDSoft, &SrcRect, DDBLT_WAIT, NULL);
    // flip to the front surface
    ret = DDFlip();
    if (ret == DDERR_SURFACELOST) {
        RestoreGraphics();
    }
    
    // wait for 1/10 of a second
    Wait_Clock(100);
    
    // see if it's time to get the hell outa here
    if (++count > 49) {
        *ttl = FALSE;
    } else {
        *ttl = TRUE;
    }
}

// the lines of text for the credits screen
// Pointer Of Strings So It Can Have A Variable Amount Of Lines
char *dodcredit_text[128] = {
    /* 1*/"DOD Design By:",
    /* 2*/"Matt Reiferson, Eric Farkas",
    /* 3*/"",
    /* 4*/"DOD Programming By:",
    /* 5*/"Matt Reiferson",
    /* 6*/"",
    /* 7*/"DOD Music/Sound By:",
    /* 8*/"Matt Reiferson, Eric Farkas, Andre' LaMothe",
    /* 9*/"",
    /*10*/"DOD Graphics By:",
    /*11*/"Matt Reiferson, Eric Farkas, Andre' LaMothe, 3D Artist",
    /*12*/"",
    /*13*/"Special Thanks To:",
    /*14*/"Andre' LaMothe & xTreme Games",
    /*15*/"The Families Of Eric & Matt",
    /*16*/"",
    /*17*/"",
    /*18*/"",
    /*19*/"",
    /*20*/"",
    /*21*/"",
    /*22*/"Hmmmm, your STILL reading this?",
    /*23*/"HIT ENTER ALREADY!!!",
    /*24*/"",
    /*25*/"",
    /*26*/"",
    /*27*/"",
    /*28*/"There's No oNe eLsE, I WON'T BE MAD IF YOU HIT ENTER!!!",
    /*29*/"Come on, Don't worry, you can leave......",
    /*30*/"",
    /*31*/"",
    /*32*/"",
    /*33*/"",
    /*34*/"Ok, Ok, There was one more @$$%*&# I forgot, But he didn't do much....",
    /*35*/"HIS name was Barbra...",
    /*36*/"Hmmm..... Isn't it spelled Barbara?",
    /*37*/"Nah, that doesn't look right either....",
    /*38*/"Oh!  Your STILL looking at this?  I didn't even realize!",
    /*39*/"",
    /*40*/"Now listen carefully, this is REALLY the last line....."
};

#define NUM_CREDIT_LINES 40

// the counters for movement and color
int count[NUM_CREDIT_LINES] = { 480, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0, // Counters For Each Line Of Intro Text
                                0, 0, 0, 0
                              };

/*
 * UpdateCredits:
 *    Updates the credits screen
 */
void UpdateCredits(BOOL *ttl)
{
    RGB rgb;               // RGB Triplet For BlitString
    HRESULT ret;
    
    int special[NUM_CREDIT_LINES] = { 1, 0, 0, 1, // Special Color For Each Line Of Intro Text
                                      0, 0, 1, 0, // Special Color For Each Line Of Intro Text
                                      0, 1, 0, 0, // Special Color For Each Line Of Intro Text
                                      1, 0, 0, 0, // Special Color For Each Line Of Intro Text
                                      0, 0, 0, 0, // Special Color For Each Line Of Intro Text
                                      0, 2, 2, 0, // Special Color For Each Line Of Intro Text
                                      0, 0, 0, 2, // Special Color For Each Line Of Intro Text
                                      2, 0, 0, 0, // Special Color For Each Line Of Intro Text
                                      0, 2, 2, 2, // Special Color For Each Line Of Intro Text
                                      2, 2, 0, 2
                                    };
    int i;  // Variable For Looping
    
    // clear out the back surface
    DDFillSurface(lpDDSBack, 0);
    
    // Init The Clock (Used For Wait_Clock)
    Start_Clock();
    
    // Loop through each line of text
    for (i = 0; i < NUM_CREDIT_LINES; i++) {
        // If the line of text has been initialized
        if (count[i]) {
            // set the red, green, blue color components based on counter
            if (special[i] == 0) {
                if (count[i] > 224) {
                    rgb.r = 255 - (count[i] % 256 + 31);
                } else {
                    rgb.r = count[i] % 256 + 31;
                }
                rgb.g = 0;
                rgb.b = 0;
            } else if (special[i] == 1) {
                if (count[i] > 224) {
                    rgb.r = 255 - (count[i] % 256 + 31);
                } else {
                    rgb.r = count[i] % 256 + 31;
                }
                rgb.g = 0;
                if (count[i] > 224) {
                    rgb.b = 255 - (count[i] % 256 + 31);
                } else {
                    rgb.b = count[i] % 256 + 31;
                }
            } else if (special[i] == 2) {
                if (count[i] > 224) {
                    rgb.r = 255 - (count[i] % 256 + 31);
                } else {
                    rgb.r = count[i] % 256 + 31;
                }
                if (count[i] > 224) {
                    rgb.g = 255 - (count[i] % 256 + 31);
                } else {
                    rgb.g = count[i] % 256 + 31;
                }
                rgb.b = 0;
            }
            // blit the string to the back surface
            BlitString(dodcredit_text[i], 30, count[i], &rgb);
            // decrement the counter by one (moves upward)
            count[i] -= 1;
            // determing if it's time to start the next line going
            if (i != (NUM_CREDIT_LINES - 1))
                if (count[i] < 460 && !count[i + 1]) {
                    count[i + 1] = 480;
                }
        }
    }
    
    // hold this intro to about 10 frames per second (100 ms)
    Wait_Clock(100);
    
    // flip to the primary surface
    ret = DDFlip();
    if (ret == DDERR_SURFACELOST) {
        RestoreGraphics();
    }
    
    // see if it's time to get the hell outa here
    *ttl = FALSE;
    for (i = 0; i < NUM_CREDIT_LINES; i++) {
        if (count[i] > 0) {
            *ttl = TRUE;
        }
    }
}

/*
 * ClearCreditsCount:
 *    Clears The Credit's Line Counter
 */
void ClearCreditsCount(void)
{
    for (int i = 0; i < NUM_CREDIT_LINES; i++) {
        if (i == 0) {
            count[i] = 480;
        } else {
            count[i] = 0;
        }
    }
}