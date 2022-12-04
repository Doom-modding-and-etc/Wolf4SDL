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

#include <SDL.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "wr_rad.h"
#include "wr_lightmap.h"
#include "wr_raycaster.h"
#include "wr_level.h"
#ifdef WOLFRAD
#include "../lw_lib/lw_protmsg.h"
#include "../lw_lib/lw_dict.h"
#include "../lw_lib/lw_img.h"
#include "../lw_lib/lw_bres.h"
#include "../../wl_def.h"

#define LIGHTMAP_WIDTH PROT_MSG_LIGHTMAP_WIDTH
#define LIGHTMAP_HEIGHT PROT_MSG_LIGHTMAP_HEIGHT
#define LIGHTMAP_PIXELS PROT_MSG_LIGHTMAP_PIXELS
#define BORDER_NU(quad) ((quad)->nu + 2)
#define BORDER_NV(quad) ((quad)->nv + 2)
#define LIGHTMAP_REGION_MAXSIZE (TEXTURESIZE * TEXTURESIZE)

typedef struct LM_Region_s
{
	int size;
	int off;
} LM_Region_t;

typedef struct WR_LightMapper_Priv_s
{
	WR_Rad_t rad;
	WR_RayCaster_t rayCaster;
	WR_Level_t level;
	lwlib_Bres_t bres;
} WR_LightMapper_Priv_t;

lwlib_DictType(LM_ScoreIndexPair_t, double, int);

static int scoreCompare(double a, double b)
{
	return a < b ? 1 : (a > b ? -1 : 0);
}

static void LM_RegionReset(LM_Region_t *region, int size)
{
	memset(region, 0, sizeof(LM_Region_t));
	region->size = size;
}

static void WR_LightMapper_RegionNextSpace(
	WR_LightMapper_t lightMapper, LM_Region_t *region, int dx, int dy)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	region->off += dx * dy;
}

static bool	WR_LightMapper_RegionIsFull(WR_LightMapper_t lightMapper, 
	LM_Region_t *region, int largestSize)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	return region->off + 2 * largestSize > region->size;
}

static bool	LM_RegionIsEmpty(LM_Region_t *region)
{
	return region->off == 0;
}

static TElement *WR_LightMapper_FindAdjacentElem(
	WR_LightMapper_t lightMapper, int u, int v, TQuad *quad)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int elemIndex;
	int edgeNum;
	TElement *ep = NULL;
	double t;
	lwlib_TEdge3f edge;
	int cornerIndex = 0;

	if (u == 0 && v == 0)
		cornerIndex = 1;
	else if (u == 0 && v == (BORDER_NV(quad) - 1))
		cornerIndex = 2;
	else if (u == (BORDER_NU(quad) - 1) && v == (BORDER_NV(quad) - 1))
		cornerIndex = 3;
	else if (u == (BORDER_NU(quad) - 1) && v == 0)
		cornerIndex = 4;
	else
	{
		if (u == 0)
		{
			edgeNum = 0;
			t = ((v - 1) + 0.5) / quad->nv;
		}
		else if (v == (BORDER_NV(quad) - 1))
		{
			edgeNum = 1;
			t = ((u - 1) + 0.5) / quad->nu;
		}
		else if (u == (BORDER_NU(quad) - 1))
		{
			edgeNum = 2;
			t = (quad->nv - (v - 1) - 1 + 0.5) / quad->nv;
		}
		else if (v == 0)
		{
			edgeNum = 3;
			t = (quad->nu - (u - 1) - 1 + 0.5) / quad->nu;
		}
		else
			assert(0);

		if (quad->neigh.p[edgeNum] != NULL)
		{
			edge = lwlib_edge3f(quad->verts[edgeNum], quad->verts[(edgeNum + 1) % 4]);
			elemIndex = WR_Rad_HitElementIndex(
				priv->rad, lwlib_edge3f_lerp(edge, t), 
				quad->neigh.p[edgeNum], false);
			ep = &rad_params.elements[elemIndex];
		}
	}

	if (ep == NULL && cornerIndex != 0)
	{
		cornerIndex--;

		if (quad->corner.p[cornerIndex] != NULL)
		{
			elemIndex = WR_Rad_HitElementIndex(
				priv->rad, quad->verts[cornerIndex], 
				quad->corner.p[cornerIndex], false);
			ep = &rad_params.elements[elemIndex];
		}
	}

	return ep;
}

