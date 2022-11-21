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
#include <ctype.h>
#include <stdlib.h>
#include "lw_vec.h"

#undef M_PI
#define PI              3.141592657
#define M_PI PI

typedef struct lwlib_Math_s
{
	const fixedpt *costable;
	const fixedpt *sintable;
	int boxFaceVertTable[6][4][3];
} lwlib_Math_t;

static lwlib_Math_t lwlib_Math =
{
	NULL, // costable
	NULL, // sintable
	{
		{
			{ 1, 0, 0 },
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 0, 1 },
		},
		{
			{ 0, 1, 0 },
			{ 0, 0, 0 },
			{ 0, 0, 1 },
			{ 0, 1, 1 },
		},
		{
			{ 1, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 1 },
			{ 1, 1, 1 },
		},
		{
			{ 0, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0, 1 },
			{ 0, 0, 1 },
		},
		{
			{ 0, 0, 1 },
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 1 },
		},
		{
			{ 0, 1, 0 },
			{ 1, 1, 0 },
			{ 1, 0, 0 },
			{ 0, 0, 0 },
		},
	}, // boxFaceVertTable
};


double lwlib_vec3_lerp(double a, double b, double t)
{
	return a * (1 - t) + b * t;
}

lwlib_TPoint2f lwlib_vec2f(double x, double y)
{
	lwlib_TPoint2f a;
	lwlib_InitVector2(a, x, y);
	return a;
}

lwlib_TPoint2f lwlib_vec2f_zero(void)
{
	static lwlib_TPoint2f a;
	return a;
}

lwlib_TPoint2f lwlib_vec2f_ones(void)
{
	static lwlib_TPoint2f a = lwlib_vec2_init(1, 1);
	return a;
}

lwlib_TPoint2f lwlib_vec2f_add(lwlib_TPoint2f a, lwlib_TPoint2f b)
{
	lwlib_TPoint2f c;
	lwlib_AddVector2(c, a, b);
	return c;
}

lwlib_TPoint2f lwlib_vec2f_sub(lwlib_TPoint2f a, lwlib_TPoint2f b)
{
	lwlib_TPoint2f c;
	lwlib_SubVector2(c, a, b);
	return c;
}

lwlib_TPoint2f lwlib_vec2f_scale(lwlib_TPoint2f a, double s)
{
	lwlib_TPoint2f c;
	lwlib_ScaleVector2(c, a, s);
	return c;
}

double lwlib_vec2f_dot(lwlib_TPoint2f a, lwlib_TPoint2f b)
{
	return lwlib_DotVector2(a, b);
}

double lwlib_vec2f_length(lwlib_TPoint2f a)
{
	return lwlib_VectorLength2(a);
}

double lwlib_vec2f_length_sq(lwlib_TPoint2f a)
{
	return lwlib_VectorLengthSq2(a);
}

lwlib_TPoint2f lwlib_vec2f_normalize(lwlib_TPoint2f a)
{
	lwlib_TPoint2f c;
	double n;

	c = a;
	lwlib_NormalizeVector2(n, c);

	return c;
}

lwlib_TPoint2f lwlib_vec2f_neg(lwlib_TPoint2f a)
{
	lwlib_TPoint2f c;
	lwlib_InvertVector2(c, a);
	return c;
}

lwlib_TPoint2f lwlib_vec2f_mult(lwlib_TPoint2f a, lwlib_TPoint2f b)
{
	lwlib_TPoint2f c;
	lwlib_MultVector2(c, a, b);
	return c;
}

double lwlib_vec2f_min(lwlib_TPoint2f a)
{
	double min = lwlib_X(a);
	if (lwlib_Y(a) < min)
		min = lwlib_Y(a);
	return min;
}

double lwlib_vec2f_max(lwlib_TPoint2f a)
{
	double max = lwlib_X(a);
	if (lwlib_Y(a) > max)
		max = lwlib_Y(a);
	return max;
}

lwlib_TPoint2f lwlib_vec2f_clip(lwlib_TPoint2f a)
{
	double max;

	max = lwlib_vec2f_max(a);
	if (max > 1.0) 
	{
		a = lwlib_vec2f_scale(a, 1.0 / max);
	}
	
	return a;
}

double lwlib_vec2f_range(lwlib_TPoint2f a)
{
	return lwlib_Y(a) - lwlib_X(a);
}

char *lwlib_vec2f_to_string(lwlib_String_t string, lwlib_TPoint2f a)
{
	return lwlib_VaString(string, "(%f,%f)", lwlib_ExpandVector2(a));
}

lwlib_TPoint2i lwlib_vec2f_int(lwlib_TPoint2f a)
{
	return lwlib_vec2i((int)lwlib_X(a), (int)lwlib_Y(a));
}

lwlib_TPoint3f lwlib_vec3f(double x, double y, double z)
{
	lwlib_TPoint3f a;
	lwlib_InitVector(a, x, y, z);
	return a;
}

lwlib_TPoint3f lwlib_vec3f_zero(void)
{
	static lwlib_TPoint3f a;
	return a;
}

lwlib_TPoint3f lwlib_vec3f_ones(void)
{
	static lwlib_TPoint3f a = lwlib_vec3_init(1, 1, 1);
	return a;
}

lwlib_TPoint3f lwlib_vec3f_same(double x)
{
	return lwlib_vec3f(x, x, x);
}

lwlib_TPoint3f lwlib_vec3f_add(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	lwlib_TPoint3f c;
	lwlib_AddVector(c, a, b);
	return c;
}

lwlib_TPoint3f lwlib_vec3f_sub(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	lwlib_TPoint3f c;
	lwlib_SubVector(c, a, b);
	return c;
}

lwlib_TPoint3f lwlib_vec3f_cross(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	lwlib_TPoint3f c;
	lwlib_CrossVector(c, a, b);
	return c;
}

lwlib_TPoint3f lwlib_vec3f_scale(lwlib_TPoint3f a, double s)
{
	lwlib_TPoint3f c;
	lwlib_ScaleVector(c, a, s);
	return c;
}

double lwlib_vec3f_dot(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	return lwlib_DotVector(a, b);
}

double lwlib_vec3f_length(lwlib_TPoint3f a)
{
	return lwlib_VectorLength(a);
}

double lwlib_vec3f_length_sq(lwlib_TPoint3f a)
{
	return lwlib_VectorLengthSq(a);
}

lwlib_TPoint3f lwlib_vec3f_normalize(lwlib_TPoint3f a)
{
	lwlib_TPoint3f c;
	double n;

	c = a;
	lwlib_NormalizeVector(n, c);

	return c;
}

lwlib_TPoint3f lwlib_vec3f_neg(lwlib_TPoint3f a)
{
	lwlib_TPoint3f c;
	lwlib_InvertVector(c, a);
	return c;
}

lwlib_TPoint3f lwlib_vec3f_mult(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	lwlib_TPoint3f c;
	lwlib_MultVector(c, a, b);
	return c;
}

double lwlib_vec3f_min(lwlib_TPoint3f a)
{
	double min = lwlib_X(a);
	if (lwlib_Y(a) < min)
		min = lwlib_Y(a);
	if (lwlib_Z(a) < min)
		min = lwlib_Z(a);
	return min;
}

double lwlib_vec3f_max(lwlib_TPoint3f a)
{
	double max = lwlib_X(a);
	if (lwlib_Y(a) > max)
		max = lwlib_Y(a);
	if (lwlib_Z(a) > max)
		max = lwlib_Z(a);
	return max;
}

lwlib_TPoint3f lwlib_vec3f_clip(lwlib_TPoint3f a)
{
	double max;

	max = lwlib_vec3f_max(a);
	if (max > 1.0) 
	{
		a = lwlib_vec3f_scale(a, 1.0 / max);
	}
	
	return a;
}

unsigned long lwlib_vec3f_quantize(lwlib_TPoint3f a)
{
	int i;
	lwlib_TPoint3i b;

	for (i = 0; i < 3; i++)
		b.v[i] = lwlib_MIN((int)(a.v[i] * 255.0 + 0.5), 255);

	#ifdef RGB_LSB_FIRST
	return (b.v[0] << 16UL) | (b.v[1] << 8UL) | (b.v[2] << 0UL);
	#else
	return (b.v[0] << 0UL) | (b.v[1] << 8UL) | (b.v[2] << 16UL);
	#endif
}

lwlib_TPoint3f lwlib_vec3f_swap_components(lwlib_TPoint3f a)
{
	return lwlib_vec3f(lwlib_Z(a), lwlib_Y(a), lwlib_X(a));
}

lwlib_TPoint3f lwlib_vec3f_transform(lwlib_TPoint3f a, double (*f)(double))
{
	lwlib_TPoint3f b;
	lwlib_TransformVector(b, a, f);
	return b;
}

int lwlib_vec3f_side(lwlib_TPlane3f plane, lwlib_TPoint3f a, double eps)
{
	double dot;
	dot = lwlib_vec3f_dot(lwlib_vec3f_sub(a, lwlib_plane_origin(plane)), lwlib_plane_normal(plane));
	return dot > eps ? 1 : (dot < -eps ? -1 : 0);
}

int lwlib_vec3f_equal(lwlib_TPoint3f a, lwlib_TPoint3f b, double eps)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (fabs(a.v[i] - b.v[i]) > eps)
			return 0;
	}

	return 1;
}

