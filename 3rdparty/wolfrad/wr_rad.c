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
** Rad
*************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wr_rad.h"
#include "wr_lightmap.h"
#include "wr_raycaster.h"
#include "wr_level.h"
#include "wr_room.h"
#ifdef WOLFRAD
#include "..\..\wl_def.h"

typedef struct WR_Rad_Priv_s
{
	double totalEnergy;	    /* total emitted energy; used for convergence checking */
	double maxEnergySum;    /* remaining energy left in system */
	int *visibleElems;      /* an array of element indices for all elements visible from a patch */
	WR_RayCaster_t rayCaster;
	WR_Level_t level;
} WR_Rad_Priv_t;

TRadParams rad_params;  /* global input parameters */

lwlib_EnumStr(
	WR_QualityLevel_t,
	"Low",
	"Medium",
	"High",
	);

lwlib_EnumStr(
	WR_IncludedLights_t,
	"Near",
	"Visible",
	"All",
	);

lwlib_EnumStr(
	WR_IncludedSurfaces_t,
	"Near",
	"Visible",
	"All",
	);

lwlib_EnumStr(
	WR_InsertStatObj_t,
	"Ceiling Light",
	"Chandelier",
	"Floor Lamp",
	"Light Bulb",
	"Light Bulb Bright",
	"Sky Light"
	);

lwlib_EnumStr(
	WR_InsertFloorCeil_t,
	"Fluorescent Panel",
	);

static const TSpectra black = lwlib_vec3_init(0, 0, 0);

/* Find the next shooting patch based on the unshot energy of each patch */
/* Return 0 if convergence is reached; otherwise, return 1 */
static int WR_Rad_FindShootPatch(WR_Rad_t rad, unsigned long *shootPatch)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	int i, j;
	double energySum, error, maxEnergySum = 0;
	TPatch* ep;
	TPatch* sp;
	int patchNum;

	if (priv->totalEnergy == 0.0)
	{
		return 0;
	}

	ep = rad_params.patches;
	patchNum = -1;

	for (i=0; i < rad_params.nPatches; i++, ep++)
	{
		if (ep->quad->excludeFromScene)
			continue;

		energySum = 0;
		for (j=0; j<kNumberOfRadSamples; j++)
		{
			energySum += lwlib_C(ep->unshotRad, j) * ep->area;
		}
		
		if (energySum > maxEnergySum) 
		{
			patchNum = i;
			maxEnergySum = energySum;
		}
	}

	// check if there is any light at all
	if (patchNum == -1)
	{
		return 0;
	}
	*shootPatch = patchNum;

	priv->maxEnergySum = maxEnergySum;
	error = maxEnergySum / priv->totalEnergy;

	/* check convergence */
	if (rad_params.threshold != 0.0 && 
		error < rad_params.threshold)
	{
		return 0;		/* converged */
	}

	sp = &(rad_params.patches[*shootPatch]);
	sp->convergence = error;
	sp->energySum = maxEnergySum;
	return 1;
}