static TElement *WR_LightMapper_LightFacetElem(WR_LightMapper_t lightMapper, 
	int elemIndex, TQuad *quad)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	TElement *ep = NULL;
	int u, v;

	u = elemIndex / BORDER_NV(quad);
	v = elemIndex % BORDER_NV(quad);

	if (u == 0 || u == (BORDER_NU(quad) - 1) || v == 0 || v == (BORDER_NV(quad) - 1))
	{
		ep = WR_LightMapper_FindAdjacentElem(lightMapper, u, v, quad);
	}
	else
	{
		elemIndex = (u - 1) * quad->nv + (v - 1);
		ep = &rad_params.elements[quad->startElement + elemIndex];
	}

	return ep;
}

static void LM_GenerateLightmap(lwlib_Img_t *lightmap, int off,
	TRadMap radMap)
{
	int i;
	unsigned long color;
	int offset;

	for (i = 0; i < radMap.nu * radMap.nv; i++)
	{
		// spectra quantized to rgba color
		color = lwlib_vec3f_quantize(
			lwlib_vec3f_clip(
				lwlib_vec3f_swap_components(radMap.spectra[i])
				)
			);
		color |= (unsigned long)radMap.alpha[i] << 24;

		// copy rgb components of color to lightmap pixel
		offset = (off + i) * lightmap->bpp;
		memcpy(&lightmap->data[offset], (unsigned char *)&color, 
			lightmap->bpp);
	}
}

static lwlib_Bits_t WR_LightMapper_AddPotentialDoorBits(WR_LightMapper_t lightMapper, 
	TQuad *quad, TLight *light, lwlib_Bits_t potentialDoorBits)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i;
	int processNum;

	for (i = 0; i < light->numProcesses; i++)
	{
		processNum = i + light->firstProcessIndex;

		if (RadMapLogCompareWithZero(
			LWLIB_CTX_READ(processNum, quad->radMap, TRadMap)))
		{
			lwlib_BitsSetMany(potentialDoorBits,
				LWLIB_CTX_READ(processNum, rad_params.openDoorBits,
				lwlib_Bits_t));
		}
	}

	return potentialDoorBits;
}

static void WR_LightMapper_SaveLightmapTexture(
	WR_LightMapper_t lightMapper, lwlib_Img_t *lightmap, int lm, 
	ProtMsgList_t *protMsgList)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	const int lightmapSize = LIGHTMAP_PIXELS * sizeof(uint32_t);
	ProtMsgLightMap_t *protMsgLightMap;

	protMsgLightMap = lwlib_CallocSingle(ProtMsgLightMap_t);

	memcpy(protMsgLightMap->data, lightmap->data, lightmapSize);
	vec_push(protMsgList->lightMapSeq, ProtMsgLightMap_t, *protMsgLightMap);

	free(protMsgLightMap);
	memset(lightmap->data, 0, lightmapSize);
}

static bool WR_LightMapper_IncludeQuad(WR_LightMapper_t lightMapper, TQuad *quad)
{
	int x, y;
	bool includeSurface;
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);

	includeSurface = false;
	x = lwlib_X(quad->pos) / 2;
	y = lwlib_Y(quad->pos) / 2;

	switch (rad_params.includedSurfaces)
	{
	case WR_INCLUDEDSURFACES_NEAR:
		if (abs(x - player->tilex) < 6 && 
			abs(y - (64 - 1 - player->tiley)) < 6)
		{
			includeSurface = true;
		}
		break;
	case WR_INCLUDEDSURFACES_VISIBLE:
		if (!lwlib_Bres_VisBlocked(priv->bres, 
			lwlib_vec3i(x, y, 0), 
			lwlib_vec3i(player->tilex, 
				64 - 1 - player->tiley, 0)))
		{
			includeSurface = true;
		}
		break;
	case WR_INCLUDEDSURFACES_ALL:
		includeSurface = true;
		break;
	}

	return includeSurface;
}

