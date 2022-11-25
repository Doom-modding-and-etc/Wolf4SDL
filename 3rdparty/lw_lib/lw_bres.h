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

#ifndef LWLIB_BRES_H
#define LWLIB_BRES_H
#include "lw_vec.h"
#ifdef LWLIB
#include "lw_misc.h"

typedef struct lwlib_Bres_s
{
	void *priv;
} lwlib_Bres_t;

lwlib_Bres_t lwlib_Bres_New(void);

lwlib_Bres_t lwlib_Bres_Free(lwlib_Bres_t bres);

void lwlib_Bres_ComputeVisAll(lwlib_Bres_t bres);

bool lwlib_Bres_VisBlocked(lwlib_Bres_t bres, lwlib_TPoint3i start,
	lwlib_TPoint3i end);
#endif
#endif
