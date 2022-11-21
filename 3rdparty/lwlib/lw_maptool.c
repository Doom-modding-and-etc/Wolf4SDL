/*************************************************************************
** Wolf3D Legacy
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
** LinuxWolf Library for Wolf3D Legacy
*************************************************************************/

#include <sys/types.h>
#if defined _WIN32
	#include <io.h>
#else
	#include <sys/uio.h>
	#include <unistd.h>
#endif
#include "wl_def.h"
#include "lw_maptool.h"

#define LWLIB_MAPTOOL_EDITORID "LWMTv1.0"

typedef struct lwlib_MapTool_Map_s
{
	word width;
	word height;
	char name[MAXTITLECHARS];
	word planes[MAPPLANES][maparea];
} lwlib_MapTool_Map_t;

typedef struct lwlib_MapTool_Priv_s
{
	lwlib_MapTool_Map_t maps[NUMMAPS];
} lwlib_MapTool_Priv_t;

static void lwlib_MapTool_LoadMaps(lwlib_MapTool_t mapTool)
{
	int i, j;
	maptype *mapheader;
	lwlib_MapTool_Map_t *map;
	lwlib_GetPrivAssert(lwlib_MapTool_Priv_t, mapTool);

	memset(priv->maps, 0, sizeof(priv->maps));

	for (i = 0; i < NUMMAPS; i++)
	{
		map = &priv->maps[i];
		mapheader = mapheaderseg[i];
		if (mapheader == NULL)
		{
			continue;
		}

		map->width = mapheader->width;
		map->height = mapheader->height;
		lwlib_Copy(map->name, mapheader->name);

		if (!(mapheader->width == 64 && mapheader->height == 64))
		{
			continue;
		}

		CA_CacheMap(i);
		for (j=0; j < MAPPLANES; j++)
		{
			memcpy(map->planes[j], mapsegs[j], maparea * 2);
		}
	}
}

static int lwlib_MapTool_SaveMapHead(lwlib_MapTool_t mapTool)
{
	int i;
	int off;
	int handle;
	char fname[13];
	int32_t length, pos;
	mapfiletype tinf;
	lwlib_MapTool_Map_t *map;
	lwlib_GetPrivAssert(lwlib_MapTool_Priv_t, mapTool);

	strcpy(fname,mheadname);
	strcat(fname,extension);

	handle = open(fname, O_RDWR | O_BINARY);
	if (handle == -1)
		CA_CannotOpen(fname);

	lwlib_Zero(tinf);
	tinf.RLEWtag = RLEWtag;

	off = strlen(LWLIB_MAPTOOL_EDITORID);

	for (i = 0; i < NUMMAPS; i++)
	{
		map = &priv->maps[i];
		if (!(map->width == 64 && map->height == 64))
		{
			tinf.headeroffsets[i] = -1;
			continue;
		}

		tinf.headeroffsets[i] = off;
		off += sizeof(maptype);
	}

	length = NUMMAPS * 4 + 2;
	write(handle, &tinf, length);
	
	close(handle);

	return off;
}

static byte *lwlib_MapTool_MapPlaneOnDisk(lwlib_MapTool_t mapTool,
	int mapNum, int planeNum, int *onDiskSize)
{
	int expanded, rlew_size, compressed;
	byte *planeseg, *rlew_planeseg;
	lwlib_MapTool_Map_t *map;
	lwlib_GetPrivAssert(lwlib_MapTool_Priv_t, mapTool);

	map = &priv->maps[mapNum];

	expanded = maparea * 2;
	rlew_planeseg = lwlib_CallocMany(byte, expanded + 2);
	rlew_size = CA_RLEWCompress(map->planes[planeNum], expanded,
		((word *)rlew_planeseg) + 1, RLEWtag);
	*((word *)rlew_planeseg) = (word)expanded;

	expanded = rlew_size + 2; 
	planeseg = lwlib_CallocMany(byte, (expanded * 3 / 2) + 2);
	compressed = CAL_CarmackCompress((word *)rlew_planeseg, expanded,
		((word *)planeseg) + 1);
	*((word *)planeseg) = (word)expanded;

	free(rlew_planeseg);

	if (onDiskSize != NULL)
	{
		*onDiskSize = compressed + 2;
	}
	return planeseg;
}