lwlib_TVector3f lwlib_vec3f_normal(lwlib_TPoint3f a, lwlib_TPoint3f b, lwlib_TPoint3f c)
{
	double n;
	lwlib_TVector3f v;

	v = lwlib_vec3f_cross(lwlib_vec3f_sub(b, a), lwlib_vec3f_sub(c, a));
	lwlib_NormalizeVector(n, v);

	return v;
}

lwlib_TPlane3f lwlib_vec3f_plane(lwlib_TPoint3f a, lwlib_TPoint3f b, lwlib_TPoint3f c)
{
	return lwlib_plane3f(a, lwlib_vec3f_normal(a, b, c));
}

lwlib_TPointPair3f lwlib_vec3f_pair(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	lwlib_TPointPair3f pair;
	pair.p[0] = a;
	pair.p[1] = b;
	return pair;
}

lwlib_TPoint3i lwlib_vec3f_int(lwlib_TPoint3f a)
{
	return lwlib_vec3i((int)lwlib_X(a), (int)lwlib_Y(a), (int)lwlib_Z(a));
}

int lwlib_vec3f_sphere_side(lwlib_TSphere3f a, lwlib_TPoint3f b, double eps)
{
	double dot, radiusSq;
	b = lwlib_vec3f_sub(b, a.origin);
	dot = lwlib_vec3f_dot(b, b);
	radiusSq = a.radius * a.radius;
	return dot > radiusSq + eps ? 1 : (dot < radiusSq - eps ? -1 : 0);
}

double lwlib_vec3f_face_area(lwlib_TFace3f face, lwlib_TVector3f normal)
{
	int i;
	lwlib_TPoint3f v;
	int N = face.numPoints;

	// 2A = n dot sum(0,N-1) (V(i) x V(i+1))

	v = lwlib_vec3f_zero();
	for (i = 0; i < N; i++)
	{
		v = lwlib_vec3f_add(v, 
				lwlib_vec3f_cross(LWLIB_FACE_POINT(face, i), 
				LWLIB_FACE_POINT(face, (i + 1) % N)));
	}
	
	return lwlib_vec3f_dot(normal, v) / 2.0;
}

lwlib_TFace3f lwlib_vec3f_face(void *priv, lwlib_TFacePoint3f points, int numPoints)
{
	lwlib_TFace3f face;
	face.priv = priv;
	face.points = points;
	face.numPoints = numPoints;
	return face;
}

int lwlib_vec3f_face_side(lwlib_TFace3f face, lwlib_TPlane3f plane, double eps)
{
	int i;
	int ret;
	int counted[2] = { 0, 0 };

	for (i = 0; i < face.numPoints; i++)
	{
		ret = lwlib_vec3f_side(plane, LWLIB_FACE_POINT(face, i), eps);
		if (ret > 0)
			counted[0]++;
		else if (ret < 0)
			counted[1]++;
	}

	if (counted[0] == 0 && counted[1] == 0)
		return LWLIB_FACE_SIDE_COPLANAR;
	if (counted[0] != 0 && counted[1] != 0)
		return LWLIB_FACE_SIDE_SPLIT;
	if (counted[0] != 0 && counted[1] == 0)
		return LWLIB_FACE_SIDE_FRONT;
	if (counted[0] == 0 && counted[1] != 0)
		return LWLIB_FACE_SIDE_BACK;

	assert(0);
}

lwlib_TBox3f lwlib_vec3f_face_box(lwlib_TFace3f face)
{
	int i;
	lwlib_TBox3f box = lwlib_box3f(lwlib_vec3f_zero(), lwlib_vec3f_zero());

	for (i = 0; i < face.numPoints; i++)
	{
		if (i == 0)
			box = lwlib_box3f(LWLIB_FACE_POINT(face, i), LWLIB_FACE_POINT(face, i));
		else
			box = lwlib_box3f_expand(box, LWLIB_FACE_POINT(face, i));
	}
	
	return box;
}

lwlib_TPoint3f lwlib_vec3f_face_sum(lwlib_TFace3f face)
{
	int i;
	lwlib_TPoint3f v;
	int N = face.numPoints;

	v = lwlib_vec3f_zero();
	for (i = 0; i < N; i++)
	{
		v = lwlib_vec3f_add(v, LWLIB_FACE_POINT(face, i));
	}
	return v;
}

lwlib_TPoint3f lwlib_vec3f_face_center(lwlib_TFace3f face)
{
	int N = face.numPoints;

	if (N == 0)
	{
		return lwlib_vec3f_zero();
	}

	return lwlib_vec3f_scale(lwlib_vec3f_face_sum(face), 1.0 / (double)N);
}

lwlib_TPoint3f lwlib_vec3f_face_point(void *priv, int n)
{
	return ((lwlib_TPoint3f *)priv)[n];
}

lwlib_TRegion3f lwlib_vec3f_region(void *priv, lwlib_TRegionPlane3f planes, int numPlanes)
{
	lwlib_TRegion3f region;
	lwlib_Zero(region);

	region.priv = priv;
	region.planes = planes;
	region.numPlanes = numPlanes;

	return region;
}

lwlib_TRegion3f lwlib_vec3f_region_face(lwlib_TFace3f face, lwlib_TVector3f normal, lwlib_TPlane3f *planes)
{
	int i;
	lwlib_TRegion3f region;
	lwlib_TVector3f edgeNormal;
	int memAllocated = 0;

	if (!planes)
	{
		planes = lwlib_CallocMany(lwlib_TPlane3f, face.numPoints);
		memAllocated = 1;
	}

	for (i = 0; i < face.numPoints; i++)
	{
		edgeNormal = lwlib_vec3f_cross
			(
				normal, 
				lwlib_vec3f_normalize(
					lwlib_vec3f_sub(
						LWLIB_FACE_POINT(face, (i + 1) % face.numPoints),
						LWLIB_FACE_POINT(face, i)
						)
					)
			);
		planes[i] = lwlib_plane3f(LWLIB_FACE_POINT(face, i), edgeNormal);
	}

	lwlib_Zero(region);
	region.priv = (void *)planes;
	region.planes = lwlib_vec3f_region_plane;
	region.numPlanes = face.numPoints;
	region.memAllocated = memAllocated;

	return region;
}

lwlib_TRegion3f lwlib_vec3f_region_destroy(lwlib_TRegion3f region)
{
	if (region.memAllocated)
	{
		free(region.priv);
	}
	lwlib_Zero(region);
	return region;
}

int lwlib_vec3f_region_test(lwlib_TRegion3f region, lwlib_TPoint3f a, double eps)
{
	int i;
	int ret;
	int on = 0;

	for (i = 0; i < region.numPlanes; i++)
	{
		ret = lwlib_vec3f_side(LWLIB_REGION_PLANE(region, i), a, eps);
		if (ret < 0)
			return -1;
		if (ret == 0)
			on++;
	}

	return on ? 0 : 1;
}

lwlib_TPlane3f lwlib_vec3f_region_plane(void *priv, int n)
{
	return ((lwlib_TPlane3f *)priv)[n];
}

lwlib_TPoint3f lwlib_vec3f_quad_lerp(lwlib_TPoint3f quad[4], double u, double v)
{
	lwlib_TPoint3f lerp;
	lwlib_X(lerp) = lwlib_X(quad[0]) * (1-u)*(1-v) + lwlib_X(quad[1]) * (1-v)*u + 
		lwlib_X(quad[2]) * u*v + lwlib_X(quad[3]) * v*(1-u);
	lwlib_Y(lerp) = lwlib_Y(quad[0]) * (1-u)*(1-v) + lwlib_Y(quad[1]) * (1-v)*u + 
		lwlib_Y(quad[2]) * u*v + lwlib_Y(quad[3]) * v*(1-u);
	lwlib_Z(lerp) = lwlib_Z(quad[0]) * (1-u)*(1-v) + lwlib_Z(quad[1]) * (1-v)*u + 
		lwlib_Z(quad[2]) * u*v + lwlib_Z(quad[3]) * v*(1-u);
	return lerp;
}

lwlib_TPoint3f lwlib_vec3f_rot90(lwlib_TPoint3f a)
{
	return lwlib_vec3f(-lwlib_Y(a), lwlib_X(a), lwlib_Z(a));
}

lwlib_TPoint3f lwlib_vec3f_rot90_anchor(lwlib_TPoint3f a, lwlib_TPoint3f b)
{
	return lwlib_vec3f_add(lwlib_vec3f_rot90(lwlib_vec3f_sub(a, b)), b);
}

lwlib_TVector3f lwlib_vec3f_facing_normal(int facing)
{
	lwlib_TPoint3f normal = lwlib_vec3f_zero();
	if (facing < 0)
	{
		facing = -(facing + 1);
	}
	lwlib_C(normal, facing >> 1) = lwlib_OneMinusNx2(facing & 1);
	return normal;
}

int lwlib_vec3f_sign_bits(lwlib_TPoint3f a)
{
	int i;
	int bits = 0;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(a, i) >= 0.0)
		{
			bits |= (1 << i);
		}
	}
	
	return bits;
}

lwlib_TPoint4f lwlib_vec4f(double x, double y, double z, double w)
{
	lwlib_TPoint4f a;
	lwlib_InitVector4(a, x, y, z, w);
	return a;
}

