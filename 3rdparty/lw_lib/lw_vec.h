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

#ifndef LWLIB_VEC_H
#define LWLIB_VEC_H

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "..\..\wl_def.h"
#ifdef LWLIB
#include "lw_misc.h"
#include "lw_intmap.h"
#include "..\fixedptc.h"
//VEC.H
/* Summary
 *
 *  typedef vec_t(Type) Vec;
 *  Vec v = {0};
 *  unsigned vec_size (Vec);
 *  Type vec_at (Vec, unsigned);
 *  void vec_remove_unordered (Vec, unsigned);
 *  Type *vec_inc_top (Vec, Type);   Return pointer to new top
 *  void vec_push (Vec, Type, Obj);
 *  Type vec_pop_back (Vec);
 *  void vec_resize (Vec, unsigned);
 *  void vec_reserve (Vec, Type, unsigned);
 *  void vec_clear (Vec);
 *  void vec_free (Vec, Type);
 *
 *  `vec_at` evaluates to an lvalue.
 *  Data pointer access is &vec_at(v,0);
 */

/* inspired and much of it taken from kvec.h by attractive chaos */
#define vec_t(type) struct { unsigned alloc, sz; type *data; }
#define vec_size(v) ((const unsigned)(v).sz)
#define vec_at(v,i) ((v).data[vecI_bounds_check((v).sz, (i))])
#define vec_at_unchecked(v,i) ((v).data[i])
#define vec_remove_unordered(v,i) \
    do { \
        unsigned _vecI_i = vecI_bounds_check((v).sz, (i)); \
        (v).data[_vecI_i] = (v).data[--(v).sz]; \
    } while (0)

static inline void *
vecI_grow (unsigned membsz, unsigned *alloc, void *buf)
{
    /* If *alloc is 0, set to 8, else grow by a factor of 2 */
    *alloc = ((~!*alloc + 1) & 8) + (*alloc << 1);
    return realloc(buf, membsz * (*alloc));
}

static inline void *
vecI_reserve (unsigned membsz, unsigned rq, unsigned *alloc, void *buf)
{
    /* free if 0, else round to floor_log2(rq-1) and pass it on */
    if (rq == 0)
    {
        *alloc = 0, free(buf);
        return NULL;
    }
    rq--; while (rq & (rq - 1)) rq = rq & (rq - 1); *alloc = rq;
    return vecI_grow(membsz, alloc, buf);
}

#define vecI_inc_siz(v, t) \
    ((v).sz++, \
     ((v).sz > (v).alloc ? \
        (v).data = (t *)vecI_grow(sizeof(*(v).data), &(v).alloc, (v).data) \
      : 0))

#define vec_inc_top(v, t) (vecI_inc_siz(v, t), &(v).data[(v).sz-1])
#define vec_top(v) ((v).data[(v).sz-1])
#define vec_push(v,t,obj) (*vec_inc_top(v, t) = (obj))
#define vec_pop_back(v) (assert((v).sz > 0), (v).data[--(v).sz])
#define vec_reserve(v,t,rq) \
    ((v).data = (t *)vecI_reserve(sizeof(*(v).data), (rq), &(v).alloc, (v).data))
#define vec_resize(v,t,siz) (vec_reserve((v),t,(siz)), (v).sz = (siz))
#define vec_clear(v) ((v).sz = 0)
#define vec_free(v,t) vec_resize((v),t,0)

static inline unsigned
vecI_bounds_check (unsigned sz, unsigned idx)
{
    assert(idx < sz); (void)sz;
    return idx;
}

//LW_HVEC.H
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

//LW_BITSEQ.H
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

static inline char* lwlib_BitsStr(lwlib_Bits_t bits, char* str)
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

/* common */

#define lwlib_EPSILON 0.00001
#define lwlib_MIN(a,b) ((a) < (b) ? (a) : (b))
#define lwlib_MAX(a,b) ((a) > (b) ? (a) : (b))
#define lwlib_MINMAX(a,b,c) ((c) ? ((a) > (b) ? (a) : (b)) : ((a) < (b) ? (a) : (b)))
#define lwlib_CLIP(x,a,b) lwlib_MIN(lwlib_MAX(x,a), b)

#define lwlib_plane_origin(plane) (plane).p[0]
#define lwlib_plane_normal(plane) (plane).p[1]
#define lwlib_ray_start(ray) (ray).p[0]
#define lwlib_ray_dir(ray) (ray).p[1]

#define lwlib_box_lower(box) (box).p[0]
#define lwlib_box_upper(box) (box).p[1]
#define lwlib_box_extents(box) lwlib_box_lower(box), lwlib_box_upper(box)

#define LWLIB_FACE_POINT(face, n) (face).points((face).priv, (n))
#define LWLIB_REGION_PLANE(region, n) (region).planes((region).priv, (n))

#define lwlib_Facing(name) LWLIB_FACING_ ## name

/* vector math */

/* access named component of vector */
#define lwlib_X(a) (a).v[0]
#define lwlib_Y(a) (a).v[1]
#define lwlib_Z(a) (a).v[2]
#define lwlib_W(a) (a).v[3]

/* access numbered component of vector */
#define lwlib_C(a, i) (a).v[i]

/* access mapped, numbered component of vector */ 
#define lwlib_Cm(a, i, m) lwlib_C(a, (m)[i])

