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

#include <stdio.h>
#include <stdlib.h>
#include "wr_room.h"
#include "wr_scene.h"
#include "wr_lightmap.h"
#include "wr_radmap.h"
#ifdef PS2
//#include "3rdparty/lw_lib/lw_vec.h"
#else
#include "../lw_lib/lw_img.h"
#endif
#ifdef WOLFRAD
#include "..\..\wl_def.h"

typedef struct WR_Scene_Priv_s
{
	TSceneParams sceneParams;
	WR_Rad_t rad;
	WR_Level_t level;
	lwlib_Bres_t bres;
} WR_Scene_Priv_t;

lwlib_EnumStr(
	TRadLightKey,
	"None",
	"Floor Lamp",
	"Chandelier",
	"Ceiling Light",
	"Light Bulb",
	"Light Bulb Bright",
	"Sky Light",
	"Fluorescent Panel",
	);

static void WR_Scene_InitQuad(WR_Scene_t scene, TQuad * quad, 
	lwlib_Byte_t *postsource, int postoffu, int postoffv,
	lwlib_TPoint3i pos, int qn, int doorNum, bool isSky)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	memset(quad, 0, sizeof(TQuad));
	quad->postsource = postsource;
	quad->postoffu = postoffu;
	quad->postoffv = postoffv;
	quad->postdimu = quad->postdimv = TEXTURESIZE;
	quad->pos = pos;
	quad->qn = qn;
	quad->dims[0] = 1.0;
	quad->dims[1] = 1.0;
	quad->area = quad->dims[0] * quad->dims[1];
	quad->patchLevel[0] = quad->patchLevel[1] = rad_params.patchLevel;
	quad->elementLevel[0] = quad->elementLevel[1] = rad_params.elementLevel;
	quad->nu = quad->patchLevel[0] * quad->elementLevel[0];
	quad->nv = quad->patchLevel[1] * quad->elementLevel[1];
	quad->elementArea = quad->area / (quad->nu * quad->nv);
	quad->reflectance = lwlib_vec3f(1.0, 1.0, 1.0);
	quad->color = lwlib_vec3f(1.0, 1.0, 1.0);
	quad->doorNum = doorNum;
	quad->isSky = isSky;
}

static void WR_Scene_AddQuad(WR_Scene_t scene, TQuad quad)
{
	doorobj_t *doorobj;
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);

	if (quad.doorNum > 0)
	{
		doorobj = &doorobjlist[quad.doorNum - 1];
		if (doorobj->quadIndexCount < 
			lwlib_CountArray(doorobj->quadIndexArr))
		{
			doorobj->quadIndexArr[doorobj->quadIndexCount++] = 
				vec_size(rad_params.quads);
		}
	}

	vec_push(rad_params.quads, TQuad, quad);
	if (quad.qn < WR_LEVEL_QN_TOT)
	{
		WR_Level_GetTile(priv->level, quad.pos)->
			quad_lookup[quad.qn] = vec_size(rad_params.quads);
	}
}

static void WR_Scene_UprightQuad(WR_Scene_t scene, 
	double x1, double y1, double x2, double y2, 
	lwlib_Byte_t *postsource, int postoffu, int postoffv, 
	lwlib_TPoint3i pos, int qn, int doorNum, bool isSky)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	TQuad quad;
	lwlib_Img_t *texture;

	WR_Scene_InitQuad(scene, &quad, postsource, postoffu, postoffv,
		pos, qn, doorNum, isSky);

	quad.dims[1] /= 2;
	quad.area /= 2;
	quad.patchLevel[1] /= 2;
	quad.nv /= 2;
	quad.postdimv /= 2;
	quad.halfsize[1] = 1;

	lwlib_InitVector(quad.verts[0], x1, y1, 0.0);
	lwlib_InitVector(quad.verts[1], x2, y2, 0.0);
	lwlib_InitVector(quad.verts[2], x2, y2, 1.0);
	lwlib_InitVector(quad.verts[3], x1, y1, 1.0);

	WR_Scene_AddQuad(scene, quad);
}