static int lwlib_MapTool_MapPlaneSizeOnDisk(lwlib_MapTool_t mapTool,
	int mapNum, int planeNum)
{
	int onDiskSize;
	byte *planeseg;

	planeseg = lwlib_MapTool_MapPlaneOnDisk(mapTool, mapNum, planeNum, &onDiskSize);
	free(planeseg);

	return onDiskSize;
}

static void lwlib_MapTool_SaveMapHeaders(lwlib_MapTool_t mapTool, int handle, int off)
{
	int i, j;
	int size;
	lwlib_MapTool_Map_t *map;
	maptype mapheader;
	lwlib_GetPrivAssert(lwlib_MapTool_Priv_t, mapTool);

	for (i = 0; i < NUMMAPS; i++)
	{
		map = &priv->maps[i];
		if (!(map->width == 64 && map->height == 64))
		{
			continue;
		}

		lwlib_Zero(mapheader);
		for (j = 0; j < 3; j++)
		{
			size = lwlib_MapTool_MapPlaneSizeOnDisk(mapTool, i, j);
			mapheader.planestart[j] = off;
			mapheader.planelength[j] = size;
			off += size;
		}
		mapheader.width = mapheader.height = 64;
		lwlib_Copy(mapheader.name, map->name);

		write(handle, &mapheader, sizeof(mapheader));
	}
}

static void lwlib_MapTool_SaveMapPlanes(lwlib_MapTool_t mapTool, int handle)
{
	int i, j;
	int onDiskSize;
	byte *planeseg;
	lwlib_MapTool_Map_t *map;
	lwlib_GetPrivAssert(lwlib_MapTool_Priv_t, mapTool);

	for (i = 0; i < NUMMAPS; i++)
	{
		map = &priv->maps[i];
		if (!(map->width == 64 && map->height == 64))
		{
			continue;
		}

		for (j = 0; j < 3; j++)
		{
			planeseg = lwlib_MapTool_MapPlaneOnDisk(mapTool, i, j, &onDiskSize);
			write(handle, planeseg, onDiskSize);
			free(planeseg);
		}
	}
}

static void lwlib_MapTool_SaveMaps(lwlib_MapTool_t mapTool)
{
	int off;
	int handle;
	char fname[13];
	lwlib_GetPrivAssert(lwlib_MapTool_Priv_t, mapTool);

	off = lwlib_MapTool_SaveMapHead(mapTool);

	strcpy(fname, "gamemaps.");
	strcat(fname, extension);

	handle = open(fname, O_RDWR | O_BINARY);
	if (handle == -1)
	{
		CA_CannotOpen(fname);
	}

	write(handle, LWLIB_MAPTOOL_EDITORID, strlen(LWLIB_MAPTOOL_EDITORID));

	lwlib_MapTool_SaveMapHeaders(mapTool, handle, off);
	lwlib_MapTool_SaveMapPlanes(mapTool, handle);

	close(handle);
}

lwlib_MapTool_t lwlib_MapTool_New(void)
{
	lwlib_MapTool_t mapTool;

	lwlib_Zero(mapTool);
	mapTool.priv = lwlib_CallocSingle(lwlib_MapTool_Priv_t);

	lwlib_GetPriv(lwlib_MapTool_Priv_t, mapTool);

	return mapTool;
}

lwlib_MapTool_t lwlib_MapTool_Free(lwlib_MapTool_t mapTool)
{
	lwlib_GetPriv(lwlib_MapTool_Priv_t, mapTool);

	if (priv)
	{
		free(priv);
	}

	lwlib_Zero(mapTool);
	return mapTool;
}
