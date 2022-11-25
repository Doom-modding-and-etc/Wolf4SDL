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
** LightInfo
*************************************************************************/

#include <stdlib.h>
#include "wr_lightinfo.h"
#ifdef WOLFRAD
void WR_LightInfo_Free(WR_LightInfo_t *lightInfo)
{
	int i;

	if (lightInfo == NULL)
	{
		return;
	}

	free(lightInfo->lightDoors);
	free(lightInfo->lightFacets);
	for (i = 0; i < lightInfo->numLightmaps; i++)
	{
		free(lightInfo->lightmaps[i].data);
	}
	free(lightInfo->lightmaps);
	free(lightInfo);
}
#endif