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

#include "lw_edit.h"
#include "lw_strmap.h"
#ifdef LWLIB
#define lwlib_Edit_EditMode(name) lwlib_Edit_EditMode_ ## name
#define lwlib_Edit_ItemType(name) lwlib_Edit_ItemType_ ## name
#define lwlib_Edit_ItemGroupType(name) lwlib_Edit_ItemGroupType_ ## name

#define lwlib_Edit_AccessItem(item, type) (*(item->type ## Value))

#define lwlib_Edit_CursorChar "\x0b"
#define lwlib_Edit_CursorFlashDuration 0.150

#define lwlib_Edit_IndentSize 2

#define LWLIB_EDIT_CHECKGROUPVEC(item) \
	assert( \
		(item) != NULL && \
		(item)->type == lwlib_Edit_ItemType(Group) && \
		(item)->groupType == lwlib_Edit_ItemGroupType(Vec) && \
		(item)->groupVec != NULL && \
		(item)->parentItem != NULL && \
		(item)->parentItem->type == lwlib_Edit_ItemType(Group) && \
		(item)->parentItem->groupType == lwlib_Edit_ItemGroupType(Normal) \
		)

#define LWLIB_EDIT_DEPTHSPACING 4

typedef lwlib_Byte_t byte;

typedef enum lwlib_Edit_EditMode_e
{
	lwlib_Edit_EditMode(Normal),
	lwlib_Edit_EditMode(String),
} lwlib_Edit_EditMode_t;

typedef enum lwlib_Edit_ItemType_e
{
	lwlib_Edit_ItemType(Int),
	lwlib_Edit_ItemType(Short),
	lwlib_Edit_ItemType(UChar),
	lwlib_Edit_ItemType(Double),
	lwlib_Edit_ItemType(String),
	lwlib_Edit_ItemType(Bool),
	lwlib_Edit_ItemType(Enum),
	lwlib_Edit_ItemType(Group),
} lwlib_Edit_ItemType_t;

typedef enum lwlib_Edit_ItemGroupType_s
{
	lwlib_Edit_ItemGroupType(Normal),
	lwlib_Edit_ItemGroupType(TSphere3f),
	lwlib_Edit_ItemGroupType(TPlane3f),
	lwlib_Edit_ItemGroupType(TPoint3f),
	lwlib_Edit_ItemGroupType(TPoint2f),
	lwlib_Edit_ItemGroupType(TPoint2i),
	lwlib_Edit_ItemGroupType(TBox3f),
	lwlib_Edit_ItemGroupType(Vec),
	lwlib_Edit_ItemGroupType(VecElem),
} lwlib_Edit_ItemGroupType_t;

typedef struct lwlib_Edit_Vec_s
{
	lwlib_VoidVec_t *core;
	int elemSize;
	lwlib_Edit_VecInit_t elemInit;
	lwlib_Edit_VecCleanup_t elemCleanup;
	lwlib_Edit_VecAddToEdit_t elemAddToEdit;
} lwlib_Edit_Vec_t;

struct lwlib_Edit_Item_s;
typedef struct lwlib_Edit_Item_s *lwlib_Edit_ItemHandle_t;
typedef vec_t(lwlib_Edit_ItemHandle_t) lwlib_Edit_ItemHandles_t;

typedef struct lwlib_Edit_Item_s
{
	char name[lwlib_StrMap_MaxString];
	lwlib_Edit_ItemType_t type;
	struct lwlib_Edit_Item_s *parentItem;
	int flags; 
	int itemGroupIndex;
	lwlib_Edit_ItemChangeNotify_t itemChangeNotify;
	byte *valueData;
	int valueLength;
	// lwlib_Edit_ItemType(Int)
	int *intValue;
	// lwlib_Edit_ItemType(Short)
	short *shortValue;
	// lwlib_Edit_ItemType(UChar)
	unsigned char *ucharValue;
	// lwlib_Edit_ItemType(Double)
	double *doubleValue;
	// lwlib_Edit_ItemType(String)
	char *stringValue;
	int stringSize;
	// lwlib_Edit_ItemType(Bool)
	bool *boolValue;
	// lwlib_Edit_ItemType(Enum)
	int *enumValue;
	const char **enumValueNames;
	const int *enumValues;
	int enumNumValues;
	// lwlib_Edit_ItemType(Group)
	lwlib_Edit_ItemGroupType_t groupType;
	int groupItemCount;
	lwlib_Edit_ItemHandles_t groupItems;
	lwlib_Edit_Vec_t *groupVec;
	// lwlib_Edit_ItemGroupType(TSphere3f)
	lwlib_TSphere3f *tSphere3fValue;
	// lwlib_Edit_ItemGroupType(TPlane3f)
	lwlib_TPlane3f *tPlane3fValue;
	// lwlib_Edit_ItemGroupType(TPoint3f)
	lwlib_TPoint3f *tPoint3fValue;
	// lwlib_Edit_ItemGroupType(TPoint2f)
	lwlib_TPoint2f *tPoint2fValue;
	// lwlib_Edit_ItemGroupType(TPoint2i)
	lwlib_TPoint2i *tPoint2iValue;
	// lwlib_Edit_ItemGroupType(TBox3f)
	lwlib_TBox3f *tBox3fValue;
} lwlib_Edit_Item_t;

