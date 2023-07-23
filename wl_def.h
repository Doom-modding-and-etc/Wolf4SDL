#ifndef __WL_DEF_H_
#define __WL_DEF_H_

#include "version.h"



#include <fcntl.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#ifdef N3DS
#include <3ds.h>
#elif defined(SWITCH)
#include <switch.h>
#endif
#if defined(_arch_dreamcast)
#include <kos.h>
#include <SDL.h>
#include <SDL_dreamcast.h>
#elif defined(PSVITA)
#include <vitasdk.h>
#elif !defined(_MSC_VER) 
#include <string.h>
#include <stdarg.h>
#elif defined(_XBOX)
#include <xtl.h>
#ifdef XBOX
#include <xbox.h>
#endif
#include <string.h>
#include <stdarg.h>
#endif

#ifndef N3DS
#pragma pack(1)
#endif
#if defined(_arch_dreamcast)
#define YESBUTTONNAME "A"
#define NOBUTTONNAME  "B"
#elif defined(GP2X)
#define YESBUTTONNAME "Y"
#define NOBUTTONNAME  "B"
#else
#define YESBUTTONNAME "Y"
#define NOBUTTONNAME  "N"
#endif

#include "foreign.h"

#ifndef SPEAR
#include "audiowl6.h"
#ifdef UPLOAD
#include "gfxv_apo.h"
#else
#ifdef JAPAN
#include "gfxv_jap.h"
#else
#ifdef GOODTIMES
#include "gfxv_wl6.h"
#else
#include "gfxv_apo.h"
#endif
#endif
#endif
#else
#include "audiosod.h"
#include "gfxv_sod.h"
#include "f_spear.h"
#endif

#if defined(GP2X) || defined(GP2X_940)
#include "platform/GP2X/gp2x.h"
#elif defined(PS2)
#include "platform/PS2/ps2_main.h"
#elif defined(_arch_dreamcast)
#include "platform/dc/dc_main.h"
#include "platform/dc/dc_vmu.h"
#elif defined(XBOX)
#include "platform/xbox/xboxmissing.h"
#endif

#ifdef USE_HEADER
#include <fixedptc.h>
#else
#include "3rdparty/fixedptc.h"	
#endif
#include "id_w3swrap.h"

#if !defined(_MSC_VER) && !defined(__GNUC__)
#ifdef X64_ARCH
typedef unsigned long long uintptr_t;
typedef unsigned int intptr_t;
#else
typedef long long uintptr_t;
typedef int intptr_t;
#endif
#endif

#ifdef HAVE_SIZE_T
#ifdef X64_ARCH
typedef unsigned long long size_t;
#else
typedef unsigned int size_t;
#endif
#endif

#ifndef __GNUC__
#ifdef OLD_MSVC
typedef long __int64 int64_t;
#else
typedef long long int64_t;
#endif
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef _WIN64
typedef fixedptd fixed;
#else
typedef fixedpt fixed;
#endif


typedef struct
{
    fixed x, y;
}Point;

typedef struct
{
    Point ul, lr;
}Rect;

void Quit(const char* errorStr, ...);

#if defined(C89)
#define wlinline
#else
#define wlinline __inline__ 
#endif

/* Uncomment the following line, if you get destination out of bounds
** assertion errors and want to ignore them during debugging
*/
#ifdef SEGA_SATURN
#define IGNORE_BAD_DEST
#else
/* #define IGNORE_BAD_DEST */
#endif
#ifdef IGNORE_BAD_DEST
#ifdef SEGA_SATURN
/* #undef assert */
/* #define assert(x) if(!(x))  { slPrint((char *)"asset test failed0", slLocate(10,20));return;} */
#define assert1(x) if(!(x)) { slPrint((char *)"asset test failed1", slLocate(10,20));return;}
#define assert2(x) if(!(x)) { slPrint((char *)"asset test failed2", slLocate(10,20));return;}
#define assert3(x) if(!(x)) { slPrint((char *)"asset test failed3", slLocate(10,20));return;}
#define assert4(x) if(!(x)) { slPrint((char *)"asset test failed4", slLocate(10,20));return;}
#define assert5(x) if(!(x)) { slPrint((char *)"asset test failed5", slLocate(10,20));return;}
#define assert6(x) if(!(x)) { slPrint((char *)"asset test failed6", slLocate(10,20));return;}
/* #define assert7(x) if(!(x)) { slPrint((char *)"asset test failed7", slLocate(10,20));return;} */
#define assert8(x) if(!(x)) { slPrint((char *)"asset test failed8", slLocate(10,20));return;}
#define wlassert(x) if(!(x)) return 0
#else
#define wlassert(x) if(!(x)) return
#endif
#else
#define wlassert(x) assert(x)
#endif

#include "id_pm.h"
#ifdef VIEASM
#include "id_vieasm.h"
#else
#include "id_sd.h"
#endif
#include "id_in.h"
#include "id_vl.h"
#include "id_vh.h"
#ifdef CRT
#include "id_crt.h"
#endif
#include "id_us.h"
#include "id_ca.h"
#ifdef LWUDPCOMMS
#include "id_udp.h"
#endif
#include "wl_menu.h"
#include "wl_utils.h"
#include "id_haptic.h"

/*
=============================================================================

                            MACROS

=============================================================================
*/

/* Defines which version shall be built and configures supported extra features */
#ifdef SEGA_SATURN
#define SATURN_WIDTH 320
#define SATURN_HEIGHT 200
#define SATURN_SORT_VALUE 240
#define SATURN_MAPSEG_ADDR 0x2002EA000
#define SATURN_CHUNK_ADDR 0x002F0000
/* 240 pour du 320, 264 pour du 352 */
#define		LINE_COLOR_TABLE		(VDP2_VRAM_A0	+ 0x1f400)
#endif

#define MAPSPOT(x,y,plane) (mapsegs[(plane)][((y) << MAPSHIFT) + (x)])

#ifdef MATH
#define abs(x)          ((x) > 0 ? (x) : -(x))
#define labs(x)         ((x) > 0 ? (x) : -(x))
#endif

#ifdef ORIGINAL
#define lengthof(x)     (sizeof((x)) / sizeof(*(x)))
#else
#define lengthof(x)     SDL_arraysize(x)
#endif
#define endof(x)        ((x) + lengthof((x)))

/*
** Converts a type for match the declaration for avoid warnings.
*/
#ifdef ORIGINAL
#if defined(REINTERPRET_CAST)
#define wlcast_conversion(type, expression) reinterpret_cast<type>(expression)
#elif defined(STATIC_CAST)
#define wlcast_conversion(type, expression) static_cast<type>(expression)
#else
#define wlcast_conversion(type, expression) ((type)(expression))
#endif
#else
#if defined(REINTERPRET_CAST)
#define wlcast_conversion SDL_reinterpret_cast
#elif defined(STATIC_CAST)
#define wlcast_conversion SDL_static_cast
#else
#define wlcast_conversion(type, expression) ((type)(expression))
#endif
#endif
/*
=============================================================================

                            GLOBAL CONSTANTS

=============================================================================
*/
#ifdef SEGA_SATURN
#define MAXTICS 8
#define DEMOTICS        8
#else
#define MAXTICS         10
#define DEMOTICS        4
#endif
#define WALLSHIFT       6

#define BIT_WALL        (1 << WALLSHIFT)
#define BIT_DOOR        (1 << (WALLSHIFT + 1))
#define BIT_ALLTILES    (1 << (WALLSHIFT + 2))

/* the door is the last picture before the sprites */
#define DOORWALL        (PMSpriteStart - 8)
#define DOORSTART       90          /* WALL - first door code */
#define DOOREND         101         /* WALL - last door code */

#define MAXACTORS       150         /* max number of nazis, etc / map */
#define MAXSTATS        400         /* max number of lamps, bonus, etc */
#define MAXDOORS        64          /* max number of sliding doors */
#define MAXWALLTILES    70          /* max number of wall tiles */

#ifdef SEGA_SATURN
#ifndef SPEAR
#define NB_WALL_HWRAM 25
#else
#define NB_WALL_HWRAM 27
#endif
#endif

#if WALLSHIFT >= 7
typedef unsigned short tiletype;
#else
typedef unsigned char tiletype;
#endif

/*
** tile constants
*/

#define ICONARROWS      90
#define PUSHABLETILE    98
#define EXITTILE        99          /* at end of castle */
#define AREATILE        107         /* first of NUMAREAS floor tiles */
#define NUMAREAS        37
#define ELEVATORTILE    21
#define AMBUSHTILE      106
#define ALTELEVATORTILE 107

#define NUMBERCHARS     9


/* ---------------- */

#define EXTRAPOINTS     40000

#define RUNSPEED        6000

#ifndef SEGA_SATURN
#define HEIGHTRATIO     0.50
#else
#define width_to_height(x) (x >> 1)
#endif

#ifndef SPEAR
#define LRpack      8       /* # of levels to store in endgame */
#else
#define LRpack      20
#endif

#define PLAYERSIZE      MINDIST         /* player radius */
#define MINACTORDIST    0x10000l        /* minimum dist from player center */
                                        /* to any actor center */
#ifndef M_PI
#define M_PI              3.141592657
#endif

#ifdef SPEARDEMO
#define NUMDEMOS 1
#else
#define NUMDEMOS 5
#endif
#define GLOBAL1         (1l<<16)
#define TILEGLOBAL      GLOBAL1
#define TILESHIFT       16l
#define UNSIGNEDSHIFT   8

#define ANGLES          360             /* must be divisable by 4 */
#define ANGLEQUAD       (ANGLES/4)
#define FINEANGLES      3600
#define ANG90           (FINEANGLES/4)
#define ANG180          (ANG90*2)
#define ANG270          (ANG90*3)
#define ANG360          (ANG90*4)
#define VANG90          (ANGLES/4)
#define VANG180         (VANG90*2)
#define VANG270         (VANG90*3)
#define VANG360         (VANG90*4)