static TRadMap WR_LightMapper_SumLightsRad(
	WR_LightMapper_t lightMapper, TQuad *quad, lwlib_Bits_t finalDoorBits)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	bool includeSurface;
	int i, j;
	int x, y;
	TRadMap sumRadMap;
	lwlib_Bits_t lightDoorBits;
	lwlib_Bits_t openDoorBits;
	bool foundProcess;
	int foundProcessNum;
	TLight *light;
	int processNum;

	lwlib_Zero(sumRadMap);

	for (i = 0; i < vec_size(rad_params.lights); i++)
	{
		light = &vec_at(rad_params.lights, i);

		// find the largest set of doors within the final door set via which
		// the current light source illuminates quad
		lwlib_Zero(lightDoorBits);
		foundProcess = false;
		foundProcessNum = 0;

		for (j = 0; j < light->numProcesses; j++)
		{
			processNum = j + light->firstProcessIndex;

			openDoorBits = LWLIB_CTX_READ(processNum, 
				rad_params.openDoorBits, lwlib_Bits_t);
			if 
				(
					lwlib_BitsIsSubset(openDoorBits, finalDoorBits) &&
					RadMapValid(LWLIB_CTX_READ(processNum,
						quad->radMap, TRadMap)) &&
					(
						!foundProcess ||
						lwlib_BitsCount(openDoorBits) > lwlib_BitsCount(lightDoorBits)
					)
				)
			{
				lightDoorBits = openDoorBits;
				foundProcessNum = processNum;
				foundProcess = true;
			}
		}

		// add any contribution from this light to the working radiosity map
		if (foundProcess)
		{
			if (!RadMapValid(sumRadMap))
			{
				sumRadMap = RadMapInit(BORDER_NU(quad), BORDER_NV(quad));
			}
			RadMapAdd(sumRadMap, LWLIB_CTX_READ(foundProcessNum,
				quad->radMap, TRadMap));
		}
	}

	// destroy radiosity below visible threshold
	if (RadMapValid(sumRadMap) && 
		RadMapLowSpectraScore(sumRadMap) < rad_params.lowSpectraThreshold)
	{
		RadMapFree(&sumRadMap);
		assert(!RadMapValid(sumRadMap));
	}

	if (!RadMapValid(sumRadMap) && WR_LightMapper_IncludeQuad(lightMapper, quad))
	{
		sumRadMap = RadMapInit(BORDER_NU(quad), BORDER_NV(quad));
	}

	return sumRadMap;
}

static lwlib_TVector2i WR_QnToVec(WR_Level_QuadNumber_t qn)
{
	if (qn == WR_LEVEL_QN_WESTWALL)
	{
		return lwlib_vec2i(-1, 0);
	}
	if (qn == WR_LEVEL_QN_EASTWALL)
	{
		return lwlib_vec2i(1, 0);
	}
	if (qn == WR_LEVEL_QN_NORTHWALL)
	{
		return lwlib_vec2i(0, -1);
	}
	if (qn == WR_LEVEL_QN_SOUTHWALL)
	{
		return lwlib_vec2i(0, 1);
	}
	return lwlib_vec2i_zero();
}

static lwlib_TVector3i WR_QnNextPos(WR_Level_QuadNumber_t qn,
	lwlib_TVector3i pos)
{
	lwlib_TPoint2i qnVec = WR_QnToVec(qn);
	return lwlib_vec3i(lwlib_X(pos) + lwlib_X(qnVec),
		lwlib_Y(pos) + lwlib_Y(qnVec), lwlib_Z(pos));
}

static void WR_LightMapper_FillBaseTextureRadMap(
	WR_LightMapper_t lightMapper, TRadMap radMap, TQuad *quad)
{
	int u, v;
	byte col;
	byte *postsource;
	uint32_t rgbaColor;
	TSpectra spectra;
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);

	RadMapSetAll(radMap, lwlib_vec3f_ones());

	postsource = quad->postsource;
	if (postsource != NULL)
	{
		for (u = 0; u < radMap.nu; u++)
		{
			for (v = 0; v < radMap.nv; v++)
			{
				col = postsource[((v + quad->postoffv) << TEXTURESHIFT) +
					(u + quad->postoffu)];
				rgbaColor = RGBA_FROMPAL(gamepal, col);
				spectra = lwlib_vec3f(LT_RGB_EXPAND(rgbaColor), 0, 0);
				spectra = lwlib_vec3f_scale(spectra, 1.0 / 255.0);
				RadMapSetElem(radMap, u, v, spectra);
				RadMapSetElemAlpha(radMap, u, v, col);
			}
		}
	}
}

