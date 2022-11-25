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

#ifndef WOLFRAD_LIGHTMAP_H
#define WOLFRAD_LIGHTMAP_H

#include "wr_rad.h"
#include "wr_raycaster.h"
#include "wr_level.h"
#include "wr_lightinfo.h"
#ifdef WOLFRAD
#include "..\lw_lib\lw_bres.h"
#include "..\lw_lib\lw_vec.h"

typedef struct WR_LightMapper_s
{
	void *priv;
} WR_LightMapper_t;

WR_LightMapper_t WR_LightMapper_New(void);

WR_LightMapper_t WR_LightMapper_Free(WR_LightMapper_t lightMapper);

void WR_LightMapper_GenerateLightmaps(WR_LightMapper_t lightMapper);

void WR_LightMapper_GenerateQuadRadMaps(WR_LightMapper_t lightMapper, int processNum);

lwlib_Bits_t WR_LightMapper_LitDoorsNoMark(WR_LightMapper_t lightMapper, int processNum, int depth);

bool WR_LightMapper_LightDepthReached(WR_LightMapper_t lightMapper, 
	int depth);

lwlib_Bits_t WR_LightMapper_ComputeBestDoors(WR_LightMapper_t lightMapper, TQuad *quad, lwlib_Bits_t doorBits);

void WR_LightMapper_SetRad(WR_LightMapper_t lightMapper, WR_Rad_t rad);

void WR_LightMapper_SetRayCaster(WR_LightMapper_t lightMapper,
	WR_RayCaster_t rayCaster);

void WR_LightMapper_SetLevel(WR_LightMapper_t lightMapper,
	WR_Level_t level);

WR_LightInfo_t *WR_LightMapper_LoadLightInfo(
	WR_LightMapper_t lightMapper);

WR_LightInfo_t *WR_LoadLightInfo(void);

void WR_LightMapper_SetBres(WR_LightMapper_t lightMapper, lwlib_Bres_t bres);

#endif
#endif

