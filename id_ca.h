#ifndef __ID_CA_H_
#define __ID_CA_H_

//===========================================================================

#define NUMMAPS         60
#define MAPPLANES       3
#include "wl_def.h"
#ifndef VIEASM
#define UNCACHEAUDIOCHUNK(chunk) {if(audiosegs[chunk]) {free(audiosegs[chunk]); audiosegs[chunk]=NULL;}}
#endif
//===========================================================================

typedef struct
{
    int planestart[MAPPLANES];
    unsigned short    planelength[MAPPLANES];
    unsigned short    width,height;
    char    name[16];
} maptype;

//===========================================================================

extern  unsigned short *mapsegs[MAPPLANES];
extern  maptype *mapheaderseg[NUMMAPS];

#if !defined(SEGA_SATURN) || !defined(VIEASM)
extern  unsigned char    *audiosegs[NUMSNDCHUNKS];
#endif
extern  unsigned char    *grsegs[NUMCHUNKS];

extern  char  extension[5];
#ifndef SEGA_SATURN
extern  char  graphext[5];
#ifndef VIEASM
extern  char  audioext[5];
#endif
#endif

//===========================================================================

boolean CA_LoadFile (const char *filename, void **ptr);
boolean CA_WriteFile (const char *filename, void *ptr, int length);

int CA_RLEWCompress (unsigned short *source, int length, unsigned short *dest, unsigned short rlewtag);

void CA_RLEWexpand (unsigned short *source, unsigned short *dest, int length, unsigned short rlewtag);

void CA_Startup (void);
void CA_Shutdown (void);

#ifndef SEGA_SATURN
int CA_CacheAudioChunk (int chunk);
void CA_LoadAllSounds (void);
#endif

void CA_CacheGrChunks (void);
void CA_CacheMap (int mapnum);

void CA_CannotOpen(const char *name);

#endif //__ID_CA_H_