static void WR_Scene_FlatQuad(WR_Scene_t scene, double x,
	double y, bool floor, lwlib_Byte_t *postsource,
	int postoffu, int postoffv, lwlib_TPoint3i pos, int qn)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	TQuad quad;
	double z;

	WR_Scene_InitQuad(scene, &quad, postsource, postoffu, postoffv,
		pos, qn, 0, false);

	quad.dims[0] = quad.dims[1] = 0.5;
	quad.area = quad.dims[0] * quad.dims[1];

	quad.patchLevel[0] /= 2;
	quad.patchLevel[1] /= 2;
	quad.nu /= 2;
	quad.nv /= 2;
	quad.postdimu /= 2;
	quad.postdimv /= 2;
	quad.halfsize[0] = 1;
	quad.halfsize[1] = 1;

	if (floor)
	{
		z = 0.0;
		lwlib_InitVector(quad.verts[0], x, y, z);
		lwlib_InitVector(quad.verts[1], x + 0.5, y, z);
		lwlib_InitVector(quad.verts[2], x + 0.5, y + 0.5, z);
		lwlib_InitVector(quad.verts[3], x, y + 0.5, z);
	}
	else // ceiling
	{
		z = 1.0;
		lwlib_InitVector(quad.verts[0], x, y + 0.5, z);
		lwlib_InitVector(quad.verts[1], x + 0.5, y + 0.5, z);
		lwlib_InitVector(quad.verts[2], x + 0.5, y, z);
		lwlib_InitVector(quad.verts[3], x, y, z);
	}

	WR_Scene_AddQuad(scene, quad);
}


static void WR_Scene_RadLight(WR_Scene_t scene, double x, double y,
	TRadLight radLight, lwlib_TPoint3i pos, int qn, int lightNum,
	int tileRadius)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	TQuad quad;
	double hsize;
	double z;
	int side;
	lwlib_TPoint3i tilePos;

	hsize = radLight.size / 2.0;
	z = radLight.elevation;

	WR_Scene_InitQuad(scene, &quad, NULL, 0, 0, pos, qn, 0, false);
	if (radLight.areaSize != 0.0)
	{
		quad.area = radLight.areaSize * radLight.areaSize;
	}
	else
	{
		quad.area = radLight.size * radLight.size;
	}
	quad.patchLevel[0] = quad.patchLevel[1] = radLight.patchLevel;
	quad.elementLevel[0] = quad.elementLevel[1] = radLight.elementLevel;
	quad.nu = quad.patchLevel[0] * quad.elementLevel[0];
	quad.nv = quad.patchLevel[1] * quad.elementLevel[1];
	quad.emission = radLight.emission;
	quad.lightNum = lightNum + 1;
	quad.lightTileRadius = tileRadius;

	quad.cone = radLight.cone;
	quad.cone.vertex = lwlib_vec3f_add(quad.cone.vertex, 
		lwlib_vec3f(x, y, 0));

	for (side = 0; side < 6; side++)
	{
		if (side == 0)
		{
			lwlib_InitVector(quad.verts[0],
				x - hsize, y + hsize, z - hsize);
			lwlib_InitVector(quad.verts[1],
				x - hsize, y - hsize, z - hsize);
			lwlib_InitVector(quad.verts[2],
				x - hsize, y - hsize, z + hsize);
			lwlib_InitVector(quad.verts[3],
				x - hsize, y + hsize, z + hsize);
		}
		else if (side == 1)
		{
			lwlib_InitVector(quad.verts[0],
				x + hsize, y - hsize, z - hsize);
			lwlib_InitVector(quad.verts[1],
				x + hsize, y + hsize, z - hsize);
			lwlib_InitVector(quad.verts[2],
				x + hsize, y + hsize, z + hsize);
			lwlib_InitVector(quad.verts[3],
				x + hsize, y - hsize, z + hsize);
		}
		else if (side == 2)
		{
			lwlib_InitVector(quad.verts[0],
				x - hsize, y - hsize, z - hsize);
			lwlib_InitVector(quad.verts[1],
				x + hsize, y - hsize, z - hsize);
			lwlib_InitVector(quad.verts[2],
				x + hsize, y - hsize, z + hsize);
			lwlib_InitVector(quad.verts[3],
				x - hsize, y - hsize, z + hsize);
		}
		else if (side == 3)
		{
			lwlib_InitVector(quad.verts[0],
				x + hsize, y + hsize, z - hsize);
			lwlib_InitVector(quad.verts[1],
				x - hsize, y + hsize, z - hsize);
			lwlib_InitVector(quad.verts[2],
				x - hsize, y + hsize, z + hsize);
			lwlib_InitVector(quad.verts[3],
				x + hsize, y + hsize, z + hsize);
		}
		else if (side == 4)
		{
			lwlib_InitVector(quad.verts[0],
				x - hsize, y + hsize, z - hsize);
			lwlib_InitVector(quad.verts[1],
				x + hsize, y + hsize, z - hsize);
			lwlib_InitVector(quad.verts[2],
				x + hsize, y - hsize, z - hsize);
			lwlib_InitVector(quad.verts[3],
				x - hsize, y - hsize, z - hsize);
		}
		else if (side == 5)
		{
			lwlib_InitVector(quad.verts[0],
				x - hsize, y - hsize, z + hsize);
			lwlib_InitVector(quad.verts[1],
				x + hsize, y - hsize, z + hsize);
			lwlib_InitVector(quad.verts[2],
				x + hsize, y + hsize, z + hsize);
			lwlib_InitVector(quad.verts[3],
				x - hsize, y + hsize, z + hsize);
		}

		WR_Scene_AddQuad(scene, quad);
	}
}

