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

#include <stdio.h>
#include "vec.h"
#include "pwscan.h"

#define deln lwlib_CallDelegateNoArgs
#define del lwlib_CallDelegate

static PwScan_Spot_t PwScan_SpotNeighbour(PwScan_t *pwScan,
	PwScan_Spot_t spot, PwScan_Dir_t dir)
{
	spot.x += pwScan->dirDx[dir];
	spot.y += pwScan->dirDy[dir];
	return spot;
}

static PwScan_Tile_t *PwScan_GetTile(PwScan_t *pwScan, PwScan_Spot_t spot)
{
	assert(spot.x >= 0 && spot.x < pwScan->mapDims[0] &&
		spot.y >= 0 && spot.y < pwScan->mapDims[1]);
	return &pwScan->tiles[spot.y * pwScan->mapDims[0] + spot.x];
}

static void PwScan_PushPw(PwScan_t *pwScan, PwScan_Spot_t spot,
	PwScan_Dir_t dir)
{
	int i;
	int spotId;
	PwScan_Dir_t faceDir;
	PwScan_PwDoor_t pwDoor;
	PwScan_Spot_t startSpot, nextSpot;
	PwScan_Tile_t *tile;
	lwlib_DeclDelegateAPI(PwScan_DelegateAPI_t, pwScan);

	tile = PwScan_GetTile(pwScan, spot);
	assert(del(spotHasPw, spot));

	startSpot = spot;
	tile->pushed = true;

	for (i = 0; i <= deln(pwMaxMove); i++)
	{
		if (!del(pwCanEnterSpot, spot))
		{
			break;
		}

		lwlib_Zero(pwDoor);
		pwDoor.startSpot = startSpot;
		pwDoor.spot = spot;

		for (faceDir = PwScan_DirFirst; faceDir != PwScan_DirMax;
			faceDir = pwScan->dirNext[faceDir])
		{
			nextSpot = PwScan_SpotNeighbour(pwScan, spot, faceDir);
			if (!del(spotIsSolid, nextSpot))
			{
				lwlib_BitEnable(pwDoor.faces, (int)faceDir);
			}
		}

		if (pwDoor.faces != 0)
		{
			spotId = pwDoor.spot.x + pwDoor.spot.y * pwScan->mapDims[0];
			if (!lwlib_BitsTest(pwScan->pwDoorSpotBits, spotId))
			{
				lwlib_BitsSet(pwScan->pwDoorSpotBits, spotId);
				vec_push(pwScan->pwDoorVec, PwScan_PwDoor_t, pwDoor);
			}
		}
		else
		{
			deln(unexpectedCondition);
		}

		spot = PwScan_SpotNeighbour(pwScan, spot, dir);
	}
}

static void PwScan_FoundPwPush(PwScan_t *pwScan, PwScan_Spot_t spot,
	PwScan_Dir_t dir)
{
	PwScan_PwPush_t pwPush;

	lwlib_Zero(pwPush);
	pwPush.spot = spot;
	pwPush.dir = dir;

	vec_push(pwScan->pwPushVec, PwScan_PwPush_t, pwPush);
}

static int PwScan_VisitSpot(PwScan_t *pwScan, PwScan_Spot_t spot)
{
	int pwPushCount;
	PwScan_Dir_t dir;
	PwScan_Tile_t *tile;
	PwScan_Spot_t nextSpot;
	lwlib_DeclDelegateAPI(PwScan_DelegateAPI_t, pwScan);

	assert(del(playerCanEnterSpot, spot));
	pwPushCount = 0;

	tile = PwScan_GetTile(pwScan, spot);

	assert(!tile->visited);
	tile->visited = true;

	for (dir = PwScan_DirFirst; dir != PwScan_DirMax; 
		dir = pwScan->dirNext[dir])
	{
		nextSpot = PwScan_SpotNeighbour(pwScan, spot, dir);
		if (del(playerCanEnterSpot, nextSpot))
		{
			tile = PwScan_GetTile(pwScan, nextSpot);

			if (del(spotHasPw, nextSpot) && !tile->pushed)
			{
				PwScan_FoundPwPush(pwScan, nextSpot, dir);
				pwPushCount++;
			}
			else
			{
				if (!tile->visited)
				{
					pwPushCount += PwScan_VisitSpot(pwScan, nextSpot);
				}
			}
		}
	}

	return pwPushCount;
}

static void PwScan_ClearTilesVisited(PwScan_t *pwScan)
{
	int i;

	for (i = 0; i < pwScan->numTiles; i++)
	{
		pwScan->tiles[i].visited = false;
	}
}

void PwScan_Init(PwScan_t *pwScan, int mapWidth, int mapHeight)
{
	static const int dirDx[PwScan_DirMax] = { 1, 0, -1, 0 };
	static const int dirDy[PwScan_DirMax] = { 0, 1, 0, -1 };
	static const PwScan_Dir_t dirNext[PwScan_DirMax] =
	{
		PwScan_DirNorth,
		PwScan_DirWest,
		PwScan_DirSouth,
		PwScan_DirMax,
	};

	lwlib_Zero(*pwScan);
	pwScan->dirDx = dirDx;
	pwScan->dirDy = dirDy;
	pwScan->dirNext = dirNext;
	pwScan->mapDims[0] = mapWidth;
	pwScan->mapDims[1] = mapHeight;
	pwScan->numTiles = mapWidth * mapHeight;
	pwScan->tiles = lwlib_CallocMany(PwScan_Tile_t, pwScan->numTiles);
}

void PwScan_Free(PwScan_t *pwScan)
{
	free(pwScan->tiles);
	vec_free(pwScan->pwPushVec, PwScan_PwPush_t);
	vec_free(pwScan->pwDoorVec, PwScan_PwDoor_t);
	lwlib_Zero(*pwScan);
}

void PwScan_DiscoverPwDoors(PwScan_t *pwScan)
{
	int i;
	PwScan_Spot_t spot;
	PwScan_PwPush_t *pwPush;
	lwlib_DeclDelegateAPI(PwScan_DelegateAPI_t, pwScan);

	vec_free(pwScan->pwDoorVec, PwScan_PwDoor_t);
	pwScan->pwDoorSpotBits = lwlib_BitsEmpty();

	spot = deln(playerSpawnSpot);

	while (PwScan_VisitSpot(pwScan, spot) > 0)
	{
		for (i = 0; i < (int)vec_size(pwScan->pwPushVec); i++)
		{
			pwPush = &vec_at(pwScan->pwPushVec, i);
			PwScan_PushPw(pwScan, pwPush->spot, pwPush->dir);
		}

		vec_free(pwScan->pwPushVec, PwScan_PwPush_t);

		PwScan_ClearTilesVisited(pwScan);
	}

	lwlib_BitsFree(pwScan->pwDoorSpotBits);
}
