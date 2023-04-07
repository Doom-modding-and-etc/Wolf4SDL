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

#include <assert.h>
#include <stdarg.h>
#ifdef _MSC_VER
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned __int64 uint64_t;
typedef signed __int64 int64_t;
#	include <stdint.h>
#	include <string.h>
#	include <stdarg.h>
#endif
#include <string.h>
#include <stdio.h>
#include "lw_misc.h"

static inline boolean lwlib_IntegerInRange(int a, int b, int c)
{
	return a >= b && a < c;
}

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


char *lwlib_Va(const char *format, ...)
{
	static char string[lwlib_StringSize];
	va_list argptr;

	va_start(argptr, format);
	vsnprintf(string, lwlib_StringSize - 1, format, argptr);
	string[lwlib_StringSize - 1] = '\0';
	va_end(argptr);

	return string;
}

char *lwlib_VaString(lwlib_String_t string, const char *format, ...)
{
	va_list argptr;

	va_start(argptr, format);
	vsnprintf(string, lwlib_StringSize - 1, format, argptr);
	string[lwlib_StringSize - 1] = '\0';
	va_end(argptr);

	return string;
}

void *lwlib_Calloc(size_t size)
{
	void *p;

	p = malloc(size);
	if (p != NULL)
	{
		memset(p, 0, size);
	}

	return p;
}

int lwlib_strncasecmp(const char *s1, const char *s2, int n)
{
	int c1, c2;

	do 
	{
		c1 = *s1++;
		c2 = *s2++;

		if (!n--)
			return 0;			// strings are equal until end point
		if (c1 != c2) 
		{
			if (c1 >= 'a' && c1 <= 'z')
				c1 -= ('a' - 'A');
			if (c2 >= 'a' && c2 <= 'z')
				c2 -= ('a' - 'A');
			if (c1 != c2)
				return -1;		// strings not equal
		}
	} while (c1);

	return 0;					// strings are equal
}

int lwlib_strcasecmp(const char *s1, const char *s2)
{
	return lwlib_strncasecmp(s1, s2, 0xFFFFFF);
}

int lwlib_IndexOf(int val, const int *values, int numValues)
{
	int i;

	if (values != NULL)
	{
		for (i = 0; i < numValues; i++)
		{
			if (values[i] == val)
			{
				break;
			}
		}
		if (i == numValues)
		{
			i = -1;
		}
	}
	else
	{
		i = -1;
	}

	return i;
}

void (*lwlib_LogPrintfCallback)(const char *message) = NULL;

int lwlib_LogPrintf(const char *format, ...)
{
	int ret;
	va_list argptr;
	char message[lwlib_StringSize];

	va_start(argptr, format);
	ret = vsnprintf(message, lwlib_StringSize - 1, format, argptr);
	message[lwlib_StringSize - 1] = '\0';
	va_end(argptr);

	#ifdef LWLIB_LOGPRINTF_STDOUT
	printf("%s", message);
	#endif

	if (lwlib_LogPrintfCallback != NULL)
	{
		lwlib_LogPrintfCallback(message);
	}

	return ret;
}

