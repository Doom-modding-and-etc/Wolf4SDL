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
** Room
*************************************************************************/

#ifndef WR_LEVEL_H
#define WR_LEVEL_H

#include "..\lw_lib\lw_vec.h"
#ifdef WOLFRAD
#include "..\lw_lib\lw_misc.h"

typedef enum
{ 
	WR_LEVEL_DIRTYPE_EAST,
	WR_LEVEL_DIRTYPE_NORTH,
	WR_LEVEL_DIRTYPE_WEST,
	WR_LEVEL_DIRTYPE_SOUTH,
	WR_LEVEL_DIRTYPE_NODIR,
} WR_Level_DirType_t;

typedef enum WR_Level_TileContent_e
{
	WR_LEVEL_TILECONTENT_EMPTY,
	WR_LEVEL_TILECONTENT_WALL,
} WR_Level_TileContent_t;

typedef enum
{
	WR_LEVEL_QN_WESTWALL,
	WR_LEVEL_QN_EASTWALL,
	WR_LEVEL_QN_SOUTHWALL,
	WR_LEVEL_QN_NORTHWALL,
	WR_LEVEL_QN_FLOOR,
	WR_LEVEL_QN_CEILING,
	WR_LEVEL_QN_TOT,
} WR_Level_QuadNumber_t;

typedef struct
{
	WR_Level_TileContent_t content;
	int quad_lookup[WR_LEVEL_QN_TOT];
} WR_Level_Tile_t;

typedef struct WR_Level_s
{
	void *priv;
} WR_Level_t;

WR_Level_t WR_Level_New(void);

WR_Level_t WR_Level_Free(WR_Level_t level);

WR_Level_Tile_t *WR_Level_GetTileAt(WR_Level_t level, int x, int y);

void WR_Level_InitFromLive(WR_Level_t level);

void WR_Level_ReloadCurInfo(void);

static inline WR_Level_Tile_t *WR_Level_GetTile(WR_Level_t level,
	lwlib_TPoint3i pos)
{
	return WR_Level_GetTileAt(level, lwlib_X(pos), lwlib_Y(pos));
}

static inline bool WR_Level_TileIs(WR_Level_t level,
	lwlib_TPoint3i pos, WR_Level_TileContent_t content)
{
	return WR_Level_GetTile(level, pos)->content == 
		content;
}

static inline bool WR_Level_TileIsEmpty(WR_Level_t level,
	lwlib_TPoint3i pos)
{
	return WR_Level_TileIs(level, pos, 
		WR_LEVEL_TILECONTENT_EMPTY);
}

static inline bool WR_Level_TileIsWall(WR_Level_t level,
	lwlib_TPoint3i pos)
{
	return WR_Level_TileIs(level, pos, 
		WR_LEVEL_TILECONTENT_WALL);
}

static inline bool WR_Level_TileIsEmptyAt(WR_Level_t level,
	int x, int y)
{
	return WR_Level_TileIsEmpty(level, lwlib_vec3i(x, y, 0));
}

static inline bool WR_Level_TileIsWallAt(WR_Level_t level,
	int x, int y)
{
	return WR_Level_TileIsWall(level, lwlib_vec3i(x, y, 0));
}
#endif
#endif