typedef struct lwlib_Edit_CopyRegister_s
{
	lwlib_Edit_ItemType_t type;
	byte *valueData;
	int valueLength;
} lwlib_Edit_CopyRegister_t;

typedef struct lwlib_Edit_Priv_s
{
	lwlib_Edit_Item_t *topItem;
	lwlib_Edit_Item_t *nav;
	vec_t(lwlib_Str_t) groupItemPath;
	lwlib_String_t selectedItemName;
	bool active;
	int cursorCol;
	lwlib_Fs_t fs;
	bool fsOwner;
	lwlib_Edit_EditMode_t editMode;
	lwlib_Edit_GroupItemUpdate_t groupItemUpdate;
	lwlib_Edit_CopyRegister_t copyRegisters[10];
	int scroll;
	lwlib_TPoint2i selectedRange;
	bool gotSelectedRange;
	lwlib_TPoint2i titleRange;
} lwlib_Edit_Priv_t;

static int lwlib_Edit_Item_Index(lwlib_Edit_Item_t *item, const char *itemName)
{
	int i;

	for (i = 0; i < vec_size(item->groupItems); i++)
	{
		if (strcmp(vec_at(item->groupItems, i)->name, itemName) == 0)
		{
			return i;
		}
	}

	return -1;
}

static lwlib_Edit_Item_t *lwlib_Edit_FindItem(
	lwlib_Edit_t edit, const char *itemName)
{
	int itemIndex;
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	assert(priv->nav->type == lwlib_Edit_ItemType(Group));

	item = NULL;
	itemIndex = lwlib_Edit_Item_Index(priv->nav, itemName);

	if (itemIndex != -1)
	{
		item = vec_at(priv->nav->groupItems, itemIndex);
	}

	return item;
}

static bool lwlib_Edit_Item_Exists(lwlib_Edit_Item_t *item, const char *itemName)
{
	return lwlib_Edit_Item_Index(item, itemName) != -1;
}

static void lwlib_Edit_Item_Clear(lwlib_Edit_Item_t *item)
{
	int i;
	lwlib_Edit_Item_t *subItem;

	for (i = 0; i < vec_size(item->groupItems); i++)
	{
		subItem = vec_at(item->groupItems, i);
		lwlib_Edit_Item_Clear(subItem);
		free(subItem);
	}
	vec_free(item->groupItems, lwlib_Edit_ItemHandle_t);
	free(item->groupVec);

	lwlib_Zero(*item);
}

static lwlib_Edit_Item_t *lwlib_Edit_AddItem(lwlib_Edit_t edit, const char *itemName, 
	lwlib_Edit_ItemType_t itemType)
{
	int itemIndex;
	lwlib_Edit_Item_t *newItem;
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = NULL;
	assert(priv->nav->type == lwlib_Edit_ItemType(Group));

	if (lwlib_Edit_Item_Exists(priv->nav, itemName))
	{
		itemIndex = lwlib_Edit_Item_Index(priv->nav, itemName);
		newItem = vec_at(priv->nav->groupItems, itemIndex);
		lwlib_Edit_Item_Clear(newItem);
		lwlib_StrCopy(newItem->name, itemName);
		newItem->type = itemType;
		newItem->parentItem = priv->nav;
		newItem->itemGroupIndex = itemIndex;
	}
	else
	{
		item = lwlib_CallocSingle(lwlib_Edit_Item_t);
		lwlib_StrCopy(item->name, itemName);
		item->type = itemType;
		item->parentItem = priv->nav;
		item->itemGroupIndex = priv->nav->groupItemCount++;
		vec_push(priv->nav->groupItems, lwlib_Edit_ItemHandle_t, item);

		newItem = vec_top(priv->nav->groupItems);
	}

	assert(newItem != NULL);
	return newItem;
}