#define lwlib_AddVector2(c,a,b) lwlib_X(c)=lwlib_X(a)+lwlib_X(b), lwlib_Y(c)=lwlib_Y(a)+lwlib_Y(b)
#define lwlib_SubVector2(c,a,b) lwlib_X(c)=lwlib_X(a)-lwlib_X(b), lwlib_Y(c)=lwlib_Y(a)-lwlib_Y(b)
#define lwlib_DotVector2(a,b) (lwlib_X(a)*lwlib_X(b) + lwlib_Y(a)*lwlib_Y(b))
#define lwlib_ScaleVector2(c,a,b) lwlib_X(c)=lwlib_X(a)*(b), lwlib_Y(c)=lwlib_Y(a)*(b)
#define lwlib_DivideVector2(c,a,b) lwlib_X(c)=lwlib_X(a)/(b), lwlib_Y(c)=lwlib_Y(a)/(b)
#define lwlib_NormalizeVector2(n,a) \
	( \
		(n)=sqrt(lwlib_DotVector2(a,a)), \
		(n) ? \
			(lwlib_X(a)/=n, lwlib_Y(a)/=n) : \
			0 \
	)
#define lwlib_InitVector2(v,a,b) lwlib_X(v) = (a), lwlib_Y(v) = (b)
#define lwlib_InvertVector2(a,b) lwlib_X(a) = -lwlib_X(b), lwlib_Y(a) = -lwlib_Y(b)
#define lwlib_MultVector2(c,a,b) lwlib_X(c) = lwlib_X(a) * lwlib_X(b), lwlib_Y(c) = lwlib_Y(a) * lwlib_Y(b)
#define lwlib_VectorLength2(v) sqrt(lwlib_DotVector2(v,v))
#define lwlib_VectorLengthSq2(v) lwlib_DotVector2(v,v)
#define lwlib_TransformVector2(a,b,f) lwlib_X(a) = f(lwlib_X(b)), lwlib_Y(a) = f(lwlib_Y(b))
#define lwlib_PackVector2(a) (a)[0], (a)[1]
#define lwlib_UnpackVector2(a, b) (b)[0] = lwlib_X(a), (b)[1] = lwlib_Y(a)
#define lwlib_ExpandVector2(a) lwlib_X(a), lwlib_Y(a)

#define lwlib_vec2_init(x,y) { { (x), (y) } }

#define lwlib_AddVector(c,a,b) lwlib_X(c)=lwlib_X(a)+lwlib_X(b), lwlib_Y(c)=lwlib_Y(a)+lwlib_Y(b), lwlib_Z(c)=lwlib_Z(a)+lwlib_Z(b)
#define lwlib_SubVector(c,a,b) lwlib_X(c)=lwlib_X(a)-lwlib_X(b), lwlib_Y(c)=lwlib_Y(a)-lwlib_Y(b), lwlib_Z(c)=lwlib_Z(a)-lwlib_Z(b)
#define lwlib_CrossVector(c,a,b)	lwlib_X(c) = lwlib_Y(a)*lwlib_Z(b) - lwlib_Z(a)*lwlib_Y(b), \
                				lwlib_Y(c) = lwlib_Z(a)*lwlib_X(b) - lwlib_X(a)*lwlib_Z(b), \
                				lwlib_Z(c) = lwlib_X(a)*lwlib_Y(b) - lwlib_Y(a)*lwlib_X(b)
#define lwlib_DotVector(a,b) (lwlib_X(a)*lwlib_X(b) + lwlib_Y(a)*lwlib_Y(b) + lwlib_Z(a)*lwlib_Z(b))
#define lwlib_ScaleVector(c,a,b) lwlib_X(c)=lwlib_X(a)*(b), lwlib_Y(c)=lwlib_Y(a)*(b), lwlib_Z(c)=lwlib_Z(a)*(b)
#define lwlib_DivideVector(c,a,b) lwlib_X(c)=lwlib_X(a)/(b), lwlib_Y(c)=lwlib_Y(a)/(b), lwlib_Z(c)=lwlib_Z(a)/(b)
#define lwlib_NormalizeVector(n,a) \
	( \
		(n)=sqrt(lwlib_DotVector(a,a)), \
		(n) ? \
			(lwlib_X(a)/=n, lwlib_Y(a)/=n, lwlib_Z(a)/=n) : \
			0 \
	)
#define lwlib_InitVector(v,a,b,c) lwlib_X(v) = (a), lwlib_Y(v) = (b), lwlib_Z(v) = (c)
#define lwlib_InvertVector(a,b) lwlib_X(a) = -lwlib_X(b), lwlib_Y(a) = -lwlib_Y(b), lwlib_Z(a) = -lwlib_Z(b)
#define lwlib_MultVector(c,a,b) lwlib_X(c) = lwlib_X(a) * lwlib_X(b), lwlib_Y(c) = lwlib_Y(a) * lwlib_Y(b), lwlib_Z(c) = lwlib_Z(a) * lwlib_Z(b)
#define lwlib_VectorLength(v) sqrt(lwlib_DotVector(v,v))
#define lwlib_VectorLengthSq(v) lwlib_DotVector(v,v)
#define lwlib_TransformVector(a,b,f) lwlib_X(a) = f(lwlib_X(b)), lwlib_Y(a) = f(lwlib_Y(b)), lwlib_Z(a) = f(lwlib_Z(b))
#define lwlib_PackVector(a) (a)[0], (a)[1], (a)[2]
#define lwlib_UnpackVector(a, b) (b)[0] = lwlib_X(a), (b)[1] = lwlib_Y(a), (b)[2] = lwlib_Z(a)
#define lwlib_ExpandVector(a) lwlib_X(a), lwlib_Y(a), lwlib_Z(a)

#define lwlib_vec3_init(x,y,z) { { (x), (y), (z) } }

