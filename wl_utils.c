// WL_UTILS.C

#include "wl_utils.h"

/*
===================
=
= safe_malloc
=
= Wrapper for malloc with a NULL check
=
===================
*/

void *safe_malloc (size_t size, const char *fname, u32 line)
{
    void *ptr;

    ptr = malloc(size);

    if (!ptr)
        Quit ("SafeMalloc: Out of memory at %s: line %u",fname,line);

    return ptr;
}


fixed FixedMul (fixed a, fixed b)
{
	return (fixed)(((s64)a * b + 0x8000) >> FRACBITS);
}

fixed FixedDiv (fixed a, fixed b)
{
	s64 c = ((s64)a << FRACBITS) / (s64)b;

	return (fixed)c;
}

word READWORD (byte *ptr)
{
    word val = ptr[0] | ptr[1] << 8;

    return val;
}

longword READLONGWORD (byte *ptr)
{
    longword val = ptr[0] | ptr[1] << 8 | ptr[2] << 16 | ptr[3] << 24;

    return val;
}
