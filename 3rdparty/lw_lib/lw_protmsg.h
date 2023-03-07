/*************************************************************************
** LinuxWolf Protocol
** Copyright (C) 2009-2010 by Wolfy
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
** Protocol Messages
*************************************************************************/

#ifndef LW_PROT_MSG_H
#define LW_PROT_MSG_H

#include <stdio.h>
#include "lw_vec.h"
#ifdef LWLIB

#define PROT_MSG_LIGHTMAP_WIDTH 2048
#define PROT_MSG_LIGHTMAP_HEIGHT 2048
#define PROT_MSG_LIGHTMAP_PIXELS \
	(PROT_MSG_LIGHTMAP_WIDTH * PROT_MSG_LIGHTMAP_HEIGHT)

typedef	enum ProtBool_e
{
	PROT_FALSE,
	PROT_TRUE
} ProtBool_t;

typedef	enum ProtStatus_e
{
	PROT_STATUS_OK,
	PROT_STATUS_SIGNATURE_INVALID,
	PROT_STATUS_READ_FAILED,
	PROT_STATUS_WRITE_FAILED,
	PROT_STATUS_OPEN_FAILED,
} ProtStatus_t;

typedef struct ProtMsgLightFacet_s
{
	unsigned long x, y;
	unsigned long qn;
	unsigned long lm; // lightmap index
	unsigned long off;
	unsigned long nu, nv;
	unsigned long lightDoorStart;
	unsigned long lightDoorCount;
	unsigned long lightDoorMask;
	double spriteLitLevel[3];
} ProtMsgLightFacet_t;

typedef ProtMsgLightFacet_t ProtMsgLightFacetSeq_t;

typedef struct ProtMsgLightDoor_s
{
	unsigned long doorNum;
} ProtMsgLightDoor_t;

typedef ProtMsgLightDoor_t ProtMsgLightDoorSeq_t;

typedef struct ProtMsgLightInfo_s
{
	unsigned long numLightmaps;
	unsigned long lightmapWidth;
	unsigned long lightmapHeight;
} ProtMsgLightInfo_t;

typedef ProtMsgLightInfo_t ProtMsgLightInfoSeq_t;

typedef struct ProtMsgLightMap_s
{
	uint32_t data[PROT_MSG_LIGHTMAP_PIXELS];
} ProtMsgLightMap_t;

typedef ProtMsgLightMap_t ProtMsgLightMapSeq_t;

typedef struct ProtMsgList_s
{
	ProtMsgLightFacetSeq_t lightFacetSeq;
	ProtMsgLightDoorSeq_t lightDoorSeq;
	ProtMsgLightInfoSeq_t lightInfoSeq;
	ProtMsgLightMapSeq_t lightMapSeq;
} ProtMsgList_t;

ProtMsgList_t protMsgNewList(void);

void protMsgFreeList(ProtMsgList_t *protMsgList);

ProtStatus_t protMsgReadFile(ProtMsgList_t *protMsgList, FILE *fp);

ProtStatus_t protMsgReadFileOpen(ProtMsgList_t *protMsgList, 
	const char *fname);

ProtStatus_t protMsgWriteFile(const ProtMsgList_t *protMsgList, FILE *fp);

ProtStatus_t protMsgWriteFileCreate(const ProtMsgList_t *protMsgList,
	const char *fname);

#endif
#endif
