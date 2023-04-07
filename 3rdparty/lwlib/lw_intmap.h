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

#ifndef LWLIB_INTMAP_H
#define LWLIB_INTMAP_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "lw_misc.h"
#include "uthash.h"

#define lwlib_IntMapIterDecl(it) lwlib_IntMapHashUser_t *it, *tmp ## it
#define lwlib_IntMapIterLoop(intMap, it) HASH_ITER(hh, (intMap).users, it, tmp ## it)
#define lwlib_IntMapCount(intMap) HASH_COUNT(intMap.users)

#define lwlib_IntMapInsertPairs(intMap, ...) \
do { \
    int i; \
    static const int arr[] = { __VA_ARGS__ }; \
    for (i = 0; i < lwlib_CountArray(arr) / 2; i++) \
    { \
        lwlib_IntMapInsert(&(intMap), \
            arr[i * 2], (lwlib_Word_t)arr[i * 2 + 1]); \
    } \
} while(0)

typedef int lwlib_IntMapKey_t;

typedef struct lwlib_IntMapHashUser_s
{
	lwlib_IntMapKey_t key;
    lwlib_Word_t val;
    UT_hash_handle hh;
} lwlib_IntMapHashUser_t;

typedef struct lwlib_IntMap_s
{
	lwlib_IntMapHashUser_t *users;
} lwlib_IntMap_t;

static inline lwlib_IntMap_t lwlib_IntMapZero(void)
{
	lwlib_IntMap_t zero;
	lwlib_Zero(zero);
	return zero;
}

static inline boolean lwlib_IntMapContains(lwlib_IntMap_t *intMap, lwlib_IntMapKey_t key)
{
	lwlib_IntMapHashUser_t *s;
	HASH_FIND_INT(intMap->users, &key, s);
	return s != NULL;
}

static inline lwlib_Word_t lwlib_IntMapElem(lwlib_IntMap_t *intMap, lwlib_IntMapKey_t key)
{
	lwlib_IntMapHashUser_t *s;
	lwlib_Word_t val = 0;

	HASH_FIND_INT(intMap->users, &key, s);
	if (s != NULL)
	{
		val = s->val;
	}

	return val;
}

static inline void lwlib_IntMapInsert(lwlib_IntMap_t *intMap, lwlib_IntMapKey_t key, lwlib_Word_t val)
{
	lwlib_IntMapHashUser_t *s;

	s = (lwlib_IntMapHashUser_t *)lwlib_Calloc(sizeof(lwlib_IntMapHashUser_t));
	if (s != NULL)
	{
		s->key = key;
		s->val = val;
		HASH_ADD_INT(intMap->users, key, s);
	}
}

static inline void lwlib_IntMapDeleteAll(lwlib_IntMap_t *intMap)
{
	lwlib_IntMapHashUser_t *current_user, *tmp;

	HASH_ITER(hh, intMap->users, current_user, tmp)
	{
		HASH_DEL(intMap->users, current_user);
		free(current_user); // optional; its up to you!
	}

	intMap->users = NULL;
}

static inline void lwlib_IntMapDelete(lwlib_IntMap_t *intMap,
	lwlib_IntMapKey_t key)
{
	lwlib_IntMapHashUser_t *s;
	lwlib_Word_t val = 0;

	HASH_FIND_INT(intMap->users, &key, s);
	if (s != NULL)
	{
		HASH_DEL(intMap->users, s);
	}
}

static inline void lwlib_IntMapClear(lwlib_IntMap_t *intMap)
{
	HASH_CLEAR(hh,intMap->users);
}

#endif