static lwlib_Edit_Item_t *lwlib_Edit_AddItem_WithValue(lwlib_Edit_t edit, const char *itemName, 
	lwlib_Edit_ItemType_t itemType, void *valueData, int valueLength)
{
	lwlib_Edit_Item_t *newItem;
	newItem = lwlib_Edit_AddItem(edit, itemName, itemType);
	newItem->valueData = (byte *)valueData;
	newItem->valueLength = valueLength;
	return newItem;
}

static void lwlib_Edit_ResetItemSelectors(lwlib_Edit_t edit)
{
	int i;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_StrSeq_t path;
	lwlib_Str_t itemName;

	lwlib_Edit_NavigateTop(edit);
	lwlib_Zero(path);

	for (i = 0; i < vec_size(priv->groupItemPath); i++)
	{
		itemName = vec_at(priv->groupItemPath, i);
		if (lwlib_Edit_NavigateItem(edit, itemName.v))
		{
			vec_push(path, lwlib_Str_t, itemName);
		}
		else
		{
			break;
		}
	}

	vec_free(priv->groupItemPath, lwlib_Str_t);
	lwlib_Copy(priv->groupItemPath, path);

	if (!lwlib_Edit_Item_Exists(priv->nav, priv->selectedItemName))
	{
		lwlib_StrCopy(priv->selectedItemName, "");
		if (vec_size(priv->nav->groupItems) > 0)
		{
			lwlib_StrCopy(priv->selectedItemName, 
				vec_at(priv->nav->groupItems, 0)->name);
		}
	}

	lwlib_Edit_NavigateTop(edit);
}

static bool lwlib_Edit_NavigateItemByPath(lwlib_Edit_t edit, lwlib_StrSeq_t itemPath)
{
	int i;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	for (i = 0; i < vec_size(itemPath); i++)
	{
		if (!lwlib_Edit_NavigateItem(edit, vec_at(itemPath, i).v))
		{
			return false;
		}
	}

	return true;
}

static void lwlib_Edit_Item_DisplayString(lwlib_Edit_t edit, 
	lwlib_Edit_Item_t *item, lwlib_String_t dispStr)
{
	int index;
	int enumValue;
	lwlib_TPlane3f tPlane3fValue;
	lwlib_TSphere3f tSphere3fValue;
	lwlib_String_t groupValueStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	switch (item->type)
	{
	case lwlib_Edit_ItemType(Int):
		lwlib_VaString(dispStr, "%s %d", 
			item->name, lwlib_Edit_AccessItem(item, int));
		break;
	case lwlib_Edit_ItemType(Short):
		lwlib_VaString(dispStr, "%s %d", 
			item->name, (int)lwlib_Edit_AccessItem(item, short));
		break;
	case lwlib_Edit_ItemType(UChar):
		lwlib_VaString(dispStr, "%s %d", 
			item->name, (int)lwlib_Edit_AccessItem(item, uchar));
		break;
	case lwlib_Edit_ItemType(Double):
		lwlib_VaString(dispStr, "%s %.2f", 
			item->name, lwlib_Edit_AccessItem(item, double));
		break;
	case lwlib_Edit_ItemType(String):
		lwlib_VaString(dispStr, "%s \"%s\"", 
			item->name, item->stringValue);
		break;
	case lwlib_Edit_ItemType(Bool):
		lwlib_VaString(dispStr, "%s %s", 
			item->name, lwlib_Edit_AccessItem(item, bool) ? "true" : "false");
		break;
	case lwlib_Edit_ItemType(Enum):
		enumValue = lwlib_Edit_AccessItem(item, enum);
		if (item->enumValues != NULL)
		{
			index = lwlib_IndexOf(enumValue, item->enumValues, 
				item->enumNumValues);
			if (index != -1)
			{
				lwlib_VaString(dispStr, "%s 0x%X:%s", 
					item->name, item->enumValues[index], 
					item->enumValueNames[index]);
			}
			else
			{
				lwlib_VaString(dispStr, "%s", item->name);
			}
		}
		else
		{
			assert(enumValue >= 0 && enumValue < item->enumNumValues);
			lwlib_VaString(dispStr, "%s %d:%s", item->name, 
				enumValue, item->enumValueNames[enumValue]);
		}
		break;
	case lwlib_Edit_ItemType(Group):
		lwlib_StrCopy(groupValueStr, "");
		if (item->groupType == lwlib_Edit_ItemGroupType(TPoint3f))
		{
			lwlib_VaString(groupValueStr, "(%.2f, %.2f, %.2f)", 
				lwlib_ExpandVector(lwlib_Edit_AccessItem(item, tPoint3f)));
			lwlib_VaString(dispStr, "%s %s", item->name, groupValueStr);
		}
		else if (item->groupType == lwlib_Edit_ItemGroupType(TPoint2f))
		{
			lwlib_VaString(groupValueStr, "(%.2f, %.2f)", 
				lwlib_ExpandVector(lwlib_Edit_AccessItem(item, tPoint2f)));
			lwlib_VaString(dispStr, "%s %s", item->name, groupValueStr);
		}
		else if (item->groupType == lwlib_Edit_ItemGroupType(TPoint2i))
		{
			lwlib_VaString(groupValueStr, "(%d, %d)", 
				lwlib_ExpandVector(lwlib_Edit_AccessItem(item, tPoint2i)));
			lwlib_VaString(dispStr, "%s %s", item->name, groupValueStr);
		}
		else if (item->groupType == lwlib_Edit_ItemGroupType(TPlane3f))
		{
			tPlane3fValue = lwlib_Edit_AccessItem(item, tPlane3f);
			lwlib_VaString(groupValueStr, "(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f)", 
				lwlib_ExpandVector(lwlib_plane_origin(tPlane3fValue)),
				lwlib_ExpandVector(lwlib_plane_normal(tPlane3fValue))
				);
			lwlib_VaString(dispStr, "%s %s", item->name, groupValueStr);
		}
		else if (item->groupType == lwlib_Edit_ItemGroupType(TSphere3f))
		{
			tSphere3fValue = lwlib_Edit_AccessItem(item, tSphere3f);
			lwlib_VaString(groupValueStr, "(%.2f, %.2f, %.2f) %.2f", 
				lwlib_ExpandVector(tSphere3fValue.origin),
				tSphere3fValue.radius
				);
			lwlib_VaString(dispStr, "%s %s", item->name, groupValueStr);
		}
		else if (item->groupType == lwlib_Edit_ItemGroupType(Vec))
		{
			LWLIB_EDIT_CHECKGROUPVEC(item);
			lwlib_VaString(dispStr, "%s[%d]", item->name, 
				vec_size(*(item->groupVec->core)));
		}
		else if (item->groupType == lwlib_Edit_ItemGroupType(VecElem))
		{
			lwlib_VaString(dispStr, "%s", item->name);
		}
		else
		{
			lwlib_VaString(dispStr, "%s", item->name);
		}

		break;
	}
}

