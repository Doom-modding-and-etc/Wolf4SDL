//
//	WL Edit
//	WL_ED.c - Game Editor
//	By Linux Wolf - Team RayCAST
//

#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_EDITOR)
#include "wl_shade.h"
#include "wl_ed.h"

#define ED_STATEFILE "editstate.wle"

typedef struct ED_StatObj_s
{
	int x, y;
	int actor;
} ED_StatObj_t;

typedef vec_t(ED_StatObj_t) ED_StatObjVec_t;

typedef struct ED_FloorCeil_s
{
	int x, y;
	int floor, ceil;
} ED_FloorCeil_t;

typedef hvec_t(ED_FloorCeil_t) ED_FloorCeilHVec_t;

typedef struct ED_Map_s
{
	int index;
	ED_StatObjVec_t statObjs;
	lwlib_IntMap_t spotToActorMap;
	ED_FloorCeilHVec_t floorCeils;
} ED_Map_t;

typedef vec_t(ED_Map_t) ED_MapVec_t;

typedef struct ED_InsertStatObjActorPair_s
{
	WR_InsertStatObj_t insertStatObj;
	int actor;
} ED_InsertStatObjActorPair_t;

typedef vec_t(ED_InsertStatObjActorPair_t) ED_InsertStatObjActorPairVec_t;

typedef struct ED_ExtraStatInfo_s
{
	int actor;
	bool blocking;
	lwlib_String_t spriteFileName;
	int radLightKey;
	bool fullBright;
	bool dirEnable;
	bool dirVertical;
	bool blockLine;
	t_compshape *shape;
} ED_ExtraStatInfo_t;

typedef hvec_t(ED_ExtraStatInfo_t) ED_ExtraStatInfoHVec_t;

typedef struct ED_Priv_s
{
	lwlib_Edit_t edit;
	lwlib_Edit_Persist_t persist;
	ED_MapVec_t maps;
	ED_InsertStatObjActorPairVec_t insertStatObjActorPairs;
	ED_ExtraStatInfoHVec_t extraStatInfos;
	int skyPic;
} ED_Priv_t;

static void ED_StatObj_AddToEdit(void *elem, lwlib_Edit_t edit)
{
	ED_StatObj_t *statObj;
	statObj = (ED_StatObj_t *)elem;
	lwlib_Edit_AddItem_int(edit, "x", &statObj->x);
	lwlib_Edit_AddItem_int(edit, "y", &statObj->y);
	lwlib_Edit_AddItem_int(edit, "actor", &statObj->actor);
}

static void ED_FloorCeil_AddToEdit(void *elem, lwlib_Edit_t edit)
{
	ED_FloorCeil_t *floorCeil;
	floorCeil = (ED_FloorCeil_t *)elem;
	lwlib_Edit_AddItem_int(edit, "x", &floorCeil->x);
	lwlib_Edit_AddItem_int(edit, "y", &floorCeil->y);
	lwlib_Edit_AddItem_int(edit, "floor", &floorCeil->floor);
	lwlib_Edit_AddItem_int(edit, "ceil", &floorCeil->ceil);
}

static void ED_Map_AddToEdit(void *elem, lwlib_Edit_t edit)
{
	ED_Map_t *map;
	map = (ED_Map_t *)elem;
	lwlib_Edit_AddItem_int(edit, "index", &map->index);
	lwlib_Edit_AddVec(edit, "statObjs", map->statObjs,
		NULL, NULL, ED_StatObj_AddToEdit);
	lwlib_Edit_AddVec(edit, "floorCeils", map->floorCeils.vec,
		NULL, NULL, ED_FloorCeil_AddToEdit);
}

static void ED_InsertStatObjActorPair_AddToEdit(void *elem, lwlib_Edit_t edit)
{
	ED_InsertStatObjActorPair_t *insertStatObjActorPair;
	insertStatObjActorPair = (ED_InsertStatObjActorPair_t *)elem;
	lwlib_Edit_AddItem_enum(edit, "insertStatObj",
		(int *)&insertStatObjActorPair->insertStatObj, 
		lwlib_UseEnumStr(WR_InsertStatObj_t),
		NULL, lwlib_UseEnumStrCount(WR_InsertStatObj_t));
	lwlib_Edit_AddItem_int(edit, "actor", &insertStatObjActorPair->actor);
}

