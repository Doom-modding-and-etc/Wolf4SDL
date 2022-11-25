/*************************************************************************
** WolfRad
** Copyright (C) 2009-2010 by LinuxWolf
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
** RayCaster
*************************************************************************/

#include "wr_rad.h"
#include "wr_room.h"
#include "wr_raycaster.h"
#include "..\lw_lib\lw_bres.h"
#ifdef WOLFRAD
#include "..\..\wl_def.h"

typedef struct WR_RayCaster_Priv_s
{
	WR_Level_t level;
	WR_Rad_t rad;
	lwlib_Bres_t bres;
} WR_RayCaster_Priv_t;

static unsigned long WR_RayCaster_CollideRay(
	WR_RayCaster_t rayCaster, lwlib_TPoint3i pos, lwlib_TPoint3f start, 
	lwlib_TVector3f dir, lwlib_TPoint3f end, int axis, int qn)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	int quad_index;
	double t;
	lwlib_TPoint3f hit;
	TQuad *quad;
	lwlib_TPoint3f scaled_dir;

	assert(lwlib_C(dir, axis) != 0.0);

	/* fast-lookup of quadrilateral hit by ray */
	quad_index = WR_Level_GetTile(priv->level, pos)->quad_lookup[qn];
	if (quad_index == 0)
	{
		return 0;
	}

	--quad_index;
	if (quad_index < 0 || quad_index >= vec_size(rad_params.quads))
	{
		return 0;
	}

	quad = &vec_at(rad_params.quads, quad_index);
	if (quad->excludeFromScene)
	{
		return 0;
	}

	/* find hit point */
	t = ( lwlib_C(end, axis) - lwlib_C(start, axis) ) / 
		lwlib_C(dir, axis);
	scaled_dir = dir;
	lwlib_ScaleVector(scaled_dir, scaled_dir, t);
	lwlib_AddVector(hit, start, scaled_dir);

	/* calculate index to element hit by ray */
	return WR_Rad_HitElementIndex(priv->rad, hit, quad, true);
}

static int WR_RayCaster_ShootAt(WR_RayCaster_t rayCaster, unsigned long shootPatch, 
	unsigned long elementIndex, lwlib_TPoint3f point)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	int hit;
	lwlib_TLine line;
	TPatch *sp;
	TLight *light;
	unsigned long hitElement;

	sp = &rad_params.patches[shootPatch];

	line.point = sp->raisedCenter;
	line.direction = lwlib_vec3f_normalize(lwlib_vec3f_sub(point, sp->raisedCenter));

	// ray may strike the light cone before hitting element
	if (sp->quad->lightNum != 0)
	{
		hit = lwlib_hit_line_cone(line, sp->quad->cone, NULL);
		if (!(sp->quad->cone.acceptRay ? hit : !hit))
			return 0;
	}

	// attempt to hit element with a ray fired from patch
	hitElement = WR_RayCaster_ShootRay(rayCaster, line.point,
		line.direction, elementIndex);
	if (hitElement != elementIndex + 1)
		return 0;
	
	return 1;
}

static bool WR_RayCaster_ElementIsPushWall(WR_RayCaster_t rayCaster,
	int elementIndex)
{
	int doorNum;
	TElement *ep;
	doorobj_t *doorobj;
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);

	ep = &rad_params.elements[elementIndex];
	doorNum = ep->patch->quad->doorNum;

	if (doorNum != 0)
	{
		doorobj = &doorobjlist[doorNum - 1];
		return doorobj->lock == dr_pwdoor;
	}

	return false;
}

WR_RayCaster_t WR_RayCaster_New(void)
{
	WR_RayCaster_t rayCaster;
	lwlib_Zero(rayCaster);

	lwlib_GetPriv(WR_RayCaster_Priv_t, rayCaster);
	priv = lwlib_CallocSingle(WR_RayCaster_Priv_t);

	rayCaster.priv = priv;
	return rayCaster;
}