#define lwlib_AddVector4(c,a,b) lwlib_X(c)=lwlib_X(a)+lwlib_X(b), lwlib_Y(c)=lwlib_Y(a)+lwlib_Y(b), lwlib_Z(c)=lwlib_Z(a)+lwlib_Z(b), lwlib_W(c)=lwlib_W(a)+lwlib_W(b)
#define lwlib_SubVector4(c,a,b) lwlib_X(c)=lwlib_X(a)-lwlib_X(b), lwlib_Y(c)=lwlib_Y(a)-lwlib_Y(b), lwlib_Z(c)=lwlib_Z(a)-lwlib_Z(b), lwlib_W(c)=lwlib_W(a)-lwlib_W(b)
#define lwlib_DotVector4(a,b) (lwlib_X(a)*lwlib_X(b) + lwlib_Y(a)*lwlib_Y(b) + lwlib_Z(a)*lwlib_Z(b) + lwlib_W(a)*lwlib_W(b))
#define lwlib_ScaleVector4(c,a,b) lwlib_X(c)=lwlib_X(a)*(b), lwlib_Y(c)=lwlib_Y(a)*(b), lwlib_Z(c)=lwlib_Z(a)*(b), lwlib_W(c)=lwlib_W(a)*(b)
#define lwlib_DivideVector4(c,a,b) lwlib_X(c)=lwlib_X(a)/(b), lwlib_Y(c)=lwlib_Y(a)/(b), lwlib_Z(c)=lwlib_Z(a)/(b), lwlib_W(c)=lwlib_W(a)/(b)
#define lwlib_NormalizeVector4(n,a) \
	( \
		(n)=sqrt(lwlib_DotVector4(a,a)), \
		(n) ? \
			(lwlib_X(a)/=n, lwlib_Y(a)/=n, lwlib_Z(a)/=n, lwlib_W(a)/=n) : \
			0 \
	)
#define lwlib_InitVector4(v,a,b,c,d) lwlib_X(v) = (a), lwlib_Y(v) = (b), lwlib_Z(v) = (c), lwlib_W(v) = (d)
#define lwlib_InvertVector4(a,b) lwlib_X(a) = -lwlib_X(b), lwlib_Y(a) = -lwlib_Y(b), lwlib_Z(a) = -lwlib_Z(b), lwlib_W(a) = -lwlib_W(b)
#define lwlib_MultVector4(c,a,b) lwlib_X(c) = lwlib_X(a) * lwlib_X(b), lwlib_Y(c) = lwlib_Y(a) * lwlib_Y(b), lwlib_Z(c) = lwlib_Z(a) * lwlib_Z(b), lwlib_W(c) = lwlib_W(a) * lwlib_W(b)
#define lwlib_VectorLength4(v) sqrt(lwlib_DotVector4(v,v))
#define lwlib_VectorLengthSq4(v) lwlib_DotVector4(v,v)
#define lwlib_TransformVector4(a,b,f) lwlib_X(a) = f(lwlib_X(b)), lwlib_Y(a) = f(lwlib_Y(b)), lwlib_Z(a) = f(lwlib_Z(b)), lwlib_W(a) = f(lwlib_W(b))
#define lwlib_PackVector4(a) (a)[0], (a)[1], (a)[2], (a)[3]
#define lwlib_UnpackVector4(a, b) (b)[0] = lwlib_X(a), (b)[1] = lwlib_Y(a), (b)[2] = lwlib_Z(a), (b)[3] = lwlib_W(a)
#define lwlib_ExpandVector4(a) lwlib_X(a), lwlib_Y(a), lwlib_Z(a), lwlib_W(a)

#define lwlib_vec4_init(x,y,z,w) { { (x), (y), (z), (w) } }

#define lwlib_edge3f lwlib_vec3f_pair
#define lwlib_ray3f lwlib_vec3f_pair
#define lwlib_plane3f lwlib_vec3f_pair
#define lwlib_box3f lwlib_vec3f_pair

#define lwlib_box3i lwlib_vec3i_pair

#define lwlib_vec4b_black lwlib_vec4b(0x00, 0x00, 0x00, 0xff)
#define lwlib_vec4b_red lwlib_vec4b(0xff, 0x00, 0x00, 0xff)
#define lwlib_vec4b_green lwlib_vec4b(0x00, 0xff, 0x00, 0xff)
#define lwlib_vec4b_yellow lwlib_vec4b(0xff, 0xff, 0x00, 0xff)
#define lwlib_vec4b_blue lwlib_vec4b(0x00, 0x00, 0xff, 0xff)
#define lwlib_vec4b_cyan lwlib_vec4b(0x00, 0xff, 0xff, 0xff)
#define lwlib_vec4b_magenta lwlib_vec4b(0xff, 0x00, 0xff, 0xff)
#define lwlib_vec4b_white lwlib_vec4b(0xff, 0xff, 0xff, 0xff)

typedef double (*lwlib_TFunc)(double x);

typedef enum {
	LWLIB_FACE_SIDE_FRONT,
	LWLIB_FACE_SIDE_BACK,
	LWLIB_FACE_SIDE_SPLIT,
	LWLIB_FACE_SIDE_COPLANAR,
	LWLIB_FACE_SIDE_MAX,
} lwlib_TFaceSide;

typedef enum {
	LWLIB_OP_ADD,
	LWLIB_OP_SUB,
	LWLIB_OP_MULT,
	LWLIB_OP_DIVIDE,
	LWLIB_OP_SHIFTLEFT,
	LWLIB_OP_SHIFTRIGHT,
} lwlib_TVectorOp;

typedef enum lwlib_Facing_e
{
	lwlib_Facing(PositiveX),
	lwlib_Facing(NegativeX),
	lwlib_Facing(PositiveY),
	lwlib_Facing(NegativeY),
	lwlib_Facing(PositiveZ),
	lwlib_Facing(NegativeZ),
	lwlib_Facing(Last) = lwlib_Facing(NegativeZ),
} lwlib_Facing_t;

typedef struct { 
	double v[2];
} lwlib_TPoint2f;

typedef struct { 
	double v[3];
} lwlib_TPoint3f;

typedef lwlib_TPoint3f lwlib_TVector3f;