static void ED_ExtraStatInfo_AddToEdit(void *elem, lwlib_Edit_t edit)
{
	ED_ExtraStatInfo_t *extraStatInfo;
	extraStatInfo = (ED_ExtraStatInfo_t *)elem;
	lwlib_Edit_AddItem_int(edit, "actor", &extraStatInfo->actor);
	lwlib_Edit_AddItem_bool(edit, "blocking", &extraStatInfo->blocking);
	lwlib_Edit_AddItem_string(edit, "spriteFileName", 
		extraStatInfo->spriteFileName, 
		sizeof(extraStatInfo->spriteFileName));
	lwlib_Edit_AddItem_enum(edit, "radLightKey",
		(int *)&extraStatInfo->radLightKey, 
		lwlib_UseEnumStr(TRadLightKey),
		NULL, lwlib_UseEnumStrCount(TRadLightKey));
	lwlib_Edit_AddItem_bool(edit, "fullBright", 
		&extraStatInfo->fullBright);
	lwlib_Edit_AddItem_bool(edit, "dirEnable", 
		&extraStatInfo->dirEnable);
	lwlib_Edit_AddItem_bool(edit, "dirVertical", 
		&extraStatInfo->dirVertical);
	lwlib_Edit_AddItem_bool(edit, "blockLine", 
		&extraStatInfo->blockLine);
}

static void ED_AddToEdit(ED_t ed, lwlib_Edit_t edit)
{
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	lwlib_Edit_NavigateTop(edit);
	lwlib_Edit_AddPersistGroup(edit, &priv->persist);
	lwlib_Edit_AddVec(edit, "maps", priv->maps,
		NULL, NULL, ED_Map_AddToEdit);
	lwlib_Edit_AddVec(edit, "insertStatObjActorPairs", 
		priv->insertStatObjActorPairs,
		NULL, NULL, ED_InsertStatObjActorPair_AddToEdit);
	lwlib_Edit_AddVec(edit, "extraStatInfos", priv->extraStatInfos.vec,
		NULL, NULL, ED_ExtraStatInfo_AddToEdit);
	lwlib_Edit_AddItem_int(edit, "skyPic", &priv->skyPic);
}

static void ED_RecordTileToActor(ED_t ed, ED_Map_t *mapPtr,
	ED_StatObj_t *statObj)
{
	int spot;

	spot = (statObj->y << mapshift) + statObj->x;
	lwlib_IntMapInsert(&mapPtr->spotToActorMap, spot,
		statObj->actor);
}

static int ED_GetExtraStatInfoKey(ED_ExtraStatInfo_t *extraStatInfo)
{
	return extraStatInfo->actor;
}

static int ED_GetFloorCeilKey(ED_FloorCeil_t *floorCeil)
{
	return floorCeil->x + (floorCeil->y * MAPSIZE);
}

static void ED_DeriveData(ED_t ed)
{
	int i, j;
	ED_Map_t *mapPtr;
	ED_StatObj_t *statObjPtr;
	ED_ExtraStatInfo_t *extraStatInfo;
	SDL_Surface *surf;
	lwlib_String_t tmpStr;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	for (i = 0; i < vec_size(priv->maps); i++)
	{
		mapPtr = &vec_at(priv->maps, i);
		for (j = 0; j < vec_size(mapPtr->statObjs); j++)
		{
			statObjPtr = &vec_at(mapPtr->statObjs, j);
			ED_RecordTileToActor(ed, mapPtr, statObjPtr);
		}

		hvec_init_keys(mapPtr->floorCeils, ED_FloorCeil_t, 
			ED_GetFloorCeilKey);
	}

	for (i = 0; i < hvec_size(priv->extraStatInfos); i++)
	{
		extraStatInfo = &hvec_at(priv->extraStatInfos, i);
		surf = SDL_LoadBMP(lwlib_VaString(tmpStr, "sprites/%s.bmp", 
			extraStatInfo->spriteFileName));
		if (surf != NULL)
		{
			if (surf->w == TEXTURESIZE && 
				surf->h == TEXTURESIZE && surf->format->BytesPerPixel == 1)
			{
				extraStatInfo->shape = BuildSprite(surf);
			}
			SDL_FreeSurface(surf);
		}
	}

	hvec_init_keys(priv->extraStatInfos, ED_ExtraStatInfo_t, 
		ED_GetExtraStatInfoKey);
}

