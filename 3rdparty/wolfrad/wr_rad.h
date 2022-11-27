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
** Rad
*************************************************************************/
#ifndef WOLFRAD_RAD_H
#define WOLFRAD_RAD_H

#ifndef WOLFRAD_RAD_TYPE
#define WOLFRAD_RAD_TYPE
typedef struct WR_Rad_s
{
	void *priv;
} WR_Rad_t;
#endif
#ifdef PS2
#include "3rdparty/lw_lib/lw_vec.h"
#include "3rdparty/lw_lib/lw_ctx.h"
#else
#include "..\lw_lib\lw_vec.h"
#include "..\lw_lib\lw_ctx.h"
#endif
#ifdef WOLFRAD
#include "wr_radmap.h"
#include "wr_raycaster.h"
#include "wr_rad.h"
#include "wr_level.h"

#define kNumberOfRadSamples	3

typedef lwlib_TVector3f TSpectra;

typedef struct {
	int width, height;
	int bpp;
	unsigned char *data; /* image data in RGBA (if alpha) of RGB format */
} TTexture;

struct SQuad;

typedef struct SQuad4P {
	struct SQuad *p[4];
} TQuad4P;

/* a quadrilateral */
typedef struct SQuad {
	int excludeFromScene;              /* 0 for enabled; 1 for disabled */
	lwlib_TPoint3i pos;                /* tile position in level */
	int qn;                            /* quad number in tile */
	lwlib_TPoint3f verts[4];           /* vertices of the quadrilateral */
	double dims[2];                    /* dimensions of the quadrilateral */
	double area;                       /* area of the quadrilateral */
	double elementArea;                /* area of all elements in quad */
	lwlib_TVector3f normal;            /* normal of the quadrilateral */
	lwlib_TVector3f up;                /* up direction on the quadrilateral */
	lwlib_TVector3f right;             /* right direction on the quadrilateral */
	TQuad4P neigh;                     /* neighbouring quadrilaterals */
	TQuad4P corner;                    /* corner quadrilaterals */
	TSpectra reflectance;              /* diffuse reflectance of the quadrilateral */
	TSpectra emission;                 /* emission of the quadrilateral */
	TTexture texture;                  /* texture of the quadrilateral */
	short patchLevel[2];               /* patch subdivision level (how fine to subdivide the quadrilateral?) */
	short elementLevel[2];             /* element subdivision level (how fine to subdivide a patch?) */
	int nu;                            /* number of elements across quad in up direction */
	int nv;                            /* number of elements across quad in right direction */
	int halfsize[2];                   /* 1 if direction is halfsized; 0 otherwise */
	int startElement;                  /* index to first element of quad */
	int doorNum;                       /* 0 means no door; otherwise (index + 1) */
	int lightNum;                      /* 0 means no light; otherwise (index + 1) */
	lwlib_TCone cone;                  /* radiosity cone for ray intersection tests */
	TSpectra color;                    /* average sample of texture at patch center */
	TRadMap LWLIB_CTX_DECL(radMap);    /* map of element radiosities for this quad */
	lwlib_Byte_t *postsource;          /* base texture */
    int postoffu, postoffv;            /* view offset of base texture */
	int postdimu, postdimv;            /* view dimensions of base texture */
	int lightTileRadius;               /* tile box extent of light */
	bool isSky;                        /* sky wall (no modulate) */
} TQuad;

typedef TQuad TQuadSeq;

typedef struct {
	TSpectra reflectance;             /* diffuse reflectance of the patch */
	TSpectra emission;                /* emission of the patch */
	lwlib_TPoint3f center;            /* center of the patch where hemi-cubes will be placed */
	lwlib_TVector3f normal;           /* normal of the patch; for orienting the hemi-cube */
	lwlib_TPoint3f raisedCenter;      /* center of the patch raised slightly above surface */
	TSpectra unshotRad;               /* unshot radiosity of the patch */
	double area;                      /* area of the patch */
	TQuad *quad;                      /* pointer to the parent quad */
	/* extra display-only attributes */
	double convergence;
	double energySum;
	TSpectra color;                   /* average sample of texture at patch center */
	lwlib_TPoint3f verts[4];          /* vertices of the element */
} TPatch;

typedef struct {
	TSpectra rad;                  /* total radiosity of the element */
	TPatch* patch;                 /* pointer to the parent patch */
	double u, v;
} TElement;

typedef struct {
	lwlib_TPoint3f camera;           /* camera location */
	lwlib_TVector3f direction;       /* direction of interest */
	lwlib_TVector3f up;              /* view up vector */
	lwlib_TVector3f right;           /* view right vector */
	unsigned short xRes, yRes; /* resolution of the frame buffer */
	unsigned long* buffer;     /* pointer to the frame buffer */
	long wid;                  /* id or pointer to the window associated with the view */
} TView;	

/* radiosity light parameters */
typedef struct {
	TSpectra emission;      /* color of light source */
	double size;            /* this size is used to calculate quad points of light faces */
	double elevation;       /* height off ground */
	short patchLevel;       /* how fine to subdivide a light face */
	short elementLevel;     /* how fine to subdivide a patch on light face */
	double areaSize;        /* this size is used to calculate quad areas of light faces */
	lwlib_TCone cone;             /* rays may intersect this cone */
} TRadLight;

