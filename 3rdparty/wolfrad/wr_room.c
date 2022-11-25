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
** Room
*************************************************************************/

#include <stdlib.h>
#include "wr_rad.h"
#include "wr_room.h"
#include "wr_lightmap.h"
#ifdef PS2
//#include "lw_vec.h"
#else
#include "..\..\wl_def.h"
#endif
#ifdef WOLFRAD

typedef struct WR_Room_Priv_s
{
	TPatch* pPatch;
	TElement* pElement;
	WR_Rad_t rad;
	WR_Level_t level;
	WR_LightMapper_t lightMapper;
} WR_Room_Priv_t;

/* Compute the xyz coordinates of a point on a quadrilateral given its u, v coordinates using bi-linear mapping */
void UVToXYZ(const lwlib_TPoint3f quad[4], double u, double v,
	lwlib_TPoint3f* xyz)
{
	lwlib_X(*xyz) = lwlib_X(quad[0]) * (1-u)*(1-v) + lwlib_X(quad[1]) * (1-u)*v + lwlib_X(quad[2]) * u*v + lwlib_X(quad[3]) * u*(1-v);
	lwlib_Y(*xyz) = lwlib_Y(quad[0]) * (1-u)*(1-v) + lwlib_Y(quad[1]) * (1-u)*v + lwlib_Y(quad[2]) * u*v + lwlib_Y(quad[3]) * u*(1-v);
	lwlib_Z(*xyz) = lwlib_Z(quad[0]) * (1-u)*(1-v) + lwlib_Z(quad[1]) * (1-u)*v + lwlib_Z(quad[2]) * u*v + lwlib_Z(quad[3]) * u*(1-v);
}

static TSpectra GetSpectra(double u, double v, TTexture *texture)
{
	int i;
	int x, y;
	TSpectra spectra;
	unsigned char *data;

	x = (int)(v * texture->width);
	if (x >= texture->width)
		x = texture->width - 1;
	if (x < 0)
		x = 0;

	y = (int)((1 - u) * texture->height);
	if (y >= texture->height)
		y = texture->height - 1;
	if (y < 0)
		y = 0;

	data = &texture->data[(y * texture->width + x) * texture->bpp];

	if (texture->bpp >= 3)
	{
		for (i = 0; i < 3; i++)
			lwlib_C(spectra, i) = (double)data[i] / 255.0;
	}
	else if (texture->bpp == 1)
	{
		for (i = 0; i < 3; i++)
			lwlib_C(spectra, i) = (double)data[0] / 255.0;
	}
	else
	{
		fprintf(stderr, "Cannot read spectra from invalid texture\n");
		abort();
	}
	
	return spectra;
}

/* compute reflectivity of patch from texture map at the given u, v coordinates. */
static TSpectra AveragePatchSpectra(double u, double v, double du, double dv, TTexture *texture)
{
	int x, y;
	TSpectra reflectance, spectra;
	int nx, ny;
	double dw, dh;

	reflectance = lwlib_vec3f(0.0, 0.0, 0.0);

	if (du < 0.0)
	{
		u += du;
		du = -du;
	}

	if (dv < 0.0)
	{
		v += dv;
		dv = -dv;
	}

	nx = (int)(dv * texture->width);
	ny = (int)(du * texture->height);

	dw = 0.0;
	if (texture->width > 0)
		dw = 1.0 / texture->width;

	dh = 0.0;
	if (texture->height > 0)
		dh = 1.0 / texture->height;

	if (nx > 0 && ny > 0)
	{
		for (x = 0; x < nx; x++)
		{
			for (y = 0; y < ny; y++)
			{
				spectra = GetSpectra(u + (y + 0.5) * dh, v + (x + 0.5) * dw, texture);
				reflectance = lwlib_vec3f_add(reflectance, spectra);
			}
		}
		reflectance = lwlib_vec3f_scale(reflectance, 1.0 / (nx * ny));
	}
	else if (nx > 0) // ny == 0
	{
		for (x = 0; x < nx; x++)
		{
			spectra = GetSpectra(u + du / 2.0, v + (x + 0.5) * dw, texture);
			reflectance = lwlib_vec3f_add(reflectance, spectra);
		}
		reflectance = lwlib_vec3f_scale(reflectance, 1.0 / nx);
	}
	else if (ny > 0) // nx == 0
	{
		for (y = 0; y < ny; y++)
		{
			spectra = GetSpectra(u + (y + 0.5) * dh, v + dv / 2.0, texture);
			reflectance = lwlib_vec3f_add(reflectance, spectra);
		}
		reflectance = lwlib_vec3f_scale(reflectance, 1.0 / ny);
	}
	else // nx == 0 && ny == 0
	{
		reflectance = GetSpectra(u + du / 2.0, v + dv / 2.0, texture);
	}

	return reflectance;
}