static void WR_Rad_DistributeRad(WR_Rad_t rad,
    unsigned long shootPatch)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	unsigned long i, j;
	TPatch *sp;
	TElement *ep;
	int elementIndex;
	double formfactor;
	double angle;
	lwlib_TVector3f a, b;
	lwlib_TVector3f c;
	double d;
	int numVisibleElems;
	TSpectra deltaRad;
	double w;
	lwlib_TPoint3f verts[4];
	double area;
	TQuad *equad;
	double du, dv;
	lwlib_TPoint3f elemCenter;
	lwlib_TPoint3f elemVerts[4];

	/* get the center of shootPatch */
	sp = &(rad_params.patches[shootPatch]);
	
	// compute the formfactors 

	/*
	 * Hoyt C. Hottel and Adel F. Sarofin. Radiative Transfer. McGraw-Hill, New-York, 1967.
	 *
	 * form factor between finite and differential area:
	 *
	 *   diFij = ( 1 / (2 pi) ) sum l=0, L-1 ( ( angle(Rl, Rl(+)1) /
	 *           |Rl x Rl(+)1| ) (Rl x Rl(+)1) dot ni ) ... (1)
	 *
	 * where
	 *
	 *   - ni is the unit normal to dAi
	 *   - angle(a,b) is the signed angle between two vectors. The sign is
	 *     positive if 'b' is rotated clockwise from 'a' looking at them 
	 *     in the opposite direction to ni
	 *   - (+) represents addition modulo L
	 *   - L is the number of vertices of surface element j
	 *   - Rl is the vector from dAi to lth vertex of surface element j
	 *
	 * due to counter clockwise ordering of vertices, the angle between Rl and Rl(+)1:
	 * 
	 *   angle(Rl, RL(+)1) = -acos( Rl dot Rl(+)1 / |Rl| |Rl(+)1| ) ... (2)
	 *
	 */
	numVisibleElems = WR_RayCaster_SearchElements(
		priv->rayCaster, shootPatch, priv->visibleElems);
	for (i = 0; i < numVisibleElems; i++)
	{
		elementIndex = priv->visibleElems[i];
		ep = &rad_params.elements[elementIndex];
		equad = ep->patch->quad;

		du = 1.0 / equad->nu;
		dv = 1.0 / equad->nv;
		UVToXYZ(equad->verts, ep->u + du/2.0, ep->v + dv/2.0,
			&elemCenter);
		UVToXYZ(equad->verts, ep->u, ep->v, &elemVerts[0]);
		UVToXYZ(equad->verts, ep->u, ep->v + dv, &elemVerts[1]);
		UVToXYZ(equad->verts, ep->u + du, ep->v + dv, &elemVerts[2]);
		UVToXYZ(equad->verts, ep->u + du, ep->v, &elemVerts[3]);

		for (j = 0; j < 4; j++)
		{
			verts[j] = lwlib_vec3f_sub(elemVerts[j], sp->center);
		}
		area = equad->elementArea / 
			lwlib_vec3f_length(lwlib_vec3f_sub(elemCenter, 
				sp->center));

		formfactor = 0.0;
		for (j = 0; j < 4; j++)
		{
			a = verts[j];
			b = verts[(j + 1) % 4];
			c = lwlib_vec3f_cross(a, b);
			d = lwlib_vec3f_dot(c, sp->normal);

			angle = -acos(lwlib_vec3f_dot(a, b) / 
				(lwlib_vec3f_length(a) * lwlib_vec3f_length(b)));
			formfactor += (angle / lwlib_vec3f_length(c)) * d;
		}
		formfactor *= 1.0 / (2 * PI);

		/* compute reciprocal form-factor */
		if (formfactor != 0.0)
			formfactor *= sp->area / area;

		/* This is a potential source of hemi-cube aliasing */
		/* To do this right, we need to subdivide the shooting patch
		and reshoot. For now we just clip it to unity */
		if (formfactor > 1.0)
			formfactor = 1.0;	

		/* distribute unshotRad to every element */
		if (formfactor != 0.0) 
		{
			for (j=0; j<kNumberOfRadSamples; j++)
			{
				lwlib_C(deltaRad, j) = lwlib_C(sp->unshotRad, j) * 
					formfactor * lwlib_C(ep->patch->reflectance, j);
			}

			/* increment element's radiosity and patch's unshot 
			   radiosity */
			w = area / ep->patch->area;
			for (j=0; j<kNumberOfRadSamples; j++) 
			{
				lwlib_C(ep->rad, j) += lwlib_C(deltaRad, j);
				lwlib_C(ep->patch->unshotRad, j) += 
					lwlib_C(deltaRad, j) * w;
			}

			/* update light tile extent of element's quad */
			if (sp->quad->lightTileRadius > 
				ep->patch->quad->lightTileRadius)
			{
				ep->patch->quad->lightTileRadius =
					sp->quad->lightTileRadius;
			}
		}
	}

	/* reset shooting patch's unshot radiosity */
	sp->unshotRad = black;
}

WR_Rad_t WR_Rad_New(void)
{
	WR_Rad_t rad;
	lwlib_Zero(rad);

	lwlib_GetPriv(WR_Rad_Priv_t, rad);
	priv = lwlib_CallocSingle(WR_Rad_Priv_t);

	rad.priv = priv;
	return rad;
}

WR_Rad_t WR_Rad_Free(WR_Rad_t rad)
{
	lwlib_GetPriv(WR_Rad_Priv_t, rad);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(rad);
	return rad;
}

/* Initialize radiosity based on the input parameters p */
void WR_Rad_Init(WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	unsigned long i;
	int j;
	TPatch*	pp;
	TElement* ep;
	
	priv->visibleElems = (int *)calloc(rad_params.nElements, sizeof(int));
	
	/* initialize radiosity */
	pp = rad_params.patches;
	for (i=rad_params.nPatches; i--; pp++)
	{
		pp->unshotRad = pp->emission;
	}

	ep = rad_params.elements;
	for (i = 0; i < rad_params.nElements; i++, ep++)
	{
		pp = ep->patch;
		ep->rad = pp->emission;
	}

	/* compute total energy */
	priv->totalEnergy = 0;
	pp = rad_params.patches;
	for (i=rad_params.nPatches; i--; pp++)
	{
		for (j=0; j<kNumberOfRadSamples; j++)
		{
			priv->totalEnergy += lwlib_C(pp->emission, j) * pp->area;
		}
	}
}