lwlib_TPoint4f lwlib_vec4f_zero(void)
{
	static lwlib_TPoint4f a;
	return a;
}

lwlib_TPoint4f lwlib_vec4f_ones(void)
{
	static lwlib_TPoint4f a = lwlib_vec4_init(1, 1, 1, 1);
	return a;
}

lwlib_TPoint4f lwlib_vec4f_add(lwlib_TPoint4f a, lwlib_TPoint4f b)
{
	lwlib_TPoint4f c;
	lwlib_AddVector4(c, a, b);
	return c;
}

lwlib_TPoint4f lwlib_vec4f_sub(lwlib_TPoint4f a, lwlib_TPoint4f b)
{
	lwlib_TPoint4f c;
	lwlib_SubVector4(c, a, b);
	return c;
}

lwlib_TPoint4f lwlib_vec4f_scale(lwlib_TPoint4f a, double s)
{
	lwlib_TPoint4f c;
	lwlib_ScaleVector4(c, a, s);
	return c;
}

double lwlib_vec4f_dot(lwlib_TPoint4f a, lwlib_TPoint4f b)
{
	return lwlib_DotVector4(a, b);
}

double lwlib_vec4f_length(lwlib_TPoint4f a)
{
	return lwlib_VectorLength4(a);
}

double lwlib_vec4f_length_sq(lwlib_TPoint4f a)
{
	return lwlib_VectorLengthSq4(a);
}

lwlib_TPoint4f lwlib_vec4f_normalize(lwlib_TPoint4f a)
{
	lwlib_TPoint4f c;
	double n;

	c = a;
	lwlib_NormalizeVector4(n, c);

	return c;
}

lwlib_TPoint4f lwlib_vec4f_neg(lwlib_TPoint4f a)
{
	lwlib_TPoint4f c;
	lwlib_InvertVector4(c, a);
	return c;
}

lwlib_TPoint4f lwlib_vec4f_mult(lwlib_TPoint4f a, lwlib_TPoint4f b)
{
	lwlib_TPoint4f c;
	lwlib_MultVector4(c, a, b);
	return c;
}

double lwlib_vec4f_min(lwlib_TPoint4f a)
{
	double min = lwlib_X(a);
	if (lwlib_Y(a) < min)
		min = lwlib_Y(a);
	if (lwlib_Z(a) < min)
		min = lwlib_Z(a);
	if (lwlib_W(a) < min)
		min = lwlib_W(a);
	return min;
}

double lwlib_vec4f_max(lwlib_TPoint4f a)
{
	double max = lwlib_X(a);
	if (lwlib_Y(a) > max)
		max = lwlib_Y(a);
	if (lwlib_Z(a) > max)
		max = lwlib_Z(a);
	if (lwlib_W(a) > max)
		max = lwlib_W(a);
	return max;
}

lwlib_TPoint4f lwlib_vec4f_clip(lwlib_TPoint4f a)
{
	double max;

	max = lwlib_vec4f_max(a);
	if (max > 1.0) 
	{
		a = lwlib_vec4f_scale(a, 1.0 / max);
	}
	
	return a;
}

lwlib_TPoint3i lwlib_vec3i(int x, int y, int z)
{
	lwlib_TPoint3i a;
	lwlib_InitVector(a, x, y, z);
	return a;
}

lwlib_TPoint3i lwlib_vec3i_zero(void)
{
	static lwlib_TPoint3i a;
	return a;
}

lwlib_TPoint3i lwlib_vec3i_ones(void)
{
	static lwlib_TPoint3i a = { { 1, 1, 1 } };
	return a;
}

lwlib_TPoint3i lwlib_vec3i_same(int x)
{
	return lwlib_vec3i(x, x, x);
}

lwlib_TPoint3i lwlib_vec3i_add(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	lwlib_TPoint3i c;
	lwlib_AddVector(c, a, b);
	return c;
}

lwlib_TPoint3i lwlib_vec3i_sub(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	lwlib_TPoint3i c;
	lwlib_SubVector(c, a, b);
	return c;
}

lwlib_TPoint3i lwlib_vec3i_cross(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	lwlib_TPoint3i c;
	lwlib_CrossVector(c, a, b);
	return c;
}

lwlib_TPoint3i lwlib_vec3i_scale(lwlib_TPoint3i a, int s)
{
	lwlib_TPoint3i c;
	lwlib_ScaleVector(c, a, s);
	return c;
}

lwlib_TPoint3i lwlib_vec3i_divide(lwlib_TPoint3i a, int d)
{
	lwlib_TPoint3i c;
	lwlib_DivideVector(c, a, d);
	return c;
}

lwlib_TPoint3i lwlib_vec3i_op_scalar(lwlib_TPoint3i a, lwlib_TVectorOp op, int b)
{
#define OP_SCALAR(vectorOp, binaryOp) \
case vectorOp: \
	for (i = 0; i < 3; i++) \
	{ \
		lwlib_C(a, i) = lwlib_C(a, i) binaryOp b; \
	} \
	break;

	int i;
	switch (op)
	{
		OP_SCALAR(LWLIB_OP_ADD, +)
		OP_SCALAR(LWLIB_OP_SUB, -)
		OP_SCALAR(LWLIB_OP_MULT, *)
		OP_SCALAR(LWLIB_OP_DIVIDE, /)
		OP_SCALAR(LWLIB_OP_SHIFTLEFT, <<)
		OP_SCALAR(LWLIB_OP_SHIFTRIGHT, >>)
		default:
			assert(0);
	}
	return a;
#undef OP_SCALAR
}

int lwlib_vec3i_dot(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	return lwlib_DotVector(a, b);
}

int lwlib_vec3i_length_sq(lwlib_TPoint3i a)
{
	return lwlib_VectorLengthSq(a);
}

lwlib_TPoint3i lwlib_vec3i_neg(lwlib_TPoint3i a)
{
	lwlib_TPoint3i c;
	lwlib_InvertVector(c, a);
	return c;
}

int lwlib_vec3i_min(lwlib_TPoint3i a)
{
	int min = lwlib_X(a);
	if (lwlib_Y(a) < min)
		min = lwlib_Y(a);
	if (lwlib_Z(a) < min)
		min = lwlib_Z(a);
	return min;
}

int lwlib_vec3i_max(lwlib_TPoint3i a)
{
	int max = lwlib_X(a);
	if (lwlib_Y(a) > max)
		max = lwlib_Y(a);
	if (lwlib_Z(a) > max)
		max = lwlib_Z(a);
	return max;
}

lwlib_TPoint3i lwlib_vec3i_transform(lwlib_TPoint3i a, int (*f)(int))
{
	lwlib_TPoint3i b;
	lwlib_TransformVector(b, a, f);
	return b;
}

int lwlib_vec3i_equal(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (a.v[i] != b.v[i])
		{
			return 0;
		}
	}

	return 1;
}

lwlib_TPointPair3i lwlib_vec3i_pair(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	lwlib_TPointPair3i pair;
	pair.p[0] = a;
	pair.p[1] = b;
	return pair;
}

int lwlib_vec3i_check_pow2(lwlib_TPoint3i a)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (!lwlib_IntegerIsPow2(a.v[i]))
		{
			return 0;
		}
	}

	return 1;
}

int lwlib_vec3i_sign_bits(lwlib_TPoint3i a)
{
	int i;
	int bits = 0;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(a, i) >= 0)
		{
			bits |= (1 << i);
		}
	}
	
	return bits;
}

lwlib_TPoint3i lwlib_vec3i_from_bits(int bits)
{
	int i;
	lwlib_TPoint3i a = lwlib_vec3i_zero();

	for (i = 0; i < 3; i++)
	{
		if (bits & (1 << i))
		{
			lwlib_C(a, i) = 1;
		}
	}

	return a;
}

lwlib_TPoint3i lwlib_vec3i_bounded_inc(lwlib_TPoint3i a, lwlib_TPoint3i b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(a, i) < lwlib_C(b, i) - 1)
		{
			lwlib_C(a, i)++;
			break;
		}
		else // lwlib_C(a, i) == lwlib_C(b, i) - 1
		{
			lwlib_C(a, i) = 0;
		}
	}

	return a;
}

lwlib_TPoint3i lwlib_vec3i_box_inc(lwlib_TPoint3i a, lwlib_TPoint3i b, lwlib_TPoint3i c)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(c, i) < lwlib_C(b, i) - 1)
		{
			lwlib_C(c, i)++;
			break;
		}
		else // lwlib_C(c, i) == lwlib_C(b, i) - 1
		{
			lwlib_C(c, i) = lwlib_C(a, i);
		}
	}

	return c;
}

char *lwlib_vec3i_to_string(lwlib_String_t string, lwlib_TPoint3i a)
{
	return lwlib_VaString(string, "(%d,%d,%d)", lwlib_ExpandVector(a));
}

