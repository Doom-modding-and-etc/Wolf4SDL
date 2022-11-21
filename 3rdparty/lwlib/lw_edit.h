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

#ifndef LWLIB_EDIT_H
#define LWLIB_EDIT_H

#include "lw_vec.h"
#include "lw_misc.h"
#include "lw_fs.h"
#include "vec.h"

#define lwlib_Edit_ItemFlag(name) LWLIB_EDIT_ITEMFLAG_ ## name
#define lwlib_Edit_ItemFlagMask(name) lwlib_Pow2(LWLIB_EDIT_ITEMFLAG_ ## name)

#define lwlib_Edit_CheckPersist(persist, fncall) \
do { \
	if ((persist).commit) \
	{ \
		lwlib_Edit_t fsEdit; \
		(persist).commit = false; \
 \
		fsEdit = lwlib_Edit_NewFs((persist).fileName, (persist).fileMode); \
		fncall; \
		lwlib_Edit_Free(fsEdit); \
	} \
} while(0)

#define lwlib_Edit_AddGroup(edit, itemName, fncall) \
do { \
	lwlib_Edit_AddItem_group(edit, itemName); \
	fncall; \
	lwlib_Edit_NavigateBack(edit); \
} while(0)

#define lwlib_Edit_AddVec(edit, itemName, vec, \
	elemInit, elemCleanup, elemAddToEdit) \
	lwlib_Edit_AddItem_vec(edit, itemName, \
		(lwlib_VoidVec_t *)&(vec), \
		sizeof(vec_at(vec, 0)), \
		elemInit, elemCleanup, elemAddToEdit \
		)

typedef enum lwlib_Edit_ItemFlag_e
{
	lwlib_Edit_ItemFlag(ReadOnly),
	lwlib_Edit_ItemFlag(GroupExpand),
	lwlib_Edit_ItemFlag(Disabled),
} lwlib_Edit_ItemFlag_t;

typedef struct lwlib_Edit_Persist_s
{
	lwlib_String_t fileName;
	lwlib_Fs_FileMode_t fileMode;
	bool commit;
} lwlib_Edit_Persist_t;

typedef struct lwlib_Edit_s
{
	void *priv;
} lwlib_Edit_t;

typedef void (*lwlib_Edit_VecInit_t)(void *elem);
typedef void (*lwlib_Edit_VecCleanup_t)(void *elem);
typedef void (*lwlib_Edit_VecAddToEdit_t)(void *elem, lwlib_Edit_t edit);
typedef bool (*lwlib_Edit_ItemChangeNotify_t)(lwlib_Edit_t edit);

typedef lwlib_AnonCallback_t(
	bool (*fn)(void *anon, lwlib_Edit_t edit, const char *itemName)
	) lwlib_Edit_GroupItemUpdate_t;

lwlib_Edit_t lwlib_Edit_New(void);

lwlib_Edit_t lwlib_Edit_NewFs(const char *fileName, lwlib_Fs_FileMode_t fileMode);

lwlib_Edit_t lwlib_Edit_Free(lwlib_Edit_t edit);

void lwlib_Edit_NavigateTop(lwlib_Edit_t edit);

bool lwlib_Edit_NavigateItem(lwlib_Edit_t edit, const char *itemName);

bool lwlib_Edit_NavigateBack(lwlib_Edit_t edit);

void lwlib_Edit_AddItem_int(lwlib_Edit_t edit, const char *itemName, int *value);

void lwlib_Edit_AddItem_short(lwlib_Edit_t edit, const char *itemName, short *value);

void lwlib_Edit_AddItem_uchar(lwlib_Edit_t edit, const char *itemName, unsigned char *value);

void lwlib_Edit_AddItem_double(lwlib_Edit_t edit, const char *itemName, double *value);

void lwlib_Edit_AddItem_string(lwlib_Edit_t edit, const char *itemName, 
	char *value, int valueSize);

void lwlib_Edit_AddItem_bool(lwlib_Edit_t edit, const char *itemName, 
	bool *value);

void lwlib_Edit_AddItem_enum(lwlib_Edit_t edit, const char *itemName,
	int *value, const char *valueNames[], const int *values, int numValues);

void lwlib_Edit_AddItem_group(lwlib_Edit_t edit, const char *itemName);

void lwlib_Edit_AddItem_vec(lwlib_Edit_t edit, const char *itemName, 
	lwlib_VoidVec_t *value, int elemSize, 
	lwlib_Edit_VecInit_t elemInit, lwlib_Edit_VecCleanup_t elemCleanup, 
	lwlib_Edit_VecAddToEdit_t elemAddToEdit);

void lwlib_Edit_AddItem_TPoint3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPoint3f *value);

void lwlib_Edit_AddItem_TPoint2f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPoint2f *value);

void lwlib_Edit_AddItem_TPoint2i(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPoint2i *value);

void lwlib_Edit_AddItem_TPlane3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPlane3f *value);

void lwlib_Edit_AddItem_TBox3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TBox3f *value);

void lwlib_Edit_AddItem_TSphere3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TSphere3f *value);

void lwlib_Edit_AddItem_persist(lwlib_Edit_t edit, lwlib_Edit_Persist_t *persist);

void lwlib_Edit_ClearItems(lwlib_Edit_t edit);

int lwlib_Edit_GetItemFlags(lwlib_Edit_t edit, const char *itemName);

void lwlib_Edit_SetItemFlags(lwlib_Edit_t edit, const char *itemName, int itemFlags);

void lwlib_Edit_SetActive(lwlib_Edit_t edit, bool active);

bool lwlib_Edit_IsActive(lwlib_Edit_t edit);

void lwlib_Edit_SetFs(lwlib_Edit_t edit, lwlib_Fs_t fs);

void lwlib_Edit_AddPersistGroup(lwlib_Edit_t edit, 
	lwlib_Edit_Persist_t *persist);

void lwlib_Edit_SetVecInit(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_VecInit_t elemInit);

void lwlib_Edit_SetVecCleanup(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_VecCleanup_t elemCleanup);

void lwlib_Edit_SetVecAddToEdit(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_VecAddToEdit_t elemAddToEdit);

void lwlib_Edit_SetItemChangeNotify(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_ItemChangeNotify_t itemChangeNotify);

void lwlib_Edit_SetGroupItemUpdate(lwlib_Edit_t edit,
	lwlib_Edit_GroupItemUpdate_t groupItemUpdate);

#endif
