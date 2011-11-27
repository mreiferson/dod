/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodintro.h
 * Description: the include file for introduction routines, and updating of them
 *
 */

#ifndef DOD_INTROH
#define DOD_INTROH

/*
 * Function Declarations
 */
void UpdateXGamesIntro(BOOL *ttl);
void UpdateDODIntro(BOOL *ttl);
void UpdateDSoftIntro(BOOL *ttl);
void UpdateCredits(BOOL *ttl);
void ClearCreditsCount(void);

#endif