static bool lwlib_Edit_Item_Selected(lwlib_Edit_t edit, lwlib_Edit_Item_t *item)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	return strcmp(item->name, priv->selectedItemName) == 0 ||
		(priv->selectedItemName[0] == '\0' && item->itemGroupIndex == 0);
}

static int lwlib_Edit_Item_CursorCol(lwlib_Edit_t edit, lwlib_Edit_Item_t *item)
{
	int cursorCol;
	int intValue;
	double doubleValue;
	lwlib_String_t tmpStr;
	lwlib_TPoint2i colRange;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	if (!lwlib_Edit_Item_Selected(edit, item))
	{
		return -1;
	}

	cursorCol = -1;
	switch (item->type)
	{
	case lwlib_Edit_ItemType(Int):
		intValue = lwlib_Edit_AccessItem(item, int);
		lwlib_Snprintf(tmpStr, "%d", intValue);
		colRange = lwlib_vec2i(intValue < 0 ? 1 : 0, strlen(tmpStr));
		cursorCol = lwlib_CLIP(priv->cursorCol, lwlib_X(colRange), lwlib_Y(colRange) - 1);
		break;
	case lwlib_Edit_ItemType(Short):
		intValue = (int)lwlib_Edit_AccessItem(item, short);
		lwlib_Snprintf(tmpStr, "%d", intValue);
		colRange = lwlib_vec2i(intValue < 0 ? 1 : 0, strlen(tmpStr));
		cursorCol = lwlib_CLIP(priv->cursorCol, lwlib_X(colRange), lwlib_Y(colRange) - 1);
		break;
	case lwlib_Edit_ItemType(UChar):
		intValue = (int)lwlib_Edit_AccessItem(item, uchar);
		lwlib_Snprintf(tmpStr, "%d", intValue);
		colRange = lwlib_vec2i(0, strlen(tmpStr));
		cursorCol = lwlib_CLIP(priv->cursorCol, lwlib_X(colRange), lwlib_Y(colRange) - 1);
		break;
	case lwlib_Edit_ItemType(Double):
		doubleValue = lwlib_Edit_AccessItem(item, double);
		lwlib_Snprintf(tmpStr, "%.2f", doubleValue);
		colRange = lwlib_vec2i(doubleValue < 0.0 ? 1 : 0, strlen(tmpStr));
		cursorCol = lwlib_CLIP(priv->cursorCol, lwlib_X(colRange), lwlib_Y(colRange) - 1);
		break;
	case lwlib_Edit_ItemType(String):
		cursorCol = lwlib_CLIP(priv->cursorCol, 0, strlen(item->stringValue));
		break;
	case lwlib_Edit_ItemType(Bool):
		cursorCol = 0;
		break;
	case lwlib_Edit_ItemType(Enum):
		cursorCol = 0;
		break;
	case lwlib_Edit_ItemType(Group):
		break;
	}

	return cursorCol;
}

