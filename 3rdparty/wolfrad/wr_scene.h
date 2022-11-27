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
** Quadrilateral Generator for Wolf3D Map
*************************************************************************/

#ifndef WOLFRAD_SCENE_H
#define WOLFRAD_SCENE_H

#include "wr_rad.h"
#include "wr_level.h"
#ifdef PS2
//include "3rdparty/lw_lib/lw_vec.h"
#include "3rdparty/lw_lib/lw_vec.h"
#else
#include "..\lw_lib\lw_vec.h"
#endif
#ifdef WOLFRAD
#include "..\lw_lib\lw_bres.h"

typedef enum
{
	WR_RADLIGHTKEY_NONE,
	WR_RADLIGHTKEY_FLOORLAMP,
	WR_RADLIGHTKEY_CHANDELIER,
	WR_RADLIGHTKEY_CEILINGLIGHT,
	WR_RADLIGHTKEY_LIGHTBULB,
	WR_RADLIGHTKEY_LIGHTBULBBRIGHT,
	WR_RADLIGHTKEY_SKYLIGHT,
	WR_RADLIGHTKEY_FLOORCEILSTART,
	WR_RADLIGHTKEY_FLUORESCENTPANEL = WR_RADLIGHTKEY_FLOORCEILSTART,
	WR_RADLIGHTKEY_MAX,
} TRadLightKey;

lwlib_EnumStrExtern(TRadLightKey);

typedef struct {
	int radLightKey;
	int tileRadius;                   /* max distance reached by light */
	lwlib_Seq(TRadLight) radLights;   /* radiosity light sources */
} TLightProfile;

lwlib_SeqType(TLightProfile);

typedef struct {
	lwlib_Seq(TLightProfile) lightProfiles;
} TSceneParams;

typedef struct WR_Scene_s
{
	void *priv;
} WR_Scene_t;

WR_Scene_t WR_Scene_New(void);

WR_Scene_t WR_Scene_Free(WR_Scene_t scene);

void WR_Scene_SetParams(WR_Scene_t scene, TSceneParams sceneParams);

void WR_Scene_ResetParams(WR_Scene_t scene);

void WR_Scene_Generate_Quads(WR_Scene_t scene);

void WR_Scene_ClearQuads(WR_Scene_t scene);

void WR_Scene_ToggleLightStates(WR_Scene_t scene, int lightNum);

void WR_Scene_SetAllLightStates(WR_Scene_t scene, bool excludeFromScene);

void WR_Scene_ToggleDoorStates(WR_Scene_t scene, lwlib_Bits_t doorBits);

void WR_Scene_SetRad(WR_Scene_t scene, WR_Rad_t rad);

void WR_Scene_SetLevel(WR_Scene_t scene, WR_Level_t level);

void WR_Scene_SetBres(WR_Scene_t scene, lwlib_Bres_t bres);

void WR_Scene_ResetProcesses(WR_Scene_t scene);

#endif
#endif
