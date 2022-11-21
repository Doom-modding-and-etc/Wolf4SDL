/*************************************************************************
** Dictionary
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
** Dictionary Library
*************************************************************************/

#ifndef LWLIB_DICT_H
#define LWLIB_DICT_H

#include <stdlib.h>
#include "lw_misc.h"
#include "vec.h"

#define lwlib_Dict(pairtype) pairtype ## Dict

#define lwlib_DictCmpFn(pairtype) pairtype ## CmpFn_t
#define lwlib_DictCmpFnType(pairtype, keytype) \
	typedef int (*lwlib_DictCmpFn(pairtype))(keytype a, keytype b)

#define lwlib_DictType(pairtype, firsttype, secondtype) \
	typedef struct { \
		firsttype first; \
		secondtype second; \
		void *pDict; \
	} pairtype; \
	 \
	lwlib_SeqType(pairtype); \
	lwlib_DictCmpFnType(pairtype, firsttype); \
	 \
	typedef struct { \
		lwlib_Seq(pairtype) seq; \
		lwlib_DictCmpFn(pairtype) cmpFn; \
		int (*qsortCmpFn)(const void *, const void *); \
	} lwlib_Dict(pairtype); \
	 \
	static inline int pairtype ## QSortCmpFn(const void *a, const void *b) \
	{ \
		assert(a && b && ((pairtype *)a)->pDict == ((pairtype *)b)->pDict); \
		lwlib_Dict(pairtype) *dict = (lwlib_Dict(pairtype) *)((pairtype *)a)->pDict; \
		assert(dict && dict->cmpFn); \
		return dict->cmpFn(*(firsttype *)a, *(firsttype *)b); \
	}

#define lwlib_DictSetCmpFn(dict, pairtype, fn) \
do { \
	(dict).cmpFn = fn; \
	(dict).qsortCmpFn = pairtype ## QSortCmpFn; \
} while (0)

#define lwlib_DictAppend(dict, firstval, secondval, type) \
do { \
	type pair; \
	lwlib_Zero(pair); \
	pair.first = firstval; \
	pair.second = secondval; \
	pair.pDict = &(dict); \
	vec_push((dict).seq, type, pair); \
} while (0)

#define lwlib_DictLen(dict) vec_size((dict).seq)

#define lwlib_DictElem(dict, index) vec_at((dict).seq, index)

#define lwlib_DictKey(dict, index) lwlib_DictElem(dict, index).first
#define lwlib_DictValue(dict, index) lwlib_DictElem(dict, index).second

#define lwlib_DictClear(dict, type) \
do { \
	vec_free((dict).seq, type); \
	lwlib_Zero(dict); \
} while (0)

#define lwlib_DictSort(dict) \
do { \
	assert((dict).qsortCmpFn); \
	if (lwlib_DictLen(dict) > 0) \
	{ \
	qsort(&lwlib_DictElem(dict, 0), lwlib_DictLen(dict), \
		sizeof(lwlib_DictElem(dict, 0)), (dict).qsortCmpFn); \
	} \
} while(0)

#endif
