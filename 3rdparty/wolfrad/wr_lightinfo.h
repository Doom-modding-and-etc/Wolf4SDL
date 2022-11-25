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
** LightInfo
*************************************************************************/

#ifndef WR_LIGHTINFO_H
#define WR_LIGHTINFO_H


#ifdef PS2
//#include "lw_vec.h"
#else
#include "..\lw_lib\lw_vec.h"
#endif
#ifdef WOLFRAD
#include "wr_level.h"

typedef struct WR_LightInfo_Tile_s
{
	int lightFacetLookup[WR_LEVEL_QN_TOT];
} WR_LightInfo_Tile_t;

typedef struct WR_LightInfo_LightDoor_s
{
	int doorNum;
} WR_LightInfo_LightDoor_t;

typedef struct WR_LightInfo_LightFacet_s
{
	int x, y;
	int qn;
	int lm;
	int off;
	int nu, nv;
	WR_LightInfo_LightDoor_t *lightDoorFirst;
	int lightDoorCount;
	unsigned int lightDoorMask;
	lwlib_TPoint3f spriteLitLevel;
} WR_LightInfo_LightFacet_t;

typedef struct WR_LightInfo_Lightmap_s
{
	unsigned char *data;
} WR_LightInfo_Lightmap_t;

typedef struct WR_LightInfo_s
{
	int valid;
	WR_LightInfo_Tile_t tiles[128][128];
	WR_LightInfo_LightDoor_t *lightDoors;
	WR_LightInfo_LightFacet_t *lightFacets;
	WR_LightInfo_Lightmap_t *lightmaps;
	int lmWidth, lmHeight, lmPitch;
    int lmWidthShift, lmPitchShift;
	int numLightFacets;
	int numLightmaps;
} WR_LightInfo_t;

void WR_LightInfo_Free(WR_LightInfo_t *lightInfo);
#endif
#endif