/* Mesh a quarilateral into patches and elements */
/* Output goes to params */
static void WR_Room_MeshQuad(WR_Room_t room, TQuad* quad)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	int nu, nv;
	double du, dv;
	int i, j;
	double u, v;
	float fi, fj;
	int pi, pj;
	double norm;
	lwlib_TPoint3f p1, p2;

	/* Index to first element */
	quad->startElement = (int)(priv->pElement - rad_params.elements);

	/* Calculate direction vectors of quad from vertices */
	lwlib_SubVector(quad->right, quad->verts[1], quad->verts[0]);
	lwlib_NormalizeVector(norm, quad->right);

	lwlib_SubVector(quad->up, quad->verts[3], quad->verts[0]);
	lwlib_NormalizeVector(norm, quad->up);

	lwlib_CrossVector(quad->normal, quad->right, quad->up);
	lwlib_NormalizeVector(norm, quad->normal);
	
	/* Calculate elements */
	nu = quad->nu;
	nv = quad->nv;
	du = 1.0 / nu;
	dv = 1.0 / nv;
	for (i = 0, u = 0; i < nu; i++, u += du)
	{
		for (j = 0, v = 0; j < nv; j++, v += dv, priv->pElement++)
		{
			priv->pElement->u = u;
			priv->pElement->v = v;
			/* find out the parent patch */
			fi = (float)i / (float)nu;
			fj = (float)j / (float)nv;
			pi = (int)(fi * (float)(quad->patchLevel[0]));
			pj = (int)(fj * (float)(quad->patchLevel[1]));
			priv->pElement->patch = priv->pPatch + pi * quad->patchLevel[1] + pj;
		}
	}
	
	/* Calculate patches */
	nu = quad->patchLevel[0];
	nv = quad->patchLevel[1];
	du = 1.0 / nu;
	dv = 1.0 / nv;
	for (i = 0, u = 0; i < nu; i++, u += du)
	{
		for (j = 0, v = 0; j < nv; j++, v += dv, priv->pPatch++)
		{
			UVToXYZ(quad->verts, u + du/2.0, v + dv/2.0, &priv->pPatch->center);
			priv->pPatch->normal = quad->normal;
			priv->pPatch->raisedCenter = lwlib_vec3f_add(priv->pPatch->center, 
				lwlib_vec3f_scale(priv->pPatch->normal, rad_params.worldSize*0.0001));
			priv->pPatch->color = quad->color;
			if (rad_params.colorReflectance)
				priv->pPatch->reflectance = priv->pPatch->color;
			else
				priv->pPatch->reflectance = quad->reflectance;
			priv->pPatch->emission = quad->emission;
			priv->pPatch->area = quad->area / (nu*nv);
			priv->pPatch->quad = quad;
			UVToXYZ(quad->verts, u, v, &priv->pPatch->verts[0]);
			UVToXYZ(quad->verts, u, v + dv, &priv->pPatch->verts[1]);
			UVToXYZ(quad->verts, u + du, v + dv, &priv->pPatch->verts[2]);
			UVToXYZ(quad->verts, u + du, v, &priv->pPatch->verts[3]);
		}
	}
}

static TQuad *WR_Room_FindQuadForEdge(WR_Room_t room, TQuad *quad, lwlib_TEdge3f edge)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	int i, j;
	int qn;
	TQuad *quadOther;
	lwlib_TEdge3f edgeOther;
	TQuad *quadNeighbour = NULL;
	lwlib_TVector3f edgeVec;
	double angle;
	double bestAngle = -2.0;
	WR_Level_Tile_t *tile;
	int quadNum;

	edgeVec = lwlib_vec3f_normalize(lwlib_edge3f_vec(edge));

	for (i = 0; i < 9; i++)
	{
		tile = WR_Level_GetTileAt(priv->level,
			lwlib_X(quad->pos) - 1 + (i % 3), 
			lwlib_Y(quad->pos) - 1 + (i / 3));
		if (tile == NULL)
		{
			continue;
		}

		for (qn = 0; qn < WR_LEVEL_QN_TOT; qn++)
		{
			quadNum = tile->quad_lookup[qn];
			if (quadNum != 0)
			{
				quadNum--;

				quadOther = &vec_at(rad_params.quads, quadNum);
				if (quadOther->excludeFromScene)
				{
					continue;
				}

				for (j = 0; j < 4; j++)
				{
					edgeOther = lwlib_edge3f(quadOther->verts[j], 
						quadOther->verts[(j + 1) % 4]);
					if (lwlib_edge3f_adj(edge, edgeOther, lwlib_EPSILON))
					{
						break;
					}
				}

				if (j != 4)
				{
					angle = lwlib_vec3f_dot(lwlib_vec3f_cross(quad->normal, 
						quadOther->normal), lwlib_vec3f_neg(edgeVec));
					if 
						(
							angle > bestAngle || 
							(
								fabs(bestAngle) <= lwlib_EPSILON &&
								fabs(angle) <= lwlib_EPSILON &&
								lwlib_vec3f_equal(
									quad->normal, quadOther->normal,
									lwlib_EPSILON
									)
							)
						)
					{
						quadNeighbour = quadOther;
						bestAngle = angle;
					}
				}
			}
		}
	}

	return quadNeighbour;
}

