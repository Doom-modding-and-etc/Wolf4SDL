/*************************************************************************
** Batman: No Man's Land
** Copyright (C) 2012 by LinuxWolf - Team RayCAST
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
** LinuxWolf Library for Batman: No Man's Land
*************************************************************************/

#ifndef LWLIB_MISC_H
#define LWLIB_MISC_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lw_vec.h"
#ifdef LWLIB
typedef byte lwlib_Byte_t;
typedef unsigned long int lwlib_Word_t;
typedef int vec_t;

#define lwlib_Zero(x) memset(&(x), 0, sizeof(x))

#define lwlib_Copy(dst, src) memcpy(&(dst), &(src), sizeof(dst))

#define lwlib_CallocSingle(type) (type *)lwlib_Calloc(sizeof(type))
#define lwlib_CallocMany(type, count) (type *)lwlib_Calloc(sizeof(type) * (count));

extern void *lwlib_Calloc(size_t size);

#define lwlib_ResetElem(ptr, dst, src) \
	if ((ptr) != NULL) \
	{ \
		(ptr) = (dst) + ((ptr) - (src)); \
	}

#define lwlib_CountArray(x) (sizeof(x) / sizeof((x)[0]))
#define lwlib_AssertArrayRange(x, a) assert((x) >= 0 && (x) < lwlib_CountArray(a))

#define lwlib_RepeatArrayN(array, count, indexname, elemname, stmts) \
do { \
	int indexname; \
	for (indexname = 0; indexname < count; indexname++) \
	{ \
		typeof((array)[0]) elemname = (array)[indexname]; \
		(void)elemname; \
		stmts; \
		(array)[indexname] = elemname; \
	} \
} while(0)

#define lwlib_RepeatArrayNRev(array, count, indexname, elemname, stmts) \
do { \
	int indexname; \
	for (indexname = (count) - 1; indexname >= 0; indexname--) \
	{ \
		typeof((array)[0]) elemname = (array)[indexname]; \
		(void)elemname; \
		stmts; \
		(array)[indexname] = elemname; \
	} \
} while(0)

#define lwlib_RepeatArray(array, indexname, elemname, stmts) \
	lwlib_RepeatArrayN(array, lwlib_CountArray(array), indexname, elemname, stmts)

#define lwlib_RepeatN(count, indexname, stmts) \
do { \
	int indexname; \
	for (indexname = 0; indexname < count; indexname++) \
	{ \
		stmts; \
	} \
} while(0)

// sparse ones algorithm for bit counting
static inline int lwlib_BitCount(unsigned int n)
{
	int count = 0;

	while (n)
	{
		count++;
		n &= (n - 1);
	}

	return count;
}

#define lwlib_IntegerIsPow2(x) (lwlib_BitCount(x) == 1)

#define lwlib_GetPriv(type, obj) \
	type *priv = (type *)(obj).priv; \
	(void)priv

#define lwlib_GetPrivEx(type, obj, privEx) \
	type *privEx = (type *)(obj).priv; \
	(void)privEx

#define lwlib_GetPrivAssert(type, obj) \
	lwlib_GetPriv(type, obj); \
	assert(priv != NULL); \

#define lwlib_GetPrivAssertEx(type, obj, privEx) \
	type *privEx = (type *)(obj).priv; \
	assert(privEx != NULL); \

#define lwlib_StrCopy(dest, src) \
do { \
	strncpy(dest, src, sizeof(dest) - 1); \
	(dest)[sizeof(dest) - 1] = '\0'; \
} while(0)

#define lwlib_Snprintf(dest, fmt, ...) \
do { \
	snprintf(dest, sizeof(dest) - 1, fmt, __VA_ARGS__); \
	dest[sizeof(dest) - 1] = '\0'; \
} while(0)

#define lwlib_BitOn(value, index) (((value) & (1 << (index))) != 0)
#define lwlib_BitOff(value, index) (!lwlib_BitOn(value, index))

#define lwlib_BitSetR(value, index, bit) \
	( \
		((value) & ~(1 << (index))) | \
		((bit) ? (1 << (index)) : 0) \
	)

#define lwlib_BitSet(value, index, bit) \
	value = lwlib_BitSetR(value, index, bit)

#define lwlib_BitEnable(value, index) \
	value = lwlib_BitSetR(value, index, 1)

