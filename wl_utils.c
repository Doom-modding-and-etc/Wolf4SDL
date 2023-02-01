// WL_UTILS.C

#include "wl_utils.h"
#ifndef N3DS
#ifndef SWITCH

/*
===================
=
= safe_malloc
=
= Wrapper for malloc with a NULL check
=
===================
*/
void *safe_malloc (size_t size, const char *fname, uint32_t line)
{
    void *ptr;

    ptr = malloc(size);

    if (!ptr)
        Quit ("SafeMalloc: Out of memory at %s: line %u",fname,line);

    return ptr;
}
#endif
#endif

/*
===================
=
= FixedMul
=
= Multiplication of a Fixed value by a and b
=
===================
*/
fixed FixedMul (fixed a, fixed b)
{
#ifdef SEGA_SATURN
    return MTH_Mul2(a, b);
#else
	return (fixed)(((int64_t)a * b + 0x8000) >> FRACBITS);
#endif
}

/*
===================
=
= FixedDiv
=
= Division of the fixed value a by b
=
===================
*/
fixed FixedDiv (fixed a, fixed b)
{
	int64_t c = ((int64_t)a << FRACBITS) / (int64_t)b;

	return (fixed)c;
}

/*
===================
=
= READWORD
=
= Read the word value pointer.
=
===================
*/
word READWORD (byte *ptr)
{
    word val = ptr[0] | ptr[1] << 8;
#ifdef SEGA_SATURN
    ptr += 2;
#endif
    return val;
}

/*
===================
=
= READLONGWORD
=
= Reads the longword pointer value
=
===================
*/
longword READLONGWORD (byte *ptr)
{
    longword val = ptr[0] | ptr[1] << 8 | ptr[2] << 16 | ptr[3] << 24;

    return val;
}

/*
===================
=
= atan2fix
=
= Another unknown calculation math
=
===================
*/
#ifdef SEGA_SATURN
short atan2fix(fixed x, fixed y)
{
    boolean negative;
    long long quot;
    fixed tang;
    int offset;
    int res;
    if (x < 0) {
        x = -x;
        negative = true;
        offset = 180;
    }
    else {
        negative = false;
        offset = 0;
    }
    if (y < 0) {
        y = -y;
        negative = !negative;
        if (negative)
            offset = 360;
    }
    if (x == 0)
        return negative ? 270 : 90;
    if (y == 0)
        return offset;
    quot = ((long long)y << 32) / x;
    tang = (fixed)quot;
    if (quot != tang) {
        /* Overflow.  */
        res = 90;
    }
    else {
        int low = 0;
        int high = FINEANGLES / 4 - 1;

        while (low + 1 < high) {
            res = (low + high) >> 1;
            if (finetangent[res] < tang)
                high = res;
            else
                low = res;
        }
        res = res / (FINEANGLES / ANGLES);
    }
    if (negative)
        res = -res;
    return res + offset;
}
#endif