typedef struct {
	lwlib_TPoint3f p[2];
} lwlib_TPointPair3f;

typedef lwlib_TPointPair3f lwlib_TEdge3f;
typedef lwlib_TPointPair3f lwlib_TPlane3f;
typedef lwlib_TPointPair3f lwlib_TRay3f;
typedef lwlib_TPointPair3f lwlib_TBox3f;

typedef lwlib_TPoint3f (*lwlib_TFacePoint3f)(void *priv, int n);

typedef struct {
	void *priv;
	lwlib_TFacePoint3f points;
	int numPoints;
} lwlib_TFace3f;

typedef lwlib_TPlane3f (*lwlib_TRegionPlane3f)(void *priv, int n);

typedef struct {
	void *priv;
	lwlib_TRegionPlane3f planes;
	int numPlanes;
	int memAllocated;
} lwlib_TRegion3f;

typedef lwlib_TVector3f (*lwlib_TFunc3f)(lwlib_TVector3f v);

typedef struct
{
	lwlib_TPoint3f origin;
	double radius;
} lwlib_TSphere3f;

typedef struct { 
	double v[4];
} lwlib_TPoint4f;

typedef struct { 
	int v[2];
} lwlib_TPoint2i;

typedef lwlib_TPoint2i lwlib_TVector2i;

typedef struct {
	lwlib_TPoint2i p[2];
} lwlib_TPointPair2i;

typedef lwlib_TPointPair2i lwlib_TBox2i;

typedef struct { 
	int v[3];
} lwlib_TPoint3i;

typedef lwlib_TPoint3i lwlib_TVector3i;

typedef struct {
	lwlib_TPoint3i p[2];
} lwlib_TPointPair3i;

typedef lwlib_TPointPair3i lwlib_TBox3i;

typedef struct {
	unsigned char v[3];
} lwlib_TPoint3b;

typedef struct {
	unsigned char v[4];
} lwlib_TPoint4b;

double lwlib_vec3_lerp(double a, double b, double t);

extern lwlib_TPoint2f lwlib_vec2f(double x, double y);
extern lwlib_TPoint2f lwlib_vec2f_zero(void);
extern lwlib_TPoint2f lwlib_vec2f_ones(void);
extern lwlib_TPoint2f lwlib_vec2f_add(lwlib_TPoint2f a, lwlib_TPoint2f b);
extern lwlib_TPoint2f lwlib_vec2f_sub(lwlib_TPoint2f a, lwlib_TPoint2f b);
extern lwlib_TPoint2f lwlib_vec2f_scale(lwlib_TPoint2f a, double s);
extern double lwlib_vec2f_dot(lwlib_TPoint2f a, lwlib_TPoint2f b);
extern double lwlib_vec2f_length(lwlib_TPoint2f a);
extern double lwlib_vec2f_length_sq(lwlib_TPoint2f a);
extern lwlib_TPoint2f lwlib_vec2f_normalize(lwlib_TPoint2f a);
extern lwlib_TPoint2f lwlib_vec2f_neg(lwlib_TPoint2f a);
extern lwlib_TPoint2f lwlib_vec2f_mult(lwlib_TPoint2f a, lwlib_TPoint2f b);
extern double lwlib_vec2f_min(lwlib_TPoint2f a);
extern double lwlib_vec2f_max(lwlib_TPoint2f a);
extern lwlib_TPoint2f lwlib_vec2f_clip(lwlib_TPoint2f a);
extern double lwlib_vec2f_range(lwlib_TPoint2f a);
extern char *lwlib_vec2f_to_string(lwlib_String_t string, lwlib_TPoint2f a);
extern lwlib_TPoint2i lwlib_vec2f_int(lwlib_TPoint2f a);

