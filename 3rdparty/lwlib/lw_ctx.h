/*************************************************************************
** Vec3
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
** Vec3 Library
*************************************************************************/

#ifndef LWLIB_CTX_H
#define LWLIB_CTX_H

#define LWLIB_CTX_DECL(var) var[1]

#define LWLIB_CTX_READ(ctxNum, var, type) (*(const type *)ctxRead(&(var)[0], ctxNum))

#define LWLIB_CTX_WRITE(ctxNum, var, val, type) \
do { \
	type ctxTmpVal = val; \
	ctxWrite(&(var)[0], &ctxTmpVal, sizeof(ctxTmpVal), ctxNum); \
} while(0) 

#define LWLIB_CTX_ZERO(ctxNum, var, type) \
do { \
	type ctxTmpVal; \
	memset(&ctxTmpVal, 0, sizeof(ctxTmpVal)); \
	ctxWrite(&(var)[0], &ctxTmpVal, sizeof(ctxTmpVal), ctxNum); \
} while(0) 

const void *ctxRead(const void *var, int ctxNum);

void ctxWrite(void *var, const void *val, int size, int ctxNum);

void ctxReset(void);

#endif