static void ED_Load(ED_t ed)
{
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	lwlib_StrCopy(priv->persist.fileName, ED_STATEFILE);
	priv->persist.fileMode = lwlib_Fs_FileMode(ReadText);
	priv->persist.commit = true;

	lwlib_Edit_CheckPersist(priv->persist, 
		ED_AddToEdit(ed, fsEdit));
}

static void ED_Save(ED_t ed)
{
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	lwlib_StrCopy(priv->persist.fileName, ED_STATEFILE);
	priv->persist.fileMode = lwlib_Fs_FileMode(WriteText);
	priv->persist.commit = true;

	lwlib_Edit_CheckPersist(priv->persist, 
		ED_AddToEdit(ed, fsEdit));
}

static ED_Map_t *ED_FindCurrentMap(ED_t ed)
{
	int i;
	ED_Map_t *mapPtr;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	for (i = 0; i < vec_size(priv->maps); i++)
	{
		mapPtr = &vec_at(priv->maps, i);
		if (mapPtr->index == mapon + 10 * gamestate.episode)
		{
			return mapPtr;
		}
	}

	return NULL;
}

ED_t ED_New(void)
{
	ED_t ed;

	lwlib_Zero(ed);
	ed.priv = lwlib_CallocSingle(ED_Priv_t);

	lwlib_GetPriv(ED_Priv_t, ed);
	priv->edit = lwlib_Edit_New();

	ED_Load(ed);
	ED_DeriveData(ed);

	return ed;
}

ED_t ED_Free(ED_t ed)
{
	int i;
	ED_Map_t *mapPtr;
	lwlib_GetPriv(ED_Priv_t, ed);

	if (priv)
	{
		hvec_free(priv->extraStatInfos, ED_ExtraStatInfo_t);
		vec_free(priv->insertStatObjActorPairs, 
			ED_InsertStatObjActorPair_t);
		for (i = 0; i < vec_size(priv->maps); i++)
		{
			mapPtr = &vec_at(priv->maps, i);
			hvec_free(mapPtr->floorCeils, ED_FloorCeil_t);
			vec_free(mapPtr->statObjs, ED_StatObj_t);
			lwlib_IntMapClear(&mapPtr->spotToActorMap);
		}
		vec_free(priv->maps, ED_Map_t);
		lwlib_Edit_Free(priv->edit);
		free(priv);
	}

	lwlib_Zero(ed);
	return ed;
}

void ED_ProcessInsertStatObj(ED_t ed, int insertStatObj)
{
	int i;
	ED_Map_t map;
	ED_Map_t *mapPtr;
	ED_StatObj_t statObj;
	statobj_t *check;
	int picNumMap[WR_INSERTSTATOBJ_MAX];
	ED_InsertStatObjActorPair_t *insertStatObjActorPair;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	for (i = 0; i < vec_size(priv->insertStatObjActorPairs); i++)
	{
		insertStatObjActorPair = &vec_at(priv->insertStatObjActorPairs, i);
		if (insertStatObjActorPair->insertStatObj >= 0 &&
			insertStatObjActorPair->insertStatObj < WR_INSERTSTATOBJ_MAX)
		{
			picNumMap[insertStatObjActorPair->insertStatObj] = 
				insertStatObjActorPair->actor;
		}
	}

	mapPtr = ED_FindCurrentMap(ed);
	if (mapPtr == NULL)
	{
		lwlib_Zero(map);
		map.index = mapon;
		vec_push(priv->maps, ED_Map_t, map);
		mapPtr = &vec_top(priv->maps);
	}

	lwlib_Zero(statObj);
	statObj.x = player->tilex;
	statObj.y = player->tiley;
	statObj.actor = picNumMap[insertStatObj];

	vec_push(mapPtr->statObjs, ED_StatObj_t, statObj);
	ED_RecordTileToActor(ed, mapPtr, &statObj);

	if (statObj.actor != 0)
	{
		SpawnStatic(statObj.x, statObj.y, statObj.actor - 23, statObj.actor);
	}
	else
	{
		check = statat[statObj.x][statObj.y];
		if (check != NULL)
		{
			check->shapenum = -1;
			statat[check->tilex][check->tiley] = NULL;
			actorat[check->tilex][check->tiley] = NULL;
		}
	}

	ED_Save(ed);
}

