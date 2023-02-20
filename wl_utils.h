// WL_UTILS.H

#ifndef __WL_UTILS_H_
#define __WL_UTILS_H_

#include "wl_def.h"

#define SafeMalloc(s)    safe_malloc ((s),__FILE__,__LINE__)
#define FRACBITS         16

#define GetTicks() ((SDL_GetTicks()*7)/100)

#define ISPOINTER(x) ((((uintptr_t)(x)) & ~0xffff) != 0)

extern void     *safe_malloc (size_t size, const char *fname, unsigned int line);
extern fixed    FixedMul (fixed a, fixed b);
extern fixed    FixedDiv (fixed a, fixed b);

extern unsigned short     READWORD (unsigned char *ptr);
extern unsigned int READLONGWORD (unsigned char *ptr);

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
#endif