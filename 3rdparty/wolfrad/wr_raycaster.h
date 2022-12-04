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
#ifndef WOLFRAD_RAYCASTER_H
#define WOLFRAD_RAYCASTER_H

#include "wr_rad.h"
#include "wr_level.h"
#ifdef PS2
#include "3rdparty/lw_lib/lw_bres.h"
#else
#include "../lw_lib/lw_bres.h"
#endif
#ifdef WOLFRAD

#ifndef WOLFRAD_RAYCASTER_TYPE
#define WOLFRAD_RAYCASTER_TYPE
typedef struct WR_RayCaster_s
{
	void *priv;
} WR_RayCaster_t;
#endif

WR_RayCaster_t WR_RayCaster_New(void);

WR_RayCaster_t WR_RayCaster_Free(WR_RayCaster_t rayCaster);

int WR_RayCaster_SearchElements(WR_RayCaster_t rayCaster,
	unsigned long shootPatch, int *elems);

void WR_RayCaster_SetRad(WR_RayCaster_t rayCaster, WR_Rad_t rad);

void WR_RayCaster_SetLevel(WR_RayCaster_t rayCaster, WR_Level_t level);

void WR_RayCaster_SetBres(WR_RayCaster_t rayCaster, lwlib_Bres_t bres);

unsigned long WR_RayCaster_ShootRay(WR_RayCaster_t rayCaster, 
	lwlib_TPoint3f start, lwlib_TVector3f direction, 
	unsigned long targetElementIndex);
#endif
#endif
