//
//	WL Edit
//	WL_ED.h - Game Editor
//	By Linux Wolf - Team RayCAST
//
#ifndef WL_ED_H
#define WL_ED_H
#include "version.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_EDITOR)

typedef struct ED_s
{
	void *priv;
} ED_t;

ED_t ED_New(void);

ED_t ED_Free(ED_t ed);

void ED_ProcessInsertStatObj(ED_t ed, int insertStatObj);

void ED_ProcessInsertFloorCeil(ED_t ed, int insertFloorCeil);

int ED_CheckInsertStatObj(ED_t ed, int x, int y, int tile);

bool ED_SpawnActor(ED_t ed, int tile, int x, int y);

int ED_CheckFloorCeil(ED_t ed, int x, int y, int spot);

bool ED_WallPicIsSky(ED_t ed, int wallpic);
#endif
#endif