/* Main iterative loop */
void WR_Rad_Run(WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	int shoots = 0;
	unsigned long shootPatch;
	TPatch* sp;

	while (WR_Rad_FindShootPatch(rad, &shootPatch)) 
	{
		sp = &rad_params.patches[shootPatch];

		// LW: report radiosity patch shoot
		#if 0
		lwlib_LogPrintf("DoRad: sp[%lu]: shoot %d, energy %f/%f\n", 
			shootPatch, shoots, priv->maxEnergySum, 
			priv->totalEnergy);
		#endif

		WR_Rad_DistributeRad(rad, shootPatch);

		shoots++;
		if (shoots == rad_params.maxShoots)
			break;
	}
}

/* Clean up */
void WR_Rad_CleanUp(WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	free(priv->visibleElems);
}

#if 0
/* initialise intensity of element using a point-source illumination model */
static void IlluminateElement(TElement *element)
{
	int i;
	lwlib_TVector3f N, L;
	TLight *light;
	unsigned long elementIndex;
	double Fatt;
	double dL, dLsq;
	TElement *ep;

	/* radiosity light sources are not seen and are as such not affected by this model */
	if (element->patch->quad->isRadLight)
	{
		return;
	}

	N = element->normal;
	element->intensity = vec3f(0.0, 0.0, 0.0);

	/*
	 * set emission for all elements using an illumination model for point light
	 * sources:
	 *
	 *   I = Ia Ka + Fatt Ip Kd (N dot L)
	 *
	 * where Ia is ambient intensity, Ka is light reflectance coefficient for
	 * ambient light, Ip is point light source intensity, Kd is light
	 * reflectance coefficient of material, N is element normal, L is light
	 * direction, and Fatt is the light-source attenuation factor.
	 *
	 * the light-source attenuation factor is:
	 *
	 *  Fatt = min ( 1 / ( c1 + c2 dL + c3 dL dL ), 1 )
	 *
	 * here c1, c2 and c3 are light-defined constants, dL is distance from
	 * light source to element center.
	 *
	 */
	for (i = 0; i < rad_params->nLights; i++)
	{
		light = &rad_params->lights[i];
		
		L = lwlib_vec3f_sub(light->position, element->center);
		if (lwlib_vec3f_dot(L, N) <= EPSILON)
			continue;

		dLsq = lwlib_vec3f_dot(L, L);
		if (dLsq <= EPSILON)
			continue;
		dL = sqrt(dLsq);

		L = lwlib_vec3f_scale(L, 1.0 / dL);

		elementIndex = ShootRay(light->position, lwlib_vec3f_neg(L));
		if (elementIndex == 0)
			continue;
		elementIndex--;

		ep = &rad_params->elements[elementIndex];
		if (ep != element)
			continue;

		Fatt = MIN(1.0 / lwlib_vec3f_dot(vec3f(1.0, dL, dLsq), light->attenuation), 1.0);

		element->intensity = 
			lwlib_vec3f_add(
				element->intensity,
				lwlib_vec3f_scale(
					light->intensity, 
					Fatt * lwlib_vec3f_dot(N, L)
					)
				);
	}
}
#endif

TRadParams *WR_Rad_GetParams(WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	return &rad_params;
}

void WR_Rad_SetRayCaster(WR_Rad_t rad, WR_RayCaster_t rayCaster)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	priv->rayCaster = rayCaster;
}

void WR_Rad_SetLevel(WR_Rad_t rad, WR_Level_t level)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	priv->level = level;
}

int WR_Rad_HitElementIndex(WR_Rad_t rad, lwlib_TPoint3f hit, TQuad *quad,
	bool checkPost)
{
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);
	int elementIndex;
	double u, v;
	lwlib_TPoint3f inquadHit;
	int x, y;

	/* find u, v coordinates of hit on quadrilateral */
	lwlib_SubVector(inquadHit, hit, quad->verts[0]);
	u = lwlib_DotVector(inquadHit, quad->up) / quad->dims[0];
	v = lwlib_DotVector(inquadHit, quad->right) / quad->dims[1];

	u = lwlib_CLIP(u, 0.0, 1.0);
	v = lwlib_CLIP(v, 0.0, 1.0);

	if (checkPost && quad->postsource != NULL)
	{
		x = v * quad->postdimv;
		x = lwlib_MIN(x, quad->postdimv - 1);

		y = u * quad->postdimu;
		y = lwlib_MIN(y, quad->postdimu - 1);
		y = quad->postdimu - 1 - y;

		if (quad->postsource[((quad->postoffv + x) << TEXTURESHIFT) +
			(quad->postoffu + y)] == 0xff)
		{
			return 0;
		}
	}

	/* calculate index to element hit by ray */
	elementIndex = quad->startElement + 
		lwlib_CLIP((int)(u * quad->nu), 0, quad->nu - 1) * quad->nv + 
		lwlib_CLIP((int)(v * quad->nv), 0, quad->nv - 1);

	elementIndex = lwlib_CLIP(elementIndex, quad->startElement, 
		quad->startElement + quad->nu * quad->nv - 1);
	
	return checkPost ? elementIndex + 1 : elementIndex;
}

