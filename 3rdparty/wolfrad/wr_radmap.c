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
** Lightmap
*************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wr_radmap.h"
#include "wr_rad.h"
#include "wr_room.h"
#include "wr_lightmap.h"
#ifdef WOLFRAD
TRadMap RadMapInit(int nu, int nv)
{
	TRadMap radMap;
	lwlib_Zero(radMap);

	if (nu == 0 || nv == 0)
	{
		return radMap;
	}

	radMap.nu = nu;
	radMap.nv = nv;
	radMap.spectra = (lwlib_TVector3f *)calloc(
		sizeof(lwlib_TVector3f), nu * nv);
	radMap.alpha = (lwlib_Byte_t *)calloc(
		sizeof(lwlib_Byte_t), nu * nv);

	return radMap;
}

TRadMap RadMapEmpty(void)
{
	return RadMapInit(0, 0);
}

void RadMapAdd(TRadMap dstRadMap, TRadMap srcRadMap)
{
	int i;

	assert(dstRadMap.nu == srcRadMap.nu && dstRadMap.nv == srcRadMap.nv && 
		dstRadMap.spectra && srcRadMap.spectra);

	for (i = 0; i < srcRadMap.nu * srcRadMap.nv; i++)
	{
		dstRadMap.spectra[i] = 
			lwlib_vec3f_add(dstRadMap.spectra[i], srcRadMap.spectra[i]);
	}
}

void RadMapFree(TRadMap *radMap)
{
	assert(radMap);

	free(radMap->spectra);
	free(radMap->alpha);
	lwlib_Zero(*radMap);
}

void RadMapTransform(TRadMap radMap, lwlib_TFunc func)
{
	int i;

	assert(RadMapValid(radMap));

	for (i = 0; i < radMap.nu * radMap.nv; i++)
	{
		radMap.spectra[i] = lwlib_vec3f_transform(radMap.spectra[i],
            func);
	}
}

lwlib_TVector3f RadMapAverage(TRadMap radMap)
{
	int i;
	lwlib_TVector3f averageSpectra = lwlib_vec3f_zero();

	if (RadMapValid(radMap))
	{
		for (i = 0; i < radMap.nu * radMap.nv; i++)
		{
			averageSpectra = lwlib_vec3f_add(averageSpectra, radMap.spectra[i]);
		}

		averageSpectra = lwlib_vec3f_scale(averageSpectra, 1.0 / (radMap.nu * radMap.nv));
	}
	
	return averageSpectra;
}

lwlib_TVector3f RadMapAverageTransform(TRadMap radMap, lwlib_TFunc3f vecFunc)
{
	int i;
	lwlib_TVector3f averageSpectra = lwlib_vec3f_zero();

	if (RadMapValid(radMap))
	{
		for (i = 0; i < radMap.nu * radMap.nv; i++)
		{
			averageSpectra = lwlib_vec3f_add(averageSpectra, 
				vecFunc(radMap.spectra[i]));
		}

		averageSpectra = lwlib_vec3f_scale(averageSpectra, 1.0 / (radMap.nu * radMap.nv));
	}
	
	return averageSpectra;
}

double RadMapLowSpectraScore(TRadMap radMap)
{
	return lwlib_vec3f_max(RadMapAverage(radMap));
}

bool RadMapLogCompare(TRadMap radMapA, TRadMap radMapB)
{
	int i, j;
	double k;
	lwlib_TVector3f correctedRadA, correctedRadB;
	double x1, x2;
	int nu, nv;

	nu = nv = 0;
	if (RadMapValid(radMapA) && RadMapValid(radMapB))
	{
		assert(radMapA.nu == radMapB.nu && radMapA.nv == radMapB.nv);
		nu = radMapA.nu;
		nv = radMapA.nv;
	}
	else if (RadMapValid(radMapA))
	{
		nu = radMapA.nu;
		nv = radMapA.nv;
	}
	else if (RadMapValid(radMapB))
	{
		nu = radMapB.nu;
		nv = radMapB.nv;
	}

	for (i = 0; i < nu * nv; i++)
	{
		correctedRadA = lwlib_vec3f_zero();
		if (RadMapValid(radMapA))
		{
			correctedRadA = radMapA.spectra[i];
		}

		correctedRadB = lwlib_vec3f_zero();
		if (RadMapValid(radMapB))
		{
			correctedRadB = radMapB.spectra[i];
		}

		for (j = 0; j < 3; j++)
		{
			x1 = lwlib_C(correctedRadA, j);
			x2 = lwlib_C(correctedRadB, j);

			if (fabs(x1 - x2) > 0.0)
			{
				return true;
			}
		}
	}

	return false;
}