WR_RayCaster_t WR_RayCaster_Free(WR_RayCaster_t rayCaster)
{
	lwlib_GetPriv(WR_RayCaster_Priv_t, rayCaster);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(rayCaster);
	return rayCaster;
}

unsigned long WR_RayCaster_ShootRay(WR_RayCaster_t rayCaster, 
	lwlib_TPoint3f start, lwlib_TVector3f direction,
	unsigned long targetElementIndex)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	int i, j;
	int qn;
	double s;
	lwlib_TPoint3f a, b;
	lwlib_TVector3f v;
	lwlib_TVector3f V;
	lwlib_TVector3f G;
	lwlib_TPoint3i pos, nextpos;
	int m[3];
	int mlen = 0;
	double min_g = 0.0;
	int min_g_i = 0;
	int elementIndex;
	TElement *ep;

	/* define vectors a = start, v = direction */
	a = start;
	v = direction;

	/* let pos = int( floor(a / 0.5) ) */
	for (i = 0; i < 2; i++)
	{
		lwlib_C(pos, i) = (int)floor(lwlib_C(a, i) / 0.5);
	}
	lwlib_Z(pos) = 0;

	/* ray cannot start within a wall tile */
	assert(!WR_Level_TileIsWall(priv->level, pos));

	/* 
	 * define vector b where 
	 *
	 *   b(i) = floor( a(i) / 0.5 ) * 0.5 + 0.5
	 *
	 * if v(i) > 0; otherwise
	 *
	 *   b(i) = floor( a(i) / 0.5 ) * 0.5.
	 *
	 */
	for (i = 0; i < 2; i++)
	{
		lwlib_C(b, i) = floor(lwlib_C(a, i) / 0.5) * 0.5;
		if (lwlib_C(v, i) > 0.0)
			lwlib_C(b, i) += 0.5;
	}

	if (lwlib_Z(v) > 0.0)
		lwlib_Z(b) = 1.0;
	else
		lwlib_Z(b) = 0.0;

	/*
	 * to determine the closest plane hit by ray within current tile, find the
	 * smallest of t(x), t(y), t(z) where for any i
	 *
	 *   t(i) = ( b(i) - a(i) ) / v(i)  ... (1)
	 *
	 * let A = abs( prod(all i) v(i) )
	 * let V(i) = abs( prod(j != i) v(j) ) sign( v(i) )
	 * let G(i) = V(i) ( b(i) - a(i) )
	 *
	 * If both sides of (1) are scaled by A > 0, (1) becomes
	 *
	 *   A t(i) = G(i)                  ... (2)
	 *
	 * determining the closest plane hit by ray within current tile is
	 * equivalent to finding the value for i such that G(i) is smallest.
	 *
	 * in the above, v(i) is non-zero for all i
	 *
	 * when closest plane is determined, value for i is used to advance vector
	 * b if next tile is not a wall:
	 *
	 *   b(i) += sign( v(i) )
	 *
	 * or alternatively, G(i) is advanced or re-evaluated:
	 *
	 *   G(i) += V(i) sign( v(i) )
	 *
	 * if next tile is a wall, determine the intersection point using (1) and
	 * then substitute for t in
	 *
	 *   x = a + t v                  ... (3)
	 *
	 * return the element hit by ray.
	 *
	 */

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

	/* V(i) = abs( prod(j != i) v(j) ) sign( v(i) ) */
	for (i = 0; i < mlen; i++)
	{
		/* abs( prod(j != i) v(j) ) */
		lwlib_Cm(V, i, m) = 1.0;
		for (j = 0; j < mlen; j++)
		{
			if (j == i)
				continue;
			lwlib_Cm(V, i, m) *= lwlib_Cm(v, j, m);
		}
		lwlib_Cm(V, i, m) = fabs(lwlib_Cm(V, i, m));

		/* sign must be non-zero since we are mapped into non-zero
		 * components */
		s = lwlib_Sign(lwlib_Cm(v, i, m));
		assert(s != 0.0);
		lwlib_Cm(V, i, m) *= s;
	}

	while (true)
	{
		/* G(i) = V(i) ( b(i) - a(i) ) */
		for (i = 0; i < mlen; i++)
		{
			lwlib_Cm(G, i, m) = lwlib_Cm(V, i, m) * 
				( lwlib_Cm(b, i, m) - lwlib_Cm(a, i, m) );
		}
		
		/* find the value for i such that G(i) is smallest */
		min_g = rad_params.worldSize;
		min_g_i = -1;
		for (i = 0; i < mlen; i++)
		{
			if (lwlib_Cm(G, i, m) < min_g)
			{
				min_g = lwlib_Cm(G, i, m);
				min_g_i = i;
			}
		}
		assert(min_g_i >= 0 && min_g_i < mlen);

		/* determine next tile */
		nextpos = pos;
		lwlib_Cm(nextpos, min_g_i, m) += 
			(int)lwlib_Sign(lwlib_Cm(v, min_g_i, m));

		/* check if ray hit floor or ceiling of current tile */
		if (m[min_g_i] == 2)
		{
			/* find number of intersecting quad */
			qn = lwlib_Z(v) > 0.0 ? WR_LEVEL_QN_CEILING : WR_LEVEL_QN_FLOOR;

			/* return element hit by ray */
			return WR_RayCaster_CollideRay(rayCaster, pos, a, v, b,
				m[min_g_i], qn);
		}

		/* there must be a next tile */
		if (lwlib_Cm(nextpos, min_g_i, m) < 0 ||
			lwlib_Cm(nextpos, min_g_i, m) >= 128)
		{
			/* return background element */
			return 0;
		}

		/* check if ray hit side of current tile */
		{
			/* find number of intersecting quad */
			if (m[min_g_i] == 0)
			{
				qn = lwlib_X(v) > 0.0 ? 
					WR_LEVEL_QN_EASTWALL : WR_LEVEL_QN_WESTWALL;
			}
			else if (m[min_g_i] == 1)
			{
				qn = lwlib_Y(v) > 0.0 ? 
					WR_LEVEL_QN_NORTHWALL : WR_LEVEL_QN_SOUTHWALL;
			}
			else
			{
				assert(0);
			}

			/* return element hit by ray */
			elementIndex = WR_RayCaster_CollideRay(rayCaster, pos,
				a, v, b, m[min_g_i], qn);
			if (elementIndex != 0)
			{
				if (elementIndex - 1 != targetElementIndex && 
					WR_RayCaster_ElementIsPushWall(rayCaster,
					elementIndex - 1))
				{
					// continue ray tracing
				}
				else
				{
					return elementIndex;
				}
			}
		}

		/* advance to next tile */
		pos = nextpos;

		/* check if ray enters a solid tile */
		if (WR_Level_TileIsWall(priv->level, pos))
		{
			assert(0);
		}
		/* ray enters empty tile */
		else
		{
			/* advance b to boundary of empty tile */
			lwlib_Cm(b, min_g_i, m) += 
				0.5 * lwlib_Sign(lwlib_Cm(v, min_g_i, m));
		}
	}

	return 0;
}

