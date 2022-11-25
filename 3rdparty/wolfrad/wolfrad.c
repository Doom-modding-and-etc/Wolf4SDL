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
** WolfRad
*************************************************************************/

#ifdef _WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif

#include "wolfrad.h"
#include "wr_scene.h"
#include "wr_level.h"
#include "wr_room.h"
#include "wr_lightmap.h"
#include "..\lw_lib\lw_bres.h"
#ifdef WOLFRAD
#include "..\..\wl_def.h"

#define CONTROLPANEL_WIDTH 30
#define CONTROLPANEL_HEIGHT 17

#define PROGRESS_WIDTH 30
#define PROGRESS_HEIGHT 16
#define PROGRESS_MAXLINES 12

typedef struct WolfRad_ProgressState_s
{
	lwlib_String_t partialStr;
	uint32_t lastTimeCount;
	lwlib_StrSeq_t strs;
	bool cancelled;
} WolfRad_ProgressState_t;

typedef struct WolfRad_Priv_s
{
	WR_Level_t level;
	WR_Scene_t scene;
	WR_Rad_t rad;
	WR_Room_t room;
	WR_LightMapper_t lightMapper;
	WR_RayCaster_t rayCaster;
	lwlib_Bres_t bres;
	WolfRad_ProgressState_t progressState;
} WolfRad_Priv_t;

static void WolfRad_SetRadParams(void)
{
	rad_params.worldSize = 64;
	rad_params.addAmbient = 0;

	switch (rad_params.qualityLevel)
	{
	case WR_QUALITYLEVEL_LOW:
		rad_params.patchLevel = 4;
		rad_params.elementLevel = 2;
		rad_params.nu = 8;
		rad_params.nv = 8;
		rad_params.maxShoots = 12;
		break;
	case WR_QUALITYLEVEL_MEDIUM:
		rad_params.patchLevel = 8;
		rad_params.elementLevel = 2;
		rad_params.nu = 16;
		rad_params.nv = 16;
		rad_params.maxShoots = 40;
		break;
	case WR_QUALITYLEVEL_HIGH:
		rad_params.patchLevel = 16;
		rad_params.elementLevel = 2;
		rad_params.nu = 32;
		rad_params.nv = 32;
		rad_params.maxShoots = 80;
		break;
	}

	rad_params.colorReflectance = 1;
	rad_params.pointSourceModel = 0;
	rad_params.lowSpectraThreshold = 0.005;
	rad_params.maxLightRadDepth = 0;
	rad_params.lowSpectraDoorThreshold = 0.025;
	rad_params.litDoorLimit = 4;
	rad_params.maxDoorBitsPerQuad = 4;
}

