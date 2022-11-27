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
** RadMap
*************************************************************************/

#ifndef WOLFRAD_RADMAP_H
#define WOLFRAD_RADMAP_H

#ifdef PS2
#include "3rdparty/lw_lib/lw_vec.h"
#else
#include "..\lw_lib\lw_vec.h"
#endif
#ifdef WOLFRAD
#define RadMapValid(radMap) ((radMap).spectra != NULL)

typedef struct {
	lwlib_TVector3f *spectra;
	lwlib_Byte_t *alpha;
	int nu, nv;
} TRadMap;

typedef struct {
	int u, v;
	int du, dv;
} TRadMapRegion;

TRadMap RadMapInit(int nu, int nv);

TRadMap RadMapEmpty(void);

void RadMapAdd(TRadMap dstRadMap, TRadMap srcRadMap);

void RadMapFree(TRadMap *radMap);

void RadMapTransform(TRadMap radMap, lwlib_TFunc func);

lwlib_TVector3f RadMapAverage(TRadMap radMap);

lwlib_TVector3f RadMapAverageTransform(TRadMap radMap, lwlib_TFunc3f vecFunc);

double RadMapLowSpectraScore(TRadMap radMap);

bool RadMapLogCompare(TRadMap radMapA, TRadMap radMapB);

bool RadMapLogCompareWithZero(TRadMap radMap);

void RadMapStretchCopy(TRadMap dstRadMap, TRadMapRegion dstRegion, 
	TRadMap srcRadMap, TRadMapRegion srcRegion, bool halfoff);

TRadMapRegion RadMapRegion(int u, int v, int du, int dv);

TRadMap RadMapTranspose(TRadMap radMap);

TRadMap RadMapFlip(TRadMap radMap, int axis);

void RadMapSetAll(TRadMap radMap, lwlib_TVector3f spectra);

void RadMapModulate(TRadMap dstRadMap, int dstU, int dstV, 
	TRadMap srcRadMap);

void RadMapPrint(TRadMap radMap, int u, int v, const char *str);

static inline lwlib_TVector3f RadMapElem(TRadMap radMap, int u, int v)
{
	if (RadMapValid(radMap))
	{
		assert(u >= 0 && u < radMap.nu && v >= 0 && v < radMap.nv);
		return radMap.spectra[u * radMap.nv + v];
	}
	
	return lwlib_vec3f_zero();
}

static inline lwlib_Byte_t RadMapElemAlpha(TRadMap radMap, int u, int v)
{
	if (RadMapValid(radMap))
	{
		assert(u >= 0 && u < radMap.nu && v >= 0 && v < radMap.nv);
		return radMap.alpha[u * radMap.nv + v];
	}
	
	return 0x00;
}

static inline void RadMapSetElem(TRadMap radMap, int u, int v,
	lwlib_TVector3f spectra)
{
	assert(RadMapValid(radMap) && u >= 0 && u < radMap.nu && v >= 0 && 
		v < radMap.nv);
	radMap.spectra[u * radMap.nv + v] = spectra;
}

static inline void RadMapSetElemAlpha(TRadMap radMap, int u, int v,
	lwlib_Byte_t alpha)
{
	assert(RadMapValid(radMap) && u >= 0 && u < radMap.nu && v >= 0 &&
		v < radMap.nv);
	
	radMap.alpha[u * radMap.nv + v] = alpha;
}
#endif
#endif