static const char *lwlib_Edit_NavItemDesc(lwlib_Edit_t edit, lwlib_String_t navItemDesc)
{
	lwlib_Edit_Item_t *item;
	lwlib_String_t tmpStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	item = priv->nav;
	lwlib_VaString(navItemDesc, "%s", item->name);

	while (item->parentItem != NULL && item->parentItem != priv->topItem)
	{
		item = item->parentItem;
		lwlib_VaString(tmpStr, "%s -> %s", item->name, navItemDesc);
		lwlib_VaString(navItemDesc, "%s", tmpStr);
	}

	return navItemDesc;
}

static int lwlib_Edit_NavItemDepth(lwlib_Edit_t edit)
{
	int depth;
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	depth = 0;
	item = priv->nav;

	while (item->parentItem != NULL)
	{
		depth++;
		item = item->parentItem;
	}

	return depth;
}

static const char *lwlib_Edit_NavItemDepthSpacing(lwlib_Edit_t edit, lwlib_String_t depthStr)
{
	int depth;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	depth = lwlib_Edit_NavItemDepth(edit);
	depth *= LWLIB_EDIT_DEPTHSPACING;
	depth = lwlib_MIN(depth, lwlib_StringSize - 1);
	memset(depthStr, ' ', depth);
	depthStr[depth] = '\0';

	return depthStr;
}

static void lwlib_Edit_CycleSelectedItem(lwlib_Edit_t edit, int cycle)
{
	lwlib_Edit_Item_t *selectedItem, *nextItem;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	selectedItem = vec_at(priv->nav->groupItems, 
		lwlib_Edit_Item_Index(priv->nav, priv->selectedItemName));
	assert(lwlib_BitOff(selectedItem->flags, lwlib_Edit_ItemFlag(Disabled)));
	
	do
	{
		selectedItem = vec_at(priv->nav->groupItems, (selectedItem->itemGroupIndex + 
			cycle + priv->nav->groupItemCount) % priv->nav->groupItemCount);
	}
	while (lwlib_BitOn(selectedItem->flags, lwlib_Edit_ItemFlag(Disabled)));

	lwlib_StrCopy(priv->selectedItemName, selectedItem->name);
}

lwlib_Edit_t lwlib_Edit_New(void)
{
	lwlib_Edit_t edit;

	lwlib_Zero(edit);
	edit.priv = lwlib_CallocSingle(lwlib_Edit_Priv_t);

	lwlib_GetPriv(lwlib_Edit_Priv_t, edit);
	priv->topItem = lwlib_CallocSingle(lwlib_Edit_Item_t);
	lwlib_StrCopy(priv->topItem->name, "top");
	priv->topItem->type = lwlib_Edit_ItemType(Group);
	priv->nav = priv->topItem;

	return edit;
}

lwlib_Edit_t lwlib_Edit_NewFs(const char *fileName, lwlib_Fs_FileMode_t fileMode)
{
	lwlib_Edit_t edit;
	lwlib_Fs_t fs;

	edit = lwlib_Edit_New();

	fs = lwlib_Fs_New(fileName, fileMode);
	lwlib_Edit_SetFs(edit, fs);

	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	priv->fsOwner = true;

	return edit;
}

lwlib_Edit_t lwlib_Edit_Free(lwlib_Edit_t edit)
{
	int i;
	lwlib_GetPriv(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_ClearItems(edit);
	if (priv)
	{
		if (priv->fsOwner)
		{
			lwlib_Fs_Free(priv->fs);
		}
		lwlib_Edit_Item_Clear(priv->topItem);
		free(priv->topItem);
		vec_free(priv->groupItemPath, lwlib_Str_t);
		for (i = 0; i < lwlib_CountArray(priv->copyRegisters); i++)
		{
			free(priv->copyRegisters[i].valueData);
		}
		free(priv);
	}
	lwlib_Zero(edit);
	return edit;
}

void lwlib_Edit_NavigateTop(lwlib_Edit_t edit)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	priv->nav = priv->topItem;
}

bool lwlib_Edit_NavigateItem(lwlib_Edit_t edit, const char *itemName)
{
	bool found = false;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	if (priv->nav->type == lwlib_Edit_ItemType(Group) && 
		lwlib_Edit_Item_Exists(priv->nav, itemName))
	{
		priv->nav = vec_at(priv->nav->groupItems, 
			lwlib_Edit_Item_Index(priv->nav, itemName));
		found = true;
	}

	return found;
}