lwlib_TPoint3i lwlib_vec3i_from_string(const char *string)
{
	int i;
	lwlib_String_t tmpstr;
	lwlib_TPoint3i pos;
	char *p, *q;
	char ch;
	int state;

	i = 0;
	lwlib_StrCopy(tmpstr, string);
	pos = lwlib_vec3i_zero();
	p = tmpstr;
	state = 0;

	while (1)
	{
		ch = *p;
		if (isspace(ch))
		{
			continue;
		}
		if (ch == '\0')
		{
			return pos;
		}

		switch (state)
		{
		case 0:
			if (ch == '(')
			{
				state = 1;
			}
			else
			{
				return pos;
			}
			break;
		case 1:
			if (ch == '-' || isdigit(ch))
			{
				q = p;
				state = 2;
			}
			else
			{
				return pos;
			}
			break;
		case 2:
			if (isdigit(ch))
			{
				// no action
			}
			else if (ch == ',' || ch == ')')
			{
				*p = '\0';
				lwlib_C(pos, i) = atoi(q);
				i++;

				if (ch == ',')
				{
					if (i < 3)
					{
						state = 1;
					}
					else
					{
						return pos;
					}
				}
				else if (ch == ')')
				{
					return pos;
				}
			}
			else
			{
				return pos;
			}
			break;
		}
		p++;
	}

	return pos;
}

lwlib_TVector3i lwlib_vec3i_facing_normal(int facing)
{
	lwlib_TPoint3i normal = lwlib_vec3i_zero();
	if (facing < 0)
	{
		facing = -(facing + 1);
	}
	lwlib_C(normal, facing >> 1) = lwlib_OneMinusNx2(facing & 1);
	return normal;
}

lwlib_TPoint3f lwlib_vec3i_float(lwlib_TPoint3i a)
{
	return lwlib_vec3f(lwlib_ExpandVector(a));
}

lwlib_TPoint2i lwlib_vec2i(int x, int y)
{
	lwlib_TPoint2i a;
	lwlib_InitVector2(a, x, y);
	return a;
}

lwlib_TPoint2i lwlib_vec2i_zero(void)
{
	static lwlib_TPoint2i a;
	return a;
}

lwlib_TPoint2i lwlib_vec2i_ones(void)
{
	static lwlib_TPoint2i a = { { 1, 1 } };
	return a;
}

lwlib_TPoint2i lwlib_vec2i_same(int x)
{
	return lwlib_vec2i(x, x);
}

lwlib_TPoint2i lwlib_vec2i_add(lwlib_TPoint2i a, lwlib_TPoint2i b)
{
	lwlib_TPoint2i c;
	lwlib_AddVector2(c, a, b);
	return c;
}

lwlib_TPoint2i lwlib_vec2i_sub(lwlib_TPoint2i a, lwlib_TPoint2i b)
{
	lwlib_TPoint2i c;
	lwlib_SubVector2(c, a, b);
	return c;
}

lwlib_TPoint2i lwlib_vec2i_scale(lwlib_TPoint2i a, int s)
{
	lwlib_TPoint2i c;
	lwlib_ScaleVector2(c, a, s);
	return c;
}

lwlib_TPoint2i lwlib_vec2i_divide(lwlib_TPoint2i a, int d)
{
	lwlib_TPoint2i c;
	lwlib_DivideVector2(c, a, d);
	return c;
}

lwlib_TPoint2i lwlib_vec2i_op_scalar(lwlib_TPoint2i a, lwlib_TVectorOp op, int b)
{
#define OP_SCALAR(vectorOp, binaryOp) \
case vectorOp: \
	for (i = 0; i < 2; i++) \
	{ \
		lwlib_C(a, i) = lwlib_C(a, i) binaryOp b; \
	} \
	break;

	int i;
	switch (op)
	{
		OP_SCALAR(LWLIB_OP_ADD, +)
		OP_SCALAR(LWLIB_OP_SUB, -)
		OP_SCALAR(LWLIB_OP_MULT, *)
		OP_SCALAR(LWLIB_OP_DIVIDE, /)
		OP_SCALAR(LWLIB_OP_SHIFTLEFT, <<)
		OP_SCALAR(LWLIB_OP_SHIFTRIGHT, >>)
		default:
			assert(0);
	}
	return a;
#undef OP_SCALAR
}

int lwlib_vec2i_dot(lwlib_TPoint2i a, lwlib_TPoint2i b)
{
	return lwlib_DotVector2(a, b);
}

int lwlib_vec2i_length_sq(lwlib_TPoint2i a)
{
	return lwlib_VectorLengthSq2(a);
}

lwlib_TPoint2i lwlib_vec2i_neg(lwlib_TPoint2i a)
{
	lwlib_TPoint2i c;
	lwlib_InvertVector2(c, a);
	return c;
}

int lwlib_vec2i_min(lwlib_TPoint2i a)
{
	int min = lwlib_X(a);
	if (lwlib_Y(a) < min)
		min = lwlib_Y(a);
	return min;
}

int lwlib_vec2i_max(lwlib_TPoint2i a)
{
	int max = lwlib_X(a);
	if (lwlib_Y(a) > max)
		max = lwlib_Y(a);
	return max;
}

lwlib_TPoint2i lwlib_vec2i_transform(lwlib_TPoint2i a, int (*f)(int))
{
	lwlib_TPoint2i b;
	lwlib_TransformVector2(b, a, f);
	return b;
}

int lwlib_vec2i_equal(lwlib_TPoint2i a, lwlib_TPoint2i b)
{
	int i;

	for (i = 0; i < 2; i++)
	{
		if (a.v[i] != b.v[i])
		{
			return 0;
		}
	}

	return 1;
}

lwlib_TPointPair2i lwlib_vec2i_pair(lwlib_TPoint2i a, lwlib_TPoint2i b)
{
	lwlib_TPointPair2i pair;
	pair.p[0] = a;
	pair.p[1] = b;
	return pair;
}

int lwlib_vec2i_check_pow2(lwlib_TPoint2i a)
{
	int i;

	for (i = 0; i < 2; i++)
	{
		if (!lwlib_IntegerIsPow2(a.v[i]))
		{
			return 0;
		}
	}

	return 1;
}

int lwlib_vec2i_sign_bits(lwlib_TPoint2i a)
{
	int i;
	int bits = 0;

	for (i = 0; i < 2; i++)
	{
		if (lwlib_C(a, i) >= 0)
		{
			bits |= (1 << i);
		}
	}
	
	return bits;
}

lwlib_TPoint2i lwlib_vec2i_from_bits(int bits)
{
	int i;
	lwlib_TPoint2i a = lwlib_vec2i_zero();

	for (i = 0; i < 2; i++)
	{
		if (bits & (1 << i))
		{
			lwlib_C(a, i) = 1;
		}
	}

	return a;
}

lwlib_TPoint2i lwlib_vec2i_bounded_inc(lwlib_TPoint2i a, lwlib_TPoint2i b)
{
	int i;

	for (i = 0; i < 2; i++)
	{
		if (lwlib_C(a, i) < lwlib_C(b, i) - 1)
		{
			lwlib_C(a, i)++;
			break;
		}
		else // lwlib_C(a, i) == lwlib_C(b, i) - 1
		{
			lwlib_C(a, i) = 0;
		}
	}

	return a;
}

lwlib_TPoint2i lwlib_vec2i_box_inc(lwlib_TPoint2i a, lwlib_TPoint2i b, lwlib_TPoint2i c)
{
	int i;

	for (i = 0; i < 2; i++)
	{
		if (lwlib_C(c, i) < lwlib_C(b, i) - 1)
		{
			lwlib_C(c, i)++;
			break;
		}
		else // lwlib_C(c, i) == lwlib_C(b, i) - 1
		{
			lwlib_C(c, i) = lwlib_C(a, i);
		}
	}

	return c;
}

char *lwlib_vec2i_to_string(lwlib_String_t string, lwlib_TPoint2i a)
{
	return lwlib_VaString(string, "(%d,%d)", lwlib_ExpandVector2(a));
}

lwlib_TPoint2i lwlib_vec2i_from_string(const char *string)
{
	int i;
	lwlib_String_t tmpstr;
	lwlib_TPoint2i pos;
	char *p, *q;
	char ch;
	int state;

	i = 0;
	lwlib_StrCopy(tmpstr, string);
	pos = lwlib_vec2i_zero();
	p = tmpstr;
	state = 0;

	while (1)
	{
		ch = *p;
		if (isspace(ch))
		{
			continue;
		}
		if (ch == '\0')
		{
			return pos;
		}

		switch (state)
		{
		case 0:
			if (ch == '(')
			{
				state = 1;
			}
			else
			{
				return pos;
			}
			break;
		case 1:
			if (ch == '-' || isdigit(ch))
			{
				q = p;
				state = 2;
			}
			else
			{
				return pos;
			}
			break;
		case 2:
			if (isdigit(ch))
			{
				// no action
			}
			else if (ch == ',' || ch == ')')
			{
				*p = '\0';
				lwlib_C(pos, i) = atoi(q);
				i++;

				if (ch == ',')
				{
					if (i < 2)
					{
						state = 1;
					}
					else
					{
						return pos;
					}
				}
				else if (ch == ')')
				{
					return pos;
				}
			}
			else
			{
				return pos;
			}
			break;
		}
		p++;
	}

	return pos;
}

lwlib_TVector2i lwlib_vec2i_facing_normal(int facing)
{
	lwlib_TPoint2i normal = lwlib_vec2i_zero();
	if (facing < 0)
	{
		facing = -(facing + 1);
	}
	lwlib_C(normal, facing >> 1) = lwlib_OneMinusNx2(facing & 1);
	return normal;
}

