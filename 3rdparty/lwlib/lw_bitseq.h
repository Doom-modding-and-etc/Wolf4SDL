/*************************************************************************
** Vec3
** Copyright (C) 2009-2010 by Wolfy
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**************************************************************************
** Vec3 Library
*************************************************************************/

#ifndef VEC3_BITSEQ_H
#define VEC3_BITSEQ_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lw_intmap.h"

#define lwlib_BitsSet(bits, index) \
	lwlib_IntMapInsert(&(bits).intMap, (lwlib_IntMapKey_t)(index), 0)

#define lwlib_BitsUnset(bits, index) \
	lwlib_IntMapDelete(&(bits).intMap, (lwlib_IntMapKey_t)(index))

#define lwlib_BitsIter(bits) HASH_ITER(hh, (bits).intMap.users, (bits).it, (bits).tmpit)

#define lwlib_BitsIndex(bits) (bits).it->key

#define lwlib_BitsFree(bits) lwlib_IntMapClear(&(bits).intMap)

typedef struct lwlib_Bits_s
{
	lwlib_IntMap_t intMap;
	lwlib_IntMapIterDecl(it);
} lwlib_Bits_t;

static inline lwlib_Bits_t lwlib_BitsEmpty(void)
{
	lwlib_Bits_t zero;
	lwlib_Zero(zero);
	return zero;
}

static inline int lwlib_BitsTest(lwlib_Bits_t bits, int index)
{
	return lwlib_IntMapContains(&bits.intMap, index);
}

static inline lwlib_Bits_t lwlib_BitsUnion(lwlib_Bits_t a, lwlib_Bits_t b)
{
	lwlib_Bits_t c = lwlib_BitsEmpty();

	lwlib_BitsIter(a)
	{
		lwlib_BitsSet(c, lwlib_BitsIndex(a));
	}

	lwlib_BitsIter(b)
	{
		lwlib_BitsSet(c, lwlib_BitsIndex(b));
	}

	return c;
}

static inline int lwlib_BitsIsSubset(lwlib_Bits_t a, lwlib_Bits_t b)
{
	lwlib_BitsIter(a)
	{
		if (!lwlib_BitsTest(b, lwlib_BitsIndex(a)))
		{
			return 0;
		}
	}

	return 1;
}

static inline int lwlib_BitsCount(lwlib_Bits_t bits)
{
	return lwlib_IntMapCount(bits.intMap);
}

static inline lwlib_Bits_t lwlib_BitsSubset(lwlib_Bits_t bits, int index)
{
	int i;
	int count;

	lwlib_Bits_t subset = lwlib_BitsEmpty();

	count = lwlib_BitsCount(bits);
	assert(count < 32 && index < (1 << count));

	i = 0;
	lwlib_BitsIter(bits)
	{
		if (index & (1 << i))
			lwlib_BitsSet(subset, lwlib_BitsIndex(bits));
		i++;
	}

	return subset;
}

static inline void lwlib_BitsPrint(lwlib_Bits_t bits)
{
	printf("[ ");
	lwlib_BitsIter(bits)
	{
		printf("%d ", lwlib_BitsIndex(bits));
	}
	printf("]");
}

#define lwlib_BitsSetMany(a, b) \
do { \
	lwlib_Bits_t c = b; \
	lwlib_BitsIter(c) \
	{ \
		lwlib_BitsSet(a, lwlib_BitsIndex(c)); \
	} \
} while(0)

#define lwlib_BitsUnsetMany(a, b) \
do { \
	lwlib_Bits_t c = b; \
	lwlib_BitsIter(c) \
	{ \
		lwlib_BitsUnset(a, lwlib_BitsIndex(c)); \
	} \
} while(0)

static inline lwlib_Bits_t lwlib_BitsCopy(lwlib_Bits_t bits)
{
	lwlib_Bits_t copy = lwlib_BitsEmpty();
	lwlib_BitsSetMany(copy, bits);
	return copy;
}

static inline lwlib_Bits_t lwlib_BitsAll(int count)
{
	int i;
	lwlib_Bits_t bits;

	lwlib_Zero(bits);
	for (i = 0; i < count; i++)
	{
		lwlib_BitsSet(bits, i);
	}

	return bits;
}

static inline char *lwlib_BitsStr(lwlib_Bits_t bits, char *str)
{
	char tmpstr[32];

	str[0] = '\0';

	sprintf(tmpstr, "[ ");
	strcat(str, tmpstr);

	lwlib_BitsIter(bits)
	{
		sprintf(tmpstr, "%d ", lwlib_BitsIndex(bits));
		strcat(str, tmpstr);
	}
	sprintf(tmpstr, "]");
	strcat(str, tmpstr);

	return str;
}

#endif