#define MINDIST         0x5800l


#define MAPSHIFT        6
#define MAPSIZE         (1 << MAPSHIFT)
#define MAPAREA         (MAPSIZE * MAPSIZE)

#define TEXTURESHIFT    6

#if TEXTURESHIFT == 8
#define FIXED2TEXSHIFT  0
#elif TEXTURESHIFT == 7
#define FIXED2TEXSHIFT  2
#else
#define FIXED2TEXSHIFT  4
#endif

#define TEXTURESIZE     (1 << TEXTURESHIFT)
#define TEXTUREMASK     (TEXTURESIZE * (TEXTURESIZE - 1))

enum
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

#define STATUSLINES     40

#define STARTAMMO       8

#ifdef SEGA_SATURN
typedef unsigned long long mapbitmap[MAPSIZE];
static wlinline boolean getmapbit(mapbitmap m, int x, int y)
{
    return (m[x] & (1ull << y)) != 0;
}
static wlinline void setmapbit(mapbitmap m, int x, int y)
{
    m[x] |= (1ull << y);
}

extern mapbitmap objactor;

static wlinline void clearmapbit(mapbitmap m, int x, int y)
{
    m[x] &= ~(1ull << y);
}
#define obj_id(ob) ((ob) - objlist)
#define getactorflag(x, y) getmapbit(objactor, x, y)
#define setactorflag(x, y) setmapbit(objactor, x, y)
#define clearactorflag(x, y) clearmapbit(objactor, x, y)

/* Record actor location.  */
#define move_actor(o) do { \
    setactorflag((o)->tilex, (o)->tiley); \
    actorat[(o)->tilex][(o)->tiley] = obj_id(o); \
    } while (0)

/* Record Door location.  */
#define set_door_actor(x, y, doornum) actorat[x][y] = doornum | 0x80
/* Record wall location.  */
#define set_wall_at(x, y, tile) actorat[x][y] = tile
/* Clear location.  */
#define clear_actor(x, y) do { \
    actorat[x][y] = 0; \
    clearactorflag(x, y); \
    } while (0)
/* nonzero if something other than a door is at given location.  */
#define obj_actor_at(x, y) getactorflag(x, y)
/* nonzero if a wall is at given location.  */
#define wall_actor_at(x, y) (actorat[x][y] && actorat[x][y] < 128 \
			    && !getactorflag(x, y))
/* nonzero if a door or wall is at given location.  */
#define solid_actor_at(x, y) (actorat[x][y] && !getactorflag(x, y))
/* zero if given location is empty.  */
#define any_actor_at(x, y) (actorat[x][y] != 0 || getactorflag(x, y))
/* The id of the actor at given location.  */
#define get_actor_at(x, y) \
  (obj_actor_at(x, y) ? actorat[x][y] & 0xff : actorat[x][y] & 0x7f)
#endif

/* object flag values */

typedef enum
{
    FL_SHOOTABLE = 0x00000001,
    FL_BONUS = 0x00000002,
    FL_NEVERMARK = 0x00000004,
    FL_VISIBLE = 0x00000008,
    FL_ATTACKMODE = 0x00000010,
    FL_FIRSTATTACK = 0x00000020,
    FL_AMBUSH = 0x00000040,
    FL_NONMARK = 0x00000080,
    FL_FULLBRIGHT = 0x00000100,
#ifdef USE_DIR3DSPR
    /* you can choose one of the following values in wl_act1.cpp
    ** to make a static sprite a directional 3d sprite
    ** (see example at the end of the statinfo array)
    */
    FL_DIR_HORIZ_MID = 0x00000200,
    FL_DIR_HORIZ_FW = 0x00000400,
    FL_DIR_HORIZ_BW = 0x00000600,
    FL_DIR_VERT_MID = 0x00000a00,
    FL_DIR_VERT_FW = 0x00000c00,
    FL_DIR_VERT_BW = 0x00000e00,

    /* these values are just used to improve readability of code */
    FL_DIR_NONE = 0x00000000,
    FL_DIR_POS_MID = 0x00000200,
    FL_DIR_POS_FW = 0x00000400,
    FL_DIR_POS_BW = 0x00000600,
    FL_DIR_POS_MASK = 0x00000600,
    FL_DIR_VERT_FLAG = 0x00000800,
    FL_DIR_MASK = 0x00000e00,
#endif
    /* next free bit is   0x00001000 */
} objflag_t;


/*
** sprite constants
*/

enum
{
    SPR_DEMO,
#ifndef APOGEE_1_0
    SPR_DEATHCAM,
#endif
    /*
    ** static sprites
    */
    SPR_STAT_0, SPR_STAT_1, SPR_STAT_2, SPR_STAT_3,
    SPR_STAT_4, SPR_STAT_5, SPR_STAT_6, SPR_STAT_7,

    SPR_STAT_8, SPR_STAT_9, SPR_STAT_10, SPR_STAT_11,
    SPR_STAT_12, SPR_STAT_13, SPR_STAT_14, SPR_STAT_15,

    SPR_STAT_16, SPR_STAT_17, SPR_STAT_18, SPR_STAT_19,
    SPR_STAT_20, SPR_STAT_21, SPR_STAT_22, SPR_STAT_23,

    SPR_STAT_24, SPR_STAT_25, SPR_STAT_26, SPR_STAT_27,
    SPR_STAT_28, SPR_STAT_29, SPR_STAT_30, SPR_STAT_31,

    SPR_STAT_32, SPR_STAT_33, SPR_STAT_34, SPR_STAT_35,
    SPR_STAT_36, SPR_STAT_37, SPR_STAT_38, SPR_STAT_39,

    SPR_STAT_40, SPR_STAT_41, SPR_STAT_42, SPR_STAT_43,
    SPR_STAT_44, SPR_STAT_45, SPR_STAT_46, SPR_STAT_47,

#ifdef SPEAR
    SPR_STAT_48, SPR_STAT_49, SPR_STAT_50, SPR_STAT_51,
#endif

    /*
    ** guard
    */
    SPR_GRD_S_1, SPR_GRD_S_2, SPR_GRD_S_3, SPR_GRD_S_4,
    SPR_GRD_S_5, SPR_GRD_S_6, SPR_GRD_S_7, SPR_GRD_S_8,

    SPR_GRD_W1_1, SPR_GRD_W1_2, SPR_GRD_W1_3, SPR_GRD_W1_4,
    SPR_GRD_W1_5, SPR_GRD_W1_6, SPR_GRD_W1_7, SPR_GRD_W1_8,

    SPR_GRD_W2_1, SPR_GRD_W2_2, SPR_GRD_W2_3, SPR_GRD_W2_4,
    SPR_GRD_W2_5, SPR_GRD_W2_6, SPR_GRD_W2_7, SPR_GRD_W2_8,

    SPR_GRD_W3_1, SPR_GRD_W3_2, SPR_GRD_W3_3, SPR_GRD_W3_4,
    SPR_GRD_W3_5, SPR_GRD_W3_6, SPR_GRD_W3_7, SPR_GRD_W3_8,

    SPR_GRD_W4_1, SPR_GRD_W4_2, SPR_GRD_W4_3, SPR_GRD_W4_4,
    SPR_GRD_W4_5, SPR_GRD_W4_6, SPR_GRD_W4_7, SPR_GRD_W4_8,

    SPR_GRD_PAIN_1, SPR_GRD_DIE_1, SPR_GRD_DIE_2, SPR_GRD_DIE_3,
    SPR_GRD_PAIN_2, SPR_GRD_DEAD,

    SPR_GRD_SHOOT1, SPR_GRD_SHOOT2, SPR_GRD_SHOOT3,

    /*
    ** dogs
    */
    SPR_DOG_W1_1, SPR_DOG_W1_2, SPR_DOG_W1_3, SPR_DOG_W1_4,
    SPR_DOG_W1_5, SPR_DOG_W1_6, SPR_DOG_W1_7, SPR_DOG_W1_8,

    SPR_DOG_W2_1, SPR_DOG_W2_2, SPR_DOG_W2_3, SPR_DOG_W2_4,
    SPR_DOG_W2_5, SPR_DOG_W2_6, SPR_DOG_W2_7, SPR_DOG_W2_8,

    SPR_DOG_W3_1, SPR_DOG_W3_2, SPR_DOG_W3_3, SPR_DOG_W3_4,
    SPR_DOG_W3_5, SPR_DOG_W3_6, SPR_DOG_W3_7, SPR_DOG_W3_8,

    SPR_DOG_W4_1, SPR_DOG_W4_2, SPR_DOG_W4_3, SPR_DOG_W4_4,
    SPR_DOG_W4_5, SPR_DOG_W4_6, SPR_DOG_W4_7, SPR_DOG_W4_8,

    SPR_DOG_DIE_1, SPR_DOG_DIE_2, SPR_DOG_DIE_3, SPR_DOG_DEAD,
    SPR_DOG_JUMP1, SPR_DOG_JUMP2, SPR_DOG_JUMP3,



    /*
    ** ss
    */
    SPR_SS_S_1, SPR_SS_S_2, SPR_SS_S_3, SPR_SS_S_4,
    SPR_SS_S_5, SPR_SS_S_6, SPR_SS_S_7, SPR_SS_S_8,

    SPR_SS_W1_1, SPR_SS_W1_2, SPR_SS_W1_3, SPR_SS_W1_4,
    SPR_SS_W1_5, SPR_SS_W1_6, SPR_SS_W1_7, SPR_SS_W1_8,

    SPR_SS_W2_1, SPR_SS_W2_2, SPR_SS_W2_3, SPR_SS_W2_4,
    SPR_SS_W2_5, SPR_SS_W2_6, SPR_SS_W2_7, SPR_SS_W2_8,