int lwlib_vec2i_range(lwlib_TPoint2i a)
{
	return lwlib_Y(a) - lwlib_X(a);
}

lwlib_TPoint2f lwlib_vec2i_float(lwlib_TPoint2i a)
{
	return lwlib_vec2f(lwlib_ExpandVector2(a));
}

lwlib_TEdge3f lwlib_edge3f_reverse(lwlib_TEdge3f a)
{
	lwlib_TEdge3f b;
	b.p[0] = a.p[1];
	b.p[1] = a.p[0];
	return b;
}

int lwlib_edge3f_equal(lwlib_TEdge3f a, lwlib_TEdge3f b, double eps)
{
	return lwlib_vec3f_equal(a.p[0], b.p[0], eps) && 
		lwlib_vec3f_equal(a.p[1], b.p[1], eps);
}

int lwlib_edge3f_adj(lwlib_TEdge3f a, lwlib_TEdge3f b, double eps)
{
	return lwlib_edge3f_equal(lwlib_edge3f_reverse(a), b, eps);
}

lwlib_TVector3f lwlib_edge3f_vec(lwlib_TEdge3f a)
{
	return lwlib_vec3f_sub(a.p[1], a.p[0]);
}

lwlib_TPoint3f lwlib_edge3f_lerp(lwlib_TEdge3f a, double t)
{
	return lwlib_vec3f_add(lwlib_vec3f_scale(a.p[0], 1 - t), lwlib_vec3f_scale(a.p[1], t));
}

lwlib_TBox3f lwlib_box3f_cell(lwlib_TPoint3f a)
{
	return lwlib_box3f(a, lwlib_vec3f_add(a, lwlib_vec3f_ones()));
}

lwlib_TBox3f lwlib_box3f_combine(lwlib_TBox3f a, lwlib_TBox3f b)
{
	int i;
	lwlib_TBox3f c;

	for (i = 0; i < 3; i++)
	{
		lwlib_C(lwlib_box_lower(c), i) = lwlib_MIN(lwlib_C(lwlib_box_lower(a), i), lwlib_C(lwlib_box_lower(b), i));
		lwlib_C(lwlib_box_upper(c), i) = lwlib_MAX(lwlib_C(lwlib_box_upper(a), i), lwlib_C(lwlib_box_upper(b), i));
	}

	return c;
}

lwlib_TBox3f lwlib_box3f_expand(lwlib_TBox3f a, lwlib_TPoint3f b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		lwlib_C(lwlib_box_lower(a), i) = lwlib_MIN(lwlib_C(lwlib_box_lower(a), i), lwlib_C(b, i));
		lwlib_C(lwlib_box_upper(a), i) = lwlib_MAX(lwlib_C(lwlib_box_upper(a), i), lwlib_C(b, i));
	}

	return a;
}

lwlib_TBox3f lwlib_box3f_grow(lwlib_TBox3f a, double size)
{
	return lwlib_box3f(
		lwlib_vec3f_sub(lwlib_box_lower(a), lwlib_vec3f_same(size)), 
		lwlib_vec3f_add(lwlib_box_upper(a), lwlib_vec3f_same(size))
		);
}

int lwlib_box3f_touch(lwlib_TBox3f a, lwlib_TBox3f b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(lwlib_box_lower(a), i) >= lwlib_C(lwlib_box_upper(b), i) ||
			lwlib_C(lwlib_box_upper(a), i) <= lwlib_C(lwlib_box_lower(b), i))
		{
			return 0;
		}
	}

	return 1;
}

lwlib_TBox3f lwlib_box3f_intersect(lwlib_TBox3f a, lwlib_TBox3f b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		lwlib_C(lwlib_box_lower(a), i) = 
			lwlib_MAX
				(
					lwlib_C(lwlib_box_lower(a), i), 
					lwlib_C(lwlib_box_lower(b), i)
				);
		lwlib_C(lwlib_box_upper(a), i) = 
			lwlib_MIN
				(
					lwlib_C(lwlib_box_upper(a), i), 
					lwlib_C(lwlib_box_upper(b), i)
				);
	}

	return a;
}

lwlib_TPoint3f lwlib_box3f_face_vert(const lwlib_TBox3f box, int facing, int vertNum)
{
	const int *row;
	assert(facing >= -6 && facing < 6 && vertNum >= 0 && vertNum < 4);

	if (facing >= 0)
	{
		row = lwlib_Math.boxFaceVertTable[facing][vertNum];
	}
	else
	{
		facing = -(facing + 1);
		facing += lwlib_OneMinusNx2(facing & 1);
		row = lwlib_Math.boxFaceVertTable[facing][3 - vertNum];
	}

	lwlib_TPoint3f vert;
	lwlib_RepeatN(3, i, 
		lwlib_C(vert, i) = lwlib_C(box.p[row[i]], i));
	return vert;
}

bool lwlib_box3f_face_visible(const lwlib_TBox3f box, int facing, lwlib_TPoint3f viewPos)
{
	lwlib_TPoint3f p;

	p = lwlib_box3f_face_vert(box, facing, 0);

	if (facing < 0)
	{
		facing = -(facing + 1);
	}
	return (lwlib_C(viewPos, facing >> 1) - lwlib_C(p, facing >> 1)) * 
		lwlib_OneMinusNx2(facing & 1) >= 0;
}

lwlib_TPlane3f lwlib_box3f_face_plane(const lwlib_TBox3f box, int facing)
{
	return lwlib_plane3f(lwlib_box3f_face_vert(box, facing, 0), lwlib_vec3f_facing_normal(facing));
}

lwlib_TBox3f lwlib_box3f_translate(lwlib_TBox3f a, lwlib_TVector3f b)
{
	return lwlib_box3f(lwlib_vec3f_add(lwlib_box_lower(a), b), lwlib_vec3f_add(lwlib_box_upper(a), b));
}

lwlib_TBox3f lwlib_box3f_rotate(lwlib_TBox3f a, lwlib_TMat3f b)
{
	int i;
	lwlib_TBox3f c;
	lwlib_TPoint3f v;

	for (i = 0; i < 8; i++)
	{
		v = lwlib_mat3f_xform(b, lwlib_box3f_corner(a, i));
		if (i == 0)
		{
			c = lwlib_box3f(v, v);
		}
		else
		{
			c = lwlib_box3f_expand(c, v);
		}
	}

	return c;
}

lwlib_TPoint3f lwlib_box3f_corner(lwlib_TBox3f a, int index)
{
	lwlib_TPoint3i b;
	b = lwlib_vec3i_from_bits(index);
	return lwlib_vec3f(lwlib_X(a.p[lwlib_X(b)]), lwlib_Y(a.p[lwlib_Y(b)]), lwlib_Z(a.p[lwlib_Z(b)]));
}

lwlib_TPoint3f lwlib_box3f_center(lwlib_TBox3f a)
{
	return lwlib_vec3f_scale(lwlib_vec3f_add(a.p[0], a.p[1]), 0.5);
}

lwlib_TVector3f lwlib_box3f_span(lwlib_TBox3f a)
{
	return lwlib_vec3f_sub(a.p[1], a.p[0]);
}

bool lwlib_box3f_empty(lwlib_TBox3f a)
{
	return lwlib_X(lwlib_box_lower(a)) >= lwlib_X(lwlib_box_upper(a)) || 
		lwlib_Y(lwlib_box_lower(a)) >= lwlib_Y(lwlib_box_upper(a)) || 
		lwlib_Z(lwlib_box_lower(a)) >= lwlib_Z(lwlib_box_upper(a));
}

lwlib_TBox3i lwlib_box3i_cell(lwlib_TPoint3i a)
{
	return lwlib_box3i(a, lwlib_vec3i_add(a, lwlib_vec3i_ones()));
}

int lwlib_box3i_test(lwlib_TBox3i a, lwlib_TPoint3i b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (b.v[i] < lwlib_box_lower(a).v[i] || 
			b.v[i] >= lwlib_box_upper(a).v[i])
		{
			return 0;
		}
	}

	return 1;
}

lwlib_TVector3i lwlib_box3i_span(lwlib_TBox3i a)
{
	return lwlib_vec3i_sub(a.p[1], a.p[0]);
}

lwlib_TBox3i lwlib_box3i_surface_facing(lwlib_TBox3i a, int facing)
{
	const int rem = facing & 1;
	const int div = facing >> 1;
	lwlib_C(a.p[rem], div) = lwlib_C(a.p[1 - rem], div);
	lwlib_C(a.p[1 - rem], div) += lwlib_OneMinusNx2(rem);
	return a;
}

int lwlib_box3i_touch(lwlib_TBox3i a, lwlib_TBox3i b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(lwlib_box_lower(a), i) >= lwlib_C(lwlib_box_upper(b), i) ||
			lwlib_C(lwlib_box_upper(a), i) <= lwlib_C(lwlib_box_lower(b), i))
		{
			return 0;
		}
	}

	return 1;
}

lwlib_TBox3i lwlib_box3i_intersect(lwlib_TBox3i a, lwlib_TBox3i b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		lwlib_C(lwlib_box_lower(a), i) = 
			lwlib_MAX
				(
					lwlib_C(lwlib_box_lower(a), i), 
					lwlib_C(lwlib_box_lower(b), i)
				);
		lwlib_C(lwlib_box_upper(a), i) = 
			lwlib_MIN
				(
					lwlib_C(lwlib_box_upper(a), i), 
					lwlib_C(lwlib_box_upper(b), i)
				);
	}

	return a;
}