bool lwlib_Edit_NavigateBack(lwlib_Edit_t edit)
{
	bool found = false;
	lwlib_Edit_Item_t *oldNav;
	lwlib_String_t tmpStr, navItemDesc, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	oldNav = priv->nav;

	if (priv->nav->parentItem != NULL)
	{
		priv->nav = priv->nav->parentItem;
		found = true;
	}

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitBlock(priv->fs, NULL, 0, lwlib_Fs_VarType_Informative,
		lwlib_VaString(tmpStr, "%s[%s]", depthStr, oldNav->name));

	return found;
}

void lwlib_Edit_AddItem_int(lwlib_Edit_t edit, const char *itemName, int *value)
{
	lwlib_Edit_Item_t *newItem;
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(Int),
		value, sizeof(int));
	newItem->intValue = value;

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitVar(priv->fs, value, lwlib_Fs_VarType_Int, 
		lwlib_VaString(tmpStr, "%s%s", depthStr, itemName));
}

void lwlib_Edit_AddItem_short(lwlib_Edit_t edit, const char *itemName, short *value)
{
	lwlib_Edit_Item_t *newItem;
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(Short), 
		value, sizeof(short));
	newItem->shortValue = value;

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitVar(priv->fs, value, lwlib_Fs_VarType_Short, 
		lwlib_VaString(tmpStr, "%s%s", depthStr, itemName));
}

void lwlib_Edit_AddItem_uchar(lwlib_Edit_t edit, const char *itemName, unsigned char *value)
{
	lwlib_Edit_Item_t *newItem;
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(UChar), 
		value, sizeof(unsigned char));
	newItem->ucharValue = value;

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitVar(priv->fs, value, lwlib_Fs_VarType_UChar, 
		lwlib_VaString(tmpStr, "%s%s", depthStr, itemName));
}

void lwlib_Edit_AddItem_double(lwlib_Edit_t edit, const char *itemName, double *value)
{
	lwlib_Edit_Item_t *newItem;
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(Double), 
		value, sizeof(double));
	newItem->doubleValue = value;

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitVar(priv->fs, value, lwlib_Fs_VarType_Double,
		lwlib_VaString(tmpStr, "%s%s", depthStr, itemName));
}

void lwlib_Edit_AddItem_string(lwlib_Edit_t edit, const char *itemName, 
	char *value, int valueSize)
{
	int len;
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_Item_t *newItem;

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(String),
		value, valueSize);
	newItem->stringValue = value;
	newItem->stringSize = valueSize;

	if (lwlib_Fs_Valid(priv->fs) &&
		lwlib_Fs_GetFileMode(priv->fs) == lwlib_Fs_FileMode(ReadText))
	{
		len = valueSize;
	}
	else
	{
		len = strlen(value);
	}

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitBlock(priv->fs, value, len, lwlib_Fs_VarType_String,
		lwlib_VaString(tmpStr, "%s%s", depthStr, itemName));
	value[len] = '\0';
}

void lwlib_Edit_AddItem_bool(lwlib_Edit_t edit, const char *itemName, 
	bool *value)
{
	lwlib_Edit_Item_t *newItem;
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(Bool),
		value, sizeof(bool));
	newItem->boolValue = value;
	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitVar(priv->fs, value, lwlib_Fs_VarType_Bool, 
		lwlib_VaString(tmpStr, "%s%s", depthStr, itemName));
}

void lwlib_Edit_AddItem_enum(lwlib_Edit_t edit, const char *itemName,
	int *value, const char *valueNames[], const int *values, int numValues)
{
	int index;
	lwlib_Edit_Item_t *newItem;
	lwlib_String_t dispStr, tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	newItem = lwlib_Edit_AddItem_WithValue(edit, itemName, lwlib_Edit_ItemType(Enum),
		value, sizeof(int));
	newItem->enumValue = value;
	newItem->enumValueNames = valueNames;
	newItem->enumValues = values;
	newItem->enumNumValues = numValues;

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Edit_Item_DisplayString(edit, newItem, dispStr);
	lwlib_Fs_CommitVar(priv->fs, value, lwlib_Fs_VarType_Int, 
		lwlib_VaString(tmpStr, "%s%s", depthStr, dispStr));
}

void lwlib_Edit_AddItem_group(lwlib_Edit_t edit, const char *itemName)
{
	lwlib_String_t tmpStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitBlock(priv->fs, NULL, 0, lwlib_Fs_VarType_Informative,
		lwlib_VaString(tmpStr, "%s[%s]", depthStr, itemName));

	lwlib_Edit_AddItem(edit, itemName, lwlib_Edit_ItemType(Group));
	lwlib_Edit_NavigateItem(edit, itemName);
}