    SPR_SS_W3_1, SPR_SS_W3_2, SPR_SS_W3_3, SPR_SS_W3_4,
    SPR_SS_W3_5, SPR_SS_W3_6, SPR_SS_W3_7, SPR_SS_W3_8,

    SPR_SS_W4_1, SPR_SS_W4_2, SPR_SS_W4_3, SPR_SS_W4_4,
    SPR_SS_W4_5, SPR_SS_W4_6, SPR_SS_W4_7, SPR_SS_W4_8,

    SPR_SS_PAIN_1, SPR_SS_DIE_1, SPR_SS_DIE_2, SPR_SS_DIE_3,
    SPR_SS_PAIN_2, SPR_SS_DEAD,

    SPR_SS_SHOOT1, SPR_SS_SHOOT2, SPR_SS_SHOOT3,

    /*
    ** mutant
    */
    SPR_MUT_S_1, SPR_MUT_S_2, SPR_MUT_S_3, SPR_MUT_S_4,
    SPR_MUT_S_5, SPR_MUT_S_6, SPR_MUT_S_7, SPR_MUT_S_8,

    SPR_MUT_W1_1, SPR_MUT_W1_2, SPR_MUT_W1_3, SPR_MUT_W1_4,
    SPR_MUT_W1_5, SPR_MUT_W1_6, SPR_MUT_W1_7, SPR_MUT_W1_8,

    SPR_MUT_W2_1, SPR_MUT_W2_2, SPR_MUT_W2_3, SPR_MUT_W2_4,
    SPR_MUT_W2_5, SPR_MUT_W2_6, SPR_MUT_W2_7, SPR_MUT_W2_8,

    SPR_MUT_W3_1, SPR_MUT_W3_2, SPR_MUT_W3_3, SPR_MUT_W3_4,
    SPR_MUT_W3_5, SPR_MUT_W3_6, SPR_MUT_W3_7, SPR_MUT_W3_8,

    SPR_MUT_W4_1, SPR_MUT_W4_2, SPR_MUT_W4_3, SPR_MUT_W4_4,
    SPR_MUT_W4_5, SPR_MUT_W4_6, SPR_MUT_W4_7, SPR_MUT_W4_8,

    SPR_MUT_PAIN_1, SPR_MUT_DIE_1, SPR_MUT_DIE_2, SPR_MUT_DIE_3,
    SPR_MUT_PAIN_2, SPR_MUT_DIE_4, SPR_MUT_DEAD,

    SPR_MUT_SHOOT1, SPR_MUT_SHOOT2, SPR_MUT_SHOOT3, SPR_MUT_SHOOT4,

    /*
    ** officer
    */
    SPR_OFC_S_1, SPR_OFC_S_2, SPR_OFC_S_3, SPR_OFC_S_4,
    SPR_OFC_S_5, SPR_OFC_S_6, SPR_OFC_S_7, SPR_OFC_S_8,

    SPR_OFC_W1_1, SPR_OFC_W1_2, SPR_OFC_W1_3, SPR_OFC_W1_4,
    SPR_OFC_W1_5, SPR_OFC_W1_6, SPR_OFC_W1_7, SPR_OFC_W1_8,

    SPR_OFC_W2_1, SPR_OFC_W2_2, SPR_OFC_W2_3, SPR_OFC_W2_4,
    SPR_OFC_W2_5, SPR_OFC_W2_6, SPR_OFC_W2_7, SPR_OFC_W2_8,

    SPR_OFC_W3_1, SPR_OFC_W3_2, SPR_OFC_W3_3, SPR_OFC_W3_4,
    SPR_OFC_W3_5, SPR_OFC_W3_6, SPR_OFC_W3_7, SPR_OFC_W3_8,

    SPR_OFC_W4_1, SPR_OFC_W4_2, SPR_OFC_W4_3, SPR_OFC_W4_4,
    SPR_OFC_W4_5, SPR_OFC_W4_6, SPR_OFC_W4_7, SPR_OFC_W4_8,

    SPR_OFC_PAIN_1, SPR_OFC_DIE_1, SPR_OFC_DIE_2, SPR_OFC_DIE_3,
    SPR_OFC_PAIN_2, SPR_OFC_DIE_4, SPR_OFC_DEAD,

    SPR_OFC_SHOOT1, SPR_OFC_SHOOT2, SPR_OFC_SHOOT3,

#ifndef SPEAR
    /*
    ** ghosts
    */
    SPR_BLINKY_W1, SPR_BLINKY_W2, SPR_PINKY_W1, SPR_PINKY_W2,
    SPR_CLYDE_W1, SPR_CLYDE_W2, SPR_INKY_W1, SPR_INKY_W2,

    /*
    ** hans
    */
    SPR_BOSS_W1, SPR_BOSS_W2, SPR_BOSS_W3, SPR_BOSS_W4,
    SPR_BOSS_SHOOT1, SPR_BOSS_SHOOT2, SPR_BOSS_SHOOT3, SPR_BOSS_DEAD,

    SPR_BOSS_DIE1, SPR_BOSS_DIE2, SPR_BOSS_DIE3,

    /*
    ** schabbs
    */
    SPR_SCHABB_W1, SPR_SCHABB_W2, SPR_SCHABB_W3, SPR_SCHABB_W4,
    SPR_SCHABB_SHOOT1, SPR_SCHABB_SHOOT2,

    SPR_SCHABB_DIE1, SPR_SCHABB_DIE2, SPR_SCHABB_DIE3, SPR_SCHABB_DEAD,
    SPR_HYPO1, SPR_HYPO2, SPR_HYPO3, SPR_HYPO4,

    /*
    ** fake
    */
    SPR_FAKE_W1, SPR_FAKE_W2, SPR_FAKE_W3, SPR_FAKE_W4,
    SPR_FAKE_SHOOT, SPR_FIRE1, SPR_FIRE2,

    SPR_FAKE_DIE1, SPR_FAKE_DIE2, SPR_FAKE_DIE3, SPR_FAKE_DIE4,
    SPR_FAKE_DIE5, SPR_FAKE_DEAD,

    /*
    ** hitler
    */
    SPR_MECHA_W1, SPR_MECHA_W2, SPR_MECHA_W3, SPR_MECHA_W4,
    SPR_MECHA_SHOOT1, SPR_MECHA_SHOOT2, SPR_MECHA_SHOOT3, SPR_MECHA_DEAD,

    SPR_MECHA_DIE1, SPR_MECHA_DIE2, SPR_MECHA_DIE3,

    SPR_HITLER_W1, SPR_HITLER_W2, SPR_HITLER_W3, SPR_HITLER_W4,
    SPR_HITLER_SHOOT1, SPR_HITLER_SHOOT2, SPR_HITLER_SHOOT3, SPR_HITLER_DEAD,

    SPR_HITLER_DIE1, SPR_HITLER_DIE2, SPR_HITLER_DIE3, SPR_HITLER_DIE4,
    SPR_HITLER_DIE5, SPR_HITLER_DIE6, SPR_HITLER_DIE7,

    /*
    ** giftmacher
    */
    SPR_GIFT_W1, SPR_GIFT_W2, SPR_GIFT_W3, SPR_GIFT_W4,
    SPR_GIFT_SHOOT1, SPR_GIFT_SHOOT2,

    SPR_GIFT_DIE1, SPR_GIFT_DIE2, SPR_GIFT_DIE3, SPR_GIFT_DEAD,
#endif
    /*
    ** Rocket, smoke and small explosion
    */
    SPR_ROCKET_1, SPR_ROCKET_2, SPR_ROCKET_3, SPR_ROCKET_4,
    SPR_ROCKET_5, SPR_ROCKET_6, SPR_ROCKET_7, SPR_ROCKET_8,

    SPR_SMOKE_1, SPR_SMOKE_2, SPR_SMOKE_3, SPR_SMOKE_4,
    SPR_BOOM_1, SPR_BOOM_2, SPR_BOOM_3,

    /*
    ** Angel of Death's DeathSparks(tm)
    */
#ifdef SPEAR
    SPR_HROCKET_1, SPR_HROCKET_2, SPR_HROCKET_3, SPR_HROCKET_4,
    SPR_HROCKET_5, SPR_HROCKET_6, SPR_HROCKET_7, SPR_HROCKET_8,

    SPR_HSMOKE_1, SPR_HSMOKE_2, SPR_HSMOKE_3, SPR_HSMOKE_4,
    SPR_HBOOM_1, SPR_HBOOM_2, SPR_HBOOM_3,

    SPR_SPARK1, SPR_SPARK2, SPR_SPARK3, SPR_SPARK4,
#endif

#ifndef SPEAR
    /*
    ** gretel
    */
    SPR_GRETEL_W1, SPR_GRETEL_W2, SPR_GRETEL_W3, SPR_GRETEL_W4,
    SPR_GRETEL_SHOOT1, SPR_GRETEL_SHOOT2, SPR_GRETEL_SHOOT3, SPR_GRETEL_DEAD,

    SPR_GRETEL_DIE1, SPR_GRETEL_DIE2, SPR_GRETEL_DIE3,

    /*
    ** fat face
    */
    SPR_FAT_W1, SPR_FAT_W2, SPR_FAT_W3, SPR_FAT_W4,
    SPR_FAT_SHOOT1, SPR_FAT_SHOOT2, SPR_FAT_SHOOT3, SPR_FAT_SHOOT4,

    SPR_FAT_DIE1, SPR_FAT_DIE2, SPR_FAT_DIE3, SPR_FAT_DEAD,

