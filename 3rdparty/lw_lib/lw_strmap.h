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

#ifndef LWLIB_STRMAP_H
#define LWLIB_STRMAP_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "lw_misc.h"
#ifdef LWLIB
#include "../uthash/uthash.h"

#define lwlib_StrMap_MaxString 128

#define lwlib_StrMapIterDecl(it) lwlib_StrMapHashUser_t *it, *tmp ## it
#define lwlib_StrMapIterLoop(strMap, it) HASH_ITER(hh, (strMap).users, it, tmp ## it)

typedef char lwlib_StrMapKey_t[lwlib_StrMap_MaxString];

typedef struct lwlib_StrMapHashUser_s
{
	lwlib_StrMapKey_t key;
    lwlib_Word_t val;
    UT_hash_handle hh;
} lwlib_StrMapHashUser_t;

typedef struct lwlib_StrMap_s
{
	lwlib_StrMapHashUser_t *users;
} lwlib_StrMap_t;

static inline lwlib_StrMap_t lwlib_StrMapZero(void)
{
	lwlib_StrMap_t zero;
	lwlib_Zero(zero);
	return zero;
}

static inline bool lwlib_StrMapContains(lwlib_StrMap_t *strMap, const lwlib_StrMapKey_t key)
{
	lwlib_StrMapHashUser_t *s;
	HASH_FIND_STR(strMap->users, key, s);
	return s != NULL;
}

static inline lwlib_Word_t lwlib_StrMapElem(lwlib_StrMap_t *strMap, const lwlib_StrMapKey_t key)
{
	lwlib_StrMapHashUser_t *s;
	lwlib_Word_t val = 0;

	HASH_FIND_STR(strMap->users, key, s);
	if (s != NULL)
	{
		val = s->val;
	}

	return val;
}

static inline void lwlib_StrMapInsert(lwlib_StrMap_t *strMap, const lwlib_StrMapKey_t key, lwlib_Word_t val)
{
	lwlib_StrMapHashUser_t *s;

	s = (lwlib_StrMapHashUser_t *)lwlib_Calloc(sizeof(lwlib_StrMapHashUser_t));
	if (s != NULL)
	{
		memcpy(s->key, key, sizeof(lwlib_StrMapKey_t));
		s->val = val;
		HASH_ADD_STR(strMap->users, key, s);
	}
}

static inline void lwlib_StrMapClear(lwlib_StrMap_t *strMap)
{
	lwlib_StrMapHashUser_t *current_user, *tmp;

	HASH_ITER(hh, strMap->users, current_user, tmp)
	{
		HASH_DEL(strMap->users, current_user);
		free(current_user);
	}

	strMap->users = NULL;
}
#endif
#endif