#define lwlib_BitDisable(value, index) \
	value = lwlib_BitSetR(value, index, 0)

#define lwlib_EnumStrExtern(type) \
	extern const char *type ## _Str[]; \
	extern const int type ## _StrCount

#define lwlib_EnumStr(type, ...) \
	const char *type ## _Str[] = \
	{ \
		__VA_ARGS__ \
	}; \
	const int type ## _StrCount = lwlib_CountArray(type ## _Str)

#define lwlib_UseEnumStr(type) type ## _Str

#define lwlib_UseEnumStrCount(type) type ## _StrCount

#define lwlib_StringSize 128

typedef char lwlib_String_t[lwlib_StringSize];

extern char *lwlib_Va(const char *format, ...);

extern char *lwlib_VaString(lwlib_String_t string, const char *format, ...);

#define lwlib_BoolString(b) ((b) ? "true" : "false")

#define lwlib_Seq(elemtype) elemtype ## Seq
#define lwlib_SeqType(elemtype) \
	typedef vec_t(elemtype) lwlib_Seq(elemtype)

typedef struct lwlib_Str_s
{
	lwlib_String_t v;
} lwlib_Str_t;

typedef lwlib_Str_t lwlib_StrSeq_t;

static inline lwlib_Str_t lwlib_Str(const char *v)
{
	lwlib_Str_t str;
	lwlib_StrCopy(str.v, v);
	return str;
}

static inline lwlib_Str_t lwlib_Str_FromChar(char ch)
{
	lwlib_Str_t str;
	str.v[0] = ch;
	str.v[1] = '\0';
	return str;
}

static inline lwlib_Str_t lwlib_Str_SubStr(lwlib_Str_t a, int i, int j)
{
	int len;
	lwlib_Str_t str;
	const char *v;

	v = a.v;

	assert
		(
			v != NULL && i >= 0 && i <= strlen(v) && 
			(
				j == -1 || 
				(
					j >= 0 && j <= strlen(v) && i <= j
				)
			)
		);

	len = (j == -1 ? (strlen(v) - i) : (j - i));
	memcpy(str.v, &v[i], len);
	str.v[len] = '\0';

	return str;
}

static inline lwlib_Str_t lwlib_Str_Cat(lwlib_Str_t a, lwlib_Str_t b)
{
	lwlib_Str_t c;
	lwlib_VaString(c.v, "%s%s", a.v, b.v);
	return c;
}

static inline lwlib_Str_t lwlib_Str_InsertStr(lwlib_Str_t a, lwlib_Str_t b, int i)
{
	assert(i >= 0 && i <= strlen(a.v));
	return lwlib_Str_Cat(lwlib_Str_SubStr(a, 0, i), 
		lwlib_Str_Cat(b, lwlib_Str_SubStr(a, i, -1)));
}

static inline lwlib_Str_t lwlib_Str_DeleteChar(lwlib_Str_t a, int i)
{
	assert(i >= 0 && i < strlen(a.v));
	return lwlib_Str_Cat(
		lwlib_Str_SubStr(a, 0, i), 
		lwlib_Str_SubStr(a, i + 1, -1)
		);
}

static inline int lwlib_Str_IndexOf(lwlib_Str_t a, char ch)
{
	char *p;
	p = strchr(a.v, ch);
	return p != NULL ? (int)(p - a.v) : -1;
}

static inline lwlib_Str_t lwlib_Str_Cut(lwlib_Str_t a, char ch, int field)
{
	int index;
	lwlib_Str_t b, c;

	c = a;

	while (field >= 0)
	{
		index = lwlib_Str_IndexOf(c, ch);
		if (index == -1)
		{
			return c;
		}
		b = lwlib_Str_SubStr(c, 0, index);
		if (field == 0)
		{
			return b;
		}
		c = lwlib_Str_SubStr(c, index + 1, -1);
		field--;
	}

	return a;
}

#define lwlib_StrSeqAppend(seq, ...) \
do { \
	int dummyIndex; \
	static const char *dummyArr[] = \
	{ \
		__VA_ARGS__ \
	}; \
	for (dummyIndex = 0; dummyIndex < lwlib_CountArray(dummyArr); dummyIndex++) \
	{ \
		vec_push(seq, lwlib_Str_t, lwlib_Str(dummyArr[dummyIndex])); \
	} \
} while (0)

