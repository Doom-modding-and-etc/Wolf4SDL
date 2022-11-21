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

#ifndef LWLIB_MAPTOOL_H
#define LWLIB_MAPTOOL_H

typedef struct
{
	void *priv;
} lwlib_MapTool_t;

lwlib_MapTool_t lwlib_MapTool_New(void);

lwlib_MapTool_t lwlib_MapTool_Free(lwlib_MapTool_t mapTool);

#endif
