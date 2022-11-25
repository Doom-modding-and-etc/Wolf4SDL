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

#ifndef WOLFRAD_ROOM_H
#define WOLFRAD_ROOM_H

#include "wr_rad.h"
#include "wr_level.h"
#include "wr_lightmap.h"
#ifdef WOLFRAD

typedef struct WR_Room_s
{
	void *priv;
} WR_Room_t;

WR_Room_t WR_Room_New(void);

WR_Room_t WR_Room_Free(WR_Room_t room);

void WR_Room_InitParams(WR_Room_t room);

void WR_Room_DestroyParams(WR_Room_t room);

void WR_Room_ComputeNeighbours(WR_Room_t room);

void WR_Room_SetRad(WR_Room_t room, WR_Rad_t rad);

void WR_Room_SetLevel(WR_Room_t room, WR_Level_t level);

void WR_Room_SetLightMapper(WR_Room_t room, WR_LightMapper_t lightMapper);

void UVToXYZ(const lwlib_TPoint3f quad[4], double u, double v,
	lwlib_TPoint3f* xyz);

#endif
#endif