void lwlib_Edit_AddItem_vec(lwlib_Edit_t edit, const char *itemName, 
	lwlib_VoidVec_t *value, int elemSize, lwlib_Edit_VecInit_t elemInit,
	lwlib_Edit_VecCleanup_t elemCleanup,
	lwlib_Edit_VecAddToEdit_t elemAddToEdit)
{
	int i;
	int sz;
	lwlib_Edit_Item_t *vecItem, *newItem;
	lwlib_String_t tmpStr, descStr, depthStr;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	assert(value != NULL && elemSize != 0);

	lwlib_Edit_AddItem_group(edit, itemName);
	lwlib_Edit_AddItem_group(edit, "elem");

	newItem = priv->nav;
	newItem->flags = lwlib_Edit_ItemFlagMask(GroupExpand);
	newItem->groupType = lwlib_Edit_ItemGroupType(Vec);
	newItem->groupVec = lwlib_CallocSingle(lwlib_Edit_Vec_t);
	newItem->groupVec->core = value;
	newItem->groupVec->elemSize = elemSize;
	newItem->groupVec->elemInit = elemInit;
	newItem->groupVec->elemCleanup = elemCleanup;
	newItem->groupVec->elemAddToEdit = elemAddToEdit;

	sz = value->sz;
	lwlib_Snprintf(descStr, "%s:vecsize", itemName);
	lwlib_Edit_NavItemDepthSpacing(edit, depthStr);
	lwlib_Fs_CommitVar(priv->fs, &sz, lwlib_Fs_VarType_Int, 
		lwlib_VaString(tmpStr, "%s%s", depthStr, descStr));

	// handle growing or shrinking vector
	if (sz != value->sz)
	{
		// clean up elements before deleting
		if (sz < value->sz)
		{
			if (elemCleanup != NULL)
			{
				for (i = sz; i < value->sz; i++)
				{
					elemCleanup((byte *)value->data + (elemSize * i));
				}
			}
		}

		// resize vector
		value->data = vecI_reserve(elemSize, sz, 
			&value->alloc, value->data);

		// init new elements
		if (sz > value->sz)
		{
			memset((byte *)value->data + (elemSize * value->sz), 
				0, elemSize * (sz - value->sz));
			if (elemInit != NULL)
			{
				for (i = value->sz; i < sz; i++)
				{
					elemInit((byte *)value->data + (elemSize * i));
				}
			}
		}

		value->sz = sz;
	}

	for (i = 0; i < value->sz; i++)
	{
		lwlib_Snprintf(tmpStr, "%s:%d", itemName, i);
		lwlib_Edit_AddItem_group(edit, tmpStr);
		priv->nav->groupType = lwlib_Edit_ItemGroupType(VecElem);

		if (elemAddToEdit != NULL)
		{
			elemAddToEdit((byte *)value->data + (elemSize * i), edit);
		}

		lwlib_Edit_NavigateBack(edit);
	}

	lwlib_Edit_NavigateBack(edit); // leaving "elem"
	lwlib_Edit_NavigateBack(edit); // leaving "<itemName>"
}

