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

#include <assert.h>
#ifdef _MSC_VER
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned __int64 uint64_t;
typedef signed __int64 int64_t;
#else
#	include <stdint.h>
#	include <string.h>
#	include <stdarg.h>
#endif
#include <string.h>
#include "lw_misc.h"
#include "lw_intmap.h"
#include "lw_ctx.h"

#define MEM_BLOCK_BYTES 4096

typedef struct MemBlock_s
{
	uint8_t bytes[MEM_BLOCK_BYTES];
} MemBlock_t;
lwlib_SeqType(MemBlock_t);

static int m_ctxNum;
static lwlib_IntMap_t m_PJLVarArray;

static lwlib_Seq(MemBlock_t) m_memBlockSeq;
static int m_memBlockUsed;
static unsigned long m_memTotalUsed;

static unsigned long memAlloc(int size)
{
	MemBlock_t memBlock;
	assert(size > 0 && size <= MEM_BLOCK_BYTES);

	if (vec_size(m_memBlockSeq) == 0 ||
		m_memBlockUsed + size > MEM_BLOCK_BYTES)
	{
		if (m_memBlockUsed + size > MEM_BLOCK_BYTES)
		{
			m_memTotalUsed += MEM_BLOCK_BYTES - m_memBlockUsed;
			m_memBlockUsed = 0;
		}

		memset(&memBlock, 0, sizeof(memBlock));
		vec_push(m_memBlockSeq, MemBlock_t, memBlock);
	}

	const unsigned long location = m_memTotalUsed;
	m_memTotalUsed += size;
	m_memBlockUsed += size;

	return location;
}

static void *memLocToAddr(unsigned long location)
{
	const unsigned long blockIndex = location / MEM_BLOCK_BYTES;
	const unsigned long blockOffset = location % MEM_BLOCK_BYTES;
	MemBlock_t* memBlock;


	assert(blockIndex < vec_size(m_memBlockSeq));

	memBlock = 
		&vec_at(m_memBlockSeq, blockIndex);
	return (void *)&memBlock->bytes[blockOffset];
}

static void memFreeAll(void)
{
	vec_free(m_memBlockSeq, MemBlock_t);
	m_memBlockUsed = 0;
	m_memTotalUsed = 0;
}

const void *ctxRead(const void *var, int ctxNum)
{
	int Index; // array index

	Index = (int)var;
	if (lwlib_IntMapContains(&m_PJLVarArray, Index))
	{
		lwlib_IntMap_t *PJLCtxArray = (lwlib_IntMap_t *)
			lwlib_IntMapElem(&m_PJLVarArray, Index);

		Index = (int)ctxNum;
		if (lwlib_IntMapContains(PJLCtxArray, Index))
		{
			return memLocToAddr((unsigned long)lwlib_IntMapElem(
				PJLCtxArray, Index));
		}
	}

	return var;
}

void ctxWrite(void *var, const void *val, int size, int ctxNum)
{
	int Index; // array index

	Index = (int)var;
	if (lwlib_IntMapContains(&m_PJLVarArray, Index))
	{
		lwlib_IntMap_t *PJLCtxArray = (lwlib_IntMap_t *)
			lwlib_IntMapElem(&m_PJLVarArray, Index);

		Index = (int)ctxNum;
		if (lwlib_IntMapContains(PJLCtxArray, Index))
		{
			unsigned long location = (unsigned long)
				lwlib_IntMapElem(PJLCtxArray, Index);
			memcpy(memLocToAddr(location), val, size);
		}
		else
		{
			unsigned long location = memAlloc(size);
			memcpy(memLocToAddr(location), val, size);

			Index = (int)ctxNum;
			lwlib_IntMapInsert(PJLCtxArray, Index, 
				(lwlib_Word_t)location);
		}
	}
	else
	{
		unsigned long location = memAlloc(size);
		memcpy(memLocToAddr(location), val, size);

		lwlib_IntMap_t *PJLCtxArray = lwlib_CallocSingle(lwlib_IntMap_t);

		Index = (int)ctxNum;
		lwlib_IntMapInsert(PJLCtxArray, Index, 
			(lwlib_Word_t)location);

		Index = (int)var;
		lwlib_IntMapInsert(&m_PJLVarArray, Index, 
			(lwlib_Word_t)PJLCtxArray);
	}
}

void ctxReset(void)
{
	lwlib_IntMapIterDecl(it);

	lwlib_IntMapIterLoop(m_PJLVarArray, it)
	{
		lwlib_IntMap_t *PJLCtxArray = (lwlib_IntMap_t *)it->val;
		lwlib_IntMapClear(PJLCtxArray);
		free(PJLCtxArray);
	}
	lwlib_IntMapClear(&m_PJLVarArray);

	memFreeAll();
	m_ctxNum = 0;
}

void ctxTest(void)
{
	int LWLIB_CTX_DECL(x) = { 0 };
	assert(LWLIB_CTX_READ(0, x, int) == 0);
	assert(LWLIB_CTX_READ(1, x, int) == 0);

	LWLIB_CTX_WRITE(0, x, 2, int);
	assert(LWLIB_CTX_READ(0, x, int) == 2);
	assert(LWLIB_CTX_READ(1, x, int) == 0);

	LWLIB_CTX_WRITE(1, x, 3, int);
	assert(LWLIB_CTX_READ(0, x, int) == 2);
	assert(LWLIB_CTX_READ(1, x, int) == 3);
}