static TLightProfile *WR_Scene_FindLightProfile(WR_Scene_t scene, 
	int radLightKey)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i;
	TLightProfile *lp;

	for (i = 0; i < vec_size(priv->sceneParams.lightProfiles); i++)
	{
		lp = &vec_at(priv->sceneParams.lightProfiles, i);
		if (radLightKey != -1 && lp->radLightKey == radLightKey)
		{
			return lp;
		}
	}

	return NULL;
}

WR_Scene_t WR_Scene_New(void)
{
	WR_Scene_t scene;
	lwlib_Zero(scene);

	lwlib_GetPriv(WR_Scene_Priv_t, scene);
	priv = lwlib_CallocSingle(WR_Scene_Priv_t);

	scene.priv = priv;
	return scene;
}

WR_Scene_t WR_Scene_Free(WR_Scene_t scene)
{
	lwlib_GetPriv(WR_Scene_Priv_t, scene);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(scene);
	return scene;
}

void WR_Scene_SetParams(WR_Scene_t scene, TSceneParams sceneParams)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	WR_Scene_ResetParams(scene);
	priv->sceneParams = sceneParams;
}

void WR_Scene_ResetParams(WR_Scene_t scene)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i;
	TLightProfile *lp;

	for (i = 0; i < vec_size(priv->sceneParams.lightProfiles); i++)
	{
		lp = &vec_at(priv->sceneParams.lightProfiles, i);
		vec_free(lp->radLights, TRadLight);
	}

	vec_free(priv->sceneParams.lightProfiles, TLightProfile);
	lwlib_Zero(priv->sceneParams);
}

static void WR_Scene_Generate_RadQuads(WR_Scene_t scene, 
	lwlib_TPoint3i pos, double x, double y, TLightProfile *lp,
	int lightNum)
{
	int i;
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);

	for (i = 0; i < vec_size(lp->radLights); i++)
	{
		WR_Scene_RadLight(scene, x, y, vec_at(lp->radLights, i), pos,
			WR_LEVEL_QN_TOT, lightNum, lp->tileRadius);
	}
}

static byte *AllocTex(byte *tex)
{
	const int size = TEXTURESIZE * TEXTURESIZE;
	byte *allocTex;

	if (tex == NULL)
	{
		return NULL;
	}

	allocTex = lwlib_CallocMany(byte, size);
	memcpy(allocTex, tex, size);

	return allocTex;
}

static lwlib_Byte_t *FloorCeilTex(int x, int y, int floor)
{
	lwlib_Byte_t *tex;

	int i;
	byte ceiling;
	static byte texdata[TEXTUREPIXELS];
	lwlib_Byte_t *patchtex;
	SDL_Surface *surf;
	int spot;
	int floortex, ceiltex;
	lwlib_String_t tmpStr;

	spot = MAPSPOT(x, 64 - 1 - y, 2);
	spot = ED_CheckFloorCeil(ed, x, 64 - 1 - y, spot);

	if (floor)
	{
		memset(texdata, 0x19, TEXTUREPIXELS);

		floortex = spot & 0xff;
		surf = SDL_LoadBMP(lwlib_VaString(tmpStr, "textures/floor/%d.bmp", 
			floortex));
	}
	else
	{
		ceiling = vgaCeiling[gamestate.episode * 10 + mapon];
		memset(texdata, ceiling, TEXTUREPIXELS);

		ceiltex = (spot >> 8) & 0xff;
		surf = SDL_LoadBMP(lwlib_VaString(tmpStr, 
			"textures/ceiling/%d.bmp", ceiltex));
	}

	tex = texdata;

	if (surf != NULL)
	{
		if (surf->w == TEXTURESIZE && 
			surf->h == TEXTURESIZE && surf->format->BytesPerPixel == 1)
		{
			SDL_LockSurface(surf);
			patchtex = (byte *)surf->pixels;
			for (i = 0; i < TEXTURESIZE * TEXTURESIZE; i++)
			{
				if (patchtex[i] != 0xff)
				{
					tex[i] = patchtex[i];
				}
			}
			SDL_UnlockSurface(surf);
		}
		SDL_FreeSurface(surf);
	}

	return AllocTex(tex);
}

static lwlib_Byte_t *DoorTex(int x, int y, int lock, door_face_t face,
	door_orient_t orient)
{
	lwlib_Byte_t *tex;

	tex = PM_GetTexture(
		DOORPAGE_ABSOLUTE(
			doorlock_to_doorpage[lock][face]
			) + orient
		);
	if (tex == NULL)
	{
		return NULL;
	}

	return AllocTex(tex);
}

