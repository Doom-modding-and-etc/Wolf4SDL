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

#ifndef LWLIB_FS_H
#define LWLIB_FS_H

#include <stdlib.h>
#include "../fixedptc.h"
#include "lw_misc.h"
#ifdef LWLIB
#define lwlib_Fs_Valid(fs) ((fs).priv != NULL)
#define lwlib_Fs_FileMode(name) LWLIB_FS_FILEMODE_ ## name

#define lwlib_Fs_CommitBlock(fs, block, len, varType, desc) \
do { \
	if (lwlib_Fs_Valid(fs)) \
	{ \
		lwlib_Fs_Commit(fs, block, len, varType, desc); \
	} \
} while (0)

#define lwlib_Fs_CommitVar(fs, var, varType, desc) \
	lwlib_Fs_CommitBlock(fs, var, sizeof(*(var)), varType, desc)

typedef enum lwlib_Fs_FileMode_e
{
	lwlib_Fs_FileMode(Read),
	lwlib_Fs_FileMode(Write),
	lwlib_Fs_FileMode(ReadText),
	lwlib_Fs_FileMode(WriteText),
	lwlib_Fs_FileMode(Last) = lwlib_Fs_FileMode(WriteText),
} lwlib_Fs_FileMode_t;

typedef enum lwlib_Fs_VarType_e
{
	lwlib_Fs_VarType_Int,
	lwlib_Fs_VarType_Short,
	lwlib_Fs_VarType_UChar,
	lwlib_Fs_VarType_Double,
	lwlib_Fs_VarType_Bool,
	lwlib_Fs_VarType_Raw,
	lwlib_Fs_VarType_String,
	lwlib_Fs_VarType_Informative,
} lwlib_Fs_VarType_t;

typedef struct lwlib_Fs_s
{
	void *priv;
} lwlib_Fs_t;

lwlib_EnumStrExtern(lwlib_Fs_FileMode);

lwlib_Fs_t lwlib_Fs_New(const char *fileName, lwlib_Fs_FileMode_t fileMode);

lwlib_Fs_t lwlib_Fs_Free(lwlib_Fs_t fs);

void lwlib_Fs_Commit(lwlib_Fs_t fs, void *buf, int count, 
	lwlib_Fs_VarType_t varType, const char *desc);

bool lwlib_Fs_LoadEntireFile(lwlib_Fs_t fs);

lwlib_Byte_t *lwlib_Fs_GetData(lwlib_Fs_t fs);

int lwlib_Fs_GetLength(lwlib_Fs_t fs);

lwlib_Fs_FileMode_t lwlib_Fs_GetFileMode(lwlib_Fs_t fs);
#endif
#endif