static TQuad *WR_Room_FindQuadForCorner(WR_Room_t room, TQuad *quad, lwlib_TPoint3f cornerPoint)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	int i, j;
	int qn;
	TQuad *quadOther;
	lwlib_TPoint3f cornerOther;
	WR_Level_Tile_t *tile;
	int quadNum;

	for (i = 0; i < 4; i++)
	{
		tile = WR_Level_GetTileAt(priv->level,
			lwlib_X(quad->pos) - 1 + 2 * (i % 2), 
			lwlib_Y(quad->pos) - 1 + 2 * (i / 2));
		if (tile == NULL)
		{
			continue;
		}

		for (qn = 0; qn < WR_LEVEL_QN_TOT; qn++)
		{
			quadNum = tile->quad_lookup[qn];
			if (quadNum != 0)
			{
				quadNum--;

				quadOther = &vec_at(rad_params.quads, quadNum);
				if (quadOther->excludeFromScene)
					continue;

				for (j = 0; j < 4; j++)
				{
					cornerOther = quadOther->verts[j];
					if (lwlib_vec3f_equal(cornerPoint, cornerOther, lwlib_EPSILON))
						break;
				}

				if (j != 4)
				{
					return quadOther;
				}
			}
		}
	}

	return NULL;
}

WR_Room_t WR_Room_New(void)
{
	WR_Room_t room;
	lwlib_Zero(room);

	lwlib_GetPriv(WR_Room_Priv_t, room);
	priv = lwlib_CallocSingle(WR_Room_Priv_t);

	room.priv = priv;
	return room;
}

WR_Room_t WR_Room_Free(WR_Room_t room)
{
	lwlib_GetPriv(WR_Room_Priv_t, room);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(room);
	return room;
}

void WR_Room_ComputeNeighbours(WR_Room_t room)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	int i, j;
	TQuad *quad;
	lwlib_TPoint3f cornerPoint;
	lwlib_TEdge3f edge;

	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		if (quad->excludeFromScene)
			continue;

		for (j = 0; j < 4; j++)
		{
			if (quad->neigh.p[j] == NULL || quad->neigh.p[j]->excludeFromScene)
			{
				edge = lwlib_edge3f(quad->verts[j], quad->verts[(j + 1) % 4]);
				quad->neigh.p[j] = WR_Room_FindQuadForEdge(room, quad, edge);
			}
			if (quad->corner.p[j] == NULL || quad->corner.p[j]->excludeFromScene)
			{
				cornerPoint = quad->verts[j];
				quad->corner.p[j] = WR_Room_FindQuadForCorner(room, quad, cornerPoint);
			}
		}
	}
}

/* Initialize input parameters */
void WR_Room_InitParams(WR_Room_t room)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	int i;
	TQuad *quad;

	/* compute the total number of patches */
	rad_params.nPatches = 0;
	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		rad_params.nPatches += 
			quad->patchLevel[0] * quad->patchLevel[1];
	}
	rad_params.patches = (TPatch *)calloc(rad_params.nPatches, sizeof(TPatch));

	/* compute the total number of elements */
	rad_params.nElements = 0;
	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		rad_params.nElements += quad->nu * quad->nv;
	}
	rad_params.elements = (TElement *)calloc(rad_params.nElements, sizeof(TElement));

	/* mesh the room to patches and elements */
	priv->pPatch = rad_params.patches;
	priv->pElement = rad_params.elements;
	lwlib_LogPrintf("Meshing quads.\n");
	for (i=0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		WR_Room_MeshQuad(room, &vec_at(rad_params.quads, i));
	}

	lwlib_LogPrintf("-> Total Patches : %lu\n", rad_params.nPatches);
	lwlib_LogPrintf("-> Total Elements : %lu\n", rad_params.nElements);
}

void WR_Room_DestroyParams(WR_Room_t room)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	if (rad_params.patches)
		free(rad_params.patches);
	if (rad_params.elements)
		free(rad_params.elements);
}

void WR_Room_SetRad(WR_Room_t room, WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	priv->rad = rad;
}

void WR_Room_SetLevel(WR_Room_t room, WR_Level_t level)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	priv->level = level;
}

void WR_Room_SetLightMapper(WR_Room_t room, WR_LightMapper_t lightMapper)
{
	lwlib_GetPrivAssert(WR_Room_Priv_t, room);
	priv->lightMapper = lightMapper;
}
#endif