    /*
    ** bj
    */
#ifdef APOGEE_1_0
    SPR_BJ_W1 = 360,
#elif defined(APOGEE_1_1) && defined(UPLOAD)
    SPR_BJ_W1 = 406,
#else
    SPR_BJ_W1,
#endif
    SPR_BJ_W2, SPR_BJ_W3, SPR_BJ_W4,
    SPR_BJ_JUMP1, SPR_BJ_JUMP2, SPR_BJ_JUMP3, SPR_BJ_JUMP4,
#else
    /*
    ** THESE ARE FOR 'SPEAR OF DESTINY'
    */

    /*
    ** Trans Grosse
    */
    SPR_TRANS_W1, SPR_TRANS_W2, SPR_TRANS_W3, SPR_TRANS_W4,
    SPR_TRANS_SHOOT1, SPR_TRANS_SHOOT2, SPR_TRANS_SHOOT3, SPR_TRANS_DEAD,

    SPR_TRANS_DIE1, SPR_TRANS_DIE2, SPR_TRANS_DIE3,

    /*
    ** Wilhelm
    */
    SPR_WILL_W1, SPR_WILL_W2, SPR_WILL_W3, SPR_WILL_W4,
    SPR_WILL_SHOOT1, SPR_WILL_SHOOT2, SPR_WILL_SHOOT3, SPR_WILL_SHOOT4,

    SPR_WILL_DIE1, SPR_WILL_DIE2, SPR_WILL_DIE3, SPR_WILL_DEAD,

    /*
    ** UberMutant
    */
    SPR_UBER_W1, SPR_UBER_W2, SPR_UBER_W3, SPR_UBER_W4,
    SPR_UBER_SHOOT1, SPR_UBER_SHOOT2, SPR_UBER_SHOOT3, SPR_UBER_SHOOT4,

    SPR_UBER_DIE1, SPR_UBER_DIE2, SPR_UBER_DIE3, SPR_UBER_DIE4,
    SPR_UBER_DEAD,

    /*
    ** Death Knight
    */
    SPR_DEATH_W1, SPR_DEATH_W2, SPR_DEATH_W3, SPR_DEATH_W4,
    SPR_DEATH_SHOOT1, SPR_DEATH_SHOOT2, SPR_DEATH_SHOOT3, SPR_DEATH_SHOOT4,

    SPR_DEATH_DIE1, SPR_DEATH_DIE2, SPR_DEATH_DIE3, SPR_DEATH_DIE4,
    SPR_DEATH_DIE5, SPR_DEATH_DIE6, SPR_DEATH_DEAD,

    /*
    ** Ghost
    */
    SPR_SPECTRE_W1, SPR_SPECTRE_W2, SPR_SPECTRE_W3, SPR_SPECTRE_W4,
    SPR_SPECTRE_F1, SPR_SPECTRE_F2, SPR_SPECTRE_F3, SPR_SPECTRE_F4,

    /*
    ** Angel of Death
    */
    SPR_ANGEL_W1, SPR_ANGEL_W2, SPR_ANGEL_W3, SPR_ANGEL_W4,
    SPR_ANGEL_SHOOT1, SPR_ANGEL_SHOOT2, SPR_ANGEL_TIRED1, SPR_ANGEL_TIRED2,

    SPR_ANGEL_DIE1, SPR_ANGEL_DIE2, SPR_ANGEL_DIE3, SPR_ANGEL_DIE4,
    SPR_ANGEL_DIE5, SPR_ANGEL_DIE6, SPR_ANGEL_DIE7, SPR_ANGEL_DEAD,

#endif

    /*
    ** player attack frames
    */
    SPR_KNIFEREADY, SPR_KNIFEATK1, SPR_KNIFEATK2, SPR_KNIFEATK3,
    SPR_KNIFEATK4,

    SPR_PISTOLREADY, SPR_PISTOLATK1, SPR_PISTOLATK2, SPR_PISTOLATK3,
    SPR_PISTOLATK4,

    SPR_MACHINEGUNREADY, SPR_MACHINEGUNATK1, SPR_MACHINEGUNATK2, MACHINEGUNATK3,
    SPR_MACHINEGUNATK4,

    SPR_CHAINREADY, SPR_CHAINATK1, SPR_CHAINATK2, SPR_CHAINATK3,
    SPR_CHAINATK4,
#ifdef COMPASS
    SPR_DIR_N, SPR_DIR_NE, SPR_DIR_E, SPR_DIR_SE,      /* Directions N-SE */
    SPR_DIR_S, SPR_DIR_SW, SPR_DIR_W, SPR_DIR_NW,      /* Directions S-NW */
#endif
};


/*
=============================================================================

                               GLOBAL TYPES

=============================================================================
*/

typedef enum
{
    di_north,
    di_east,
    di_south,
    di_west
} controldir_t;

typedef enum
{
    dr_normal,
    dr_lock1,
    dr_lock2,
    dr_lock3,
    dr_lock4,
    dr_elevator,
} door_t;

typedef enum
{
    ac_badobject = -1,
    ac_no,
    ac_yes,
#ifndef SEGA_SATURN
    ac_always
#endif
} activetype;

typedef enum
{
    nothing,
    playerobj,
    inertobj,
    guardobj,
    officerobj,
    ssobj,
    dogobj,
    bossobj,
    schabbobj,
    fakeobj,
    mechahitlerobj,
    mutantobj,
    needleobj,
    fireobj,
    bjobj,
    ghostobj,
    realhitlerobj,
    gretelobj,
    giftobj,
    fatobj,
    rocketobj,

    spectreobj,
    angelobj,
    transobj,
    uberobj,
    willobj,
    deathobj,
    hrocketobj,
    sparkobj
} classtype;

typedef enum
{
    none,
    block,
    bo_gibs,
    bo_alpo,
    bo_firstaid,
    bo_key1,
    bo_key2,
    bo_key3,
    bo_key4,
    bo_cross,
    bo_chalice,
    bo_bible,
    bo_crown,
    bo_clip,
    bo_clip2,
    bo_machinegun,
    bo_chaingun,
    bo_food,
    bo_fullheal,
    bo_25clip,
    bo_spear
} wl_stat_t;

typedef enum
{
    east,
    northeast,
    north,
    northwest,
    west,
    southwest,
    south,
    southeast,
    nodir
} dirtype;


typedef enum
{
    en_guard,
    en_officer,
    en_ss,
    en_dog,
    en_boss,
    en_schabbs,
    en_fake,
    en_hitler,
    en_mutant,
    en_blinky,
    en_clyde,
    en_pinky,
    en_inky,
    en_gretel,
    en_gift,
    en_fat,
    en_spectre,
    en_angel,
    en_trans,
    en_uber,
    en_will,
    en_death,
    NUMENEMIES
} enemy_t;

typedef void (*statefunc) (void*);

#if defined(SEGA_SATURN) && defined(EMBEDDED) 
typedef struct statestruct
{
    boolean	rotate;
    int shapenum; /* a shapenum of -1 means get from ob->temp1 */
    int tictime;
    void (*think)(), (*action)();
    int next; /* stateenum */
} statetype;
#else
typedef struct statestruct
{
    boolean rotate;
    short   shapenum;           /* a shapenum of -1 means get from ob->temp1 */
    short   tictime;
    void    (*think) (void*), (*action) (void*);
    struct  statestruct* next;
} statetype;
#endif

/*
**---------------------
**
** trivial actor structure
**
**---------------------
*/

typedef struct statstruct
{
    unsigned char      tilex, tiley;
    short     shapenum;           /* if shapenum == -1 the obj has been removed */
    unsigned char* visspot;
    unsigned int flags;
    unsigned char      itemnumber;
#ifdef PUSHOBJECT /* Objects that can be pushed */
    unsigned char      pushable;
#endif   
} statobj_t;

#ifdef PUSHOBJECT /* Objects that can be pushed */
/* Place this value on the floor below an item that you want to be pushable */
#define PUSHITEMMARKER    145   /* Add this value to your mapdata defines - change value if required */


extern  void ResetFloorCode(int tilex, int tiley);
/* Arrays for quick checking locations on the map */
extern  char            dx4dir[4];
extern  char            dy4dir[4];
#endif

/*
---------------------
**
** door actor structure
**
**---------------------
*/

typedef enum
{
    dr_open,
    dr_closed,
    dr_opening,
    dr_closing
} doortype;

typedef struct doorstruct
{
    unsigned char     tilex, tiley;
    boolean  vertical;
    unsigned char     lock;
    unsigned int action;
    short    ticcount;
#ifdef BLAKEDOORS
    boolean doubledoor;
#endif
} doorobj_t;


/*
**--------------------
**
** thinking actor structure
**
**--------------------
*/

typedef struct objstruct
{
    activetype  active;
    short       ticcount;
    classtype   obclass;
#if !defined(SEGA_SATURN) && !defined(EMBEDDED)
    statetype* state;
#else
    int		id;
    int		state; /* stateenum */
#endif

    unsigned int    flags;              /* FL_SHOOTABLE, etc */

    unsigned int     distance;           /* if negative, wait for that door to open */
    dirtype     dir;

    int       x, y;
    unsigned short        tilex, tiley;
    unsigned char        areanumber;

    short       viewx;
    unsigned short        viewheight;
    fixed       transx, transy;      /* in global coord */

    short       angle;
    short       hitpoints;
    uintptr_t     speed;

    short       temp1, temp2, hidden;
    struct objstruct* next, * prev;
} objtype;

enum
{
    bt_nobutton = -1,
    bt_attack = 0,
    bt_strafe,
    bt_run,
    bt_use,
    bt_readyknife,
    bt_readypistol,
    bt_readymachinegun,
    bt_readychaingun,
#ifdef EXTRACONTROLS
    bt_moveforward,
    bt_movebackward,
    bt_strafeleft,
    bt_straferight,
#endif
    bt_nextweapon,
    bt_prevweapon,
    bt_esc,
    bt_pause,
#ifndef EXTRACONTROLS
    bt_strafeleft,
    bt_straferight,
    bt_moveforward,
    bt_movebackward,
#endif
    bt_turnleft,
    bt_turnright,
    NUMBUTTONS
};