static void WolfRad_SetSceneParams(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	TLightProfile lp;

	const lwlib_TPoint3f white = lwlib_vec3f(1.0, 1.0, 1.0);
	const lwlib_TPoint3f orange = lwlib_vec3f(1.0, 0.5, 0.0);
	const lwlib_TPoint3f red = lwlib_vec3f(1.0, 0.0, 0.0);
	const lwlib_TPoint3f green = lwlib_vec3f(0.0, 1.0, 0.0);
	const lwlib_TPoint3f yellow = lwlib_vec3f(1.0, 1.0, 0.0);
	const lwlib_TPoint3f white3p_green1p =
		lwlib_vec3f_clip(
			lwlib_vec3f_add(
				lwlib_vec3f_scale(white, 3),
				green
				)
			);
	const lwlib_TPoint3f white3p_yellow1p =
		lwlib_vec3f_clip(
			lwlib_vec3f_add(
				lwlib_vec3f_scale(white, 3),
				yellow
				)
			);

	lwlib_TCone ceilingRadLightCone = {
		lwlib_vec3_init(0.0, 0.0, 0.9),    /* vertex */
		lwlib_vec3_init(0.0, 0.0, -1.0),   /* axis */
		80.0,                        /* angle */
		0.0,                         /* lowerRim */
		0.3,                         /* upperRim */
		0,                           /* acceptRay */
		1,                           /* trivialTest */
		-1.0,                        /* minRayProj */
		0.1,                         /* maxRayProj */
	};                               /* cone */

	TRadLight commonRadLight = {
		lwlib_vec3_init(1.0, 1.0, 1.0), /* emission */
		0.01,                           /* size */
		0.8,                            /* elevation */
		1,                              /* patchLevel */
		1,                              /* elementLevel */
		0.15,                           /* areaSize */
	};

	TRadLight ceilingLowerRadLight = commonRadLight;
	ceilingLowerRadLight.emission = lwlib_vec3f_scale(white, 120.0);
	ceilingLowerRadLight.cone = ceilingRadLightCone;
	ceilingLowerRadLight.cone.acceptRay = 0;

	TRadLight redCeilingUpperRadLight = commonRadLight;
	redCeilingUpperRadLight.emission = lwlib_vec3f_scale(red, 45.0);
	redCeilingUpperRadLight.cone = ceilingRadLightCone;
	redCeilingUpperRadLight.cone.acceptRay = 1;

	TRadLight greenCeilingUpperRadLight = commonRadLight;
	greenCeilingUpperRadLight.emission = lwlib_vec3f_scale(
		white3p_green1p, 100.0);
	greenCeilingUpperRadLight.cone = ceilingRadLightCone;
	greenCeilingUpperRadLight.cone.acceptRay = 1;

	lwlib_Seq(TLightProfile) lightProfiles;
	lwlib_Zero(lightProfiles);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_FLOORLAMP;
	lp.tileRadius = 8;
	TRadLight streetLightRadLight = commonRadLight;
	streetLightRadLight.emission = lwlib_vec3f_scale(white, 125.0);
	vec_push(lp.radLights, TRadLight, streetLightRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_CHANDELIER;
	lp.tileRadius = 8;
	TRadLight chandelierRadLight = commonRadLight;
	chandelierRadLight.emission = lwlib_vec3f_scale(white3p_yellow1p, 150.0);
	vec_push(lp.radLights, TRadLight, chandelierRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_CEILINGLIGHT;
	lp.tileRadius = 8;
	vec_push(lp.radLights, TRadLight, ceilingLowerRadLight);
	vec_push(lp.radLights, TRadLight, greenCeilingUpperRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_LIGHTBULB;
	lp.tileRadius = 4;
	TRadLight lightBulbRadLight = commonRadLight;
	lightBulbRadLight.emission = lwlib_vec3f_scale(white, 70.0);
	vec_push(lp.radLights, TRadLight, lightBulbRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_LIGHTBULBBRIGHT;
	lp.tileRadius = 6;
	TRadLight lightBulbBrightRadLight = commonRadLight;
	lightBulbBrightRadLight.emission = lwlib_vec3f_scale(white, 100.0);
	vec_push(lp.radLights, TRadLight, lightBulbBrightRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_SKYLIGHT;
	lp.tileRadius = 9;
	TRadLight skyLightRadLight = commonRadLight;
	skyLightRadLight.emission = lwlib_vec3f_scale(white, 400.0);
	vec_push(lp.radLights, TRadLight, skyLightRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	lwlib_Zero(lp);
	lp.radLightKey = WR_RADLIGHTKEY_FLUORESCENTPANEL;
	lp.tileRadius = 6;
	TRadLight fluorescentPanelRadLight = commonRadLight;
	fluorescentPanelRadLight.emission = lwlib_vec3f_scale(white, 80.0);
	vec_push(lp.radLights, TRadLight, fluorescentPanelRadLight);
	vec_push(lightProfiles, TLightProfile, lp);

	TSceneParams sceneParams;
	lwlib_Zero(sceneParams);

	sceneParams.lightProfiles = lightProfiles;
	WR_Scene_SetParams(priv->scene, sceneParams);
}

static bool WolfRad_AllowDoorSubset(lwlib_Bits_t litDoorSubset, 
	int index)
{
	if (lwlib_BitCount(index) > 1)
	{
		return false;
	}

	return true;
}

static void WolfRad_LightRad(WolfRad_t wolfRad, int lightNum,
	lwlib_Bits_t openDoorBits, int depth)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	int i;
	int count;
	TLight *light;
	int processNum;
	int doorNum;
	lwlib_Bits_t litDoorBits;
	lwlib_Bits_t litDoorSubset;

	if (WolfRad_Cancelled(wolfRad))
	{
		return;
	}

	// open doors designated by door set
	WR_Scene_ToggleDoorStates(priv->scene, openDoorBits);

	WR_Room_ComputeNeighbours(priv->room);

	// report radiosity process start
	light = &vec_at(rad_params.lights, lightNum);
	lwlib_LogPrintf("DoRad: light %d/%d, doorBits [ ", 
		lightNum, vec_size(rad_params.lights));
	lwlib_BitsIter(openDoorBits)
	{
		doorNum = lwlib_BitsIndex(openDoorBits);
		lwlib_LogPrintf("%d%s ", doorNum, doorNum >= doorcount ? "*" : "");
	}
	lwlib_LogPrintf("]\n");

	// compute radiosity for this light and door set
	WR_Rad_Init(priv->rad);
	WR_Rad_Run(priv->rad);
	WR_Rad_CleanUp(priv->rad);

	// produce radiosity maps for each quad here
	processNum = rad_params.totalProcesses;
	WR_LightMapper_GenerateQuadRadMaps(priv->lightMapper, processNum);

	// advance to next radiosity process
	rad_params.totalProcesses++;

	// find new doors seen by light
	litDoorBits = WR_LightMapper_LitDoorsNoMark(
		priv->lightMapper, processNum, depth);

	// close doors designated by door set
	WR_Scene_ToggleDoorStates(priv->scene, openDoorBits);

	// attach door states and light source to this process
	LWLIB_CTX_WRITE(processNum, rad_params.openDoorBits,
		lwlib_BitsCopy(openDoorBits), lwlib_Bits_t);
	LWLIB_CTX_WRITE(processNum, rad_params.lightNum, lightNum, int);

	// increment number of radiosity processes for this light
	if (light->numProcesses == 0)
	{
		light->firstProcessIndex = rad_params.totalProcesses - 1;
	}
	light->numProcesses++;

	// mark newly lit doors
	lwlib_BitsSetMany(rad_params.markDoorBits, litDoorBits);

	if (!WR_LightMapper_LightDepthReached(priv->lightMapper,
		depth))
	{
		// compute radiosity for every subset of newly lit doors
		count = lwlib_BitsCount(litDoorBits);

		for (i = 1; i < (1 << count); i++)
		{
			if (!WolfRad_AllowDoorSubset(litDoorBits, i))
			{
				continue;
			}

			litDoorSubset = lwlib_BitsSubset(litDoorBits, i);

			// ensure open doors stay open in the process below
			lwlib_BitsSetMany(litDoorSubset, openDoorBits);
			WolfRad_LightRad(wolfRad, lightNum, litDoorSubset,
				depth + 1);

			lwlib_BitsFree(litDoorSubset);
		}
	}

	// return newly lit doors back to unmarked state
	lwlib_BitsUnsetMany(rad_params.markDoorBits, litDoorBits);

	// destroy newly lit door set
	lwlib_BitsFree(litDoorBits);
}

static void WolfRad_ProcessLevel(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	int i;
	TLight *light;
	lwlib_Bits_t lightDoorBits;
	TRadParams *radParams;

	WolfRad_SetRadParams();

	WolfRad_SetSceneParams(wolfRad);

	lwlib_LogPrintf("Computing Cell to Cell Visibility\n");
	lwlib_Bres_ComputeVisAll(priv->bres);

	if (WolfRad_Cancelled(wolfRad))
	{
		return;
	}

	/* How many quads are needed? */
	lwlib_LogPrintf("Generating Quads\n");
	WR_Scene_Generate_Quads(priv->scene);

	if (WolfRad_Cancelled(wolfRad))
	{
		return;
	}

	lwlib_LogPrintf("-> Total Quads : %d\n", vec_size(rad_params.quads));

	/* Meshing quads into patches and elements */
	WR_Room_InitParams(priv->room);

	lwlib_LogPrintf("-> Total Lights : %d\n", vec_size(rad_params.lights));
	lwlib_LogPrintf("-> Total Doors : %d\n", doorcount);
	lwlib_LogPrintf("-> Total Static Objects : %d\n", laststatobj - statobjlist);

	for (i = 0; i < vec_size(rad_params.lights); i++)
	{
		if (WolfRad_Cancelled(wolfRad))
		{
			goto cancelled;
		}

		light = &vec_at(rad_params.lights, i);

		// turn off all other lights
		WR_Scene_ToggleLightStates(priv->scene, i);

		lwlib_Zero(lightDoorBits);
		WolfRad_LightRad(wolfRad, i, lightDoorBits, 0);

		// turn them back on
		WR_Scene_ToggleLightStates(priv->scene, i);

		lwlib_BitsFree(lightDoorBits);
	}

	WR_LightMapper_GenerateLightmaps(priv->lightMapper);

cancelled:
	WR_Room_DestroyParams(priv->room);
	WR_Scene_ClearQuads(priv->scene);
}

static void WolfRad_LogPrintfCallback(const char *message)
{
	WolfRad_Progress(wolfRad, message, false);
}

static void WolfRad_HandleGo(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	IN_ClearKeysDown();

	lwlib_LogPrintfCallback = WolfRad_LogPrintfCallback;
	WolfRad_Run(wolfRad);

	lwlib_LogPrintf("Press any key to continue...\n");
	lwlib_LogPrintfCallback = NULL;

	WolfRad_Progress(wolfRad, NULL, true);

	vec_free(priv->progressState.strs, lwlib_Str_t);
	lwlib_Zero(priv->progressState);

	WolfRad_ReloadCurLightInfo();
	IN_Ack();
}

static void WolfRad_HandleInsertStatObj(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	ED_ProcessInsertStatObj(ed, rad_params.insertStatObj);
}

static void WolfRad_HandleInsertFloorCeil(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	ED_ProcessInsertFloorCeil(ed, rad_params.insertFloorCeil);
}

WolfRad_t WolfRad_New(void)
{
	WolfRad_t wolfRad;

	lwlib_Zero(wolfRad);
	wolfRad.priv = lwlib_CallocSingle(WolfRad_Priv_t);

	lwlib_GetPriv(WolfRad_Priv_t, wolfRad);
	priv->level = WR_Level_New();
	priv->scene = WR_Scene_New();
	priv->rad = WR_Rad_New();
	priv->room = WR_Room_New();
	priv->lightMapper = WR_LightMapper_New();
	priv->rayCaster = WR_RayCaster_New();
	priv->bres = lwlib_Bres_New();

	WR_Scene_SetRad(priv->scene, priv->rad);
	WR_Scene_SetLevel(priv->scene, priv->level);
	WR_Scene_SetBres(priv->scene, priv->bres);

	WR_Room_SetRad(priv->room, priv->rad);
	WR_Room_SetLevel(priv->room, priv->level);
	WR_Room_SetLightMapper(priv->room, priv->lightMapper);

	WR_LightMapper_SetRad(priv->lightMapper, priv->rad);
	WR_LightMapper_SetRayCaster(priv->lightMapper, priv->rayCaster);
	WR_LightMapper_SetLevel(priv->lightMapper, priv->level);
	WR_LightMapper_SetBres(priv->lightMapper, priv->bres);

	WR_Rad_SetRayCaster(priv->rad, priv->rayCaster);
	WR_Rad_SetLevel(priv->rad, priv->level);

	WR_RayCaster_SetRad(priv->rayCaster, priv->rad);
	WR_RayCaster_SetLevel(priv->rayCaster, priv->level);
	WR_RayCaster_SetBres(priv->rayCaster, priv->bres);

	return wolfRad;
}

WolfRad_t WolfRad_Free(WolfRad_t wolfRad)
{
	lwlib_GetPriv(WolfRad_Priv_t, wolfRad);
	if (priv)
	{
		WR_Level_Free(priv->level);
		WR_Scene_Free(priv->scene);
		WR_Rad_Free(priv->rad);
		WR_Room_Free(priv->room);
		WR_LightMapper_Free(priv->lightMapper);
		WR_RayCaster_Free(priv->rayCaster);
		lwlib_Bres_Free(priv->bres);
		free(priv);
	}
	lwlib_Zero(wolfRad);
	return wolfRad;
}

void WolfRad_Run(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);

	WR_Level_InitFromLive(priv->level);
	WolfRad_ProcessLevel(wolfRad);
}

void WolfRad_ControlPanel(WolfRad_t wolfRad)
{
	int i;
	int option;
	bool esc;
	lwlib_String_t tmpStr;
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	struct OptionInfo_s
	{
		const char *name;
		int *value;
		int maxValue;
		const char **strings;
		void (*handler)(WolfRad_t);
	} optionInfos[] =
	{
		{ "Start Radiosity", NULL, 0, NULL, WolfRad_HandleGo },
		{ "Quality", &rad_params.qualityLevel, 
			WR_QUALITYLEVEL_MAX, lwlib_UseEnumStr(WR_QualityLevel_t) },
		{ "Lights", &rad_params.includedLights,
			WR_INCLUDEDLIGHTS_MAX, lwlib_UseEnumStr(WR_IncludedLights_t) },
		{ "Surfaces", &rad_params.includedSurfaces,
			WR_INCLUDEDSURFACES_MAX, lwlib_UseEnumStr(WR_IncludedSurfaces_t) },
		{ "", NULL, 0, NULL, NULL },
		{ "Insert or Remove", NULL, 0, NULL, WolfRad_HandleInsertStatObj },
		{ "Static Object", &rad_params.insertStatObj,
			WR_INSERTSTATOBJ_MAX, lwlib_UseEnumStr(WR_InsertStatObj_t) },

		{ "", NULL, 0, NULL, NULL },
		{ "Insert", NULL, 0, NULL, WolfRad_HandleInsertFloorCeil },
		{ "Floor or Ceiling", &rad_params.insertFloorCeil,
			WR_INSERTFLOORCEIL_MAX, 
			lwlib_UseEnumStr(WR_InsertFloorCeil_t) },
	};
	struct OptionInfo_s *optionInfo;
	const int numOptions = lwlib_CountArray(optionInfos);

	esc = 0;
	option = 0;

	while (!esc)
	{
		CenterWindow(CONTROLPANEL_WIDTH, CONTROLPANEL_HEIGHT);
		PrintY += CONTROLPANEL_HEIGHT;
		US_Print("Radiosity Control Panel\n");
		for (i = 0; i < numOptions; i++)
		{
			optionInfo = &optionInfos[i];
			if (optionInfo->value != NULL)
			{
				US_Print(lwlib_VaString(tmpStr, 
					"  %s%s = %s\n",
						option == i ? "-> " : "",  optionInfo->name,
						optionInfo->strings[*optionInfo->value]));
			}
			else
			{
				US_Print(lwlib_VaString(tmpStr, 
					"  %s%s\n",
						option == i ? "-> " : "",  optionInfo->name));
			}
		}
		VH_UpdateScreen(screen);
		IN_Ack();

		if (Keyboard(sc_Escape))
		{
			esc = 1;
		}
		else if (Keyboard(sc_DownArrow))
		{
			option = (option + 1) % numOptions;
			if (optionInfos[option].name[0] == '\0')
			{
				option++;
			}
		}
		else if (Keyboard(sc_UpArrow))
		{
			option = (option + numOptions - 1) % numOptions;
			if (optionInfos[option].name[0] == '\0')
			{
				option--;
			}
		}
		else if (Keyboard(sc_RightArrow) || Keyboard(sc_LeftArrow))
		{
			optionInfo = &optionInfos[option];
			if (optionInfo->value != NULL)
			{
				*optionInfo->value = 
					(*optionInfo->value + optionInfo->maxValue + 
					(Keyboard(sc_RightArrow) ? 1 : -1)) %
					optionInfo->maxValue;
			}
		}
		else if (Keyboard(sc_Enter))
		{
			optionInfo = &optionInfos[option];
			if (optionInfo->handler != NULL)
			{
				optionInfo->handler(wolfRad);
			}
			esc = 1;
		}
	}

	IN_ClearKeysDown();
}

void WolfRad_Progress(WolfRad_t wolfRad, const char *message,
	bool forceUpdate)
{
	int i;
	lwlib_String_t tmpStr;
	uint32_t curtime;
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	WolfRad_ProgressState_t *progressState;
	bool doUpdate;

	progressState = &priv->progressState;

	memcpy(tmpStr, progressState->partialStr, sizeof(tmpStr));
	lwlib_VaString(progressState->partialStr, "%s%s", tmpStr, message);

	doUpdate = false;

	if (message != NULL && strchr(message, '\n') != NULL)
	{
		vec_push(progressState->strs, lwlib_Str_t,
			lwlib_Str(progressState->partialStr));
		progressState->partialStr[0] = '\0';

		curtime = SDL_GetTicks();

		if (progressState->lastTimeCount == 0 ||
			curtime - progressState->lastTimeCount > 200)
		{
			doUpdate = true;
			progressState->lastTimeCount = curtime;
		}
	}

	if (forceUpdate)
	{
		doUpdate = true;
	}

	if (doUpdate)
	{
		CenterWindow(PROGRESS_WIDTH, PROGRESS_HEIGHT);

		i = 0;
		if (vec_size(progressState->strs) >= PROGRESS_MAXLINES)
		{
			i = vec_size(progressState->strs) - PROGRESS_MAXLINES;
		}
		while (i < vec_size(progressState->strs))
		{
			US_Print(vec_at(progressState->strs, i).v);
			i++;
		}
		VH_UpdateScreen();

		if (!progressState->cancelled)
		{
			// check for cancellation key
			IN_ProcessEvents();
			if (LastScan == sc_Escape)
			{
				progressState->cancelled = true;
				lwlib_LogPrintf("Cancelling\n");
			}
		}
	}
}

void WolfRad_ReadConfig(WolfRad_t wolfRad, int file)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);

	read(file, &rad_params.qualityLevel,
		sizeof(rad_params.qualityLevel));
	read(file, &rad_params.includedLights,
		sizeof(rad_params.includedLights));
	read(file, &rad_params.includedSurfaces,
		sizeof(rad_params.includedSurfaces));
}

void WolfRad_WriteConfig(WolfRad_t wolfRad, int file)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);

	write(file, &rad_params.qualityLevel,
		sizeof(rad_params.qualityLevel));
	write(file, &rad_params.includedLights,
		sizeof(rad_params.includedLights));
	write(file, &rad_params.includedSurfaces,
		sizeof(rad_params.includedSurfaces));
}

bool WolfRad_Cancelled(WolfRad_t wolfRad)
{
	lwlib_GetPrivAssert(WolfRad_Priv_t, wolfRad);
	return priv->progressState.cancelled;
}

void WolfRad_ReloadCurLightInfo(void)
{
	WR_Level_ReloadCurInfo();
}
#endif