static lwlib_Byte_t *WallTex(int x, int y, bool vert, bool pwall,
	bool *isSky)
{
	int wallpic;
	int tile;
	lwlib_Byte_t *tex;

	if (isSky != NULL)
	{
		*isSky = false;
	}

	if (x < 0 || x >= 64 || y < 0 || y >= 64)
	{
		return NULL;
	}

	tile = tilemap[x][64 - 1 - y];
	if (tile == 0 || (tile & 0x80) != 0 || 
		(!pwall && MAPSPOT(x, 64 - 1 - y, 1) == PUSHABLETILE))
	{
		return NULL;
	}
	tile &= ~0x40;

	wallpic = vert ? vertwall[tile] : horizwall[tile];
	if (isSky != NULL && ED_WallPicIsSky(ed, wallpic))
	{
		*isSky = true;
	}

	tex = PM_GetTexture(wallpic);
	if (tex == NULL)
	{
		return NULL;
	}
	tex = AllocTex(tex);

	return tex;
}

void WR_Scene_Generate_Quads(WR_Scene_t scene)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i, j;
	int x, y;
	int xl, yl;
	TLightProfile *lp;
	int lightNum;
	int tileStatObjNum;
	lwlib_TPoint3f radLightPoint;
	int doorNum;
	TLight light;
	doorobj_t *doorobj;
	int tile;
	bool isSky;
	statobj_t *statobj;
	lwlib_Byte_t *tex;
	bool addFloorCeil, addLeftWall, addRightWall, 
		addTopWall, addBottomWall;
	lwlib_Byte_t shapeImage[TEXTURESIZE * TEXTURESIZE];
	lwlib_Byte_t flippedShapeImage[TEXTURESIZE * TEXTURESIZE];
	int spot;
	int ceiltex;

	/* generate quads for walls and doors */
	for (x = 0; x < 64; x++)
	{
		if (WolfRad_Cancelled(wolfRad))
		{
			return;
		}

		for (y = 0; y < 64; y++)
		{
			tile = tilemap[x][64 - 1 - y];

			addFloorCeil = addLeftWall = addRightWall = 
				addTopWall = addBottomWall = false;
			if ((tile & 0x80) != 0) // door
			{
				doorobj = &doorobjlist[tile & 0x7f];
				if (doorobj->vertical)
				{
					// front and back
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 1.0,
						x + 0.5, y + 0.5,
						DoorTex(x, y, doorobj->lock, 
							dr_face_front, dr_orient_vert),
						0, 0,
						lwlib_vec3i(x * 2, y * 2 + 1, 0),
						WR_LEVEL_QN_EASTWALL,
						(tile & 0x7f) + 1,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 0.5,
						x + 0.5, y,
						DoorTex(x, y, doorobj->lock,
							dr_face_front, dr_orient_vert),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2, y * 2, 0),
						WR_LEVEL_QN_EASTWALL,
						(tile & 0x7f) + 1,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y,
						x + 0.5, y + 0.5,
						DoorTex(x, y, doorobj->lock,
							dr_face_front, dr_orient_vert),
						0, 0,
						lwlib_vec3i(x * 2 + 1, y * 2, 0),
						WR_LEVEL_QN_WESTWALL,
						(tile & 0x7f) + 1,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 0.5,
						x + 0.5, y + 1.0,
						DoorTex(x, y, doorobj->lock,
							dr_face_front, dr_orient_vert),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
						WR_LEVEL_QN_WESTWALL,
						(tile & 0x7f) + 1,
						false
						);
					// sides
					WR_Scene_UprightQuad(
						scene, 
						x, y + 1.0,
						x + 0.5, y + 1.0,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_vert),
						0, 0,
						lwlib_vec3i(x * 2, y * 2 + 1, 0),
						WR_LEVEL_QN_NORTHWALL,
						0,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 1.0,
						x + 1.0, y + 1.0,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_vert),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
						WR_LEVEL_QN_NORTHWALL,
						0,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y,
						x, y,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_vert),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2, y * 2, 0),
						WR_LEVEL_QN_SOUTHWALL,
						0,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y,
						x + 0.5, y,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_vert),
						0, 0,
						lwlib_vec3i(x * 2 + 1, y * 2, 0),
						WR_LEVEL_QN_SOUTHWALL,
						0,
						false
						);
					// walls
					addLeftWall = true;
					addRightWall = true;
				}
				else // !doorobj->vertical
				{
					// front and back
					WR_Scene_UprightQuad(
						scene, 
						x, y + 0.5,
						x + 0.5, y + 0.5,
						DoorTex(x, y, doorobj->lock, 
							dr_face_front, dr_orient_horz),
						0, 0,
						lwlib_vec3i(x * 2, y * 2, 0),
						WR_LEVEL_QN_NORTHWALL,
						(tile & 0x7f) + 1,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 0.5,
						x + 1.0, y + 0.5,
						DoorTex(x, y, doorobj->lock, 
							dr_face_front, dr_orient_horz),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2 + 1, y * 2, 0),
						WR_LEVEL_QN_NORTHWALL,
						(tile & 0x7f) + 1,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y + 0.5,
						x + 0.5, y + 0.5,
						DoorTex(x, y, doorobj->lock, 
							dr_face_front, dr_orient_horz),
						0, 0,
						lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
						WR_LEVEL_QN_SOUTHWALL,
						(tile & 0x7f) + 1,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 0.5,
						x, y + 0.5,
						DoorTex(x, y, doorobj->lock, 
							dr_face_front, dr_orient_horz),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2, y * 2 + 1, 0),
						WR_LEVEL_QN_SOUTHWALL,
						(tile & 0x7f) + 1,
						false
						);
					// sides
					WR_Scene_UprightQuad(
						scene, 
						x, y,
						x, y + 0.5,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_horz),
						0, 0,
						lwlib_vec3i(x * 2, y * 2, 0),
						WR_LEVEL_QN_WESTWALL,
						0,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x, y + 0.5,
						x, y + 1.0,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_horz),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2, y * 2 + 1, 0),
						WR_LEVEL_QN_WESTWALL,
						0,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y + 1.0,
						x + 1.0, y + 0.5,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_horz),
						0, 0,
						lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
						WR_LEVEL_QN_EASTWALL,
						0,
						false
						);
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y + 0.5,
						x + 1.0, y,
						DoorTex(x, y, doorobj->lock,
							dr_face_side, dr_orient_horz),
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2 + 1, y * 2, 0),
						WR_LEVEL_QN_EASTWALL,
						0,
						false
						);
					// walls
					addTopWall = true;
					addBottomWall = true;
				}

				addFloorCeil = true;
			}
			// wall
			else if (tile != 0 && MAPSPOT(x, 64 - 1 - y, 1) != PUSHABLETILE)
			{
			}
			// empty tile or push wall
			else
			{
				addFloorCeil = addLeftWall = addRightWall = 
					addTopWall = addBottomWall = true;
			}

			if (addLeftWall)
			{
				tex = WallTex(x - 1, y, true, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x, y,
						x, y + 0.5,
						tex,
						0, 0,
						lwlib_vec3i(x * 2, y * 2, 0),
						WR_LEVEL_QN_WESTWALL,
						0,
						isSky
						);
				}
				tex = WallTex(x - 1, y, true, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x, y + 0.5,
						x, y + 1.0,
						tex,
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2, y * 2 + 1, 0),
						WR_LEVEL_QN_WESTWALL,
						0,
						isSky
						);
				}
			}

			if (addRightWall)
			{
				tex = WallTex(x + 1, y, true, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y + 1.0,
						x + 1.0, y + 0.5,
						tex,
						0, 0,
						lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
						WR_LEVEL_QN_EASTWALL,
						0,
						isSky
						);
				}
				tex = WallTex(x + 1, y, true, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y + 0.5,
						x + 1.0, y,
						tex,
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2 + 1, y * 2, 0),
						WR_LEVEL_QN_EASTWALL,
						0,
						isSky
						);
				}
			}

			if (addBottomWall)
			{
				tex = WallTex(x, y - 1, false, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x + 1.0, y,
						x + 0.5, y,
						tex,
						0, 0,
						lwlib_vec3i(x * 2 + 1, y * 2, 0),
						WR_LEVEL_QN_SOUTHWALL,
						0,
						isSky
						);
				}
				tex = WallTex(x, y - 1, false, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y,
						x, y,
						tex,
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2, y * 2, 0),
						WR_LEVEL_QN_SOUTHWALL,
						0,
						isSky
						);
				}
			}

			if (addTopWall)
			{
				tex = WallTex(x, y + 1, false, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x, y + 1.0,
						x + 0.5, y + 1.0,
						tex,
						0, 0,
						lwlib_vec3i(x * 2, y * 2 + 1, 0),
						WR_LEVEL_QN_NORTHWALL,
						0,
						isSky
						);
				}
				tex = WallTex(x, y + 1, false, false, &isSky);
				if (tex != NULL)
				{
					WR_Scene_UprightQuad(
						scene, 
						x + 0.5, y + 1.0,
						x + 1.0, y + 1.0,
						tex,
						0, TEXTURESIZE / 2,
						lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
						WR_LEVEL_QN_NORTHWALL,
						0,
						isSky
						);
				}
			}

			if (addFloorCeil)
			{
				for (i = 0; i < 8; i++)
				{
					xl = ((i % 4) % 2);
					yl = ((i % 4) / 2);

					tex = FloorCeilTex(
						x, y, i < 4);
					if (tex != NULL)
					{
						WR_Scene_FlatQuad(
							scene,
							x + xl * 0.5,
							y + yl * 0.5,
							i < 4,
							tex,
							yl * TEXTURESIZE / 2,
							xl * TEXTURESIZE / 2,
							lwlib_vec3i(
								x * 2 + xl,
								y * 2 + yl,
								0
								),
							i < 4 ? WR_LEVEL_QN_FLOOR : 
								WR_LEVEL_QN_CEILING
							);
					}
				}
			}
		}
	}

	/* generate quads for push wall doors */
	for (i = 0; i < doorcount; i++)
	{
		if (WolfRad_Cancelled(wolfRad))
		{
			return;
		}

		doorNum = i;
		doorobj = &doorobjlist[doorNum];
		if (doorobj->lock != dr_pwdoor)
		{
			continue;
		}

		x = doorobj->tilex;
		y = 64 - 1 - doorobj->tiley;

		if (lwlib_BitOn(doorobj->faces, pwdoorfacedireast))
		{
			tex = WallTex(doorobj->start_tilex, 
				64 - 1 - doorobj->start_tiley,
				true, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x + 1.0, y,
					x + 1.0, y + 0.5,
					tex,
					0, 0,
					lwlib_vec3i((x + 1) * 2, y * 2, 0),
					WR_LEVEL_QN_WESTWALL,
					doorNum + 1,
					false
					);
			}
			tex = WallTex(doorobj->start_tilex,
				64 - 1 - doorobj->start_tiley,
				true, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x + 1.0, y + 0.5,
					x + 1.0, y + 1.0,
					tex,
					0, TEXTURESIZE / 2,
					lwlib_vec3i((x + 1) * 2, y * 2 + 1, 0),
					WR_LEVEL_QN_WESTWALL,
					doorNum + 1,
					false
					);
			}
		}
		if (lwlib_BitOn(doorobj->faces, pwdoorfacedirwest))
		{
			tex = WallTex(doorobj->start_tilex, 
				64 - 1 - doorobj->start_tiley,
				true, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x, y + 1.0,
					x, y + 0.5,
					tex,
					0, 0,
					lwlib_vec3i((x - 1) * 2 + 1, y * 2 + 1, 0),
					WR_LEVEL_QN_EASTWALL,
					doorNum + 1,
					false
					);
			}
			tex = WallTex(doorobj->start_tilex,
				64 - 1 - doorobj->start_tiley,
				true, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x, y + 0.5,
					x, y,
					tex,
					0, TEXTURESIZE / 2,
					lwlib_vec3i((x - 1) * 2 + 1, y * 2, 0),
					WR_LEVEL_QN_EASTWALL,
					doorNum + 1,
					false
					);
			}
		}
		if (lwlib_BitOn(doorobj->faces, pwdoorfacedirnorth))
		{
			tex = WallTex(doorobj->start_tilex,
				64 - 1 - doorobj->start_tiley,
				false, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x + 1.0, y + 1.0,
					x + 0.5, y + 1.0,
					tex,
					0, 0,
					lwlib_vec3i(x * 2 + 1, (y + 1) * 2, 0),
					WR_LEVEL_QN_SOUTHWALL,
					doorNum + 1,
					false
					);
			}
			tex = WallTex(doorobj->start_tilex,
				64 - 1 - doorobj->start_tiley,
				false, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y + 1.0,
					x, y + 1.0,
					tex,
					0, TEXTURESIZE / 2,
					lwlib_vec3i(x * 2, (y + 1) * 2, 0),
					WR_LEVEL_QN_SOUTHWALL,
					doorNum + 1,
					false
					);
			}
		}
		if (lwlib_BitOn(doorobj->faces, pwdoorfacedirsouth))
		{
			tex = WallTex(doorobj->start_tilex,
				64 - 1 - doorobj->start_tiley,
				false, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x, y,
					x + 0.5, y,
					tex,
					0, 0,
					lwlib_vec3i(x * 2, (y - 1) * 2 + 1, 0),
					WR_LEVEL_QN_NORTHWALL,
					doorNum + 1,
					false
					);
			}
			tex = WallTex(doorobj->start_tilex,
				64 - 1 - doorobj->start_tiley,
				false, true, NULL);
			if (tex != NULL)
			{
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y,
					x + 1.0, y,
					tex,
					0, TEXTURESIZE / 2,
					lwlib_vec3i(x * 2 + 1, (y - 1) * 2 + 1, 0),
					WR_LEVEL_QN_NORTHWALL,
					doorNum + 1,
					false
					);
			}
		}
	}

	/* generate quads for directional scenery */
	for (x = 0; x < 64; x++)
	{
		if (WolfRad_Cancelled(wolfRad))
		{
			return;
		}

		for (y = 0; y < 64; y++)
		{
			statobj = statat[x][64 - 1 - y];
			if (statobj == NULL || 
				((statobj->flags & FL_DIR_POS_MID) == 0) ||
				(statobj->shapenum <= 0 && statobj->sprite == NULL))
			{
				continue;
			}

			ShapeToImage(statobj->shapenum, shapeImage, false, 
				(t_compshape *)statobj->sprite);
			ShapeToImage(statobj->shapenum, flippedShapeImage, true, 
				(t_compshape *)statobj->sprite);

			if ((statobj->flags & FL_DIR_VERT_FLAG) != 0)
			{
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y,
					x + 0.5, y + 0.5,
					AllocTex(shapeImage),
					0, 0,
					lwlib_vec3i(x * 2 + 1, y * 2, 0),
					WR_LEVEL_QN_WESTWALL,
					0,
					false
					);
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y + 0.5,
					x + 0.5, y + 1.0,
					AllocTex(shapeImage),
					0, TEXTURESIZE / 2,
					lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
					WR_LEVEL_QN_WESTWALL,
					0,
					false
					);
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y + 1.0,
					x + 0.5, y + 0.5,
					AllocTex(flippedShapeImage),
					0, 0,
					lwlib_vec3i(x * 2, y * 2 + 1, 0),
					WR_LEVEL_QN_EASTWALL,
					0,
					false
					);
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y + 0.5,
					x + 0.5, y,
					AllocTex(flippedShapeImage),
					0, TEXTURESIZE / 2,
					lwlib_vec3i(x * 2, y * 2, 0),
					WR_LEVEL_QN_EASTWALL,
					0,
					false
					);
			}
			else // (statobj->flags & FL_DIR_VERT_FLAG) == 0
			{
				WR_Scene_UprightQuad(
					scene, 
					x, y + 0.5,
					x + 0.5, y + 0.5,
					AllocTex(shapeImage),
					0, 0,
					lwlib_vec3i(x * 2, y * 2, 0),
					WR_LEVEL_QN_NORTHWALL,
					0,
					false
					);
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y + 0.5,
					x + 1.0, y + 0.5,
					AllocTex(shapeImage),
					0, TEXTURESIZE / 2,
					lwlib_vec3i(x * 2 + 1, y * 2, 0),
					WR_LEVEL_QN_NORTHWALL,
					0,
					false
					);
				WR_Scene_UprightQuad(
					scene, 
					x + 1.0, y + 0.5,
					x + 0.5, y + 0.5,
					AllocTex(flippedShapeImage),
					0, 0,
					lwlib_vec3i(x * 2 + 1, y * 2 + 1, 0),
					WR_LEVEL_QN_SOUTHWALL,
					0,
					false
					);
				WR_Scene_UprightQuad(
					scene, 
					x + 0.5, y + 0.5,
					x, y + 0.5,
					AllocTex(flippedShapeImage),
					0, TEXTURESIZE / 2,
					lwlib_vec3i(x * 2, y * 2 + 1, 0),
					WR_LEVEL_QN_SOUTHWALL,
					0,
					false
					);
			}
		}
	}

	/* generate quads for lights */
	for (x = 0; x < 64; x++)
	{
		for (y = 0; y < 64; y++)
		{
			tile = tilemap[x][64 - 1 - y];

			if (tile != 0 || (tile & 0x80) != 0)
			{
				/* no quads */
			}
			else
			{
				/* empty tile */
				lp = NULL;
				radLightPoint = lwlib_vec3f_zero();

				/* static object lights */
				statobj = statat[x][64 - 1 - y];
				switch (rad_params.includedLights)
				{
				case WR_INCLUDEDLIGHTS_NEAR:
					if (!(abs(x - player->tilex) < 6 && 
						abs(y - (64 - 1 - player->tiley)) < 6))
					{
						statobj = NULL;
					}
					break;
				case WR_INCLUDEDLIGHTS_VISIBLE:
					if (lwlib_Bres_VisBlocked(priv->bres, 
						lwlib_vec3i(x, y, 0), 
						lwlib_vec3i(player->tilex, 
							64 - 1 - player->tiley, 0)))
					{
						statobj = NULL;
					}
					break;
				}
				if (statobj != 0)
				{
					lp = WR_Scene_FindLightProfile(scene, 
						statobj->radLightKey);
					lwlib_InitVector(radLightPoint, x + 0.5,
						y + 0.5, 0.0);
				}

				if (lp)
				{
					lightNum = vec_size(rad_params.lights);
					memset(&light, 0, sizeof(light));
					vec_push(rad_params.lights, TLight, light);
					WR_Scene_Generate_RadQuads(scene,
						lwlib_vec3i(x * 2, y * 2, 0),
						lwlib_X(radLightPoint), lwlib_Y(radLightPoint),
						lp, lightNum);
				}
			}
		}
	}

	for (x = 0; x < 64; x++)
	{
		for (y = 0; y < 64; y++)
		{
			tile = MAPSPOT(x, 64 - 1 - y, 2);

			spot = ED_CheckFloorCeil(ed, x, 64 - 1 - y, 0);
			ceiltex = ((spot >> 8) & 0xff);

			if (spot != 0 && ceiltex >= 1 && 
				(ceiltex - 1) < WR_RADLIGHTKEY_MAX)
			{
				lp = WR_Scene_FindLightProfile(scene, 
					WR_RADLIGHTKEY_FLOORCEILSTART + 
					(ceiltex - 1));
				if (lp != NULL)
				{
					lwlib_InitVector(radLightPoint, x + 0.5,
						y + 0.5, 0.0);
					lightNum = vec_size(rad_params.lights);
					memset(&light, 0, sizeof(light));
					vec_push(rad_params.lights, TLight, light);
					WR_Scene_Generate_RadQuads(scene,
						lwlib_vec3i(x * 2, y * 2, 0),
						lwlib_X(radLightPoint), lwlib_Y(radLightPoint),
						lp, lightNum);
				}
			}
		}
	}
}