typedef enum
{
    wp_none = -1,
    wp_knife,
    wp_pistol,
    wp_machinegun,
    wp_chaingun,
    NUMWEAPONS
} weapontype;


enum
{
    gd_baby,
    gd_easy,
    gd_medium,
    gd_hard
};

/*
**---------------
**
** gamestate structure
**
**---------------
*/
typedef struct
{
    short       difficulty;
    short       mapon;
    int     oldscore, score, nextextra;
    short       lives;
    short       health;
    short       ammo;
    short       keys;
    weapontype  bestweapon, weapon, chosenweapon;

    short       faceframe;
    short       attackframe, attackcount, weaponframe;

    short       episode, secretcount, treasurecount, killcount,
        secrettotal, treasuretotal, killtotal;
    uintptr_t     TimeCount;
    int     killx, killy;
    boolean     victoryflag;            /* set during victory animations */
#ifdef MAPCONTROLPARTIME
    float       partime;
#endif
#if defined(BOSS_MUSIC) && defined(VIEASM)
    short       music;
#endif
} gametype;


typedef enum
{
    ex_stillplaying,
    ex_completed,
    ex_died,
    ex_warped,
    ex_resetgame,
    ex_loadedgame,
    ex_victorious,
    ex_abort,
    ex_demodone,
    ex_secretlevel
} exit_t;


/*
=============================================================================

                             WL_MAIN DEFINITIONS

=============================================================================
*/

#ifndef SEGA_SATURN
extern  char     str[80];
extern  char     configdir[256];
extern  char     configname[13];
#endif
#ifdef SWITCH
extern PadState pad;
#endif

extern  fixed    focallength;
#ifndef SEGA_SATURN
extern  unsigned int screenofs;
#endif
extern  int      viewscreenx, viewscreeny;
extern  int      viewwidth;
extern  int      viewheight;
extern  short    centerx, centery;
extern  int      shootdelta;

extern  int      dirangle[9];

extern  boolean  startgame,
#ifndef SEGA_SATURN                 
loadedgame;
#endif
#ifdef  VIEASM
extern unsigned char soundvol, musicvol;
extern boolean reversestereo;
extern boolean allowwindow;
#endif
extern  int      mouseadjustment;
/*
** derived constants
*/
extern  int      heightnumerator;
extern  fixed    scale;

#ifndef SEGA_SATURN
/*
** command line parameter variables
*/
extern  boolean  param_debugmode;
extern  boolean  param_nowait;
extern  int      param_difficulty;
extern  int      param_tedlevel;
extern  int      param_joystickindex;
extern  int      param_joystickhat;
extern  unsigned int param_samplerate;
extern  int      param_audiobuffer;
extern  int      param_mission;
extern  boolean  param_goodtimes;
extern  boolean  param_ignorenumchunks;
#endif

void            NewGame(int difficulty, int episode);
#ifdef MAPCONTROLLEDPLSETT
void            ResetPlayer(void);
#endif
void            CalcProjection(int focal);
void            NewViewSize(int width);
boolean         SetViewSize(unsigned width, unsigned height);
boolean         LoadTheGame(FILE* file, int x, int y);
boolean         SaveTheGame(FILE* file, int x, int y);
void            ShowViewSize(int width);
void            ShutdownId(void);


/*
=============================================================================

                         WL_GAME DEFINITIONS

=============================================================================
*/

extern  gametype    gamestate;
extern  unsigned char        bordercol;

#ifndef SEGA_SATURN
extern  char        demoname[13];
#endif

#ifdef SPEAR
extern  int     spearx, speary;
extern  short    spearangle;
extern  boolean     spearflag;
#endif


void    SetupGameLevel(void);
void    GameLoop(void);
#ifdef SEGA_SATURN
wlinline void DrawPlayBorder(void);
#else
void    DrawPlayBorder(void);
#endif
void    DrawStatusBorder(unsigned char color);
void    DrawPlayScreen(void);
void    DrawPlayBorderSides(void);
#ifndef SEGA_SATURN
void    ShowActStatus(void);
#endif

void    PlayDemo(int demonumber);
void    RecordDemo(void);


/* JAB */
#ifndef SEGA_SATURN
#define PlaySoundLocTile(s,tx,ty) PlaySoundLocGlobal(s,(((fixed)(tx) << TILESHIFT) + (TILEGLOBAL/2)),(((fixed)ty << TILESHIFT) + (TILEGLOBAL/2)))
#endif
#define PlaySoundLocActor(s,ob)   PlaySoundLocGlobal(s,(ob)->x,(ob)->y)
#ifndef SEGA_SATURN
void    PlaySoundLocGlobal(unsigned short s, fixed gx, fixed gy);
#endif
void    UpdateSoundLoc(void);


/*
=============================================================================

                            WL_PLAY DEFINITIONS

=============================================================================
*/

#define BASEMOVE    35
#define RUNMOVE     70
#define BASETURN    35
#define RUNTURN     70

#define JOYSCALE    2

extern  uintptr_t     funnyticount;           /* FOR FUNNY BJ FACE */

extern  exit_t      playstate;

extern  boolean         DebugOk;

extern  boolean     madenoise;

#ifdef COMPASS
extern boolean compass;
#endif

extern  objtype     objlist[MAXACTORS];
extern  objtype* newobj, * player, * objfreelist, * killerobj;

extern  tiletype    tilemap[MAPSIZE][MAPSIZE];      /* wall values only */
extern  boolean        spotvis[MAPSIZE][MAPSIZE];
#ifdef SEGA_SATURN
extern  int         actorat[MAPSIZE][MAPSIZE];
extern	unsigned	farmapylookup[MAPSIZE];
extern statetype gamestates[MAXSTATES];
/* extern	objtype 	objlist[MAXACTORS],*new,*obj,*player,*lastobj, */
extern	objtype* neww;
#else
extern  objtype* actorat[MAPSIZE][MAPSIZE];
#endif
#ifdef REVEALMAP
extern  boolean        mapseen[MAPSIZE][MAPSIZE];
#endif
#ifdef HIGHLIGHTPUSHWALLS
/* todo saturn: noclip does not affect anything... */
extern  boolean  singlestep, godmode, noclip, ammocheat, mapreveal, highlightmode;
#else
extern  boolean  singlestep, godmode, noclip, ammocheat, mapreveal;
#endif
#ifdef EXTRACONTROLS
extern  int         controlstrafe;
#endif
#ifndef SEGA_SATURN
extern  int  extravbls;
#endif
extern  unsigned short        mapwidth, mapheight;
extern  uintptr_t    tics;
#ifndef SEGA_SATURN
extern  int         lastgamemusicoffset;
#endif

#if SDL_MAJOR_VERSION == 2
extern int gamecontrolstrafe;
#endif

/*
** control info
*/
#ifndef SEGA_SATURN
extern  boolean     mouseenabled, joystickenabled;
#endif
#ifdef EXTRACONTROLS
extern  boolean     mousemoveenabled;
#endif
extern  boolean mouselookenabled, alwaysrunenabled;
extern  int         dirscan[4];
extern  int         buttonscan[NUMBUTTONS];
#ifndef SEGA_SATURN
extern  int         buttonmouse[4];
extern  int         buttonjoy[32];
extern  boolean     buttonheld[NUMBUTTONS];
#endif
extern  int         viewsize;

/*
** current user input
*/
extern  int         controlx, controly;              /* range from -100 to 100 */
extern  int         mousecontrolx,mousecontroly;
extern  boolean     buttonstate[NUMBUTTONS];

extern  boolean     demorecord, demoplayback;
extern  char* demoptr, * lastdemoptr;
#ifndef SEGA_SATURN
extern  void* demobuffer;
#endif


void    InitActorList(void);
void    GetNewActor(void);
void    PlayLoop(void);
void    CenterWindow(unsigned short w, unsigned short h);
void    InitRedShifts(void);
void    FinishPaletteShifts(void);
void    RemoveObj(objtype* gone);
void    PollControls(void);
int     StopMusic(void);
void    StartMusic(void);
void    ContinueMusic(int offs);
#if defined(BOSS_MUSIC) && defined(VIEASM)
void    ChangeGameMusic(int song);
void    SetLevelMusic(void);
#endif
void    StartDamageFlash(int damage);
void    StartBonusFlash(void);


/*
=============================================================================

                                WL_INTER

=============================================================================
*/

void IntroScreen(void);
void PG13(void);
void DrawHighScores(void);
void CheckHighScore(int score, unsigned short other);
void Victory(void);
void LevelCompleted(void);
void ClearSplitVWB(void);

void PreloadGraphics(void);


/*
=============================================================================

                                WL_DEBUG

=============================================================================
*/

#ifdef DEBUG
extern	statetype s_grddie4;
extern	statetype s_dogdead;
extern	statetype s_ofcdie5;
extern	statetype s_mutdie5;
extern	statetype s_ssdie4;
extern	statetype s_fakestand;
extern	statetype s_fakedie6;
extern	statetype s_mechadie4;
extern	statetype s_mechastand;
#endif

int DebugKeys(void);
void ViewMap(void);
void PictureGrabber(void);

#if defined(FIXEDLOGICRATE) && defined(LAGSIMULATOR)
extern boolean lagging;
#endif

/*
=============================================================================

                            WL_DRAW DEFINITIONS

=============================================================================
*/

extern  unsigned char* vbuf;

extern  uintptr_t lasttimecount;
extern  int frameon;
extern  boolean fizzlein, fpscounter;
#ifdef AUTOMAP
extern boolean automap[MAPSIZE][MAPSIZE];
#endif

