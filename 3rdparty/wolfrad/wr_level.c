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
** Level
*************************************************************************/

#include <stdlib.h>
#include "wr_level.h"
#include "wr_lightmap.h"
#include "..\..\wl_def.h"
#ifdef WOLFRAD

typedef struct WR_Level_Priv_s
{
	WR_Level_Tile_t tiles[128][128];
} WR_Level_Priv_t;

WR_Level_t WR_Level_New(void)
{
	WR_Level_t level;
	lwlib_Zero(level);

	lwlib_GetPriv(WR_Level_Priv_t, level);
	priv = lwlib_CallocSingle(WR_Level_Priv_t);

	level.priv = priv;
	return level;
}

WR_Level_t WR_Level_Free(WR_Level_t level)
{
	lwlib_GetPriv(WR_Level_Priv_t, level);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(level);
	return level;
}

WR_Level_Tile_t *WR_Level_GetTileAt(WR_Level_t level, int x, int y)
{
	lwlib_GetPrivAssert(WR_Level_Priv_t, level);
	if (x >= 0 && x < 128 && y >= 0 && y < 128)
	{
		return &priv->tiles[x][y];
	}
	return NULL;
}

void WR_Level_InitFromLive(WR_Level_t level)
{
	int i;
	int x, y;
	doorobj_t *doorobj;
	WR_Level_Tile_t *tile;
	byte maptile;
	statobj_t *statobj;
	lwlib_GetPrivAssert(WR_Level_Priv_t, level);

	lwlib_Zero(priv->tiles);

	for (y = 0; y < 64; y++)
	{
		for (x = 0; x < 64; x++)
		{
			maptile = tilemap[x][64 - 1 - y];
			if (maptile != 0 && (maptile & 0x80) == 0 && 
				MAPSPOT(x, 64 - 1 - y, 1) != PUSHABLETILE)
			{
				for (i = 0; i < 4; i++)
				{
					tile = &priv->tiles[x * 2 + (i % 2)][y * 2 + (i / 2)];
					tile->content = WR_LEVEL_TILECONTENT_WALL;
				}
			}
		}
	}
}

void WR_Level_ReloadCurInfo(void)
{
	WR_LightInfo_Free(curLightInfo);
	curLightInfo = WR_LoadLightInfo();
}
#endif