lwlib_TPoint3i lwlib_box3i_face_vert(const lwlib_TBox3i box, int facing, int vertNum)
{
	const int *row;
	assert(facing >= -6 && facing < 6 && vertNum >= 0 && vertNum < 4);

	if (facing >= 0)
	{
		row = lwlib_Math.boxFaceVertTable[facing][vertNum];
	}
	else
	{
		facing = -(facing + 1);
		facing += ((facing & 1) == 0) ? 1 : -1;
		row = lwlib_Math.boxFaceVertTable[facing][3 - vertNum];
	}

	lwlib_TPoint3i vert;
	lwlib_RepeatN(3, i, 
		lwlib_C(vert, i) = lwlib_C(box.p[row[i]], i));
	return vert;
}

bool lwlib_box3i_face_visible(const lwlib_TBox3i box, int facing, lwlib_TPoint3i viewPos)
{
	lwlib_TPoint3i p;

	p = lwlib_box3i_face_vert(box, facing, 0);

	if (facing < 0)
	{
		facing = -(facing + 1);
	}
	return (lwlib_C(viewPos, facing >> 1) - lwlib_C(p, facing >> 1)) * 
		lwlib_OneMinusNx2(facing & 1) >= (facing & 1);
}

extern char *lwlib_box3i_string(lwlib_String_t string, lwlib_TBox3i a)
{
	return lwlib_VaString(string, "[ (%d,%d,%d) (%d,%d,%d) ]", 
		lwlib_ExpandVector(lwlib_box_lower(a)), lwlib_ExpandVector(lwlib_box_upper(a)));
}

lwlib_TBox3f lwlib_box3i_float(lwlib_TBox3i a)
{
	return lwlib_box3f(lwlib_vec3i_float(lwlib_box_lower(a)), lwlib_vec3i_float(lwlib_box_upper(a)));
}

bool lwlib_box3i_contains(lwlib_TBox3i a, lwlib_TBox3i b)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(lwlib_box_lower(a), i) > lwlib_C(lwlib_box_lower(b), i) ||
			lwlib_C(lwlib_box_upper(a), i) < lwlib_C(lwlib_box_upper(b), i))
		{
			return 0;
		}
	}

	return 1;
}

lwlib_TBox3i lwlib_box3i_translate(lwlib_TBox3i a, lwlib_TVector3i b)
{
	return lwlib_box3i(lwlib_vec3i_add(lwlib_box_lower(a), b), lwlib_vec3i_add(lwlib_box_upper(a), b));
}

bool lwlib_box3i_empty(lwlib_TBox3i a)
{
	return lwlib_X(lwlib_box_lower(a)) >= lwlib_X(lwlib_box_upper(a)) || 
		lwlib_Y(lwlib_box_lower(a)) >= lwlib_Y(lwlib_box_upper(a)) || 
		lwlib_Z(lwlib_box_lower(a)) >= lwlib_Z(lwlib_box_upper(a));
}

int lwlib_ray3f_hit_box(lwlib_TRay3f ray, lwlib_TBox3f box)
{
	int i, j;
	int m[3];
	int mlen = 0;
	const lwlib_TPoint3f a = lwlib_ray_start(ray);
	const lwlib_TVector3f v = lwlib_ray_dir(ray);
	const lwlib_TPoint3f bl = lwlib_box_lower(box);
	const lwlib_TPoint3f bu = lwlib_box_upper(box);
	lwlib_TPoint3f hit;
	double s, t;
	int tryHit = 0;

	for (i = 0; i < 3; i++)
	{
		if 
			(
				(lwlib_C(a, i) <= lwlib_C(bl, i) && lwlib_C(v, i) <= 0.0) ||
				(lwlib_C(a, i) >= lwlib_C(bu, i) && lwlib_C(v, i) >= 0.0)
			)
		{
			return 0;
		}
	}

	if (lwlib_X(a) > lwlib_X(bl) && lwlib_X(a) < lwlib_X(bu) &&
		lwlib_Y(a) > lwlib_Y(bl) && lwlib_Y(a) < lwlib_Y(bu) &&
		lwlib_Z(a) > lwlib_Z(bl) && lwlib_Z(a) < lwlib_Z(bu))
	{
		return 1;
	}

	/* define mapping to the indices of non-zero components in the direction
	 * vector of ray */
	for (i = 0; i < 3; i++)
	{
		s = lwlib_Sign(lwlib_C(v, i));
		if (s != 0.0)
		{
			m[mlen++] = i;
		}
	}

	for (i = 0; i < mlen; i++)
	{
		tryHit = 0;
		if (lwlib_Cm(a, i, m) <= lwlib_Cm(bl, i, m) && lwlib_Cm(v, i, m) > 0)
		{
			t = (lwlib_Cm(bl, i, m) - lwlib_Cm(a, i, m)) / lwlib_Cm(v, i, m);
			tryHit = 1;
		}
		else if (lwlib_Cm(a, i, m) >= lwlib_Cm(bu, i, m) && lwlib_Cm(v, i, m) < 0)
		{
			t = (lwlib_Cm(bu, i, m) - lwlib_Cm(a, i, m)) / lwlib_Cm(v, i, m);
			tryHit = 1;
		}

		if (tryHit)
		{
			hit = lwlib_vec3f_add(a, lwlib_vec3f_scale(v, t));

			for (j = 0; j < 3; j++)
			{
				if (m[i] == j)
					continue;

				if (!(lwlib_C(hit, j) >= lwlib_C(bl, j) || lwlib_C(hit, j) <= lwlib_C(bu, j)))
					break;
			}

			if (j == 3)
				return 1;
		}
	}

	return 0;
}

int lwlib_ray3f_hit_plane(lwlib_TRay3f ray, lwlib_TPlane3f plane, lwlib_TPoint3f *hit)
{
	double dist;
	double dirProj;
	double t;

	dist = 
		lwlib_vec3f_dot(
			lwlib_vec3f_sub(
				lwlib_ray_start(ray), lwlib_plane_origin(plane)
				), 
			lwlib_plane_normal(plane)
		);
	dirProj = lwlib_vec3f_dot(lwlib_ray_dir(ray), lwlib_plane_normal(plane));

	if ((dist > 0 && dirProj < 0) || (dist < 0 && dirProj > 0))
	{
		t = -dist / dirProj;
		if (hit)
		{
			*hit = lwlib_vec3f_add(lwlib_ray_start(ray), lwlib_vec3f_scale(lwlib_ray_dir(ray), t));
		}

		return 1;
	}

	return 0;
}

int lwlib_ray3f_hit_face(lwlib_TRay3f ray, lwlib_TRegion3f region, lwlib_TPlane3f plane, double eps)
{
	lwlib_TPoint3f hit;
	return lwlib_ray3f_hit_plane(ray, plane, &hit) && 
		lwlib_vec3f_region_test(region, hit, eps) >= 0;
}

lwlib_TSphere3f lwlib_sphere3f(lwlib_TPoint3f origin, double radius)
{
	lwlib_TSphere3f sphere;
	sphere.origin = origin;
	sphere.radius = radius;
	return sphere;
}

lwlib_TPoint3b lwlib_vec3b(unsigned char x, unsigned char y, unsigned char z)
{
	lwlib_TPoint3b a;
	lwlib_InitVector(a, x, y, z);
	return a;
}

lwlib_TPoint3b lwlib_vec3b_zero(void)
{
	static lwlib_TPoint3b a;
	return a;
}

lwlib_TPoint4b lwlib_vec4b(unsigned char x, unsigned char y, unsigned char z, unsigned char w)
{
	lwlib_TPoint4b a;
	lwlib_InitVector4(a, x, y, z, w);
	return a;
}

lwlib_TPoint4b lwlib_vec4b_zero(void)
{
	static lwlib_TPoint4b a;
	return a;
}

lwlib_TMat3f lwlib_mat3f_zero(int rows, int cols)
{
	lwlib_TMat3f m = 
	{
		{ 
			0.0, 0.0, 0.0,
			0.0, 0.0, 0.0,
			0.0, 0.0, 0.0,
		},
		3, 3,
	};
	m.rows = rows;
	m.cols = cols;
	return m;
}

lwlib_TMat3f lwlib_mat3f_ident(int rows, int cols)
{
	lwlib_TMat3f m = 
	{
		{ 
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
		},
		3, 3,
	};
	m.rows = rows;
	m.cols = cols;
	return m;
}

lwlib_TMat3f lwlib_mat3f_row_mat(int cols, lwlib_TPoint3f a)
{
	lwlib_TMat3f m;
	m = lwlib_mat3f_zero(1, cols);
	lwlib_C(m, 0) = lwlib_C(a, 0);
	lwlib_C(m, 1) = lwlib_C(a, 1);
	lwlib_C(m, 2) = lwlib_C(a, 2);
	return m;
}

lwlib_TMat3f lwlib_mat3f_col_mat(int rows, lwlib_TPoint3f a)
{
	lwlib_TMat3f m;
	m = lwlib_mat3f_zero(rows, 1);
	lwlib_C(m, 0) = lwlib_C(a, 0);
	lwlib_C(m, 3) = lwlib_C(a, 1);
	lwlib_C(m, 6) = lwlib_C(a, 2);
	return m;
}