static TRadMap WR_LightMapper_GenTextureRadMap(
	WR_LightMapper_t lightMapper, TQuad *quad, TRadMap radMap)
{
	int i;
	int u, v;
	TRadMap textureRadMap;
	TRadMap tmpRadMap;
	TRadMap baseTextureRadMap;
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);

	if (quad->isSky)
	{
		textureRadMap = RadMapInit(quad->postdimu, quad->postdimv);
		WR_LightMapper_FillBaseTextureRadMap(lightMapper,
			textureRadMap, quad);
	}
	else
	{
		textureRadMap = RadMapInit(quad->postdimu, quad->postdimv);

		// center
		RadMapStretchCopy(textureRadMap, 
			RadMapRegion(0, 0, textureRadMap.nu, textureRadMap.nv),
			radMap,
			RadMapRegion(1, 1, radMap.nu - 2, radMap.nv - 2),
			true);

		// flip
		tmpRadMap = textureRadMap;
		textureRadMap = RadMapFlip(textureRadMap, 0);
		RadMapFree(&tmpRadMap);

		// modulate with base texture
		baseTextureRadMap = RadMapInit(textureRadMap.nu, 
			textureRadMap.nv);
		WR_LightMapper_FillBaseTextureRadMap(lightMapper,
			baseTextureRadMap, quad);
		RadMapModulate(textureRadMap, 0, 0, baseTextureRadMap);
		RadMapFree(&baseTextureRadMap);
	}

	// transpose
	tmpRadMap = textureRadMap;
	textureRadMap = RadMapTranspose(textureRadMap);
	RadMapFree(&tmpRadMap);

	return textureRadMap;
}

WR_LightMapper_t WR_LightMapper_New(void)
{
	WR_LightMapper_t lightMapper;
	lwlib_Zero(lightMapper);

	lwlib_GetPriv(WR_LightMapper_Priv_t, lightMapper);
	priv = lwlib_CallocSingle(WR_LightMapper_Priv_t);

	lightMapper.priv = priv;
	return lightMapper;
}

WR_LightMapper_t WR_LightMapper_Free(WR_LightMapper_t lightMapper)
{
	lwlib_GetPriv(WR_LightMapper_Priv_t, lightMapper);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(lightMapper);
	return lightMapper;
}