#define lwlib_CharLits_a_to_z "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "t", "s", "u", "v", "w", "x", "y", "z"
#define lwlib_CharLits_A_to_Z "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "T", "S", "U", "V", "W", "X", "Y", "Z"
#define lwlib_CharLits_0_to_9 "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"

static inline bool lwlib_IntegerInRange(int a, int b, int c)
{
	return a >= b && a < c;
}

#define lwlib_Pow2(x) (1 << (x))

static inline int lwlib_Log2(int x)
{
	int res = 0;
	assert(x >= 1);
	while (x > 1)
	{
		res++;
		x >>= 1;
	}
	return res;
}

static inline int lwlib_NextPow2(int x)
{
    if (lwlib_IntegerIsPow2(x))
    {
        return x;
    }

    return lwlib_Pow2(lwlib_Log2(x) + 1);
}

#define lwlib_Swap(a, b, t) \
do { \
	t c = a; \
	a = b; \
	b = c; \
} while (0)

#define lwlib_DegToRad(a) (((a) * M_PI) / 180.0)
#define lwlib_RadToDeg(a) (((a) * 180.0) / M_PI)

#define lwlib_Sq(a) ((a) * (a))
#define lwlib_Sign(a) ((a) > 0.0 ? 1.0 : ((a) < 0.0 ? -1.0 : 0.0))
#define lwlib_SignInt(a) ((a) > 0 ? 1 : ((a) < 0 ? -1 : 0))

#define lwlib_OneMinusNx2(a) (1 - ((a) * 2))

#define lwlib_ObjValid(obj) ((obj).priv != NULL)

#define lwlib_ObjNullDefn(type) \
static inline type ## _t type ## _Null(void) \
{ \
	type ## _t val; \
	lwlib_Zero(val); \
	return val; \
}

#define lwlib_CalcKb(x) ((x) * 1024)
#define lwlib_CalcMb(x) (lwlib_CalcKb((x) * 1024))

int lwlib_strncasecmp(const char *s1, const char *s2, int n);

int lwlib_strcasecmp(const char *s1, const char *s2);

#define LWLIB_FRACINIT(x, argVal, argResidual, argNumerator, argDivider) \
do { \
    memset(&(x), 0, sizeof(x)); \
    (x).val = argVal; \
    (x).residual = argResidual; \
    (x).numerator = argNumerator; \
    (x).divider = argDivider; \
} while (0)

#define LWLIB_FRACINC(x) \
do { \
    (x).residual += (x).numerator; \
    while ((x).residual >= (x).divider) \
    { \
        (x).val++; \
        (x).residual -= (x).divider; \
    } \
} while (0)

typedef struct lwlib_Frac_s
{
    int val;
    int residual;
    int numerator, divider;
} lwlib_Frac_t;

#define lwlib_ObjValid(obj) ((obj).priv != NULL)

typedef vec_t lwlib_VoidVec_t;

int lwlib_IndexOf(int val, const int *values, int numValues);

#define lwlib_AnonCallback_t(func) struct { void *anon; func; }

#define lwlib_AnonCallbackInit(anonCallback, anonObj, func) \
do { \
	(anonCallback).anon = (void *)anonObj; \
	(anonCallback).fn = func; \
} while (0)

#define LWLIB_KB(x) ((x) * 1024)
#define LWLIB_MB(x) LWLIB_KB((x) * 1024)

extern void (*lwlib_LogPrintfCallback)(const char *message);

extern int lwlib_LogPrintf(const char *format, ...);

#define lwlib_DefineDelegate(name, ret, ...) \
typedef struct \
{ \
	void *obj; \
	ret (*fn)(__VA_ARGS__); \
} name

#define lwlib_InitDelegate(name, ob, f) \
do { \
	delegateAPI->name.obj = ob; \
	delegateAPI->name.fn = f; \
} while(0)

#define lwlib_DeclDelegateAPI(type, obj) \
	type *delegateAPI = (type *)&(obj)->delegateAPI

#define lwlib_CallDelegateNoArgs(delegate) \
	delegateAPI->delegate.fn(delegateAPI->delegate.obj)

#define lwlib_CallDelegate(delegate, ...) \
	delegateAPI->delegate.fn(delegateAPI->delegate.obj, __VA_ARGS__)

#endif
#endif