static void WR_Rad_GetAmbient(WR_Rad_t rad, TSpectra* ambient)
{
	TPatch* p;
	unsigned long i;
	int k;
	static int first = 1;
	static TSpectra baseSum; 
	TSpectra uSum;
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);

	uSum = black;
	if (first) 
	{
		double areaSum;
		TSpectra rSum;
		areaSum = 0;
		rSum = black;
		/* sum area and (area*reflectivity) */
		p = rad_params.patches;
		for (i = rad_params.nPatches; i--; p++) 
		{
			areaSum += p->area;
			for (k = kNumberOfRadSamples; k--; )
				lwlib_C(rSum, k) += lwlib_C(p->reflectance, k) * p->area;
		}
		for (k = kNumberOfRadSamples; k--; )
			lwlib_C(baseSum, k) = areaSum - lwlib_C(rSum, k);
		first = 0;
	}

	/* sum (unshot radiosity * area) */
	p = rad_params.patches;
	for (i=rad_params.nPatches; i--; p++) 
	{
		for (k = kNumberOfRadSamples; k--; )
			lwlib_C(uSum, k) += lwlib_C(p->unshotRad, k) * p->area;
	}
	
	/* compute ambient */
	for (k = kNumberOfRadSamples; k--; )
		lwlib_C(*ambient, k) = lwlib_C(uSum, k) / lwlib_C(baseSum, k);
}

static void PrintRadiosityTable(WR_Rad_t rad, int c)
{
	unsigned long i;
	TElement *ep;
	TPatch *pp;
	double maxRad = 0.0;
	int bandIndex;
	const int totalBands = 1000;
	int bandCounts[1000];
	double start, end;
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);

	memset(bandCounts, 0, sizeof(bandCounts));
	
	pp = rad_params.patches;
	for (i = 0; i < rad_params.nPatches; i++, pp++)
		maxRad = lwlib_MAX(lwlib_C(pp->emission, c), maxRad);
	
	if (maxRad == 0.0)
		return;

	ep = rad_params.elements;
	for (i = 0; i < rad_params.nElements; i++, ep++)
	{
		bandIndex = lwlib_MIN((int)(lwlib_C(ep->rad, c) / maxRad * totalBands), totalBands - 1);
		bandCounts[bandIndex]++;
	}

	lwlib_LogPrintf("Radiosity Table:\n");
	lwlib_LogPrintf("%14s%14s%14s\n", "begin", "end", "freq");

	for (i = 0; i < totalBands; i++)
	{
		start = maxRad / totalBands * i;
		if (bandCounts[i] == 0) // deal with consecutive bands with zero count
		{
			while (i < totalBands && bandCounts[i] == 0)
				i++;
			end = maxRad / totalBands * i;
			i--;
		}
		else
			end = maxRad / totalBands * (i + 1);

		lwlib_LogPrintf("%14.6f%14.6f%14.6f\n", start, end, (double)bandCounts[i] / rad_params.nElements * 100.0);
	}
}

void WR_Rad_SetQuadSubdivisionLevel(WR_Rad_t rad, TQuad *quad, int level)
{
	int n;
	lwlib_GetPrivAssert(WR_Rad_Priv_t, rad);

	n = (1 << (TEXTURESHIFT - level));

	quad->elementLevel[0] = quad->elementLevel[1] = 
		(n >= 4 ? 2 : 1);
	quad->patchLevel[0] = (n / quad->elementLevel[0]) /
		(quad->halfsize[0] ? 2 : 1);
	quad->patchLevel[1] = (n / quad->elementLevel[1]) /
		(quad->halfsize[1] ? 2 : 1);

	quad->nu = quad->patchLevel[0] * quad->elementLevel[0];
	quad->nv = quad->patchLevel[1] * quad->elementLevel[1];
}
#endif