void WR_LightMapper_GenerateLightmaps(WR_LightMapper_t lightMapper)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i, j;
	int count;
	lwlib_Img_t lightmap;
	const int lightmapSize = LIGHTMAP_PIXELS * sizeof(uint32_t);
	int lm = 0;
	TQuad *quad;
	LM_Region_t region;
	lwlib_Bits_t potentialDoorBits;
	lwlib_Bits_t bestDoorBits;
	lwlib_Bits_t finalDoorBits;
	TRadMap sumRadMap, textureRadMap;
	ProtMsgList_t protMsgList = protMsgNewList();
	ProtMsgLightFacet_t protMsgLightFacet;
	ProtMsgLightDoor_t protMsgLightDoor;
	ProtMsgLightInfo_t protMsgLightInfo;
	TSpectra spriteLitLevel;
	TLight *light;
	lwlib_String_t tmpStr[3];
	lwlib_String_t fname;

	lwlib_LogPrintf("Generating lightmaps for %d quads\n",
		vec_size(rad_params.quads));

	memset(&lightmap, 0, sizeof(lightmap));
	lightmap.w = LIGHTMAP_WIDTH;
	lightmap.h = LIGHTMAP_HEIGHT;
	lightmap.bpp = sizeof(uint32_t);
	lightmap.data = (unsigned char *)calloc(1, lightmapSize);

	LM_RegionReset(&region, lightmap.w * lightmap.h);

	lwlib_LogPrintf("%11s %7s %7s\n", "Quad", "Pos", "Doors");

	// lightmaps
	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		if (quad->lightNum != 0 || (quad->lightTileRadius == 0 &&
			!WR_LightMapper_IncludeQuad(lightMapper, quad)))
		{
			continue;
		}

		lwlib_Zero(potentialDoorBits);
		for (j = 0; j < vec_size(rad_params.lights); j++)
		{
			light = &vec_at(rad_params.lights, j);
			potentialDoorBits = WR_LightMapper_AddPotentialDoorBits(
				lightMapper, quad, light, potentialDoorBits);
		}

		// consider only the best doors in potential set
		bestDoorBits = WR_LightMapper_ComputeBestDoors(lightMapper, quad, 
			potentialDoorBits);

		lwlib_LogPrintf("%11s %7s %7s\n", 
			lwlib_VaString(tmpStr[0], "%d/%d", i, vec_size(rad_params.quads)), 
			lwlib_VaString(tmpStr[1], "(%03d,%03d)", lwlib_X(quad->pos),
				lwlib_Y(quad->pos)), 
			lwlib_VaString(tmpStr[2], "%02d/%02d", 
				lwlib_BitsCount(bestDoorBits),
				lwlib_BitsCount(potentialDoorBits))
			);
		lwlib_BitsFree(potentialDoorBits);

		// for every subset of open doors
		count = lwlib_BitsCount(bestDoorBits);
		for (j = 0; j < (1 << count); j++)
		{
			finalDoorBits = lwlib_BitsSubset(bestDoorBits, j);

			// sum the radiosities of all light sources affecting
			// this quad
			sumRadMap = WR_LightMapper_SumLightsRad(lightMapper, 
				quad, finalDoorBits);

			if (RadMapValid(sumRadMap))
			{
				spriteLitLevel = RadMapAverage(sumRadMap);

				textureRadMap = WR_LightMapper_GenTextureRadMap(
					lightMapper, quad, sumRadMap);
				RadMapFree(&sumRadMap);

				// emit lightmap for this quad and door set
				LM_GenerateLightmap(&lightmap, region.off,
					textureRadMap);
			}
			else
			{
				spriteLitLevel = lwlib_vec3f_zero();
				lwlib_Zero(textureRadMap);
			}

			// save facet for this quad and door set
			lwlib_Zero(protMsgLightFacet);
			protMsgLightFacet.x = lwlib_X(quad->pos);
			protMsgLightFacet.y = lwlib_Y(quad->pos);
			protMsgLightFacet.qn = quad->qn;
			protMsgLightFacet.lm = lm;
			protMsgLightFacet.off = region.off;
			protMsgLightFacet.nu = textureRadMap.nu;
			protMsgLightFacet.nv = textureRadMap.nv;
			protMsgLightFacet.lightDoorStart = 
				vec_size(protMsgList.lightDoorSeq);
			protMsgLightFacet.lightDoorCount = count;
			protMsgLightFacet.lightDoorMask = j;
			lwlib_UnpackVector(spriteLitLevel, 
				protMsgLightFacet.spriteLitLevel);
			vec_push(protMsgList.lightFacetSeq,
				ProtMsgLightFacet_t, protMsgLightFacet);

			WR_LightMapper_RegionNextSpace(lightMapper, &region, 
				textureRadMap.nv, textureRadMap.nu);

			// save texture when lightmap is full
			if (WR_LightMapper_RegionIsFull(lightMapper, &region,
				LIGHTMAP_REGION_MAXSIZE))
			{
				LM_RegionReset(&region, lightmap.w * lightmap.h);
				WR_LightMapper_SaveLightmapTexture(lightMapper,
					&lightmap, lm, &protMsgList);
				lm++;
			}

			RadMapFree(&textureRadMap);

			lwlib_BitsFree(finalDoorBits);
		}

		// save best door set
		lwlib_BitsIter(bestDoorBits)
		{
			lwlib_Zero(protMsgLightDoor);
			protMsgLightDoor.doorNum = lwlib_BitsIndex(bestDoorBits);
			vec_push(protMsgList.lightDoorSeq, 
				ProtMsgLightDoor_t, protMsgLightDoor);
		}

		lwlib_BitsFree(bestDoorBits);
	}

	// save one more texture when lightmap is not empty
	if (!LM_RegionIsEmpty(&region))
	{
		LM_RegionReset(&region, lightmap.w * lightmap.h);
		WR_LightMapper_SaveLightmapTexture(lightMapper, &lightmap, lm,
			&protMsgList);
		lm++;
	}

	free(lightmap.data);

	lwlib_LogPrintf("-> Total Lightmaps : %d\n", lm);
	lwlib_LogPrintf("-> Total Disk Space Used : %dMb\n", 
		lm * lightmapSize / lwlib_CalcMb(1));

	lwlib_Zero(protMsgLightInfo);
	protMsgLightInfo.numLightmaps = lm;
	protMsgLightInfo.lightmapWidth = LIGHTMAP_WIDTH;
	protMsgLightInfo.lightmapHeight = LIGHTMAP_HEIGHT;
	vec_push(protMsgList.lightInfoSeq, ProtMsgLightInfo_t,
		protMsgLightInfo);

	lwlib_VaString(fname, "lightinfo%d.lwf", mapon);
	protMsgWriteFileCreate(&protMsgList, fname);

	protMsgFreeList(&protMsgList);
}

