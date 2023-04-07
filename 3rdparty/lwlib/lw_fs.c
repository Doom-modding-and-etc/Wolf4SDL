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

#include <stdio.h>
#include "lw_fs.h"
#include "lw_misc.h"

// read in blocks of 64k
#define	LWLIB_MAX_READ 0x10000

#define LWLIB_FIELD_SPACE 20

#define LWLIB_QUOTECHAR '"'

typedef lwlib_Byte_t byte;

typedef struct lwlib_Fs_Priv_s
{
	FILE *fp;
	lwlib_Fs_FileMode_t fileMode;
	lwlib_Byte_t *data;
	int length;
} lwlib_Fs_Priv_t;

lwlib_EnumStr(
	lwlib_Fs_FileMode,
	"Read",
	"Write",
	"ReadText",
	"WriteText",
);

static int lwlib_GetFileLength(FILE *fp)
{
	int pos, end;

	if (fp == NULL)
	{
		return 0;
	}

	pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	end = ftell(fp);
	fseek(fp, pos, SEEK_SET);

	return end;
}

static boolean lwlib_ReadFile(void *buffer, int len, FILE *fp)
{
	int block, remaining, read;
	unsigned char *buf;

	buf = (unsigned char *)buffer;

	// read in chunks for progress bar
	remaining = len;
	while (remaining)
	{
		block = remaining;
		if(block > LWLIB_MAX_READ) 
			block = LWLIB_MAX_READ;
		read = fread(buf, 1, block, fp);

		if(read <= 0)
		{
			return false;
		}

		remaining -= read;
		buf += read;
	}

	return true;
}

static void lwlib_Fs_ConsumeLine(lwlib_Fs_t fs, lwlib_String_t s)
{
	int c, l;
	boolean more;
	lwlib_GetPrivAssert(lwlib_Fs_Priv_t, fs);

	l = 0;
	do
	{
		c = fgetc(priv->fp);
		more = (c != EOF && (char)c != '\n');

		if (s != NULL && l < lwlib_StringSize - 1 && more)
		{
			s[l] = c;
			l++;
		}
	} while (more);

	if (s != NULL)
	{
		s[l] = '\0';
	}
}

lwlib_Fs_t lwlib_Fs_New(const char *fileName, lwlib_Fs_FileMode_t fileMode)
{
	lwlib_Fs_t fs;
	FILE *fp;
	const char *mode;
	lwlib_Zero(fs);

	lwlib_GetPriv(lwlib_Fs_Priv_t, fs);
	priv = lwlib_CallocSingle(lwlib_Fs_Priv_t);

	mode = "";
	switch (fileMode)
	{
	case lwlib_Fs_FileMode(Read):
		mode = "rb";
		break;
	case lwlib_Fs_FileMode(Write):
		mode = "wb";
		break;
	case lwlib_Fs_FileMode(ReadText):
		mode = "r";
		break;
	case lwlib_Fs_FileMode(WriteText):
		mode = "w";
		break;
	}

	fp = fopen(fileName, mode);
	if (fp == NULL)
	{
		free(priv);
		return fs;
	}

	priv->fp = fp;
	priv->fileMode = fileMode;

	fs.priv = priv;
	return fs;
}

lwlib_Fs_t lwlib_Fs_Free(lwlib_Fs_t fs)
{
	lwlib_GetPriv(lwlib_Fs_Priv_t, fs);
	if (priv)
	{
		assert(priv->fp != NULL);
		fclose(priv->fp);
		free(priv);
	}
	lwlib_Zero(fs);
	return fs;
}