#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
extern  short* spanstart;
#endif
#ifndef SEGA_SATURN
extern  short* wallheight;
#endif
/*
** math tables
*/
extern  short* pixelangle;
extern  int finetangent[FINEANGLES / 4];
extern  fixed   sintable[ANGLES + ANGLES / 4];
extern  fixed* costable;

/*
** refresh variables
*/
extern  fixed   viewx, viewy;                    /* the focal point */
extern  fixed   viewsin, viewcos;

extern  int     postx;
extern  unsigned char* postsource;

extern  short   midangle;

extern  unsigned short    horizwall[MAXWALLTILES], vertwall[MAXWALLTILES];
/* todo saturn: */
#ifdef SEGA_SATURN
void ScalePost(int postx, int texture, unsigned char* postsource, unsigned char* tilemapaddr, ray_struc* ray);
#else
void    ScalePost(void);
#endif
void    ThreeDRefresh(void);
void    CalcTics(void);
#ifdef AUTOMAP
void DrawAutomap(void);
void DrawFullmap(void);
#endif

/*
=============================================================================

                             WL_SCALE DEFINITIONS

=============================================================================
*/

typedef struct
{
    unsigned short leftpix, rightpix;
    unsigned short dataofs[64];
    /* table data after dataofs[rightpix-leftpix+1] */
} compshape_t;

#ifdef USE_SHADING
void ScaleShape(int xcenter, int shapenum, int height, unsigned int flags);
#else
void ScaleShape(int xcenter, int shapenum, int height);
#endif
void SimpleScaleShape(int xcenter, int shapenum, int height);
#ifdef USE_DIR3DSPR
void Transform3DShape(statobj_t* statptr);
#endif

/*
=============================================================================

                             WL_STATE DEFINITIONS

=============================================================================
*/
#define TURNTICS        10
#define SPDPATROL       512
#define SPDDOG          1500

#if defined(SEGA_SATURN) && defined(EMBEDDED)
void	SpawnNewObj(unsigned tilex, unsigned tiley, int state); /* stateenum */
void	NewState(objtype* ob, int state); /* stateenum */
#else
void    SpawnNewObj(unsigned tilex, unsigned tiley, statetype* state);
void    NewState(objtype* ob, statetype* state);
#endif
boolean TryWalk(objtype* ob);
void    SelectChaseDir(objtype* ob);
void    SelectDodgeDir(objtype* ob);
void    SelectRunDir(objtype* ob);
void    MoveObj(objtype* ob, int move);
boolean SightPlayer(objtype* ob);

void    KillActor(objtype* ob);
void    DamageActor(objtype* ob, unsigned damage);

boolean CheckLine(objtype* ob);
boolean CheckSight(objtype* ob);

/*
=============================================================================

                             WL_AGENT DEFINITIONS

=============================================================================
*/

/*
** player state info
*/
extern  int thrustspeed;
#ifdef SWITCH
extern int JOYSTICK_DEAD_ZONE;
extern int JOYSTICK_MAX_ZONE;
extern HidAnalogStickState pos_left, pos_right;
#endif
extern  unsigned short     plux, pluy;         /* player coordinates scaled to unsigned */
extern  objtype* LastAttacker;
extern statetype s_player;
extern  short    anglefrac;
extern  int      facecount, facetimes;

void    Thrust(int angle, int speed);
void    SpawnPlayer(int tilex, int tiley, int dir);
void    TakeDamage(int points, objtype* attacker);
void    GivePoints(int points);
void    GivePoints(int points);
void    GetBonus(statobj_t* check);
void    GiveWeapon(int weapon);
void    GiveAmmo(int ammo);
void    GiveKey(int key);
void    StatusDrawFace(unsigned int picnum);
void    DrawFace(void);
void    DrawHealth(void);
void    HealSelf(int points);
void    DrawLevel(void);
void    DrawLives(void);
void    GiveExtraMan(void);
void    DrawScore(void);
void    DrawWeapon(void);
void    DrawKeys(void);
void    DrawAmmo(void);


/*
=============================================================================

                             WL_ACT1 DEFINITIONS

=============================================================================
*/

extern  statobj_t   statobjlist[MAXSTATS];
extern  statobj_t* laststatobj;

extern  doorobj_t   doorobjlist[MAXDOORS];
extern  doorobj_t* lastdoorobj;
#ifndef SEGA_SATURN
extern  short       doornum;
#endif

#ifdef BLAKEDOORS
extern  unsigned short      ldoorposition[MAXDOORS], rdoorposition[MAXDOORS];   /* leading edge of door 0=closed */
#endif
extern  unsigned short      doorposition[MAXDOORS];

extern  unsigned char      areaconnect[NUMAREAS][NUMAREAS];

extern  boolean   areabyplayer[NUMAREAS];

extern unsigned short     pwallstate;
extern unsigned short     pwallpos;        /* amount a pushable wall has been moved (0-63) */
extern unsigned short     pwallx, pwally;
extern unsigned char     pwalldir;
extern tiletype pwalltile;

void InitDoorList(void);
void InitStaticList(void);
void SpawnStatic(int tilex, int tiley, int type);
void SpawnDoor(int tilex, int tiley, boolean vertical, int lock);
void MoveDoors(void);
void MovePWalls(void);
void OpenDoor(unsigned int door);
void PlaceItemType(int itemtype, int tilex, int tiley);
void PushWall(int checkx, int checky, int dir);
void OperateDoor(int door);
void InitAreas(void);

/*
=============================================================================

                             WL_ACT2 DEFINITIONS

=============================================================================
*/

#define s_nakedbody s_static10
#if !defined(SEGA_SATURN) || !defined(EMBEDDED)
extern  statetype s_grddie1;
extern  statetype s_dogdie1;
extern  statetype s_ofcdie1;
extern  statetype s_mutdie1;
extern  statetype s_ssdie1;
extern  statetype s_bossdie1;
extern  statetype s_schabbdie1;
extern  statetype s_fakedie1;
extern  statetype s_mechadie1;
extern  statetype s_hitlerdie1;
extern  statetype s_greteldie1;
extern  statetype s_giftdie1;
extern  statetype s_fatdie1;

extern  statetype s_spectredie1;
extern  statetype s_angeldie1;
extern  statetype s_transdie0;
extern  statetype s_uberdie0;
extern  statetype s_willdie1;
extern  statetype s_deathdie1;


extern  statetype s_grdchase1;
extern  statetype s_dogchase1;
extern  statetype s_ofcchase1;
extern  statetype s_sschase1;
extern  statetype s_mutchase1;
extern  statetype s_bosschase1;
extern  statetype s_schabbchase1;
extern  statetype s_fakechase1;
extern  statetype s_mechachase1;
extern  statetype s_gretelchase1;
extern  statetype s_giftchase1;
extern  statetype s_fatchase1;

extern  statetype s_spectrechase1;
extern  statetype s_angelchase1;
extern  statetype s_transchase1;
extern  statetype s_uberchase1;
extern  statetype s_willchase1;
extern  statetype s_deathchase1;

extern  statetype s_blinkychase1;
extern  statetype s_hitlerchase1;

extern  statetype s_grdpain;
extern  statetype s_grdpain1;
extern  statetype s_ofcpain;
extern  statetype s_ofcpain1;
extern  statetype s_sspain;
extern  statetype s_sspain1;
extern  statetype s_mutpain;
extern  statetype s_mutpain1;

extern  statetype s_deathcam;

extern  statetype s_schabbdeathcam2;
extern  statetype s_hitlerdeathcam2;
extern  statetype s_giftdeathcam2;
extern  statetype s_fatdeathcam2;
#endif

extern  statetype s_rocket;
extern  statetype s_smoke1;
extern  statetype s_smoke2;
extern  statetype s_smoke3;
extern  statetype s_smoke4;
extern  statetype s_boom2;
extern  statetype s_boom3;

#ifdef SPEAR
extern  statetype s_hrocket;
extern  statetype s_hsmoke1;
extern  statetype s_hsmoke2;
extern  statetype s_hsmoke3;
extern  statetype s_hsmoke4;
extern  statetype s_hboom2;
extern  statetype s_hboom3;
#endif

/*
** guards
*/

extern  statetype s_grdstand;

extern  statetype s_grdpath1;
extern  statetype s_grdpath1s;
extern  statetype s_grdpath2;
extern  statetype s_grdpath3;
extern  statetype s_grdpath3s;
extern  statetype s_grdpath4;

extern  statetype s_grdpain;
extern  statetype s_grdpain1;

extern  statetype s_grdgiveup;

extern  statetype s_grdshoot1;
extern  statetype s_grdshoot2;
extern  statetype s_grdshoot3;
extern  statetype s_grdshoot4;

extern  statetype s_grdchase1;
extern  statetype s_grdchase1s;
extern  statetype s_grdchase2;
extern  statetype s_grdchase3;
extern  statetype s_grdchase3s;
extern  statetype s_grdchase4;

extern  statetype s_grddie1;
extern  statetype s_grddie1d;
extern  statetype s_grddie2;
extern  statetype s_grddie3;
extern  statetype s_grddie4;

/*
** ghosts
*/
extern  statetype s_blinkychase1;
extern  statetype s_blinkychase2;
extern  statetype s_inkychase1;
extern  statetype s_inkychase2;
extern  statetype s_pinkychase1;
extern  statetype s_pinkychase2;
extern  statetype s_clydechase1;
extern  statetype s_clydechase2;

/*
** dogs
*/
extern  statetype s_dogpath1;
extern  statetype s_dogpath1s;
extern  statetype s_dogpath2;
extern  statetype s_dogpath3;
extern  statetype s_dogpath3s;
extern  statetype s_dogpath4;

extern  statetype s_dogjump1;
extern  statetype s_dogjump2;
extern  statetype s_dogjump3;
extern  statetype s_dogjump4;
extern  statetype s_dogjump5;