static void WR_LightMapper_GenerateQuadRadMap(
	WR_LightMapper_t lightMapper, int processNum, TQuad *quad)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i;
	TElement* ep;
	TRadMap radMap;

	lwlib_Zero(radMap);

	for (i = 0; i < BORDER_NU(quad) * BORDER_NV(quad); i++)
	{
		ep = WR_LightMapper_LightFacetElem(lightMapper, i, quad);
		if (ep == NULL)
			continue;

		if (lwlib_vec3f_max(ep->rad) != 0.0 && !RadMapValid(radMap))
		{
			radMap = RadMapInit(BORDER_NU(quad), BORDER_NV(quad));
		}

		if (RadMapValid(radMap))
		{
			lwlib_Copy(radMap.spectra[i], ep->rad);
		}
	}

	if (RadMapValid(radMap))
	{
		LWLIB_CTX_WRITE(processNum, quad->radMap, radMap, TRadMap);
	}
}

void WR_LightMapper_GenerateQuadRadMaps(WR_LightMapper_t lightMapper,
	int processNum)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i;
	TQuad *quad;

	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		if (quad->lightNum != 0)
		{
			continue;
		}

		WR_LightMapper_GenerateQuadRadMap(lightMapper, processNum, quad);
	}
}

lwlib_Bits_t WR_LightMapper_LitDoorsNoMark(WR_LightMapper_t lightMapper, int processNum, int depth)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i, j;
	int qn;
	int quadNum;
	TQuad *quad;
	doorobj_t *door;
	WR_Level_Tile_t *tile;
	lwlib_Bits_t litDoorBits = lwlib_BitsEmpty();
	TRadMap radMap;
	double lowSpectraScore;
	int doorNum;

	lwlib_Dict(LM_ScoreIndexPair_t) scoreIndexDict;
	lwlib_Zero(scoreIndexDict);

	for (i = 0; i < doorcount; i++)
	{
		if (lwlib_BitsTest(rad_params.markDoorBits, i))
		{
			continue;
		}

		door = &doorobjlist[i];

		for (j = 0; j < door->quadIndexCount; j++)
		{
			quadNum = door->quadIndexArr[j];
			quad = &vec_at(rad_params.quads, quadNum);
			assert(quad->doorNum == i + 1);

			radMap = LWLIB_CTX_READ(processNum, quad->radMap,
				TRadMap);
			lowSpectraScore = RadMapValid(radMap) ? 
				RadMapLowSpectraScore(radMap) : 0.0;
			if 
				(
					RadMapValid(radMap) && 
					lowSpectraScore > 
						rad_params.lowSpectraDoorThreshold
				)
			{
				lwlib_DictAppend(scoreIndexDict, lowSpectraScore, i,
					LM_ScoreIndexPair_t);
				break;
			}
		}
	}

	// sort doors from highest to lowest using spectra score
	lwlib_DictSetCmpFn(scoreIndexDict, LM_ScoreIndexPair_t, 
		scoreCompare);
	lwlib_DictSort(scoreIndexDict);

	// process no more than litDoorLimit doors
	for (i = 0; i < lwlib_DictLen(scoreIndexDict) && 
		i < rad_params.litDoorLimit; i++)
	{
		lwlib_BitsSet(litDoorBits, lwlib_DictValue(scoreIndexDict, i));
	}

	lwlib_DictClear(scoreIndexDict, LM_ScoreIndexPair_t);

	return litDoorBits;
}

