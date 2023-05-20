// WL_UTILS.H

#ifndef __WL_UTILS_H_
#define __WL_UTILS_H_

#include "wl_def.h"

#define FRACBITS         16
#if SDL_MAJOR_VERSION == 2 || SDL_MAJOR_VERSION == 3 
#ifdef _WIN64
#define WL_GetTicks() SDL_GetTicks64()
#else
#define WL_GetTicks() SDL_GetTicks()
#endif
#else
#define WL_GetTicks() SDL_GetTicks()
#endif

#define GetTicks() ((WL_GetTicks()*7)/100)

#define GetTimeCount()  ((WL_GetTicks()*7)/100)

#define ISPOINTER(x) ((((uintptr_t)(x)) & ~0xffff) != 0)
extern void     *wsafe_malloc (size_t size, const char *fname, unsigned int line);
extern wlinline fixed    FixedMul (fixed a, fixed b);
extern wlinline fixed    FixedDiv (fixed a, fixed b);
#define SafeMalloc(s)    wsafe_malloc ((s),__FILE__,__LINE__)
extern wlinline unsigned short     READWORD (unsigned char *ptr);
extern wlinline unsigned int READLONGWORD (unsigned char *ptr);

extern wlinline char* wlitoa(int value, char* string, int radix);
extern wlinline char* wlltoa(long value, char* string, int radix);

#if defined(SEGA_SATURN)
extern short* LoadFile(char* filename, long* fileSize);
extern int GetFileSize(int file_id);
extern unsigned short SWAP_BYTES_16(unsigned short a);
extern unsigned int SWAP_BYTES_32(unsigned int a);
extern fixed MTH_Mul2(fixed a, fixed b);
extern void memcpyl(void* dst, void* src, int size);
extern void* memset4_fast(void*, long, size_t);
short atan2fix(fixed x, fixed y);
#endif

#endif //__WL_UTILS_H_