extern  statetype s_dogchase1;
extern  statetype s_dogchase1s;
extern  statetype s_dogchase2;
extern  statetype s_dogchase3;
extern  statetype s_dogchase3s;
extern  statetype s_dogchase4;

extern  statetype s_dogdie1;
extern  statetype s_dogdie1d;
extern  statetype s_dogdie2;
extern  statetype s_dogdie3;
extern  statetype s_dogdead;


/*
** officers
*/

extern  statetype s_ofcstand;

extern  statetype s_ofcpath1;
extern  statetype s_ofcpath1s;
extern  statetype s_ofcpath2;
extern  statetype s_ofcpath3;
extern  statetype s_ofcpath3s;
extern  statetype s_ofcpath4;

extern  statetype s_ofcpain;
extern  statetype s_ofcpain1;

extern  statetype s_ofcgiveup;

extern  statetype s_ofcshoot1;
extern  statetype s_ofcshoot2;
extern  statetype s_ofcshoot3;
extern  statetype s_ofcshoot4;

extern  statetype s_ofcchase1;
extern  statetype s_ofcchase1s;
extern  statetype s_ofcchase2;
extern  statetype s_ofcchase3;
extern  statetype s_ofcchase3s;
extern  statetype s_ofcchase4;

extern  statetype s_ofcdie1;
extern  statetype s_ofcdie2;
extern  statetype s_ofcdie3;
extern  statetype s_ofcdie4;
extern  statetype s_ofcdie5;

/*
** mutant
*/

extern  statetype s_mutstand;

extern  statetype s_mutpath1;
extern  statetype s_mutpath1s;
extern  statetype s_mutpath2;
extern  statetype s_mutpath3;
extern  statetype s_mutpath3s;
extern  statetype s_mutpath4;

extern  statetype s_mutpain;
extern  statetype s_mutpain1;

extern  statetype s_mutgiveup;

extern  statetype s_mutshoot1;
extern  statetype s_mutshoot2;
extern  statetype s_mutshoot3;
extern  statetype s_mutshoot4;

extern  statetype s_mutchase1;
extern  statetype s_mutchase1s;
extern  statetype s_mutchase2;
extern  statetype s_mutchase3;
extern  statetype s_mutchase3s;
extern  statetype s_mutchase4;

extern  statetype s_mutdie1;
extern  statetype s_mutdie2;
extern  statetype s_mutdie3;
extern  statetype s_mutdie4;
extern  statetype s_mutdie5;

/*
** SS
*/

extern  statetype s_ssstand;

extern  statetype s_sspath1;
extern  statetype s_sspath1s;
extern  statetype s_sspath2;
extern  statetype s_sspath3;
extern  statetype s_sspath3s;
extern  statetype s_sspath4;

extern  statetype s_sspain;
extern  statetype s_sspain1;

extern  statetype s_ssshoot1;
extern  statetype s_ssshoot2;
extern  statetype s_ssshoot3;
extern  statetype s_ssshoot4;
extern  statetype s_ssshoot5;
extern  statetype s_ssshoot6;
extern  statetype s_ssshoot7;
extern  statetype s_ssshoot8;
extern  statetype s_ssshoot9;

extern  statetype s_sschase1;
extern  statetype s_sschase1s;
extern  statetype s_sschase2;
extern  statetype s_sschase3;
extern  statetype s_sschase3s;
extern  statetype s_sschase4;

extern  statetype s_ssdie1;
extern  statetype s_ssdie2;
extern  statetype s_ssdie3;
extern  statetype s_ssdie4;

#ifndef SPEAR
/*
** hans
*/
extern  statetype s_bossstand;

extern  statetype s_bosschase1;
extern  statetype s_bosschase1s;
extern  statetype s_bosschase2;
extern  statetype s_bosschase3;
extern  statetype s_bosschase3s;
extern  statetype s_bosschase4;

extern  statetype s_bossdie1;
extern  statetype s_bossdie2;
extern  statetype s_bossdie3;
extern  statetype s_bossdie4;

extern  statetype s_bossshoot1;
extern  statetype s_bossshoot2;
extern  statetype s_bossshoot3;
extern  statetype s_bossshoot4;
extern  statetype s_bossshoot5;
extern  statetype s_bossshoot6;
extern  statetype s_bossshoot7;
extern  statetype s_bossshoot8;

/*
** gretel
*/
extern  statetype s_gretelstand;

extern  statetype s_gretelchase1;
extern  statetype s_gretelchase1s;
extern  statetype s_gretelchase2;
extern  statetype s_gretelchase3;
extern  statetype s_gretelchase3s;
extern  statetype s_gretelchase4;

extern  statetype s_greteldie1;
extern  statetype s_greteldie2;
extern  statetype s_greteldie3;
extern  statetype s_greteldie4;

extern  statetype s_gretelshoot1;
extern  statetype s_gretelshoot2;
extern  statetype s_gretelshoot3;
extern  statetype s_gretelshoot4;
extern  statetype s_gretelshoot5;
extern  statetype s_gretelshoot6;
extern  statetype s_gretelshoot7;
extern  statetype s_gretelshoot8;
#endif

/*
=============================================================================

                                SPEAR ACTORS

=============================================================================
*/
#ifdef SPEAR
/*
** angel
*/
extern  statetype s_angelstand;

extern  statetype s_angelchase1;
extern  statetype s_angelchase1s;
extern  statetype s_angelchase2;
extern  statetype s_angelchase3;
extern  statetype s_angelchase3s;
extern  statetype s_angelchase4;

extern  statetype s_angeldie1;
extern  statetype s_angeldie11;
extern  statetype s_angeldie2;
extern  statetype s_angeldie3;
extern  statetype s_angeldie4;
extern  statetype s_angeldie5;
extern  statetype s_angeldie6;
extern  statetype s_angeldie7;
extern  statetype s_angeldie8;
extern  statetype s_angeldie9;

extern  statetype s_angelshoot1;
extern  statetype s_angelshoot2;
extern  statetype s_angelshoot3;
extern  statetype s_angelshoot4;
extern  statetype s_angelshoot5;
extern  statetype s_angelshoot6;

extern  statetype s_angeltired;
extern  statetype s_angeltired2;
extern  statetype s_angeltired3;
extern  statetype s_angeltired4;
extern  statetype s_angeltired5;
extern  statetype s_angeltired6;
extern  statetype s_angeltired7;

extern  statetype s_spark1;
extern  statetype s_spark2;
extern  statetype s_spark3;
extern  statetype s_spark4;

/*
** trans
*/
extern  statetype s_transstand;

extern  statetype s_transchase1;
extern  statetype s_transchase1s;
extern  statetype s_transchase2;
extern  statetype s_transchase3;
extern  statetype s_transchase3s;
extern  statetype s_transchase4;

extern  statetype s_transdie0;
extern  statetype s_transdie01;
extern  statetype s_transdie1;
extern  statetype s_transdie2;
extern  statetype s_transdie3;
extern  statetype s_transdie4;

extern  statetype s_transshoot1;
extern  statetype s_transshoot2;
extern  statetype s_transshoot3;
extern  statetype s_transshoot4;
extern  statetype s_transshoot5;
extern  statetype s_transshoot6;
extern  statetype s_transshoot7;
extern  statetype s_transshoot8;

extern  statetype s_uberstand;

extern  statetype s_uberchase1;
extern  statetype s_uberchase1s;
extern  statetype s_uberchase2;
extern  statetype s_uberchase3;
extern  statetype s_uberchase3s;
extern  statetype s_uberchase4;

extern  statetype s_uberdie0;
extern  statetype s_uberdie01;
extern  statetype s_uberdie1;
extern  statetype s_uberdie2;
extern  statetype s_uberdie3;
extern  statetype s_uberdie4;
extern  statetype s_uberdie5;

extern  statetype s_ubershoot1;
extern  statetype s_ubershoot2;
extern  statetype s_ubershoot3;
extern  statetype s_ubershoot4;
extern  statetype s_ubershoot5;
extern  statetype s_ubershoot6;
extern  statetype s_ubershoot7;

/*
** will
*/
extern  statetype s_willstand;

extern  statetype s_willchase1;
extern  statetype s_willchase1s;
extern  statetype s_willchase2;
extern  statetype s_willchase3;
extern  statetype s_willchase3s;
extern  statetype s_willchase4;

extern  statetype s_willdie1;
extern  statetype s_willdie2;
extern  statetype s_willdie3;
extern  statetype s_willdie4;
extern  statetype s_willdie5;
extern  statetype s_willdie6;

extern  statetype s_willshoot1;
extern  statetype s_willshoot2;
extern  statetype s_willshoot3;
extern  statetype s_willshoot4;
extern  statetype s_willshoot5;
extern  statetype s_willshoot6;

/*
** death
*/
extern  statetype s_deathstand;

extern  statetype s_deathchase1;
extern  statetype s_deathchase1s;
extern  statetype s_deathchase2;
extern  statetype s_deathchase3;
extern  statetype s_deathchase3s;
extern  statetype s_deathchase4;

extern  statetype s_deathdie1;
extern  statetype s_deathdie2;
extern  statetype s_deathdie3;
extern  statetype s_deathdie4;
extern  statetype s_deathdie5;
extern  statetype s_deathdie6;
extern  statetype s_deathdie7;
extern  statetype s_deathdie8;
extern  statetype s_deathdie9;

extern  statetype s_deathshoot1;
extern  statetype s_deathshoot2;
extern  statetype s_deathshoot3;
extern  statetype s_deathshoot4;
extern  statetype s_deathshoot5;

/*
** spectre
*/
extern  statetype s_spectrewait1;
extern  statetype s_spectrewait2;
extern  statetype s_spectrewait3;
extern  statetype s_spectrewait4;

