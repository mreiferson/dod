/*
 * Do Or Die© PeRpLeXeD Productions
 *
 * Programmed By:  Matt Reiferson
 *
 * Module:      dodmemcpy.h
 * Description: include file for memcpy.cpp
 *
 */

#ifndef DOD_MEMCPYH
#define DOD_MEMCPYH

typedef void (*memcpy_funcptr)(void *des, void *src, long count);

extern memcpy_funcptr systovid_memcpy;
extern memcpy_funcptr blit_memcpy;

void fpucpy(void *des, void *src, long count);
void fpucpya(void *des, void *src, long count);
void Pickmemcpy(void);

#endif