bool RadMapLogCompareWithZero(TRadMap radMap)
{
	return RadMapLogCompare(radMap, RadMapEmpty());
}

// copying low-res lightmap into high-res texture
//
// includes color averaging so a border is assumed to exist around source
// region
//
// half offset can be applied to u,v when accessing source spectra
void RadMapStretchCopy(TRadMap dstRadMap, TRadMapRegion dstRegion, 
	TRadMap srcRadMap, TRadMapRegion srcRegion, bool halfoff)
{
	int i;
	int u, v;
	int usrc, vsrc, usrci, usrcr, vsrci, vsrcr;
	int usrchoff, vsrchoff;
	lwlib_TPoint3f quad[4];
	const int ru[4] = { 0, 0, 1, 1 };
	const int rv[4] = { 0, 1, 1, 0 };

	assert(dstRegion.u >= 0 && dstRegion.u + dstRegion.du <=
		dstRadMap.nu && dstRegion.v >= 0 && dstRegion.v + dstRegion.dv <=
		dstRadMap.nv);
	assert(srcRegion.u >= 0 && srcRegion.u + srcRegion.du <=
		srcRadMap.nu && srcRegion.v >= 0 && srcRegion.v + srcRegion.dv <=
		srcRadMap.nv);
	
	if (halfoff)
	{
		usrchoff = (srcRegion.u * dstRegion.du - dstRegion.du / 2);
		vsrchoff = (srcRegion.v * dstRegion.dv - dstRegion.dv / 2);
	}
	else
	{
		usrchoff = vsrchoff = 0;
	}
	
	for (u = 0; u < dstRegion.du; u++)
	{
		usrc = u * srcRegion.du + usrchoff;
		usrci = usrc / dstRegion.du;
		usrcr = usrc % dstRegion.du;

		for (v = 0; v < dstRegion.dv; v++)
		{
			vsrc = v * srcRegion.dv + vsrchoff;
			vsrci = vsrc / dstRegion.dv;
			vsrcr = vsrc % dstRegion.dv;

			for (i = 0; i < 4; i++)
			{
				quad[i] = srcRadMap.spectra[
					(usrci + ru[i]) * srcRadMap.nv + 
					(vsrci + rv[i])];
			}

			UVToXYZ(quad, (double)usrcr / dstRegion.du, 
				(double)vsrcr / dstRegion.dv,
				&dstRadMap.spectra[
					(dstRegion.u + u) * dstRadMap.nv + 
					(dstRegion.v + v)
					]);
		}
	}
}

TRadMapRegion RadMapRegion(int u, int v, int du, int dv)
{
	TRadMapRegion region;
	lwlib_Zero(region);
	region.u = u;
	region.v = v;
	region.du = du;
	region.dv = dv;
	return region;
}

TRadMap RadMapTranspose(TRadMap radMap)
{
	int u, v;
	TRadMap transposeRadMap;

	transposeRadMap = RadMapInit(radMap.nv, radMap.nu);

	for (u = 0; u < radMap.nu; u++)
	{
		for (v = 0; v < radMap.nv; v++)
		{
			transposeRadMap.spectra[v * radMap.nu + u] = 
				radMap.spectra[u * radMap.nv + v];
			transposeRadMap.alpha[v * radMap.nu + u] = 
				radMap.alpha[u * radMap.nv + v];
		}
	}

	return transposeRadMap;
}

