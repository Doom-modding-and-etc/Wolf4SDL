/*************************************************************************
** Batman: No Man's Land
** Copyright (C) 2012 by LinuxWolf - Team RayCAST
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
** LinuxWolf Library for Batman: No Man's Land
*************************************************************************/

#ifndef LW_PW_SCAN_H
#define LW_PW_SCAN_H

#include "lw_misc.h"
#ifdef LWLIB

typedef enum PwScan_Dir_e
{
	PwScan_DirFirst,
	PwScan_DirEast = PwScan_DirFirst,
	PwScan_DirNorth,
	PwScan_DirWest,
	PwScan_DirSouth,
	PwScan_DirMax,
} PwScan_Dir_t;

typedef int PwScan_DirMask_t;

typedef struct PwScan_Spot_s
{
	int x, y;
} PwScan_Spot_t;

typedef struct PwScan_PwPush_s
{
	PwScan_Spot_t spot;
	PwScan_Dir_t dir;
} PwScan_PwPush_t;

typedef PwScan_PwPush_t PwScan_PwPushVec_t;

typedef struct PwScan_PwDoor_s
{
	PwScan_Spot_t startSpot;
	PwScan_Spot_t spot;
	PwScan_DirMask_t faces;
} PwScan_PwDoor_t;

typedef PwScan_PwDoor_t PwScan_PwDoorVec_t;

lwlib_DefineDelegate(PwScan_DelegateSpotIsSolid_t,
	bool, /*(*/ void *obj, PwScan_Spot_t spot /*)*/
	);

lwlib_DefineDelegate(PwScan_DelegatePlayerCanEnterSpot_t,
	bool, /*(*/ void *obj, PwScan_Spot_t spot /*)*/
	);

lwlib_DefineDelegate(PwScan_DelegatePwCanEnterSpot_t,
	bool, /*(*/ void *obj, PwScan_Spot_t spot /*)*/
	);

lwlib_DefineDelegate(PwScan_DelegatePwMaxMove_t,
	int, /*(*/ void *obj /*)*/
	);

lwlib_DefineDelegate(PwScan_DelegatePlayerSpawnSpot_t,
	PwScan_Spot_t, /*(*/ void *obj /*)*/
	);

lwlib_DefineDelegate(PwScan_DelegateSpotHasPw_t,
	bool, /*(*/ void *obj, PwScan_Spot_t spot /*)*/
	);

lwlib_DefineDelegate(PwScan_DelegateUnexpectedCondition_t,
	void, /*(*/ void *obj /*)*/
	);

typedef struct PwScan_DelegateAPI_s
{
	PwScan_DelegateSpotIsSolid_t spotIsSolid;
	PwScan_DelegatePlayerCanEnterSpot_t playerCanEnterSpot;
	PwScan_DelegatePwCanEnterSpot_t pwCanEnterSpot;
	PwScan_DelegatePwMaxMove_t pwMaxMove;
	PwScan_DelegatePlayerSpawnSpot_t playerSpawnSpot;
	PwScan_DelegateSpotHasPw_t spotHasPw;
	PwScan_DelegateUnexpectedCondition_t unexpectedCondition;
} PwScan_DelegateAPI_t;

typedef struct PwScan_Tile_s
{
	bool visited;
	bool pushed;
} PwScan_Tile_t;

typedef struct PwScan_s
{
	PwScan_DelegateAPI_t delegateAPI;
	const int *dirDx;
	const int *dirDy;
	const PwScan_Dir_t *dirNext;
	PwScan_Tile_t *tiles;
	int numTiles;
	int mapDims[2];
	PwScan_PwPushVec_t pwPushVec;
	PwScan_PwDoorVec_t pwDoorVec;
	lwlib_Bits_t pwDoorSpotBits;
} PwScan_t;

void PwScan_Init(PwScan_t *pwScan, int mapWidth, int mapHeight);

void PwScan_Free(PwScan_t *pwScan);

void PwScan_DiscoverPwDoors(PwScan_t *pwScan);

#endif
#endif