void lwlib_Edit_AddItem_TPoint3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPoint3f *value)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_AddItem_group(edit, itemName);
	priv->nav->groupType = lwlib_Edit_ItemGroupType(TPoint3f);
	priv->nav->tPoint3fValue = value;
	lwlib_Edit_AddItem_double(edit, "x", &value->v[0]);
	lwlib_Edit_AddItem_double(edit, "y", &value->v[1]);
	lwlib_Edit_AddItem_double(edit, "z", &value->v[2]);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_AddItem_TPoint2f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPoint2f *value)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_AddItem_group(edit, itemName);
	priv->nav->groupType = lwlib_Edit_ItemGroupType(TPoint2f);
	priv->nav->tPoint2fValue = value;
	lwlib_Edit_AddItem_double(edit, "x", &value->v[0]);
	lwlib_Edit_AddItem_double(edit, "y", &value->v[1]);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_AddItem_TPoint2i(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPoint2i *value)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_AddItem_group(edit, itemName);
	priv->nav->groupType = lwlib_Edit_ItemGroupType(TPoint2i);
	priv->nav->tPoint2iValue = value;
	lwlib_Edit_AddItem_int(edit, "x", &value->v[0]);
	lwlib_Edit_AddItem_int(edit, "y", &value->v[1]);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_AddItem_TPlane3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TPlane3f *value)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_AddItem_group(edit, itemName);
	priv->nav->groupType = lwlib_Edit_ItemGroupType(TPlane3f);
	priv->nav->tPlane3fValue = value;
	lwlib_Edit_AddItem_TPoint3f(edit, "origin", &value->p[0]);
	lwlib_Edit_AddItem_TPoint3f(edit, "normal", &value->p[1]);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_AddItem_lwlib_TBox3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TBox3f *value)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_AddItem_group(edit, itemName);
	priv->nav->groupType = lwlib_Edit_ItemGroupType(TBox3f);
	priv->nav->tBox3fValue = value;
	lwlib_Edit_AddItem_TPoint3f(edit, "lower", &value->p[0]);
	lwlib_Edit_AddItem_TPoint3f(edit, "upper", &value->p[1]);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_AddItem_TSphere3f(lwlib_Edit_t edit, const char *itemName,
	lwlib_TSphere3f *value)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	lwlib_Edit_AddItem_group(edit, itemName);
	priv->nav->groupType = lwlib_Edit_ItemGroupType(TSphere3f);
	priv->nav->tSphere3fValue = value;
	lwlib_Edit_AddItem_TPoint3f(edit, "origin", &value->origin);
	lwlib_Edit_AddItem_double(edit, "radius", &value->radius);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_ClearItems(lwlib_Edit_t edit)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	lwlib_Edit_Item_Clear(priv->topItem);
	lwlib_StrCopy(priv->topItem->name, "top");
	priv->topItem->type = lwlib_Edit_ItemType(Group);
}

int lwlib_Edit_GetItemFlags(lwlib_Edit_t edit, const char *itemName)
{
	int itemFlags;
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	itemFlags = 0;

	item = lwlib_Edit_FindItem(edit, itemName);
	if (item != NULL)
	{
		itemFlags = item->flags;
	}

	return itemFlags;
}

void lwlib_Edit_SetItemFlags(lwlib_Edit_t edit, const char *itemName, int itemFlags)
{
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	item = lwlib_Edit_FindItem(edit, itemName);
	if (item != NULL)
	{
		item->flags = itemFlags;
	}
}

void lwlib_Edit_SetActive(lwlib_Edit_t edit, bool active)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	priv->active = active;
}

bool lwlib_Edit_IsActive(lwlib_Edit_t edit)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	return priv->active;
}

void lwlib_Edit_SetFs(lwlib_Edit_t edit, lwlib_Fs_t fs)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	priv->fs = fs;
}

void lwlib_Edit_AddPersistGroup(lwlib_Edit_t edit, 
	lwlib_Edit_Persist_t *persist)
{
	lwlib_Edit_AddItem_group(edit, "persist");
	lwlib_Edit_AddItem_string(edit, "fileName", 
		persist->fileName, sizeof(persist->fileName));
	lwlib_Edit_AddItem_enum(edit, "fileMode",
		(int *)&persist->fileMode, lwlib_Fs_FileMode_Str,
		NULL, lwlib_Fs_FileMode_StrCount);
	lwlib_Edit_AddItem_bool(edit, "commit", &persist->commit);
	lwlib_Edit_NavigateBack(edit);
}

void lwlib_Edit_SetVecInit(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_VecInit_t elemInit)
{
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	item = lwlib_Edit_FindItem(edit, itemName);
	if (item != NULL)
	{
		item->groupVec->elemInit = elemInit;
	}
}

void lwlib_Edit_SetVecCleanup(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_VecCleanup_t elemCleanup)
{
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	item = lwlib_Edit_FindItem(edit, itemName);
	if (item != NULL)
	{
		item->groupVec->elemCleanup = elemCleanup;
	}
}

void lwlib_Edit_SetVecAddToEdit(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_VecAddToEdit_t elemAddToEdit)
{
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	item = lwlib_Edit_FindItem(edit, itemName);
	if (item != NULL)
	{
		item->groupVec->elemAddToEdit = elemAddToEdit;
	}
}

void lwlib_Edit_SetItemChangeNotify(lwlib_Edit_t edit, const char *itemName,
	lwlib_Edit_ItemChangeNotify_t itemChangeNotify)
{
	lwlib_Edit_Item_t *item;
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);

	item = lwlib_Edit_FindItem(edit, itemName);
	if (item != NULL)
	{
		item->itemChangeNotify = itemChangeNotify;
	}
}

void lwlib_Edit_SetGroupItemUpdate(lwlib_Edit_t edit,
	lwlib_Edit_GroupItemUpdate_t groupItemUpdate)
{
	lwlib_GetPrivAssert(lwlib_Edit_Priv_t, edit);
	priv->groupItemUpdate = groupItemUpdate;
}
#endif