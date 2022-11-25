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

#ifndef WOLFRAD_H
#define WOLFRAD_H
#ifdef WOLFRAD

typedef struct WolfRad_s
{
	void *priv;
} WolfRad_t;

WolfRad_t WolfRad_New(void);

WolfRad_t WolfRad_Free(WolfRad_t wolfRad);

void WolfRad_Run(WolfRad_t wolfRad);

void WolfRad_ControlPanel(WolfRad_t wolfRad);

void WolfRad_Progress(WolfRad_t wolfRad, const char *message,
	bool forceUpdate);

void WolfRad_ReadConfig(WolfRad_t wolfRad, int file);

void WolfRad_WriteConfig(WolfRad_t wolfRad, int file);

bool WolfRad_Cancelled(WolfRad_t wolfRad);

void WolfRad_ReloadCurLightInfo(void);
#endif
#endif
