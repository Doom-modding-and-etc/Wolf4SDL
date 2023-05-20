// WL_UTILS.C

#include "wl_utils.h"

/*
===================
=
= wsafe_malloc
=
= Wrapper for malloc with a NULL check
=
===================
*/
void *wsafe_malloc(size_t size, const char *fname, unsigned int line)
{
    void *ptr;

    ptr = malloc(size);

    if (!ptr)
        Quit ("SafeMalloc: Out of memory at %s: line %u",fname,line);

    return ptr;
}

/*
===================
=
= FixedMul
=
= Multiplication of a Fixed value by a and b
=
===================
*/
wlinline fixed FixedMul (fixed a, fixed b)
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
wlinline fixed FixedDiv (fixed a, fixed b)
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
wlinline unsigned short READWORD (unsigned char *ptr)
{
    unsigned short val = ptr[0] | ptr[1] << 8;
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
wlinline unsigned int READLONGWORD (unsigned char *ptr)
{
    unsigned int val = ptr[0] | ptr[1] << 8 | ptr[2] << 16 | ptr[3] << 24;

    return val;
}


/*
===================
=
= wlitoa
=
= converts a char to a int value.
=
===================
*/
wlinline char* wlitoa(int value, char* string, int radix)
{
    w3ssnprintf(string, sizeof(string), "%d", value);
    return string;
}

/*
===================
=
= wlltoa
=
= converts a char to a long value.
=
===================
*/
wlinline char* wlltoa(long value, char* string, int radix)
{
    w3ssnprintf(string, sizeof(string), "%ld", value);
    return string;
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
    int64_t quot;
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
    quot = ((int64_t)y << 32) / x;
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