static int WR_RayCaster_SearchQuadElements(WR_RayCaster_t rayCaster,
	TQuad *quad, unsigned long shootPatch, lwlib_TPoint3f center, 
	lwlib_TVector3f normal, int *elems, int numElems)
{
	int i;
	int startElement;
	int elementIndex;
	TElement *ep;
	TQuad *equad;
	double du, dv;
	lwlib_TPoint3f elemCenter;
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);

	// need patch in front of quadrilateral
	if (lwlib_vec3f_side(
		lwlib_plane3f(quad->verts[0], quad->normal), 
			center, lwlib_EPSILON) <= 0
		)
	{
		return numElems;
	}

	// need quadrilateral in front of patch
	for (i = 0; i < 4; i++)
	{
		if (lwlib_vec3f_side(lwlib_plane3f(center, normal), 
			quad->verts[i], lwlib_EPSILON) > 0)
		{
			break;
		}
	}
	if (i == 4)
	{
		return numElems;
	}

	startElement = quad->startElement;
	du = 1.0 / quad->nu;
	dv = 1.0 / quad->nv;

	for (i = 0; i < quad->nu * quad->nv; i++)
	{
		elementIndex = startElement + i;
		ep = &rad_params.elements[elementIndex];

		UVToXYZ(quad->verts, ep->u + du/2.0, ep->v + dv/2.0,
			&elemCenter);

		// need element in front of patch
		if (lwlib_vec3f_side(lwlib_plane3f(center, normal),
			elemCenter, lwlib_EPSILON) <= 0)
		{
			continue;
		}

		if (!WR_RayCaster_ShootAt(rayCaster, shootPatch, 
			elementIndex, elemCenter))
		{
			continue;
		}

		assert(numElems < rad_params.nElements);
		elems[numElems++] = elementIndex;
	}

	return numElems;
}

