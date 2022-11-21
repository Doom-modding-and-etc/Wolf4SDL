#ifndef LW_HVEC
#define LW_HVEC

#include <assert.h>
#include <stdlib.h>
#include "lw_intmap.h"
#include "vec.h"

#define hvec_t(type) \
    struct { \
		vec_t(type) vec; \
		lwlib_IntMap_t map; \
	}

#define hvec_size(hv) vec_size((hv).vec)
#define hvec_at(hv, i) vec_at((hv).vec, i)
#define hvec_push(hv, t, obj) vec_push((hv).vec, t, obj)
#define hvec_top(hv) vec_top((hv).vec)

#define hvec_has_key(hv, key) \
	lwlib_IntMapContains(&(hv).map, key)
#define hvec_at_key(hv, key) \
	vec_at((hv).vec, (int)lwlib_IntMapElem(&(hv).map, key))
#define hvec_pval(hv, key) \
	(hvec_has_key(hv, key) ? &hvec_at_key(hv, key) : NULL)
#define hvec_push_key(hv, t, obj, key) \
	do { \
		hvec_push(hv, t, obj); \
		lwlib_IntMapInsert(&(hv).map, key, hvec_size(hv) - 1); \
	} while (0)

#define hvec_init_keys(hv, t, keyfn) \
	do { \
		int i; \
		lwlib_IntMapClear(&(hv).map); \
		for (i = 0; i < hvec_size(hv); i++) \
		{ \
			lwlib_IntMapInsert(&(hv).map, keyfn(&hvec_at(hv, i)), i); \
		} \
	} while (0)


#define hvec_free(hv, t) \
	do { \
		vec_free((hv).vec, t); \
		lwlib_IntMapClear(&(hv).map); \
	} while (0)

#endif