extern  statetype s_spectrechase1;
extern  statetype s_spectrechase2;
extern  statetype s_spectrechase3;
extern  statetype s_spectrechase4;

extern  statetype s_spectredie1;
extern  statetype s_spectredie2;
extern  statetype s_spectredie3;
extern  statetype s_spectredie4;

extern  statetype s_spectrewake;

#endif

/*
** schabb
*/
extern  statetype s_schabbstand;

extern  statetype s_schabbchase1;
extern  statetype s_schabbchase1s;
extern  statetype s_schabbchase2;
extern  statetype s_schabbchase3;
extern  statetype s_schabbchase3s;
extern  statetype s_schabbchase4;

extern  statetype s_schabbdie1;
extern  statetype s_schabbdie2;
extern  statetype s_schabbdie3;
extern  statetype s_schabbdie4;
extern  statetype s_schabbdie5;
extern  statetype s_schabbdie6;

extern  statetype s_schabbshoot1;
extern  statetype s_schabbshoot2;

extern  statetype s_needle1;
extern  statetype s_needle2;
extern  statetype s_needle3;
extern  statetype s_needle4;

extern  statetype s_schabbdeathcam;

/*
** gift
*/
extern  statetype s_giftstand;

extern  statetype s_giftchase1;
extern  statetype s_giftchase1s;
extern  statetype s_giftchase2;
extern  statetype s_giftchase3;
extern  statetype s_giftchase3s;
extern  statetype s_giftchase4;

extern  statetype s_giftdie1;
extern  statetype s_giftdie2;
extern  statetype s_giftdie3;
extern  statetype s_giftdie4;
extern  statetype s_giftdie5;
extern  statetype s_giftdie6;

extern  statetype s_giftshoot1;
extern  statetype s_giftshoot2;

extern  statetype s_needle1;
extern  statetype s_needle2;
extern  statetype s_needle3;
extern  statetype s_needle4;

extern  statetype s_giftdeathcam;

extern  statetype s_boom1;
extern  statetype s_boom2;
extern  statetype s_boom3;

/*
** fat
*/
extern  statetype s_fatstand;

extern  statetype s_fatchase1;
extern  statetype s_fatchase1s;
extern  statetype s_fatchase2;
extern  statetype s_fatchase3;
extern  statetype s_fatchase3s;
extern  statetype s_fatchase4;

extern  statetype s_fatdie1;
extern  statetype s_fatdie2;
extern  statetype s_fatdie3;
extern  statetype s_fatdie4;
extern  statetype s_fatdie5;
extern  statetype s_fatdie6;

extern  statetype s_fatshoot1;
extern  statetype s_fatshoot2;
extern  statetype s_fatshoot3;
extern  statetype s_fatshoot4;
extern  statetype s_fatshoot5;
extern  statetype s_fatshoot6;

extern  statetype s_needle1;
extern  statetype s_needle2;
extern  statetype s_needle3;
extern  statetype s_needle4;

extern  statetype s_fatdeathcam;

/*
=============================================================================

                                    HITLERS

=============================================================================
*/


/*
** fake
*/
extern  statetype s_fakestand;

extern  statetype s_fakechase1;
extern  statetype s_fakechase1s;
extern  statetype s_fakechase2;
extern  statetype s_fakechase3;
extern  statetype s_fakechase3s;
extern  statetype s_fakechase4;

extern  statetype s_fakedie1;
extern  statetype s_fakedie2;
extern  statetype s_fakedie3;
extern  statetype s_fakedie4;
extern  statetype s_fakedie5;
extern  statetype s_fakedie6;

extern  statetype s_fakeshoot1;
extern  statetype s_fakeshoot2;
extern  statetype s_fakeshoot3;
extern  statetype s_fakeshoot4;
extern  statetype s_fakeshoot5;
extern  statetype s_fakeshoot6;
extern  statetype s_fakeshoot7;
extern  statetype s_fakeshoot8;
extern  statetype s_fakeshoot9;

extern  statetype s_fire1;
extern  statetype s_fire2;


/*
** hitler
*/
extern  statetype s_mechachase1;
extern  statetype s_mechachase1s;
extern  statetype s_mechachase2;
extern  statetype s_mechachase3;
extern  statetype s_mechachase3s;
extern  statetype s_mechachase4;

extern  statetype s_mechadie1;
extern  statetype s_mechadie2;
extern  statetype s_mechadie3;
extern  statetype s_mechadie4;

extern  statetype s_mechashoot1;
extern  statetype s_mechashoot2;
extern  statetype s_mechashoot3;
extern  statetype s_mechashoot4;
extern  statetype s_mechashoot5;
extern  statetype s_mechashoot6;


extern  statetype s_hitlerchase1;
extern  statetype s_hitlerchase1s;
extern  statetype s_hitlerchase2;
extern  statetype s_hitlerchase3;
extern  statetype s_hitlerchase3s;
extern  statetype s_hitlerchase4;

extern  statetype s_hitlerdie1;
extern  statetype s_hitlerdie2;
extern  statetype s_hitlerdie3;
extern  statetype s_hitlerdie4;
extern  statetype s_hitlerdie5;
extern  statetype s_hitlerdie6;
extern  statetype s_hitlerdie7;
extern  statetype s_hitlerdie8;
extern  statetype s_hitlerdie9;
extern  statetype s_hitlerdie10;

extern  statetype s_hitlershoot1;
extern  statetype s_hitlershoot2;
extern  statetype s_hitlershoot3;
extern  statetype s_hitlershoot4;
extern  statetype s_hitlershoot5;
extern  statetype s_hitlershoot6;

extern  statetype s_hitlerdeathcam;

#ifndef SPEAR
/*
============================================================================

                                    BJ VICTORY

============================================================================
*/

/*
** BJ victory
*/

extern  statetype s_bjrun1;
extern  statetype s_bjrun1s;
extern  statetype s_bjrun2;
extern  statetype s_bjrun3;
extern  statetype s_bjrun3s;
extern  statetype s_bjrun4;

extern  statetype s_bjjump1;
extern  statetype s_bjjump2;
extern  statetype s_bjjump3;
extern  statetype s_bjjump4;

#endif

void SpawnStand(enemy_t which, int tilex, int tiley, int dir);
void SpawnPatrol(enemy_t which, int tilex, int tiley, int dir);

void SpawnDeadGuard(int tilex, int tiley);
void SpawnBoss(int tilex, int tiley);
void SpawnGretel(int tilex, int tiley);
void SpawnTrans(int tilex, int tiley);
void SpawnUber(int tilex, int tiley);
void SpawnWill(int tilex, int tiley);
void SpawnDeath(int tilex, int tiley);
void SpawnAngel(int tilex, int tiley);
void SpawnSpectre(int tilex, int tiley);
void SpawnGhosts(int which, int tilex, int tiley);
void SpawnSchabbs(int tilex, int tiley);
void SpawnGift(int tilex, int tiley);
void SpawnFat(int tilex, int tiley);
void SpawnFakeHitler(int tilex, int tiley);
void SpawnHitler(int tilex, int tiley);

void A_DeathScream(objtype* ob);
void SpawnBJVictory(void);

/*
=============================================================================

                             WL_TEXT DEFINITIONS

=============================================================================
*/

extern  char    helpfilename[], endfilename[];

extern  void    HelpScreens(void);
extern  void    EndText(void);
#ifdef AUTOINTER
extern  void    IntermissionScreens(void);
#endif
#ifdef LOGFILE
extern  void    LogDiscScreens(char* choice);
#endif
/*
=============================================================================

                             WL_MISC DEFINITIONS

=============================================================================
*/

#ifdef PLAYDEMOLIKEORIGINAL
#define DEMOCHOOSE_ORIG_SDL(orig, sdl) ((demorecord || demoplayback) ? (orig) : (sdl))
#define DEMOCOND_ORIG                  (demorecord || demoplayback)
#define DEMOIF_SDL                     if(DEMOCOND_SDL)
#else
#define DEMOCHOOSE_ORIG_SDL(orig, sdl) (sdl)
#define DEMOCOND_ORIG                  false
#define DEMOIF_SDL
#endif
#define DEMOCOND_SDL                   (!DEMOCOND_ORIG)

/*
=============================================================================

                           WL_FEATURE DEFINITIONS

=============================================================================
*/

#ifdef USE_FEATUREFLAGS
/* The currently available feature flags */
#define FF_STARSKY      0x0001
#define FF_PARALLAXSKY  0x0002
#define FF_CLOUDSKY     0x0004
#define FF_RAIN         0x0010
#define FF_SNOW         0x0020

/* 
** The ffData... variables contain the 16-bit values of the according corners of the current level.
** The corners are overwritten with adjacent tiles after initialization in SetupGameLevel
** to avoid interpretation as e.g. doors.
*/
extern int ffDataTopLeft, ffDataTopRight, ffDataBottomLeft, ffDataBottomRight;

/*************************************************************
 * Current usage of ffData... variables:
 * ffDataTopLeft:     lower 8-bit: ShadeDefID
 * ffDataTopRight:    FeatureFlags
 * ffDataBottomLeft:  CloudSkyDefID or ParallaxStartTexture
 * ffDataBottomRight: high byte: ceiling texture - low byte: floor texture
 *************************************************************/

/* The feature flags are stored as a wall in the upper right corner of each level */
static wlinline unsigned short GetFeatureFlags(void)
{
    return ffDataTopRight;
}

#endif

#ifdef USE_FLOORCEILINGTEX
extern unsigned char* ceilingsource, * floorsource;

void DrawPlanes(void);
#ifndef USE_MULTIFLATS
void GetFlatTextures(void);
#endif
#endif

#ifdef USE_PARALLAX
void DrawParallax(void);
#endif

#endif /* __WL_DEF_H_ */