bool WR_LightMapper_LightDepthReached(WR_LightMapper_t lightMapper, int depth)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	return depth >= rad_params.maxLightRadDepth;
}

static double LM_ComputeDoorScore(WR_LightMapper_t lightMapper, TQuad *quad, int doorNum)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i;
	TRadMap radMap;
	double score;
	double maxScore;
	lwlib_Bits_t openDoorBits;

	maxScore = 0.0;

	for (i = 0; i < rad_params.totalProcesses; i++)
	{
		radMap = LWLIB_CTX_READ(i, quad->radMap, TRadMap);
		openDoorBits = LWLIB_CTX_READ(i, rad_params.openDoorBits,
			lwlib_Bits_t);

		if (RadMapValid(radMap) && lwlib_BitsTest(openDoorBits, doorNum))
		{
			score = RadMapLowSpectraScore(radMap);
			if (score > maxScore)
			{
				maxScore = score;
			}
		}
	}

	return maxScore;
}

lwlib_Bits_t WR_LightMapper_ComputeBestDoors(WR_LightMapper_t lightMapper, TQuad *quad, lwlib_Bits_t doorBits)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i;
	double score;
	int doorNum;
	lwlib_Dict(LM_ScoreIndexPair_t) scoreIndexDict;
	lwlib_Bits_t bestDoorBits;

	lwlib_Zero(scoreIndexDict);

	lwlib_BitsIter(doorBits)
	{
		doorNum = lwlib_BitsIndex(doorBits);
		score = LM_ComputeDoorScore(lightMapper, quad, doorNum);
		lwlib_DictAppend(scoreIndexDict, score, doorNum,
			LM_ScoreIndexPair_t);
	}

	// sort doors from highest to lowest using spectra score
	lwlib_DictSetCmpFn(scoreIndexDict, LM_ScoreIndexPair_t, scoreCompare);
	lwlib_DictSort(scoreIndexDict);

	// process no more than maxDoorBitsPerQuad doors
	lwlib_Zero(bestDoorBits);

	for (i = 0; i < lwlib_DictLen(scoreIndexDict); i++)
	{
		if (i >= rad_params.maxDoorBitsPerQuad)
		{
			break;
		}
		lwlib_BitsSet(bestDoorBits, lwlib_DictValue(scoreIndexDict, i));
	}

	lwlib_DictClear(scoreIndexDict, LM_ScoreIndexPair_t);
	return bestDoorBits;
}

void WR_LightMapper_SetRad(WR_LightMapper_t lightMapper, WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	priv->rad = rad;
}

void WR_LightMapper_SetRayCaster(WR_LightMapper_t lightMapper,
	WR_RayCaster_t rayCaster)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	priv->rayCaster = rayCaster;
}

void WR_LightMapper_SetLevel(WR_LightMapper_t lightMapper,
	WR_Level_t level)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	priv->level = level;
}