void ED_ProcessInsertFloorCeil(ED_t ed, int insertFloorCeil)
{
	int i;
	ED_Map_t map;
	ED_Map_t *mapPtr;
	ED_FloorCeil_t floorCeil;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	mapPtr = ED_FindCurrentMap(ed);
	if (mapPtr == NULL)
	{
		lwlib_Zero(map);
		map.index = mapon;
		vec_push(priv->maps, ED_Map_t, map);
		mapPtr = &vec_top(priv->maps);
	}

	lwlib_Zero(floorCeil);
	floorCeil.x = player->tilex;
	floorCeil.y = player->tiley;
	floorCeil.ceil = insertFloorCeil + 1;

	hvec_push_key(mapPtr->floorCeils, ED_FloorCeil_t, floorCeil,
		ED_GetFloorCeilKey(&floorCeil));
	MAPSPOT(floorCeil.x, floorCeil.y, 2) = floorCeil.floor + 
		(floorCeil.ceil << 8);

	ED_Save(ed);
}

int ED_CheckInsertStatObj(ED_t ed, int x, int y, int tile)
{
	int spot;
	ED_Map_t *mapPtr;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	mapPtr = ED_FindCurrentMap(ed);
	if (mapPtr != NULL)
	{
		spot = (y << mapshift) + x;
		if (lwlib_IntMapContains(&mapPtr->spotToActorMap, spot))
		{
			tile = (int)lwlib_IntMapElem(&mapPtr->spotToActorMap, spot);
		}
	}

	return tile;
}

bool ED_SpawnActor(ED_t ed, int tile, int x, int y)
{
	ED_ExtraStatInfo_t *extraStatInfo;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	extraStatInfo = hvec_pval(priv->extraStatInfos, tile);
	if (extraStatInfo != NULL)
	{
		laststatobj->shapenum = 0;
		laststatobj->sprite = (byte *)extraStatInfo->shape;
		laststatobj->tilex = x;
		laststatobj->tiley = y;
		laststatobj->visspot = &spotvis[x][y];
		laststatobj->radLightKey = extraStatInfo->radLightKey;

		if (extraStatInfo->blocking)
		{
			actorat[x][y] = (objtype *)BLOCKINGTILE;
		}

		laststatobj->flags = 0;
		if (extraStatInfo->dirEnable)
		{
			laststatobj->flags |= 
				(extraStatInfo->dirVertical ? 
				FL_DIR_VERT_MID : FL_DIR_HORIZ_MID);
		}
		if (extraStatInfo->fullBright)
		{
			laststatobj->flags |= FL_FULLBRIGHT;
		}
		if (extraStatInfo->blockLine)
		{
			laststatobj->flags |= FL_BLOCKLINE;
		}

		statat[x][y] = laststatobj;
		laststatobj++;

		if (laststatobj == &statobjlist[MAXSTATS])
			Quit ("Too many static objects!\n");

		return true;
	}

	return false;
}

int ED_CheckFloorCeil(ED_t ed, int x, int y, int spot)
{
	ED_Map_t *mapPtr;
	ED_FloorCeil_t *floorCeil;
	lwlib_GetPrivAssert(ED_Priv_t, ed);

	mapPtr = ED_FindCurrentMap(ed);
	if (mapPtr == NULL)
	{
		return spot;
	}

	floorCeil = hvec_pval(mapPtr->floorCeils, x + y * MAPSIZE);
	if (floorCeil != NULL)
	{
		spot = floorCeil->floor + (floorCeil->ceil << 8);
	}

	return spot;
}

bool ED_WallPicIsSky(ED_t ed, int wallpic)
{
	lwlib_GetPrivAssert(ED_Priv_t, ed);
	return priv->skyPic == wallpic;
}
#endif