int WR_RayCaster_SearchElements(WR_RayCaster_t rayCaster,
	unsigned long shootPatch, int *elems)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	int i;
	int x, y;
	int cq, cr;
	lwlib_TPoint3i pos, shootpos;
	int quadIndex;
	int qn;
	TQuad *quad;
	int numElems;
	lwlib_TPoint3f center;
	lwlib_TVector3f normal;
	TPatch *sp;
	int shootTileRadius;

	/* get the center of shootPatch */
	sp = &(rad_params.patches[shootPatch]);
	center = sp->raisedCenter;
	normal = sp->normal;

	// find all elements in all visible cells seen by patch
	numElems = 0;

	shootpos = lwlib_vec3i_divide(sp->quad->pos, 2);
	shootTileRadius = sp->quad->lightTileRadius;

	for (x = 0; x < 64; x++)
	{
		for (y = 0; y < 64; y++)
		{
			pos = lwlib_vec3i(x, y, 0);

			if 
				(
					lwlib_Bres_VisBlocked(priv->bres, shootpos, pos) ||
					(	
						shootTileRadius > 0 &&
						(
							abs(lwlib_X(pos) - lwlib_X(shootpos)) >
								shootTileRadius ||
							abs(lwlib_Y(pos) - lwlib_Y(shootpos)) >
								shootTileRadius
						)
					)
				)
			{
				continue;
			}

			for (qn = 0; qn < WR_LEVEL_QN_TOT * 4; qn++)
			{
				cq = (qn / WR_LEVEL_QN_TOT);
				cr = (qn % WR_LEVEL_QN_TOT);

				pos = lwlib_vec3i(x * 2 + (cq % 2), y * 2 + (cq / 2), 0);
				quadIndex = WR_Level_GetTile(priv->level,
					pos)->quad_lookup[cr];
				if (quadIndex == 0)
					continue;

				--quadIndex;
				if (quadIndex < 0 || 
					quadIndex >= vec_size(rad_params.quads))
				{
					continue;
				}

				quad = &vec_at(rad_params.quads, quadIndex);

				numElems = WR_RayCaster_SearchQuadElements(rayCaster, 
					quad, shootPatch, center, normal, elems, numElems);
			}
		}
	}

	return numElems;
}

void WR_RayCaster_SetRad(WR_RayCaster_t rayCaster, WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	priv->rad = rad;
}

void WR_RayCaster_SetLevel(WR_RayCaster_t rayCaster, WR_Level_t level)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	priv->level = level;
}

void WR_RayCaster_SetBres(WR_RayCaster_t rayCaster, lwlib_Bres_t bres)
{
	lwlib_GetPrivAssert(WR_RayCaster_Priv_t, rayCaster);
	priv->bres = bres;
}
#endif