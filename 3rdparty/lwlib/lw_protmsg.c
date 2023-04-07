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

#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "lw_protmsg.h"

#define FILE_SIGNATURE "LWOLF"
#define FILE_SIGNATURE_LENGTH 6

#define READ(var)                               \
do                                              \
{                                               \
	int ret = fread(&var, 1, sizeof(var), fp);  \
	if (ret != sizeof(var))                     \
	{                                           \
		status = PROT_STATUS_READ_FAILED;       \
		goto error;                             \
	}                                           \
} while (0)

#define WRITE(var)                              \
do                                              \
{                                               \
	int ret = fwrite(&var, 1, sizeof(var), fp); \
	if (ret != sizeof(var))                     \
	{                                           \
		status = PROT_STATUS_WRITE_FAILED;      \
		goto error;                             \
	}                                           \
} while (0)

#define WRITE_ID(val)                           \
do                                              \
{                                               \
	unsigned long id = val;                     \
	WRITE(id);                                  \
} while (0)

#define IMPL_READ(name, type) \
	case PROT_MSG_ID_ ## name: \
	{ \
		type *name ## Elem = \
			lwlib_CallocSingle(type); \
		READ(*name ## Elem); \
		vec_push(protMsgList->name, type, *name ## Elem); \
		free(name ## Elem); \
		break; \
	}

#define IMPL_WRITE(name)                                            \
	for (i = 0; i < vec_size(protMsgList->name); i++)             \
	{                                                               \
		WRITE_ID(PROT_MSG_ID_ ## name);                             \
		WRITE(vec_at(protMsgList->name, i));                   \
	}

enum ProtMsgId_e
{
	PROT_MSG_ID_EOF,
	PROT_MSG_ID_lightFacetSeq,
	PROT_MSG_ID_lightDoorSeq,
	PROT_MSG_ID_lightInfoSeq,
	PROT_MSG_ID_lightMapSeq,
	PROT_MSG_ID_MAX,
};

ProtMsgList_t protMsgNewList(void)
{
	static ProtMsgList_t protMsgList;
	return protMsgList;
}

void protMsgFreeList(ProtMsgList_t *protMsgList)
{
	vec_free(protMsgList->lightFacetSeq, ProtMsgLightFacet_t);
	vec_free(protMsgList->lightDoorSeq, ProtMsgLightDoor_t);
	vec_free(protMsgList->lightInfoSeq, ProtMsgLightInfo_t);
	vec_free(protMsgList->lightMapSeq, ProtMsgLightMap_t);
	*protMsgList = protMsgNewList();
}

ProtStatus_t protMsgReadFile(ProtMsgList_t *protMsgList, FILE *fp)
{
	ProtStatus_t status = PROT_STATUS_OK;
	unsigned long id = PROT_MSG_ID_MAX;
	char signature[FILE_SIGNATURE_LENGTH];
	
	READ(signature);
	if (strncmp(signature, FILE_SIGNATURE, FILE_SIGNATURE_LENGTH) != 0)
	{
		status = PROT_STATUS_SIGNATURE_INVALID;
		goto error;
	}

	while (id != PROT_MSG_ID_EOF)
	{
		READ(id);
		switch (id)
		{
		IMPL_READ(lightFacetSeq, ProtMsgLightFacet_t)
		IMPL_READ(lightDoorSeq, ProtMsgLightDoor_t)
		IMPL_READ(lightInfoSeq, ProtMsgLightInfo_t)
		IMPL_READ(lightMapSeq, ProtMsgLightMap_t)
		case PROT_MSG_ID_EOF:
			break;
		}
	}

	return status;

error:
	protMsgFreeList(protMsgList);
	return status;
}

ProtStatus_t protMsgReadFileOpen(ProtMsgList_t *protMsgList, 
	const char *fname)
{
	ProtStatus_t status;
	FILE *fp;

	fp = fopen(fname, "rb");
	if (fp != NULL)
	{
		status = protMsgReadFile(protMsgList, fp);
		fclose(fp);
	}
	else
	{
		status = PROT_STATUS_OPEN_FAILED;
	}

	return status;
}

ProtStatus_t protMsgWriteFile(const ProtMsgList_t *protMsgList, FILE *fp)
{
	int i;
	ProtStatus_t status = PROT_STATUS_OK;

	WRITE(FILE_SIGNATURE);
	IMPL_WRITE(lightFacetSeq)
	IMPL_WRITE(lightDoorSeq)
	IMPL_WRITE(lightInfoSeq)
	IMPL_WRITE(lightMapSeq)
	WRITE_ID(PROT_MSG_ID_EOF);
	
	return status;

error:
	return status;
}

ProtStatus_t protMsgWriteFileCreate(const ProtMsgList_t *protMsgList,
	const char *fname)
{
    ProtStatus_t status;
    FILE *fp;

	fp = fopen(fname, "wb");
	if (fp != NULL)
	{
		status = protMsgWriteFile(protMsgList, fp);
		fclose(fp);
	}
	else
	{
		status = PROT_STATUS_OPEN_FAILED;
	}

	return status;
}