lwlib_TMat3f lwlib_mat3f_diag(int rows, int cols, lwlib_TPoint3f a)
{
	lwlib_TMat3f m;
	m = lwlib_mat3f_zero(rows, cols);
	lwlib_C(m, 0) = lwlib_C(a, 0);
	lwlib_C(m, 4) = lwlib_C(a, 1);
	lwlib_C(m, 8) = lwlib_C(a, 2);
	return m;
}

lwlib_TMat3f lwlib_mat3f_shift(int axis)
{
	assert(axis >= 0 && axis < 3);
	return lwlib_mat3f_rotate_columns(lwlib_mat3f_ident(3, 3), 2 - axis);
}

lwlib_TMat3f lwlib_mat3f_rot(double angle, int axis)
{
	double rad;
	double sinv, cosv;
	lwlib_TMat3f mat = lwlib_mat3f_ident(3, 3);
	lwlib_TMat3f shift = lwlib_mat3f_shift(axis);

	while (angle < 0)
		angle += 360;
	while (angle >= 360)
		angle -= 360;
	
	if (angle == 0)
	{
		cosv = 1;
		sinv = 0;
	}
	else if (angle == 90)
	{
		cosv = 0;
		sinv = 1;
	}
	else if (angle == 180)
	{
		cosv = -1;
		sinv = 0;
	}
	else if (angle == 270)
	{
		cosv = 0;
		sinv = -1;
	}
	else
	{
		rad = angle / 180 * M_PI;
		cosv = cos(rad);
		sinv = sin(rad);
	}

	lwlib_C(mat, 0) = cosv;
	lwlib_C(mat, 1) = -sinv;
	lwlib_C(mat, 3) = sinv;
	lwlib_C(mat, 4) = cosv;
	
	return lwlib_mat3f_mult(lwlib_mat3f_mult(lwlib_mat3f_xpose(shift), mat), shift);
}

lwlib_TMat3f lwlib_mat3f_add(lwlib_TMat3f a, lwlib_TMat3f b)
{
	int i;
	assert(a.rows == b.rows && a.cols == b.cols);
	for (i = 0; i < 3 * 3; i++)
		lwlib_C(a, i) += lwlib_C(b, i);
	return a;
}

lwlib_TMat3f lwlib_mat3f_sub(lwlib_TMat3f a, lwlib_TMat3f b)
{
	int i;
	assert(a.rows == b.rows && a.cols == b.cols);
	for (i = 0; i < 3 * 3; i++)
		lwlib_C(a, i) -= lwlib_C(b, i);
	return a;
}

lwlib_TMat3f lwlib_mat3f_mult(lwlib_TMat3f a, lwlib_TMat3f b)
{
	int i, j, k;
	lwlib_TMat3f c;

	assert(a.cols == b.rows);
	
	c = lwlib_mat3f_zero(a.rows, b.cols);
	for (i = 0; i < a.rows; i++)
	{
		for (j = 0; j < b.cols; j++)
		{
			for (k = 0; k < a.cols; k++)
				lwlib_CM(c, i, j) += lwlib_CM(a, i, k) * lwlib_CM(b, k, j);
		}
	}
	return c;
}

lwlib_TMat3f lwlib_mat3f_xpose(lwlib_TMat3f a)
{
	int i, j;
	lwlib_TMat3f b;

	b = lwlib_mat3f_zero(a.cols, a.rows);
	for (i = 0; i < a.cols; i++)
	{
		for (j = 0; j < a.rows; j++)
			lwlib_CM(b, i, j) = lwlib_CM(a, j, i);
	}
	return b;
}

lwlib_TMat3f lwlib_mat3f_scale(lwlib_TMat3f a, double b)
{
	int i, j;
	for (i = 0; i < a.rows; i++)
	{
		for (j = 0; j < a.cols; j++)
			lwlib_CM(a, i, j) *= b;
	}
	return a;
}

lwlib_TMat3f lwlib_mat3f_rotate_columns(lwlib_TMat3f a, int times)
{
	int i;

	for (i=0; i < a.rows; i++)
	{
		lwlib_RotateArrayK(&lwlib_CM(a, i, 0), a.cols, times, double);
	}

	return a;
}

lwlib_TPoint3f lwlib_mat3f_row(lwlib_TMat3f a, int row)
{
	assert(row < a.rows);
	return lwlib_vec3f(lwlib_CM(a, row, 0), lwlib_CM(a, row, 1), lwlib_CM(a, row, 2));
}

lwlib_TPoint3f lwlib_mat3f_col(lwlib_TMat3f a, int col)
{
	assert(col < a.cols);
	return lwlib_vec3f(lwlib_CM(a, 0, col), lwlib_CM(a, 1, col), lwlib_CM(a, 2, col));
}

lwlib_TPoint3f lwlib_mat3f_xform(lwlib_TMat3f a, lwlib_TPoint3f b)
{
	return lwlib_vec3f(
		lwlib_CM(a, 0, 0) * lwlib_C(b, 0) + lwlib_CM(a, 0, 1) * lwlib_C(b, 1) + lwlib_CM(a, 0, 2) * lwlib_C(b, 2),
		lwlib_CM(a, 1, 0) * lwlib_C(b, 0) + lwlib_CM(a, 1, 1) * lwlib_C(b, 1) + lwlib_CM(a, 1, 2) * lwlib_C(b, 2),
		lwlib_CM(a, 2, 0) * lwlib_C(b, 0) + lwlib_CM(a, 2, 1) * lwlib_C(b, 1) + lwlib_CM(a, 2, 2) * lwlib_C(b, 2)
		);
}

lwlib_TPlane3f lwlib_mat3f_rotate_plane(lwlib_TMat3f a, lwlib_TPlane3f b)
{
	return lwlib_plane3f(lwlib_mat3f_xform(a, lwlib_plane_origin(b)),
		lwlib_mat3f_xform(a, lwlib_plane_normal(b)));
}

char *lwlib_mat3f_to_string(lwlib_String_t string, lwlib_TMat3f a)
{
	return lwlib_VaString(string, \
		"(%.2lf,%.2lf,%.2lf);" \
		"(%.2lf,%.2lf,%.2lf);" \
		"(%.2lf,%.2lf,%.2lf)", \
		lwlib_CM(a, 0, 0), lwlib_CM(a, 0, 1), lwlib_CM(a, 0, 2),
		lwlib_CM(a, 1, 0), lwlib_CM(a, 1, 1), lwlib_CM(a, 1, 2),
		lwlib_CM(a, 2, 0), lwlib_CM(a, 2, 1), lwlib_CM(a, 2, 2));
}

int lwlib_hit_line_cone(lwlib_TLine line, lwlib_TCone cone, lwlib_TPoint3f *hit)
{
	int i;
	double t, delta, gamma, c0, c1, c2, ring;
	lwlib_TPoint3f Xt;
	lwlib_TMat3f D, P, V, M, grad, A;
	double tList[3];
	int tListLen = 0, lowerHit, upperHit;
	double rayProj;

	// zero cone cannot be hit by line
	if (cone.angle == 0.0)
		return 0;
	
	// trivial intersection test using ray angle
	if (cone.trivialTest)
	{
		rayProj = lwlib_vec3f_dot(line.direction, cone.axis);
		return rayProj >= cone.minRayProj && rayProj <= cone.maxRayProj;
	}

	assert(cone.angle > 0 && cone.angle < 90);
	gamma = cos(cone.angle * M_PI / 180.0);

	P = lwlib_mat3f_col_mat(3, line.point);
	D = lwlib_mat3f_col_mat(3, line.direction);

	V = lwlib_mat3f_col_mat(3, cone.vertex);
	A = lwlib_mat3f_col_mat(3, cone.axis);

	M = 
		lwlib_mat3f_sub(
			lwlib_mat3f_mult(A, lwlib_mat3f_xpose(A)),
			lwlib_mat3f_scale(lwlib_mat3f_ident(3, 3), gamma * gamma)
			);

	grad = lwlib_mat3f_sub(P, V);

	c2 = lwlib_X(lwlib_mat3f_mult(lwlib_mat3f_mult(lwlib_mat3f_xpose(D), M), D));
	c1 = lwlib_X(lwlib_mat3f_mult(lwlib_mat3f_mult(lwlib_mat3f_xpose(D), M), grad));
	c0 = lwlib_X(lwlib_mat3f_mult(lwlib_mat3f_mult(lwlib_mat3f_xpose(grad), M), grad));

	if (c2 != 0.0)
	{
		delta = c1 * c1 - c0 * c2;
		// no real roots
		if (delta < 0.0)
			return 0;
		// double root
		else if (delta == 0.0)
		{
			t = -c1 / c2;
			tList[tListLen++] = t;
		}
		// two distinct real roots
		else // delta > 0.0
		{
			t = (-c1 - sqrt(delta)) / c2;
			tList[tListLen++] = t;

			t = (-c1 + sqrt(delta)) / c2;
			tList[tListLen++] = t;
		}
	}
	else // c2 == 0.0
	{
		if (c1 != 0.0)
		{
			t = -c0 / (2 * c1);
			tList[tListLen++] = t;
		}
		// else line is completely contained in cone
	}

	for (i = 0; i < tListLen; i++)
	{
		t = tList[i];
		if (t < 0.0)
			continue;

		Xt = lwlib_vec3f_add(line.point, lwlib_vec3f_scale(line.direction, t));

		ring = lwlib_vec3f_dot(lwlib_vec3f_sub(Xt, cone.vertex), cone.axis);
		// handle double cone
		if (ring < 0.0)
			continue;

		lowerHit = (cone.lowerRim == 0.0 || ring >= cone.lowerRim);
		upperHit = (cone.upperRim == 0.0 || ring <= cone.upperRim);
		if (!(lowerHit && upperHit))
			continue;

		// found line-cone intersection
		if (hit != NULL)
			*hit = Xt;
		return 1;
	}

	return 0;
}