TRadMap RadMapFlip(TRadMap radMap, int axis)
{
	int u, v;
	TRadMap flipRadMap;

	flipRadMap = RadMapInit(radMap.nu, radMap.nv);

	if (axis == 0)
	{
		for (u = 0; u < radMap.nu; u++)
		{
			for (v = 0; v < radMap.nv; v++)
			{
				flipRadMap.spectra[u * radMap.nv + v] = 
					radMap.spectra[(radMap.nu - u - 1) * radMap.nv + v];
				flipRadMap.alpha[u * radMap.nv + v] = 
					radMap.alpha[(radMap.nu - u - 1) * radMap.nv + v];
			}
		}
	}
	else if (axis == 1)
	{
		for (u = 0; u < radMap.nu; u++)
		{
			for (v = 0; v < radMap.nv; v++)
			{
				flipRadMap.spectra[u * radMap.nv + v] = 
					radMap.spectra[u * radMap.nv + (radMap.nv - v - 1)];
				flipRadMap.alpha[u * radMap.nv + v] = 
					radMap.alpha[u * radMap.nv + (radMap.nv - v - 1)];
			}
		}
	}

	return flipRadMap;
}

void RadMapSetAll(TRadMap radMap, lwlib_TVector3f spectra)
{
	int i;

	for (i = 0; i < radMap.nu * radMap.nv; i++)
	{
		radMap.spectra[i] = spectra;
	}
}

void RadMapModulate(TRadMap dstRadMap, int dstU, int dstV, TRadMap srcRadMap)
{
	int u, v;
	lwlib_TVector3f spectra;

	for (u = 0; u < srcRadMap.nu; u++)
	{
		for (v = 0; v < srcRadMap.nv; v++)
		{
			spectra = RadMapElem(dstRadMap, u + dstU, v + dstV);
			spectra = lwlib_vec3f_mult(spectra, 
				RadMapElem(srcRadMap, u, v));
			RadMapSetElem(dstRadMap, u + dstU, v + dstV, spectra);

			RadMapSetElemAlpha(dstRadMap, u + dstU, v + dstV,
				RadMapElemAlpha(srcRadMap, u, v));
		}
	}
}

void RadMapPrint(TRadMap radMap, int u, int v, const char *str)
{
	lwlib_TVector3f color = lwlib_vec3f(1.0, 1.0, 1.0);
	static const char *chars[] =
	{
		"#####",
		"#   #",
		"#   #",
		"#   #",
		"#####",
		"",
		"    #",
		"    #",
		"    #",
		"    #",
		"    #",
		"",
		"#####",
		"    #",
		"#####",
		"#    ",
		"#####",
		"",
		"#####",
		"    #",
		"#####",
		"    #",
		"#####",
		"",
		"#   #",
		"#   #",
		"#####",
		"    #",
		"    #",
		"",
		"#####",
		"#    ",
		"#####",
		"    #",
		"#####",
		"",
		"#####",
		"#    ",
		"#####",
		"#   #",
		"#####",
		"",
		"#####",
		"    #",
		"    #",
		"    #",
		"    #",
		"",
		"#####",
		"#   #",
		"#####",
		"#   #",
		"#####",
		"",
		"#####",
		"#   #",
		"#####",
		"    #",
		"#####",
		"",
	};
	int x, y;
	int c;
	const char **p;

	for (c = 0; str[c] != '\0'; c++)
	{
		if (str[c] == ' ')
		{
			v += 6;
			continue;
		}
		assert(str[c] >= '0' && str[c] <= '9');

		p = chars + (int)(str[c] - '0') * 6;
		for (x = 0; x < 5; x++)
		{
			for (y = 0; y < 5; y++)
			{
				if (p[y][x] == '#')
				{
					color = RadMapElem(radMap, u + y, v + x); 
				}
			}
		}

		v += 6;
	}
}
#endif