extern lwlib_TPoint3f lwlib_vec3f(double x, double y, double z);
extern lwlib_TPoint3f lwlib_vec3f_zero(void);
extern lwlib_TPoint3f lwlib_vec3f_ones(void);
extern lwlib_TPoint3f lwlib_vec3f_same(double x);
extern lwlib_TPoint3f lwlib_vec3f_add(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern lwlib_TPoint3f lwlib_vec3f_sub(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern lwlib_TPoint3f lwlib_vec3f_cross(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern lwlib_TPoint3f lwlib_vec3f_scale(lwlib_TPoint3f a, double s);
extern double lwlib_vec3f_dot(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern double lwlib_vec3f_length(lwlib_TPoint3f a);
extern double lwlib_vec3f_length_sq(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_normalize(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_neg(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_mult(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern double lwlib_vec3f_min(lwlib_TPoint3f a);
extern double lwlib_vec3f_max(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_clip(lwlib_TPoint3f a);
extern unsigned long lwlib_vec3f_quantize(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_swap_components(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_transform(lwlib_TPoint3f a, double (*f)(double));
extern int lwlib_vec3f_side(lwlib_TPlane3f plane, lwlib_TPoint3f a, double eps);
extern int lwlib_vec3f_equal(lwlib_TPoint3f a, lwlib_TPoint3f b, double eps);
extern lwlib_TVector3f lwlib_vec3f_normal(lwlib_TPoint3f a, lwlib_TPoint3f b, lwlib_TPoint3f c);
extern lwlib_TPlane3f lwlib_vec3f_plane(lwlib_TPoint3f a, lwlib_TPoint3f b, lwlib_TPoint3f c);
extern lwlib_TPointPair3f lwlib_vec3f_pair(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern lwlib_TPoint3i lwlib_vec3f_int(lwlib_TPoint3f a);
extern int lwlib_vec3f_sphere_side(lwlib_TSphere3f a, lwlib_TPoint3f b, double eps);

extern double lwlib_vec3f_face_area(lwlib_TFace3f face, lwlib_TVector3f normal);
extern lwlib_TFace3f lwlib_vec3f_face(void *priv, lwlib_TFacePoint3f points, int numPoints);
extern int lwlib_vec3f_face_side(lwlib_TFace3f face, lwlib_TPlane3f plane, double eps);
extern lwlib_TBox3f lwlib_vec3f_face_box(lwlib_TFace3f face);
extern lwlib_TPoint3f lwlib_vec3f_face_sum(lwlib_TFace3f face);
extern lwlib_TPoint3f lwlib_vec3f_face_center(lwlib_TFace3f face);
extern lwlib_TPoint3f lwlib_vec3f_face_point(void *priv, int n);

extern lwlib_TRegion3f lwlib_vec3f_region(void *priv, lwlib_TRegionPlane3f planes, int numPlanes);
extern lwlib_TRegion3f lwlib_vec3f_region_face(lwlib_TFace3f face, lwlib_TVector3f normal, lwlib_TPlane3f *planes);
extern lwlib_TRegion3f lwlib_vec3f_region_destroy(lwlib_TRegion3f region);

extern int lwlib_vec3f_region_test(lwlib_TRegion3f region, lwlib_TPoint3f a, double eps);
extern lwlib_TPlane3f lwlib_vec3f_region_plane(void *priv, int n);

extern lwlib_TPoint3f lwlib_vec3f_quad_lerp(lwlib_TPoint3f quad[4], double u, double v);
extern lwlib_TPoint3f lwlib_vec3f_rot90(lwlib_TPoint3f a);
extern lwlib_TPoint3f lwlib_vec3f_rot90_anchor(lwlib_TPoint3f a, lwlib_TPoint3f b);
extern lwlib_TVector3f lwlib_vec3f_facing_normal(int facing);
extern int lwlib_vec3f_sign_bits(lwlib_TPoint3f a);

/* matrix math */

/* access (row,col) component of matrix */
#define lwlib_CM(a, r, c) (a).v[(r) * 3 + (c)]

typedef struct { 
	double v[3 * 3];
	int rows, cols;
} lwlib_TMat3f;

extern lwlib_TMat3f lwlib_mat3f_zero(int rows, int cols);
extern lwlib_TMat3f lwlib_mat3f_ident(int rows, int cols);
extern lwlib_TMat3f lwlib_mat3f_row_mat(int cols, lwlib_TPoint3f a);
extern lwlib_TMat3f lwlib_mat3f_col_mat(int rows, lwlib_TPoint3f a);
extern lwlib_TMat3f lwlib_mat3f_diag(int rows, int cols, lwlib_TPoint3f a);
extern lwlib_TMat3f lwlib_mat3f_shift(int axis);
extern lwlib_TMat3f lwlib_mat3f_rot(double angle, int axis);

extern lwlib_TMat3f lwlib_mat3f_add(lwlib_TMat3f a, lwlib_TMat3f b);
extern lwlib_TMat3f lwlib_mat3f_sub(lwlib_TMat3f a, lwlib_TMat3f b);
extern lwlib_TMat3f lwlib_mat3f_mult(lwlib_TMat3f a, lwlib_TMat3f b);
extern lwlib_TMat3f lwlib_mat3f_xpose(lwlib_TMat3f a);
extern lwlib_TMat3f lwlib_mat3f_scale(lwlib_TMat3f a, double b);
extern lwlib_TMat3f lwlib_mat3f_rotate_columns(lwlib_TMat3f a, int times);

extern lwlib_TPoint3f lwlib_mat3f_row(lwlib_TMat3f a, int row);
extern lwlib_TPoint3f lwlib_mat3f_col(lwlib_TMat3f a, int col);
extern lwlib_TPoint3f lwlib_mat3f_xform(lwlib_TMat3f a, lwlib_TPoint3f b);
extern lwlib_TPlane3f lwlib_mat3f_rotate_plane(lwlib_TMat3f a, lwlib_TPlane3f b);

extern char *lwlib_mat3f_to_string(lwlib_String_t string, lwlib_TMat3f a);

/* more vector math */

extern int lwlib_ray3f_hit_box(lwlib_TRay3f ray, lwlib_TBox3f box);
extern int lwlib_ray3f_hit_plane(lwlib_TRay3f ray, lwlib_TPlane3f plane, lwlib_TPoint3f *hit);
extern int lwlib_ray3f_hit_face(lwlib_TRay3f ray, lwlib_TRegion3f region, lwlib_TPlane3f plane, double eps);

extern lwlib_TEdge3f lwlib_edge3f_reverse(lwlib_TEdge3f a);
extern int lwlib_edge3f_equal(lwlib_TEdge3f a, lwlib_TEdge3f b, double eps);
extern int lwlib_edge3f_adj(lwlib_TEdge3f a, lwlib_TEdge3f b, double eps);
extern lwlib_TVector3f lwlib_edge3f_vec(lwlib_TEdge3f a);
extern lwlib_TPoint3f lwlib_edge3f_lerp(lwlib_TEdge3f a, double t);

extern lwlib_TBox3f lwlib_box3f_cell(lwlib_TPoint3f a);
extern lwlib_TBox3f lwlib_box3f_combine(lwlib_TBox3f a, lwlib_TBox3f b);
extern lwlib_TBox3f lwlib_box3f_expand(lwlib_TBox3f a, lwlib_TPoint3f b);
extern lwlib_TBox3f lwlib_box3f_grow(lwlib_TBox3f a, double size);
extern int lwlib_box3f_touch(lwlib_TBox3f a, lwlib_TBox3f b);
extern lwlib_TBox3f lwlib_box3f_intersect(lwlib_TBox3f a, lwlib_TBox3f b);
extern lwlib_TPoint3f lwlib_box3f_face_vert(const lwlib_TBox3f box, int facing, int vertNum);
extern bool lwlib_box3f_face_visible(const lwlib_TBox3f box, int facing, lwlib_TPoint3f viewPos);
extern lwlib_TPlane3f lwlib_box3f_face_plane(const lwlib_TBox3f box, int facing);
extern lwlib_TBox3f lwlib_box3f_translate(lwlib_TBox3f a, lwlib_TVector3f b);
extern lwlib_TBox3f lwlib_box3f_rotate(lwlib_TBox3f a, lwlib_TMat3f b);
extern lwlib_TPoint3f lwlib_box3f_corner(lwlib_TBox3f a, int index);
extern lwlib_TPoint3f lwlib_box3f_center(lwlib_TBox3f a);
extern lwlib_TVector3f lwlib_box3f_span(lwlib_TBox3f a);
extern bool lwlib_box3f_empty(lwlib_TBox3f a);

extern lwlib_TSphere3f lwlib_sphere3f(lwlib_TPoint3f origin, double radius);

extern lwlib_TPoint4f lwlib_vec4f(double x, double y, double z, double w);
extern lwlib_TPoint4f lwlib_vec4f_zero(void);
extern lwlib_TPoint4f lwlib_vec4f_ones(void);
extern lwlib_TPoint4f lwlib_vec4f_add(lwlib_TPoint4f a, lwlib_TPoint4f b);
extern lwlib_TPoint4f lwlib_vec4f_sub(lwlib_TPoint4f a, lwlib_TPoint4f b);
extern lwlib_TPoint4f lwlib_vec4f_scale(lwlib_TPoint4f a, double s);
extern double lwlib_vec4f_dot(lwlib_TPoint4f a, lwlib_TPoint4f b);
extern double lwlib_vec4f_length(lwlib_TPoint4f a);
extern double lwlib_vec4f_length_sq(lwlib_TPoint4f a);
extern lwlib_TPoint4f lwlib_vec4f_normalize(lwlib_TPoint4f a);
extern lwlib_TPoint4f lwlib_vec4f_neg(lwlib_TPoint4f a);
extern lwlib_TPoint4f lwlib_vec4f_mult(lwlib_TPoint4f a, lwlib_TPoint4f b);
extern double lwlib_vec4f_min(lwlib_TPoint4f a);
extern double lwlib_vec4f_max(lwlib_TPoint4f a);
extern lwlib_TPoint4f lwlib_vec4f_clip(lwlib_TPoint4f a);

extern lwlib_TPoint2i lwlib_vec2i(int x, int y);
extern lwlib_TPoint2i lwlib_vec2i_zero(void);
extern lwlib_TPoint2i lwlib_vec2i_ones(void);
extern lwlib_TPoint2i lwlib_vec2i_same(int x);
extern lwlib_TPoint2i lwlib_vec2i_add(lwlib_TPoint2i a, lwlib_TPoint2i b);
extern lwlib_TPoint2i lwlib_vec2i_sub(lwlib_TPoint2i a, lwlib_TPoint2i b);
extern lwlib_TPoint2i lwlib_vec2i_scale(lwlib_TPoint2i a, int s);
extern lwlib_TPoint2i lwlib_vec2i_divide(lwlib_TPoint2i a, int d);
extern lwlib_TPoint2i lwlib_vec2i_op_scalar(lwlib_TPoint2i a, lwlib_TVectorOp op, int b);
extern int lwlib_vec2i_dot(lwlib_TPoint2i a, lwlib_TPoint2i b);
extern int lwlib_vec2i_length_sq(lwlib_TPoint2i a);
extern lwlib_TPoint2i lwlib_vec2i_neg(lwlib_TPoint2i a);
extern int lwlib_vec2i_min(lwlib_TPoint2i a);
extern int lwlib_vec2i_max(lwlib_TPoint2i a);
extern lwlib_TPoint2i lwlib_vec2i_transform(lwlib_TPoint2i a, int (*f)(int));
extern int lwlib_vec2i_equal(lwlib_TPoint2i a, lwlib_TPoint2i b);
extern lwlib_TPointPair2i lwlib_vec2i_pair(lwlib_TPoint2i a, lwlib_TPoint2i b);
extern int lwlib_vec2i_check_pow2(lwlib_TPoint2i a);
extern int lwlib_vec2i_sign_bits(lwlib_TPoint2i a);
extern lwlib_TPoint2i lwlib_vec2i_from_bits(int bits);
extern lwlib_TPoint2i lwlib_vec2i_bounded_inc(lwlib_TPoint2i a, lwlib_TPoint2i b);
extern lwlib_TPoint2i lwlib_vec2i_box_inc(lwlib_TPoint2i a, lwlib_TPoint2i b, lwlib_TPoint2i c);
extern char *lwlib_vec2i_to_string(lwlib_String_t string, lwlib_TPoint2i a);
extern lwlib_TPoint2i lwlib_vec2i_from_string(const char *string);
extern lwlib_TVector2i lwlib_vec2i_facing_normal(int facing);
extern int lwlib_vec2i_range(lwlib_TPoint2i a);
extern lwlib_TPoint2f lwlib_vec2i_float(lwlib_TPoint2i a);

extern lwlib_TPoint3i lwlib_vec3i(int x, int y, int z);
extern lwlib_TPoint3i lwlib_vec3i_zero(void);
extern lwlib_TPoint3i lwlib_vec3i_ones(void);
extern lwlib_TPoint3i lwlib_vec3i_same(int x);
extern lwlib_TPoint3i lwlib_vec3i_add(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern lwlib_TPoint3i lwlib_vec3i_sub(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern lwlib_TPoint3i lwlib_vec3i_cross(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern lwlib_TPoint3i lwlib_vec3i_scale(lwlib_TPoint3i a, int s);
extern lwlib_TPoint3i lwlib_vec3i_divide(lwlib_TPoint3i a, int d);
extern lwlib_TPoint3i lwlib_vec3i_op_scalar(lwlib_TPoint3i a, lwlib_TVectorOp op, int b);
extern int lwlib_vec3i_dot(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern int lwlib_vec3i_length_sq(lwlib_TPoint3i a);
extern lwlib_TPoint3i lwlib_vec3i_neg(lwlib_TPoint3i a);
extern int lwlib_vec3i_min(lwlib_TPoint3i a);
extern int lwlib_vec3i_max(lwlib_TPoint3i a);
extern lwlib_TPoint3i lwlib_vec3i_transform(lwlib_TPoint3i a, int (*f)(int));
extern int lwlib_vec3i_equal(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern lwlib_TPointPair3i lwlib_vec3i_pair(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern int lwlib_vec3i_check_pow2(lwlib_TPoint3i a);
extern int lwlib_vec3i_sign_bits(lwlib_TPoint3i a);
extern lwlib_TPoint3i lwlib_vec3i_from_bits(int bits);
extern lwlib_TPoint3i lwlib_vec3i_bounded_inc(lwlib_TPoint3i a, lwlib_TPoint3i b);
extern lwlib_TPoint3i lwlib_vec3i_box_inc(lwlib_TPoint3i a, lwlib_TPoint3i b, lwlib_TPoint3i c);
extern char *lwlib_vec3i_to_string(lwlib_String_t string, lwlib_TPoint3i a);
extern lwlib_TPoint3i lwlib_vec3i_from_string(const char *string);
extern lwlib_TVector3i lwlib_vec3i_facing_normal(int facing);
extern lwlib_TPoint3f lwlib_vec3i_float(lwlib_TPoint3i a);

extern lwlib_TBox3i lwlib_box3i_cell(lwlib_TPoint3i a);
extern int lwlib_box3i_test(lwlib_TBox3i a, lwlib_TPoint3i b);
extern lwlib_TVector3i lwlib_box3i_span(lwlib_TBox3i a);
extern lwlib_TBox3i lwlib_box3i_surface_facing(lwlib_TBox3i a, int facing);
extern int lwlib_box3i_touch(lwlib_TBox3i a, lwlib_TBox3i b);
extern lwlib_TBox3i lwlib_box3i_intersect(lwlib_TBox3i a, lwlib_TBox3i b);
extern lwlib_TPoint3i lwlib_box3i_face_vert(const lwlib_TBox3i box, int facing, int vertNum);
extern bool lwlib_box3i_face_visible(const lwlib_TBox3i box, int facing, lwlib_TPoint3i viewPos);
extern char *lwlib_box3i_string(lwlib_String_t string, lwlib_TBox3i a);
extern bool lwlib_box3i_face_visible(const lwlib_TBox3i box, int facing, lwlib_TPoint3i viewPos);
extern lwlib_TBox3f lwlib_box3i_float(lwlib_TBox3i a);
extern bool lwlib_box3i_contains(lwlib_TBox3i a, lwlib_TBox3i b);
extern lwlib_TBox3i lwlib_box3i_translate(lwlib_TBox3i a, lwlib_TVector3i b);
extern bool lwlib_box3i_empty(lwlib_TBox3i a);

extern lwlib_TPoint3b lwlib_vec3b(unsigned char x, unsigned char y, unsigned char z);
extern lwlib_TPoint3b lwlib_vec3b_zero(void);

extern lwlib_TPoint4b lwlib_vec4b(unsigned char x, unsigned char y, unsigned char z, unsigned char w);
extern lwlib_TPoint4b lwlib_vec4b_zero(void);

/* line-object intersections */

typedef struct {
	lwlib_TPoint3f point;
	lwlib_TVector3f direction;
} lwlib_TLine;

typedef struct {
	lwlib_TPoint3f vertex;
	lwlib_TVector3f axis;
	double angle;
	double lowerRim;          /* distance to lower rim or 0 */
	double upperRim;          /* distance to upper rim or 0 */
	int acceptRay;            /* 1: accept ray through cone, 0: reject ray through cone */
	int trivialTest;          /* 0: use general solution, 1: use trivial solution */
	double minRayProj;        /* minimum bound on ray normal projection (trivial solution) */
	double maxRayProj;        /* maximum bound on ray normal projection (trivial solution) */
} lwlib_TCone;

extern int lwlib_hit_line_cone(lwlib_TLine line, lwlib_TCone cone, lwlib_TPoint3f *hit);

/* array rotations */

#define lwlib_RotateArray(arr, n, type)         \
do {                                            \
	int __i;                                    \
	type s;                                     \
                                                \
	assert((n) > 0);                            \
	s = (arr)[0];                               \
                                                \
	for (__i = 0; __i < (n) - 1; __i++)         \
	{                                           \
		(arr)[__i] = (arr)[__i + 1];            \
	}                                           \
                                                \
	(arr)[(n) - 1] = s;                         \
} while(0)

#define lwlib_RotateArrayK(arr, n, k, type)     \
do {                                            \
	int __i;                                    \
	int __k2 = (k);                             \
                                                \
	while (__k2 < 0)                            \
	{                                           \
		__k2 += (n);                            \
	}                                           \
	__k2 %= (n);                                \
                                                \
	for (__i = 0; __i < __k2; __i++)            \
	{                                           \
		lwlib_RotateArray((arr), (n), type);    \
	}                                           \
} while(0)

extern void lwlib_vecni_rot(int *arr, int n);
extern void lwlib_vecni_rot_k(int *arr, int n, int k);


#define lwlib_OverlapTest(name) lwlib_OverlapTest_ ## name

#define lwlib_OverlapTest_Touch false
#define lwlib_OverlapTest_Containment true

typedef bool lwlib_OverlapTest_t;

bool lwlib_vec3f_box_halfspace_overlap(lwlib_TBox3f box, lwlib_TPlane3f plane, lwlib_OverlapTest_t test);
bool lwlib_vec3f_box_sphere_overlap(lwlib_TBox3f box, lwlib_TSphere3f sphere, lwlib_OverlapTest_t test);

//
// ======================
//    FIXED POINT MATH
// ======================
//

typedef struct lwlib_vec3fixed_s
{
    fixedpt v[3];
} lwlib_vec3fixed_t;

typedef struct lwlib_mat3fixed_s
{
    fixedpt v[3 * 3];
} lwlib_mat3fixed_t;

static inline lwlib_vec3fixed_t lwlib_vec3fixed(int32_t x, int32_t y, int32_t z)
{
    lwlib_vec3fixed_t a;
    lwlib_C(a, 0) = x;
    lwlib_C(a, 1) = y;
    lwlib_C(a, 2) = z;
    return a;
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_zero(void)
{
    lwlib_vec3fixed_t a = { 0, 0, 0 };
    return a;
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_add(lwlib_vec3fixed_t a, lwlib_vec3fixed_t b)
{
    return lwlib_vec3fixed(
        lwlib_C(a, 0) + lwlib_C(b, 0), 
        lwlib_C(a, 1) + lwlib_C(b, 1),
        lwlib_C(a, 2) + lwlib_C(b, 2)
        );
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_subtract(lwlib_vec3fixed_t a, lwlib_vec3fixed_t b)
{
    return lwlib_vec3fixed(
        lwlib_C(a, 0) - lwlib_C(b, 0), 
        lwlib_C(a, 1) - lwlib_C(b, 1),
        lwlib_C(a, 2) - lwlib_C(b, 2)
        );
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_negate(lwlib_vec3fixed_t a)
{
    return lwlib_vec3fixed(-lwlib_C(a, 0), -lwlib_C(a, 1), -lwlib_C(a, 2));
}

static inline fixedpt lwlib_vec3fixed_dot(lwlib_vec3fixed_t a, lwlib_vec3fixed_t b)
{
    return fixedpt_mul(lwlib_C(a, 0), lwlib_C(b, 0)) + fixedpt_mul(lwlib_C(a, 1), lwlib_C(b, 1)) + 
        fixedpt_mul(lwlib_C(a, 2), lwlib_C(b, 2));
}

static inline lwlib_TPoint2i lwlib_vec3fixed_trunc(lwlib_vec3fixed_t a)
{
    return lwlib_vec2i(fixedpt_toint(lwlib_X(a)), fixedpt_toint(lwlib_Y(a)));
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_from2i(lwlib_TPoint2i a)
{
    return lwlib_vec3fixed(fixedpt_fromint(lwlib_X(a)), fixedpt_fromint(lwlib_Y(a)), 0);
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_scale(lwlib_vec3fixed_t a, fixedpt b)
{
    return lwlib_vec3fixed(fixedpt_mul(lwlib_X(a), b), fixedpt_mul(lwlib_Y(a), b), fixedpt_mul(lwlib_Z(a), b));
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_div(lwlib_vec3fixed_t a, fixedpt b)
{
    return lwlib_vec3fixed(fixedpt_div(lwlib_X(a), b), fixedpt_div(lwlib_Y(a), b), fixedpt_div(lwlib_Z(a), b));
}

lwlib_vec3fixed_t lwlib_vec3fixed_anglevec(int32_t angle);

static inline fixedpt lwlib_vec3fixed_length(lwlib_vec3fixed_t a)
{
    return fixedpt_sqrt(lwlib_vec3fixed_dot(a, a));
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_normalize(lwlib_vec3fixed_t a)
{
    lwlib_vec3fixed_t b;
    fixedpt n;

    n = fixedpt_sqrt(lwlib_vec3fixed_dot(a, a));
    return n ? lwlib_vec3fixed(fixedpt_div(lwlib_X(a), n), 
        fixedpt_div(lwlib_Y(a), n), fixedpt_div(lwlib_Z(a), n)) : 
        a;
}

static inline lwlib_vec3fixed_t lwlib_vec3fixed_unit(lwlib_vec3fixed_t a, lwlib_vec3fixed_t b)
{
    return lwlib_vec3fixed_normalize(lwlib_vec3fixed_subtract(b, a));
}

lwlib_mat3fixed_t lwlib_mat3fixed_zero(void);

lwlib_mat3fixed_t lwlib_mat3fixed_ident(void);

lwlib_mat3fixed_t lwlib_mat3fixed_diag(fixedpt a, fixedpt b, fixedpt c);

lwlib_vec3fixed_t lwlib_mat3fixed_mult_point(lwlib_mat3fixed_t a, lwlib_vec3fixed_t b);

static inline lwlib_vec3fixed_t lwlib_mat3fixed_mult_point2d(lwlib_mat3fixed_t a, lwlib_vec3fixed_t b)
{
    return lwlib_vec3fixed(
        fixedpt_mul(lwlib_CM(a, 0, 0), lwlib_C(b, 0)) + fixedpt_mul(lwlib_CM(a, 0, 1), lwlib_C(b, 1)),
        fixedpt_mul(lwlib_CM(a, 1, 0), lwlib_C(b, 0)) + fixedpt_mul(lwlib_CM(a, 1, 1), lwlib_C(b, 1)),
        0);
}

lwlib_mat3fixed_t lwlib_mat3fixed_rot(fixedpt angle);

lwlib_mat3fixed_t lwlib_mat3fixed_transpose(lwlib_mat3fixed_t a);

void lwlib_SetTrigTables(const fixedpt *costable, const fixedpt *sintable);

/* color space conversions */

extern lwlib_TPoint3f lwlib_rgb_to_cielab(lwlib_TPoint3f rgb);

#endif
#endif