void lwlib_Fs_Commit(lwlib_Fs_t fs, void *buf, int count, 
	lwlib_Fs_VarType_t varType, const char *desc)
{
    char ch;
	int ret;
	int *bufInt;
	double *bufDouble;
	boolean *bufBool;
	short *bufShort;
	unsigned char *bufUChar;
	char *tmpStrPtr;
	lwlib_String_t tmpStr;
	lwlib_GetPrivAssert(lwlib_Fs_Priv_t, fs);

	switch (priv->fileMode)
	{
	case lwlib_Fs_FileMode(Read):
		ret = fread(buf, count, 1, priv->fp);
		if (ret == 0)
		{
			memset((byte *)buf, 0, count);
		}
		break;
	case lwlib_Fs_FileMode(Write):
		fwrite(buf, count, 1, priv->fp);
		break;
	case lwlib_Fs_FileMode(ReadText):
		switch (varType)
		{
		case lwlib_Fs_VarType_Int:
			bufInt = (int *)buf;
			ret = fscanf(priv->fp, "%d", bufInt);
			if (ret != 1)
			{
				*bufInt = 0;
			}
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			break;
		case lwlib_Fs_VarType_Short:
			bufShort = (short *)buf;
			ret = fscanf(priv->fp, "%hd", bufShort);
			if (ret != 1)
			{
				*bufShort = 0;
			}
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			break;
		case lwlib_Fs_VarType_UChar:
			bufUChar = (unsigned char *)buf;
			ret = fscanf(priv->fp, "%hhd", bufUChar);
			if (ret != 1)
			{
				*bufUChar = 0;
			}
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			break;
		case lwlib_Fs_VarType_Double:
			bufDouble = (double *)buf;

			ret = fscanf(priv->fp, "%lf", bufDouble);
			if (ret != 1)
			{
				*bufDouble = 0.0;
			}
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			break;
		case lwlib_Fs_VarType_Bool:
			bufBool = (boolean *)buf;
			*bufBool = false;

			lwlib_Fs_ConsumeLine(fs, tmpStr);
			tmpStrPtr = strchr(tmpStr, ' ');
			if (tmpStrPtr != NULL)
			{
				*tmpStrPtr = '\0';
				if (strcmp(tmpStr, "true") == 0)
				{
					*bufBool = true;
				}
			}
			break;
		case lwlib_Fs_VarType_Raw:
			ret = fread(buf, count, 1, priv->fp);
			if (ret == 0)
			{
				memset((byte *)buf, 0, count);
			}
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			break;
		case lwlib_Fs_VarType_String:
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			tmpStrPtr = tmpStr[0] == '"' ? strchr(tmpStr + 1, '"') : NULL;
			if (tmpStrPtr != NULL)
			{
				*tmpStrPtr = '\0';
				strncpy((char *)buf, tmpStr + 1, count - 1);
				((char *)buf)[count - 1] = '\0';
			}
			else
			{
				memset((byte *)buf, 0, count);
			}
			break;
		case lwlib_Fs_VarType_Informative:
			lwlib_Fs_ConsumeLine(fs, tmpStr);
			break;
		}
		break;
	case lwlib_Fs_FileMode(WriteText):
		switch (varType)
		{
		case lwlib_Fs_VarType_Int:
			bufInt = (int *)buf;
			lwlib_Snprintf(tmpStr, "%%-%dd %s\r\n", LWLIB_FIELD_SPACE, desc);
			fprintf(priv->fp, tmpStr, *bufInt);
			break;
		case lwlib_Fs_VarType_Short:
			bufShort = (short *)buf;
			lwlib_Snprintf(tmpStr, "%%-%dhd %s\r\n", LWLIB_FIELD_SPACE, desc);
			fprintf(priv->fp, tmpStr, *bufShort);
			break;
		case lwlib_Fs_VarType_UChar:
			bufUChar = (unsigned char *)buf;
			lwlib_Snprintf(tmpStr, "%%-%dhhd %s\r\n", LWLIB_FIELD_SPACE, desc);
			fprintf(priv->fp, tmpStr, *bufUChar);
			break;
		case lwlib_Fs_VarType_Double:
			bufDouble = (double *)buf;
			lwlib_Snprintf(tmpStr, "%%-%dlf %s\r\n", LWLIB_FIELD_SPACE, desc);
			fprintf(priv->fp, tmpStr, *bufDouble);
			break;
		case lwlib_Fs_VarType_Bool:
			bufBool = (boolean *)buf;
			lwlib_Snprintf(tmpStr, "%%-%ds %s\r\n", LWLIB_FIELD_SPACE, desc);
			fprintf(priv->fp, tmpStr, *bufBool ? "true" : "false");
			break;
		case lwlib_Fs_VarType_Raw:
			fwrite(buf, count, 1, priv->fp);
			if (count < LWLIB_FIELD_SPACE)
			{
				lwlib_Snprintf(tmpStr, "%%%ds %s\r\n", LWLIB_FIELD_SPACE - count, desc);
			}
			else
			{
				lwlib_Snprintf(tmpStr, "%%s %s\r\n", desc);
			}
			fprintf(priv->fp, tmpStr, "");
			break;
		case lwlib_Fs_VarType_String:
			fputc(LWLIB_QUOTECHAR, priv->fp);
			fwrite(buf, count, 1, priv->fp);
			fputc(LWLIB_QUOTECHAR, priv->fp);

			if (count + 2 < LWLIB_FIELD_SPACE)
			{
				lwlib_Snprintf(tmpStr, "%%%ds %s\r\n",
					LWLIB_FIELD_SPACE - (count + 2), desc);
			}
			else
			{
				lwlib_Snprintf(tmpStr, "%%s %s\r\n", desc);
			}
			fprintf(priv->fp, tmpStr, "");
			break;
		case lwlib_Fs_VarType_Informative:
			lwlib_Snprintf(tmpStr, "%%%ds %s\r\n", LWLIB_FIELD_SPACE, desc);
			fprintf(priv->fp, tmpStr, "");
			break;
		}
		break;
	}
}

boolean lwlib_Fs_LoadEntireFile(lwlib_Fs_t fs)
{
	int len;
	lwlib_Byte_t *data;
	lwlib_GetPrivAssert(lwlib_Fs_Priv_t, fs);

	len = lwlib_GetFileLength(priv->fp);
	if (len == 0)
	{
		return false;
	}

	data = lwlib_CallocMany(lwlib_Byte_t, len);
	if (data == NULL)
	{
		return false;
	}

	if (!lwlib_ReadFile(data, len, priv->fp))
	{
		return false;
	}

	if (priv->data != NULL)
	{
		free(priv->data);
	}
	priv->data = data;

	priv->length = len;
	return true;
}

lwlib_Byte_t *lwlib_Fs_GetData(lwlib_Fs_t fs)
{
	lwlib_GetPrivAssert(lwlib_Fs_Priv_t, fs);
	return priv->data;
}

int lwlib_Fs_GetLength(lwlib_Fs_t fs)
{
	lwlib_GetPrivAssert(lwlib_Fs_Priv_t, fs);
	return priv->length;
}

lwlib_Fs_FileMode_t lwlib_Fs_GetFileMode(lwlib_Fs_t fs)
{
	lwlib_GetPrivAssert(lwlib_Fs_Priv_t, fs);
	return priv->fileMode;
}