void lwlib_vecni_rot(int *vec, int n)
{
	int i;
	int s;

	assert(n > 0);
	s = vec[0];

	for (i = 0; i < n - 1; i++)
	{
		vec[i] = vec[i + 1];
	}

	vec[n - 1] = s;
}

void lwlib_vecni_rot_k(int *arr, int n, int k)
{
	int i;

	while (k < 0)
	{
		k += n;
	}
	k %= n;

	for (i = 0; i < k; i++)
	{
		lwlib_vecni_rot(arr, n);
	}
}

bool lwlib_vec3f_box_halfspace_overlap(lwlib_TBox3f box, lwlib_TPlane3f plane, lwlib_OverlapTest_t test)
{
	int bits;
	lwlib_TPoint3f v;

	bits = lwlib_vec3f_sign_bits(lwlib_plane_normal(plane));
	if (test == lwlib_OverlapTest(Touch))
	{
		bits ^= 0x07;
	}

	v = lwlib_box3f_corner(box, bits);
	return lwlib_vec3f_side(plane, v, 0.0) <= 0;
}

static bool lwlib_vec3f_box_touches_sphere(lwlib_TBox3f box, lwlib_TSphere3f sphere)
{
	double dmin;
	double r2;
	int i;

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(sphere.origin, i) + sphere.radius < lwlib_C(lwlib_box_lower(box), i) ||
			lwlib_C(sphere.origin, i) - sphere.radius > lwlib_C(lwlib_box_upper(box), i))
		{
			return false;
		}
	}

	dmin = 0;
	r2 = lwlib_Sq(sphere.radius);
	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(sphere.origin, i) < lwlib_C(lwlib_box_lower(box), i))
		{
			dmin += lwlib_Sq(lwlib_C(sphere.origin, i) - lwlib_C(lwlib_box_lower(box), i));
		}
		else if (lwlib_C(sphere.origin, i) > lwlib_C(lwlib_box_upper(box), i))
		{
			dmin += lwlib_Sq(lwlib_C(sphere.origin, i) - lwlib_C(lwlib_box_upper(box), i));
		}
	}

	if (dmin <= r2)
	{
		return true;
	}

	return false;
}

static bool lwlib_vec3f_box_inside_sphere(lwlib_TBox3f box, lwlib_TSphere3f sphere)
{
	int i;
	lwlib_TPoint3f v;
	lwlib_TPoint3f boxCenter;
	lwlib_TVector3f boxHalfSpan;

	boxCenter = lwlib_box3f_center(box);
	boxHalfSpan = lwlib_vec3f_scale(lwlib_box3f_span(box), 0.5);

	for (i = 0; i < 3; i++)
	{
		if (lwlib_C(boxCenter, i) >= lwlib_C(sphere.origin, i))
		{
			lwlib_C(v, i) = lwlib_C(boxCenter, i) + lwlib_C(boxHalfSpan, i);
		}
		else
		{
			lwlib_C(v, i) = lwlib_C(boxCenter, i) - lwlib_C(boxHalfSpan, i);
		}
	}

	v = lwlib_vec3f_sub(v, sphere.origin);
	return lwlib_vec3f_dot(v, v) <= lwlib_Sq(sphere.radius);
}

bool lwlib_vec3f_box_sphere_overlap(lwlib_TBox3f box, lwlib_TSphere3f sphere, lwlib_OverlapTest_t test)
{
	return test == lwlib_OverlapTest(Touch) ? 
		lwlib_vec3f_box_touches_sphere(box, sphere) :
		lwlib_vec3f_box_inside_sphere(box, sphere);
}

//
// ======================
//    FIXED POINT MATH
// ======================
//

#define LWLIB_VEC_ANGLES 360

lwlib_vec3fixed_t lwlib_vec3fixed_anglevec(int32_t angle)
{
    fixedpt cosv, sinv;

    while (angle < 0)
    {
        angle += LWLIB_VEC_ANGLES;
    }
    while (angle >= LWLIB_VEC_ANGLES)
    {
        angle -= LWLIB_VEC_ANGLES;
    }

    cosv = lwlib_Math.costable[angle];
    sinv = lwlib_Math.sintable[angle];

    return lwlib_vec3fixed(cosv, sinv, 0);
}

lwlib_mat3fixed_t lwlib_mat3fixed_zero(void)
{
    lwlib_mat3fixed_t m = 
    {
        { 
            0, 0, 0,
            0, 0, 0,
            0, 0, 0,
        },
    };
    return m;
}

lwlib_mat3fixed_t lwlib_mat3fixed_ident(void)
{
    lwlib_mat3fixed_t m = 
    {
        { 
            fixedpt_fromint(1), 0, 0,
            0, fixedpt_fromint(1), 0,
            0, 0, fixedpt_fromint(1),
        },
    };
    return m;
}

lwlib_mat3fixed_t lwlib_mat3fixed_diag(fixedpt a, fixedpt b, fixedpt c)
{
    lwlib_mat3fixed_t m = lwlib_mat3fixed_zero();
    lwlib_CM(m, 0, 0) = a;
    lwlib_CM(m, 1, 1) = b;
    lwlib_CM(m, 2, 2) = c;
    return m;
}

lwlib_vec3fixed_t lwlib_mat3fixed_mult_point(lwlib_mat3fixed_t a, lwlib_vec3fixed_t b)
{
    return lwlib_vec3fixed(
        fixedpt_mul(lwlib_CM(a, 0, 0), lwlib_C(b, 0)) + 
        fixedpt_mul(lwlib_CM(a, 0, 1), lwlib_C(b, 1)) +
        fixedpt_mul(lwlib_CM(a, 0, 2), lwlib_C(b, 2)),
        fixedpt_mul(lwlib_CM(a, 1, 0), lwlib_C(b, 0)) +
        fixedpt_mul(lwlib_CM(a, 1, 1), lwlib_C(b, 1)) +
        fixedpt_mul(lwlib_CM(a, 1, 2), lwlib_C(b, 2)),
        fixedpt_mul(lwlib_CM(a, 2, 0), lwlib_C(b, 0)) +
        fixedpt_mul(lwlib_CM(a, 2, 1), lwlib_C(b, 1)) +
        fixedpt_mul(lwlib_CM(a, 2, 2), lwlib_C(b, 2))
        );
}

lwlib_mat3fixed_t lwlib_mat3fixed_rot(int32_t angle)
{
    lwlib_mat3fixed_t rotmat;
    fixedpt cosv, sinv;

    while (angle < 0)
    {
        angle += LWLIB_VEC_ANGLES;
    }
    while (angle >= LWLIB_VEC_ANGLES)
    {
        angle -= LWLIB_VEC_ANGLES;
    }

    cosv = lwlib_Math.costable[angle];
    sinv = lwlib_Math.sintable[angle];

    rotmat = lwlib_mat3fixed_ident();
    lwlib_C(rotmat, 0) = cosv;
    lwlib_C(rotmat, 1) = -sinv;
    lwlib_C(rotmat, 3) = sinv;
    lwlib_C(rotmat, 4) = cosv;

    return rotmat;
}

lwlib_mat3fixed_t lwlib_mat3fixed_transpose(lwlib_mat3fixed_t a)
{
    int i, j;
    lwlib_mat3fixed_t b;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            lwlib_CM(b, i, j) = lwlib_CM(a, j, i);
        }
    }

    return b;
}

void lwlib_SetTrigTables(const fixedpt *costable, const fixedpt *sintable)
{
	lwlib_Math.costable = costable;
	lwlib_Math.sintable = sintable;
}

lwlib_TPoint3f lwlib_rgb_to_cielab(lwlib_TPoint3f rgb)
{
	int c;

	// rgb to xyz
	for (c = 0; c < 3; c++)
	{
		if (lwlib_C(rgb, c) > 0.04045)
		{
			lwlib_C(rgb, c) = powf((lwlib_C(rgb, c) + 0.055) / 1.055,
				2.4);
		}
		else
		{
			lwlib_C(rgb, c) = lwlib_C(rgb, c) / 12.92;
		}
		lwlib_C(rgb, c) *= 100.0;
	}

	// xyz to cielab
	return lwlib_vec3f(
		lwlib_X(rgb) * 0.4124 + lwlib_Y(rgb) * 0.3576 + 
			lwlib_Z(rgb) * 0.1805,
		lwlib_X(rgb) * 0.2126 + lwlib_Y(rgb) * 0.7152 + 
			lwlib_Z(rgb) * 0.0722,
		lwlib_X(rgb) * 0.0193 + lwlib_Y(rgb) * 0.1192 + 
			lwlib_Z(rgb) * 0.9505
		);
}