WR_LightInfo_t *WR_LightMapper_LoadLightInfo(
	WR_LightMapper_t lightMapper)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	int i;
	int *lightFacetLookup;
	WR_LightInfo_Tile_t *lightFacetTile;
	WR_LightInfo_LightDoor_t *lightDoor;
	WR_LightInfo_LightFacet_t *lightFacet;
	WR_LightInfo_Lightmap_t *lightmap;
	ProtMsgLightInfo_t *protMsgLightInfo;
	ProtMsgLightDoor_t *protMsgLightDoor;
	ProtMsgLightFacet_t *protMsgLightFacet;
	ProtMsgLightMap_t *protMsgLightMap;
	ProtMsgList_t protMsgList;
	lwlib_String_t fname;
	WR_LightInfo_t *lightInfo;
	int lmSize;

	protMsgList = protMsgNewList();

	lwlib_VaString(fname, "lightinfo%d.lwf", mapon);
	if (protMsgReadFileOpen(&protMsgList, fname) != PROT_STATUS_OK)
	{
		return NULL;
	}

	if (vec_size(protMsgList.lightInfoSeq) != 1)
	{
		protMsgFreeList(&protMsgList);
		return NULL;
	}

	lightInfo = lwlib_CallocSingle(WR_LightInfo_t);
	lightInfo->valid = true;

	protMsgLightInfo = &vec_at(protMsgList.lightInfoSeq, 0);
	lightInfo->lmWidth = protMsgLightInfo->lightmapWidth;
	lightInfo->lmWidthShift = lwlib_Log2(lightInfo->lmWidth);
	lightInfo->lmHeight = protMsgLightInfo->lightmapHeight;
	lightInfo->lmPitch = lightInfo->lmWidth * 4;
	lightInfo->lmPitchShift = lwlib_Log2(lightInfo->lmPitch);
	assert(1 << lightInfo->lmWidthShift == lightInfo->lmWidth &&
		1 << lightInfo->lmPitchShift == lightInfo->lmPitch);

	lightInfo->lightDoors = lwlib_CallocMany(WR_LightInfo_LightDoor_t,
		vec_size(protMsgList.lightDoorSeq));
	for (i = 0; i < vec_size(protMsgList.lightDoorSeq); i++)
	{
		lightDoor = &lightInfo->lightDoors[i];
		protMsgLightDoor = &vec_at(protMsgList.lightDoorSeq, i);
		lightDoor->doorNum = protMsgLightDoor->doorNum;
	}

	lightInfo->lightFacets = lwlib_CallocMany(WR_LightInfo_LightFacet_t, 
		vec_size(protMsgList.lightFacetSeq));
	for (i = 0; i < vec_size(protMsgList.lightFacetSeq); i++)
	{
		lightFacet = &lightInfo->lightFacets[i];
		protMsgLightFacet = &vec_at(protMsgList.lightFacetSeq, i);

		lightFacet->x = protMsgLightFacet->x;
		lightFacet->y = protMsgLightFacet->y;
		lightFacet->qn = protMsgLightFacet->qn;
		lightFacet->lm = protMsgLightFacet->lm;
		lightFacet->off = protMsgLightFacet->off;
		lightFacet->nu = protMsgLightFacet->nu;
		lightFacet->nv = protMsgLightFacet->nv;
		lightFacet->lightDoorFirst = &lightInfo->lightDoors[
			protMsgLightFacet->lightDoorStart];
		lightFacet->lightDoorCount = protMsgLightFacet->lightDoorCount;
		lightFacet->lightDoorMask = protMsgLightFacet->lightDoorMask;
		lightFacet->spriteLitLevel = 
			lwlib_vec3f(lwlib_PackVector(protMsgLightFacet->spriteLitLevel));

		lightFacetTile = &lightInfo->tiles[lightFacet->x][lightFacet->y];
		lightFacetLookup = &lightFacetTile->lightFacetLookup[lightFacet->qn];
		if (*lightFacetLookup == 0)
		{
			*lightFacetLookup = i + 1;
		}
	}
	lightInfo->numLightFacets = vec_size(protMsgList.lightFacetSeq);

	lightInfo->numLightmaps = protMsgLightInfo->numLightmaps;
	lightInfo->lightmaps = lwlib_CallocMany(WR_LightInfo_Lightmap_t,
		lightInfo->numLightmaps);
	for (i = 0; i < lightInfo->numLightmaps; i++)
	{
		lightmap = &lightInfo->lightmaps[i];
		protMsgLightMap = &vec_at(protMsgList.lightMapSeq, i);

		lmSize = lightInfo->lmHeight * lightInfo->lmPitch;
		lightmap->data = lwlib_CallocMany(unsigned char, lmSize);
		memcpy(lightmap->data, protMsgLightMap->data, lmSize);
	}

	protMsgFreeList(&protMsgList);
	return lightInfo;
}

WR_LightInfo_t *WR_LoadLightInfo(void)
{
	WR_LightInfo_t *lightInfo;
	WR_LightMapper_t lightMapper;
	
	lightMapper = WR_LightMapper_New();
	lightInfo = WR_LightMapper_LoadLightInfo(lightMapper);
	WR_LightMapper_Free(lightMapper);

	return lightInfo;
}

void WR_LightMapper_SetBres(WR_LightMapper_t lightMapper, lwlib_Bres_t bres)
{
	lwlib_GetPrivAssert(WR_LightMapper_Priv_t, lightMapper);
	priv->bres = bres;
}
#endif