lwlib_SeqType(TRadLight);

typedef struct {
	int firstProcessIndex;  /* index to first radiosity process for this light */
	int numProcesses;       /* total number of radiosity processes made for this light */
} TLight;

typedef TLight TLightSeq_t;

typedef enum
{
	WR_QUALITYLEVEL_LOW,
	WR_QUALITYLEVEL_MEDIUM,
	WR_QUALITYLEVEL_HIGH,
	WR_QUALITYLEVEL_MAX,
} WR_QualityLevel_t;

lwlib_EnumStrExtern(WR_QualityLevel_t);

typedef enum
{
	WR_INCLUDEDLIGHTS_NEAR,
	WR_INCLUDEDLIGHTS_VISIBLE,
	WR_INCLUDEDLIGHTS_ALL,
	WR_INCLUDEDLIGHTS_MAX,
} WR_IncludedLights_t;

lwlib_EnumStrExtern(WR_IncludedLights_t);

typedef enum
{
	WR_INCLUDEDSURFACES_NEAR,
	WR_INCLUDEDSURFACES_VISIBLE,
	WR_INCLUDEDSURFACES_ALL,
	WR_INCLUDEDSURFACES_MAX,
} WR_IncludedSurfaces_t;

lwlib_EnumStrExtern(WR_IncludedSurfaces_t);

typedef enum
{
	WR_INSERTSTATOBJ_CEILINGLIGHT,
	WR_INSERTSTATOBJ_CHANDELIER,
	WR_INSERTSTATOBJ_FLOORLAMP,
	WR_INSERTSTATOBJ_LIGHTBULB,
	WR_INSERTSTATOBJ_LIGHTBULBBRIGHT,
	WR_INSERTSTATOBJ_SKYLIGHT,
	WR_INSERTSTATOBJ_MAX,
} WR_InsertStatObj_t;

lwlib_EnumStrExtern(WR_InsertStatObj_t);

typedef enum
{
	WR_INSERTFLOORCEIL_FLUORESCENTPANEL,
	WR_INSERTFLOORCEIL_MAX,
} WR_InsertFloorCeil_t;

lwlib_EnumStrExtern(WR_InsertFloorCeil_t);

/* Radiosity input parameters */
typedef struct {
	TQuadSeq quads;                      /* quads */
	double threshold;                    /* convergence threshold (fraction of the total emitted energy) */
	unsigned long nPatches;              /* number of patches */
	TPatch *patches;                     /* patches */
	unsigned long nElements;             /* number of elements */
	TElement *elements;                  /* elements */
	double worldSize;                    /* approximate diameter of the bounding sphere of the world used for placing near and far planes in the hemi-cube computation*/
	int addAmbient;                      /* whether or not to add the ambient approximation in display */
	short patchLevel;                    /* patch subdivision level (how fine to subdivide the quadrilateral?) */
	short elementLevel;                  /* element subdivision level (how fine to subdivide a patch?) */
	int nu;                              /* elements across largest quad in up direction */
	int nv;                              /* elements across largest quad in right direction */
	int colorReflectance;                /* sample reflectance from texture */
	int maxShoots;                       /* number of shoots until display */
	int pointSourceModel;                /* turns on point-source illumination model */
	double lowSpectraThreshold;          /* lightmaps that are too dim are not generated */
	int maxLightRadDepth;                /* light cannot traverse more than this many doors */
	double lowSpectraDoorThreshold;      /* doors that are too dim are not opened */
	int litDoorLimit;                    /* maximum number of lit doors to open */
	int maxDoorBitsPerQuad;              /* no more than this many doors will be calculated */
	TLightSeq_t lights;                  /* light sources in world */
	lwlib_Bits_t markDoorBits;             /* doors marked */
	lwlib_Bits_t LWLIB_CTX_DECL(openDoorBits);             /* open door bits */
	int LWLIB_CTX_DECL(lightNum);                          /* light source attached to this radiosity process */
	int totalProcesses;                  /* total number of radiosity processes */
	int qualityLevel;                    
	int includedLights;
	int includedSurfaces;
	int insertStatObj;
	int insertFloorCeil;
} TRadParams;

extern TRadParams rad_params;

WR_Rad_t WR_Rad_New(void);

WR_Rad_t WR_Rad_Free(WR_Rad_t rad);

/* initialization */
void WR_Rad_Init(WR_Rad_t rad);

/* main iterative loop */
void WR_Rad_Run(WR_Rad_t rad);

/* final clean up */
void WR_Rad_CleanUp(WR_Rad_t rad);

TRadParams *WR_Rad_GetParams(WR_Rad_t rad);

void WR_Rad_SetRayCaster(WR_Rad_t rad, WR_RayCaster_t rayCaster);

void WR_Rad_SetLevel(WR_Rad_t rad, WR_Level_t level);

int WR_Rad_HitElementIndex(WR_Rad_t rad, lwlib_TPoint3f hit, TQuad *quad,
	bool checkPost);

void WR_Rad_SetQuadSubdivisionLevel(WR_Rad_t rad, TQuad *quad, int level);
#endif
#endif