void WR_Scene_ResetProcesses(WR_Scene_t scene)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i, j;
	TQuad *quad;
	TRadMap radMap;

	if (rad_params.totalProcesses == 0)
	{
		return;
	}

	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);

		for (j = 0; j < rad_params.totalProcesses; j++)
		{
			radMap = LWLIB_CTX_READ(j, quad->radMap, TRadMap);
			RadMapFree(&radMap);
			LWLIB_CTX_ZERO(j, quad->radMap, TRadMap);
		}
	}

	for (i = 0; i < rad_params.totalProcesses; i++)
	{
		lwlib_Bits_t openDoorBits = LWLIB_CTX_READ(i, 
			rad_params.openDoorBits, lwlib_Bits_t);
		lwlib_BitsFree(openDoorBits);
		LWLIB_CTX_ZERO(i, rad_params.openDoorBits, lwlib_Bits_t);
	}

	lwlib_BitsFree(rad_params.markDoorBits);
	ctxReset();

	rad_params.totalProcesses = 0;
}

void WR_Scene_ClearQuads(WR_Scene_t scene)
{
	int i;
	TQuad *quad;
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);

	WR_Scene_ResetProcesses(scene);

	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		if (quad->postsource != NULL)
		{
			free(quad->postsource);
		}
	}
	vec_free(rad_params.quads, TQuad);
	vec_free(rad_params.lights, TLight);
}

void WR_Scene_ToggleLightStates(WR_Scene_t scene, int lightNum)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i;
	TQuad *quad;

	for (i = 0; i < vec_size(rad_params.quads); i++)
	{
		quad = &vec_at(rad_params.quads, i);
		if (quad->lightNum != 0 && quad->lightNum - 1 != lightNum)
		{
			quad->excludeFromScene = !quad->excludeFromScene;
		}
	}
}

void WR_Scene_SetAllLightStates(WR_Scene_t scene, bool excludeFromScene)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i;
	TQuad *quad;

	for (i = vec_size(rad_params.quads) - 1; i >= 0; i--)
	{
		quad = &vec_at(rad_params.quads, i);
		if (quad->lightNum == 0)
		{
			break;
		}

		quad->excludeFromScene = excludeFromScene;
	}
}

static void WR_Scene_ToggleQuad(WR_Scene_t scene, TQuad *quad)
{
	quad->excludeFromScene =
		!quad->excludeFromScene;

	// clear neighbours and corners if excluded
	// from scene
	if (quad->excludeFromScene)
	{
		lwlib_Zero(quad->neigh);
		lwlib_Zero(quad->corner);
	}
}

void WR_Scene_ToggleDoorStates(WR_Scene_t scene, lwlib_Bits_t doorBits)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	int i, j;
	int qn;
	int quadNum;
	int doorNum;
	TQuad *quad;
	doorobj_t *door;
	WR_Level_Tile_t *tile;
	statobj_t *statobj;

	for (i = 0; i < doorcount; i++)
	{
		door = &doorobjlist[i];
		if (!lwlib_BitsTest(doorBits, i))
		{
			continue;
		}

		for (j = 0; j < door->quadIndexCount; j++)
		{
			quadNum = door->quadIndexArr[j];

			quad = &vec_at(rad_params.quads, quadNum);
			assert(quad->doorNum == i + 1);

			WR_Scene_ToggleQuad(scene, quad);
		}
	}
}

void WR_Scene_SetRad(WR_Scene_t scene, WR_Rad_t rad)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	priv->rad = rad;
}

void WR_Scene_SetLevel(WR_Scene_t scene, WR_Level_t level)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	priv->level = level;
}

void WR_Scene_SetBres(WR_Scene_t scene, lwlib_Bres_t bres)
{
	lwlib_GetPrivAssert(WR_Scene_Priv_t, scene);
	priv->bres = bres;
}
#endif