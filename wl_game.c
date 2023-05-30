/* WL_GAME.C */
#include <math.h>
#include "wl_def.h"
#ifndef SEGA_SATURN
#include <SDL_mixer.h>
#endif

#ifdef MYPROFILE
#include <TIME.H>
#endif
#ifdef SEGA_SATURN
extern fixed MTH_Atan(fixed y, fixed x);
short atan2fix(fixed x, fixed y);

#ifdef USE_SPRITES
unsigned int position_vram = ((SATURN_WIDTH + 64) * 64);
/* unsigned int static_items=0; */
extern unsigned char wall_buffer[(SATURN_WIDTH + 64) * 64];
extern TEXTURE tex_spr[SPR_NULLSPRITE + SATURN_WIDTH];
unsigned char texture_list[SPR_NULLSPRITE];
#endif

void readChunks(Sint32 fileId, unsigned int size, unsigned int* pageOffsets, unsigned char* Chunks, unsigned char* ptr);
unsigned char* PM_DecodeSprites2(unsigned int start, unsigned int endi, unsigned int* pageOffsets, word* pageLengths, unsigned char* ptr, int fileId);

#undef atan2
/* #define atan2(a,b) slAtan(a,b) */
#define atan2(a,b) MTH_Atan(a,b)
#endif

/*
=============================================================================
                             LOCAL CONSTANTS
=============================================================================
*/


/*
=============================================================================
                             GLOBAL VARIABLES
=============================================================================
*/

boolean         ingame,fizzlein;
gametype        gamestate;
unsigned char            bordercol=VIEWCOLOR;        /* color of the Change View/Ingame border */

#ifdef SPEAR
int             spearx,speary;
short        spearangle;
boolean         spearflag;
#endif

#ifdef USE_FEATUREFLAGS
int ffDataTopLeft, ffDataTopRight, ffDataBottomLeft, ffDataBottomRight;
#endif

/*
** ELEVATOR BACK MAPS - REMEMBER (-1)!!
*/
int ElevatorBackTo[]={1,1,7,3,5,3};

void SetupGameLevel (void);
void DrawPlayScreen (void);
void GameLoop (void);
#ifdef PUSHOBJECT /* Objects that can be pushed */
/* Arrays for quick checking cardinal direction locations on the map */
char dx4dir[4] = { 0, 1, 0, -1 };  /* dx & dy based on direction */
char dy4dir[4] = { -1, 0, 1,  0 };
#endif
/*
=============================================================================
                             LOCAL VARIABLES
=============================================================================
*/



/* =========================================================================== */
/* =========================================================================== */


/*
==========================
=
= SetSoundLoc - Given the location of an object (in terms of global
=       coordinates, held in globalsoundx and globalsoundy), munges the values
=       for an approximate distance from the left and right ear, and puts
=       those values into leftchannel and rightchannel.
=
= JAB
=
==========================
*/

int leftchannel, rightchannel;
#define ATABLEMAX 15
unsigned char righttable[ATABLEMAX][ATABLEMAX * 2] = {
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 6, 0, 0, 0, 0, 0, 1, 3, 5, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 4, 0, 0, 0, 0, 0, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 4, 1, 0, 0, 0, 1, 2, 4, 6, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 6, 5, 4, 2, 1, 0, 1, 2, 3, 5, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 5, 4, 3, 2, 2, 3, 3, 5, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 6, 6, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
};
unsigned char lefttable[ATABLEMAX][ATABLEMAX * 2] = {
{ 8, 8, 8, 8, 8, 8, 8, 8, 5, 3, 1, 0, 0, 0, 0, 0, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 0, 0, 0, 0, 0, 4, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 6, 4, 2, 1, 0, 0, 0, 1, 4, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 7, 5, 3, 2, 1, 0, 1, 2, 4, 5, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 5, 3, 3, 2, 2, 3, 4, 5, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 5, 4, 4, 4, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 6, 6, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
{ 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}
};
#ifndef SEGA_SATURN
void
SetSoundLoc(fixed gx,fixed gy)
{
    fixed   xt,yt;
    fixed     x,y;

/*
** translate point to view centered coordinates
*/
    gx -= viewx;
    gy -= viewy;

/*
** calculate newx
*/
    xt = FixedMul(gx,viewcos);
    yt = FixedMul(gy,viewsin);
    x = (xt - yt) >> TILESHIFT;

/*
** calculate newy
*/
    xt = FixedMul(gx,viewsin);
    yt = FixedMul(gy,viewcos);
    y = (yt + xt) >> TILESHIFT;

    if (y >= ATABLEMAX)
        y = ATABLEMAX - 1;
    else if (y <= -ATABLEMAX)
        y = -ATABLEMAX;
    if (x < 0)
        x = -x;
    if (x >= ATABLEMAX)
        x = ATABLEMAX - 1;
    leftchannel  =  lefttable[x][y + ATABLEMAX];
    rightchannel = righttable[x][y + ATABLEMAX];

#if 0
    CenterWindow(8,1);
    US_PrintSigned(leftchannel);
    US_Print(",");
    US_PrintSigned(rightchannel);
    VL_UpdateScreen(screenBuffer);
#endif
}

/*
==========================
=
= SetSoundLocGlobal - Sets up globalsoundx & globalsoundy and then calls
=       UpdateSoundLoc() to transform that into relative channel volumes. Those
=       values are then passed to the Sound Manager so that they'll be used for
=       the next sound played (if possible).
=
= JAB
=
==========================
*/
void PlaySoundLocGlobal(unsigned short s,fixed gx,fixed gy)
{
	boolean channel;
    SetSoundLoc(gx, gy);
    SD_PositionSound(leftchannel, rightchannel);

    channel = SD_PlaySound((soundnames) s);
    if(channel)
    {
        channelSoundPos[channel - 1].globalsoundx = gx;
        channelSoundPos[channel - 1].globalsoundy = gy;
        channelSoundPos[channel - 1].valid = 1;
    }
}

void UpdateSoundLoc(void)
{
    int i;

/*    if (SoundPositioned)
    {
        SetSoundLoc(globalsoundx,globalsoundy);
        SD_SetPosition(leftchannel,rightchannel);
    }*/

#ifdef VIEASM
    for (i = 0; i < ASM_CurChannels(); i++)
#else
    for (i = 0; i < MIX_CHANNELS; i++)
#endif
    {
        if(channelSoundPos[i].valid)
        {
            SetSoundLoc(channelSoundPos[i].globalsoundx,
                channelSoundPos[i].globalsoundy);
            SD_SetPosition(i, leftchannel, rightchannel);
        }
    }
}
#endif
/*
**      JAB End
*/

/*
==========================
=
= ScanInfoPlane
=
= Spawn all actors and mark down special places
=
==========================
*/

static void ScanInfoPlane(void)
{
    unsigned char x,y;
    int      tile;
    unsigned short *start;

#ifdef SEGA_SATURN
    /* ----------------------------------------------------------------------------------- */
    unsigned char* itemmap = (unsigned char*)saturnChunk + 0x4000; /* ne pas toucher */
    /* ----------------------------------------------------------------------------------- */
#endif

    start = mapsegs[1];
    for (y=0;y<mapheight;y++)
    {
        for (x=0;x<mapwidth;x++)
        {
            tile = *start++;
            if (!tile)
                continue;

            switch (tile)
            {
                case 19:
                case 20:
                case 21:
                case 22:
                    SpawnPlayer(x,y,NORTH+tile-19);
                    break;

                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:

                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:

                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:

                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:

                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                case 61:
                case 62:

                case 63:
                case 64:
                case 65:
                case 66:
                case 67:
                case 68:
                case 69:
                case 70:
                case 71:
                case 72:
#ifdef SPEAR
                case 73:                        /* TRUCK AND SPEAR! */
                case 74:
#elif defined(USE_DIR3DSPR)                     /* just for the example */
                case 73:
#endif
                    SpawnStatic(x,y,tile-23);
                    break;

/*
** P wall
*/
                case 98:
#ifndef SEGA_SATURN
                    if (!loadedgame)
#endif
                        gamestate.secrettotal++;
                    break;
#ifdef SEGA_SATURN
#ifndef SPEAR
                case 99:
                    loaded += PRELOAD_ITEMS(SPR_DEATHCAM, SPR_DEATHCAM);
                    loaded += PRELOAD_ITEMS(SPR_BJ_W1, SPR_BJ_JUMP4);
                    break;
#endif
#endif

/*
** guard
*/
                case 180:
                case 181:
                case 182:
                case 183:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 144:
                case 145:
                case 146:
                case 147:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 108:
                case 109:
                case 110:
                case 111:
                    SpawnStand(en_guard,x,y,tile-108);
                    break;


                case 184:
                case 185:
                case 186:
                case 187:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 148:
                case 149:
                case 150:
                case 151:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 112:
                case 113:
                case 114:
                case 115:
                    SpawnPatrol(en_guard,x,y,tile-112);
                    break;

                case 124:
                    SpawnDeadGuard (x,y);
                    break;
/*
** officer
*/
                case 188:
                case 189:
                case 190:
                case 191:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 152:
                case 153:
                case 154:
                case 155:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 116:
                case 117:
                case 118:
                case 119:
                    SpawnStand(en_officer,x,y,tile-116);
                    break;


                case 192:
                case 193:
                case 194:
                case 195:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 156:
                case 157:
                case 158:
                case 159:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 120:
                case 121:
                case 122:
                case 123:
                    SpawnPatrol(en_officer,x,y,tile-120);
                    break;


/*
** ss
*/
                case 198:
                case 199:
                case 200:
                case 201:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 162:
                case 163:
                case 164:
                case 165:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 126:
                case 127:
                case 128:
                case 129:
                    SpawnStand(en_ss,x,y,tile-126);
                    break;


                case 202:
                case 203:
                case 204:
                case 205:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 166:
                case 167:
                case 168:
                case 169:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 130:
                case 131:
                case 132:
                case 133:
                    SpawnPatrol(en_ss,x,y,tile-130);
                    break;

/*
** dogs
*/
                case 206:
                case 207:
                case 208:
                case 209:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 170:
                case 171:
                case 172:
                case 173:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 134:
                case 135:
                case 136:
                case 137:
                    SpawnStand(en_dog,x,y,tile-134);
                    break;


                case 210:
                case 211:
                case 212:
                case 213:
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 36;
                    /* fall through */
                case 174:
                case 175:
                case 176:
                case 177:
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 36;
                    /* fall through */
                case 138:
                case 139:
                case 140:
                case 141:
                    SpawnPatrol(en_dog,x,y,tile-138);
                    break;

/*
** boss
*/
#ifndef SPEAR
                case 214:
                    SpawnBoss (x,y);
                    break;
                case 197:
                    SpawnGretel (x,y);
                    break;
                case 215:
                    SpawnGift (x,y);
                    break;
                case 179:
                    SpawnFat (x,y);
                    break;
                case 196:
                    SpawnSchabbs (x,y);
                    break;
                case 160:
                    SpawnFakeHitler (x,y);
                    break;
                case 178:
                    SpawnHitler (x,y);
                    break;
#else
                case 106:
                    SpawnSpectre (x,y);
                    break;
                case 107:
                    SpawnAngel (x,y);
                    break;
                case 125:
                    SpawnTrans (x,y);
                    break;
                case 142:
                    SpawnUber (x,y);
                    break;
                case 143:
                    SpawnWill (x,y);
                    break;
                case 161:
                    SpawnDeath (x,y);
                    break;

#endif

/*
** mutants
*/
                case 252:
                case 253:
                case 254:
                case 255:
#ifdef SEGA_SATURN
                    if (itemmap[SPR_MUT_S_1 + PMSpriteStart] == 0)
                        PRELOAD_ITEMS(SPR_MUT_S_1, SPR_MUT_SHOOT4);
#endif

                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 18;
                    /* fall through */
                case 234:
                case 235:
                case 236:
                case 237:
#ifdef SEGA_SATURN
                    if (itemmap[SPR_MUT_S_1 + PMSpriteStart] == 0)
                        PRELOAD_ITEMS(SPR_MUT_S_1, SPR_MUT_SHOOT4);
#endif
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 18;
                    /* fall through */
                case 216:
                case 217:
                case 218:
                case 219:
#ifdef SEGA_SATURN
                    if (itemmap[SPR_MUT_S_1 + PMSpriteStart] == 0)
                        PRELOAD_ITEMS(SPR_MUT_S_1, SPR_MUT_SHOOT4);
#endif
                    SpawnStand(en_mutant,x,y,tile-216);
                    break;

                case 256:
                case 257:
                case 258:
                case 259:
#ifdef SEGA_SATURN
                    if (itemmap[SPR_MUT_S_1 + PMSpriteStart] == 0)
                        PRELOAD_ITEMS(SPR_MUT_S_1, SPR_MUT_SHOOT4);
#endif
                    if (gamestate.difficulty<gd_hard)
                        break;
                    tile -= 18;
                    /* fall through */
                case 238:
                case 239:
                case 240:
                case 241:
#ifdef SEGA_SATURN
                    if (itemmap[SPR_MUT_S_1 + PMSpriteStart] == 0)
                        PRELOAD_ITEMS(SPR_MUT_S_1, SPR_MUT_SHOOT4);
#endif
                    if (gamestate.difficulty<gd_medium)
                        break;
                    tile -= 18;
                    /* fall through */
                case 220:
                case 221:
                case 222:
                case 223:
                    SpawnPatrol(en_mutant,x,y,tile-220);
#ifdef SEGA_SATURN
                    if (itemmap[SPR_MUT_S_1 + PMSpriteStart] == 0)
                        PRELOAD_ITEMS(SPR_MUT_S_1, SPR_MUT_SHOOT4);
#endif
                    break;

/*
** ghosts
*/
#ifndef SPEAR
                case 224:
                    SpawnGhosts (en_blinky,x,y);
                    break;
                case 225:
                    SpawnGhosts (en_clyde,x,y);
                    break;
                case 226:
                    SpawnGhosts (en_pinky,x,y);
                    break;
                case 227:
                    SpawnGhosts (en_inky,x,y);
                    break;
#endif
            }
        }
    }

}

/* ========================================================================== */

#ifdef SEGA_SATURN
void VblIn(void);

unsigned char* wallData = NULL;
#endif

/*
==================
=
= SetupGameLevel
=
==================
*/

void SetupGameLevel (void)
{
    int  x,y;
    int  mapnum;
    unsigned short *map;
    unsigned short  tile;

#ifndef SEGA_SATURN
    if (!loadedgame)
#endif
    {
        gamestate.TimeCount
            = gamestate.secrettotal
            = gamestate.killtotal
            = gamestate.treasuretotal
            = gamestate.secretcount
            = gamestate.killcount
            = gamestate.treasurecount
            = pwallstate = pwallpos = facetimes = 0;
#if defined(BOSS_MUSIC) && defined(VIEASM)
        SetLevelMusic();
#endif
        LastAttacker = NULL;
        killerobj = NULL;
    }

    if (demoplayback || demorecord)
        US_InitRndT (false);
    else
        US_InitRndT (true);

/*
** load the level
*/
    mapnum = gamestate.mapon+10*gamestate.episode;

    CA_CacheMap (mapnum);

    mapwidth = mapheaderseg[mapnum]->width;
    mapheight = mapheaderseg[mapnum]->height;

#ifdef USE_FEATUREFLAGS
    /* Temporary definition to make things clearer */
    #define MXX MAPSIZE - 1

    /* Read feature flags data from map corners and overwrite corners with adjacent tiles */
    ffDataTopLeft     = MAPSPOT(0,   0,   0); MAPSPOT(0,   0,   0) = MAPSPOT(1,       0,       0);
    ffDataTopRight    = MAPSPOT(MXX, 0,   0); MAPSPOT(MXX, 0,   0) = MAPSPOT(MXX,     1,       0);
    ffDataBottomRight = MAPSPOT(MXX, MXX, 0); MAPSPOT(MXX, MXX, 0) = MAPSPOT(MXX - 1, MXX,     0);
    ffDataBottomLeft  = MAPSPOT(0,   MXX, 0); MAPSPOT(0,   MXX, 0) = MAPSPOT(0,       MXX - 1, 0);

    #undef MXX
#endif

/*
** copy the wall data to a data segment array
*/
    memset (tilemap,0,sizeof(tilemap));
    memset (actorat,0,sizeof(actorat));
#ifdef REVEALMAP
    memset (mapseen,0,sizeof(mapseen));
#endif
#ifdef AUTOMAP
    memset(automap, 0, sizeof(automap));
#endif
    map = mapsegs[0];
    for (y=0;y<mapheight;y++)
    {
        for (x=0;x<mapwidth;x++)
        {
            tile = *map++;
            if (tile< AMBUSHTILE)
            {
                /* solid wall */
                tilemap[x][y] = (unsigned char) tile;
                actorat[x][y] = (objtype *)(uintptr_t) tile;
            }
            else
            {
                /* area floor */
                tilemap[x][y] = 0;
                actorat[x][y] = 0;
            }
        }
    }

/*
** spawn doors
*/
    InitActorList ();                       /* start spawning things with a clean slate */
    InitDoorList ();
    InitStaticList ();

#if defined(SEGA_SATURN)
    char fname[13] = "VSWAP.";
    /*	Uint32 i=0;  */
    unsigned char* Chunks;
    long fileSize;
    int i;
    unsigned int* pageOffsets;
    unsigned short* pageLengths;
    Sint32 fileId;
    long pageDataSize;
    unsigned char* itemmap; 
    strcat(fname, extension);

    fileId = GFS_NameToId((char*)fname);
    fileSize = GetFileSize(fileId);

    Chunks = (unsigned char*)saturnChunk;
    GFS_Load(fileId, 0, (void*)Chunks, 0x2000);
    ChunksInFile = Chunks[0] | Chunks[1] << 8;
    PMSpriteStart = Chunks[2] | Chunks[3] << 8;

    /* vbt : on ne charge pas les sons ! */
    ChunksInFile = Chunks[4] | Chunks[5] << 8;

    pageOffsets = (unsigned int*)saturnChunk + 0x2000;
    pageLengths = (unsigned short*)saturnChunk + (ChunksInFile + 1) * sizeof(int32_t);

    for (i = 0; i < ChunksInFile; i++)
    {
        pageOffsets[i] = Chunks[6] << 0 | Chunks[7] << 8 | Chunks[8] << 16 | Chunks[9] << 24;
        Chunks += 4;
    }

    for (i = PMSpriteStart; i < ChunksInFile; i++)
    {
        pageLengths[i - PMSpriteStart] = Chunks[6] | Chunks[7] << 8;
        Chunks += 2;
    }

    /* fread(pageLengths, sizeof(word), ChunksInFile, file); */
    pageDataSize = fileSize - pageOffsets[0];
    if (pageDataSize > (unsigned int) -1)
        Quit("The page file \"%s\" is too large!", fname);

    pageOffsets[ChunksInFile] = fileSize;

    itemmap = (unsigned char*)saturnChunk + 0x4000;
    memset(itemmap, 0x00, 0x2000); /* itemmap et itemmap communs, ne pas toucher � la taille du memset */
#endif

    map = mapsegs[0];
    for (y=0;y<mapheight;y++)
    {
        for (x=0;x<mapwidth;x++)
        {
            tile = *map++;
            if (tile >= DOORSTART && tile <= DOOREND)
            {
                /* door */
                switch (tile)
                {
                case 90:
                case 92:
                case 94:
                case 96:
                case 98:
                case 100:
                    SpawnDoor((unsigned char)x, (unsigned char)y, true, (tile - 90) / 2);
                    break;
                case 91:
                case 93:
                case 95:
                case 97:
                case 99:
                case 101:
                    SpawnDoor((unsigned char)x, (unsigned char)y, false, (tile - 91) / 2);
                    break;
                }
            }
        }
    }

#ifdef USE_SPRITES
    memset(texture_list, 0x00, SPR_NULLSPRITE);
    position_vram = (SATURN_WIDTH + 64) * 64;

    if (viewheight == screenHeight)
        VL_ClearScreen(0);
#endif

/*
** spawn actors
*/
    ScanInfoPlane ();


/*
** take out the ambush markers
*/
    map = mapsegs[0];
    for (y=0;y<mapheight;y++)
    {
        for (x=0;x<mapwidth;x++)
        {
            tile = *map++;
            if (tile == AMBUSHTILE)
            {
                if (*map >= AREATILE)
                    tile = *map;
                if (*(map-1-mapwidth) >= AREATILE)
                    tile = *(map-1-mapwidth);
                if (*(map-1+mapwidth) >= AREATILE)
                    tile = *(map-1+mapwidth);
                if ( *(map-2) >= AREATILE)
                    tile = *(map-2);

                *(map-1) = tile;
            }
        }
    }
#ifdef SEGA_SATURN

    int total = (int)(laststatobj - &statobjlist[0]);

    for (int i = 0; i <= total; i++)
    {
        if (statobjlist[i].shapenum != -1)
        {
            itemmap[statobjlist[i].shapenum + PMSpriteStart] = 1;
    }
    }
    /* ----------------------------------------------------------------------------------- */

    /* ----------------------------------------------------------------------------------- */
    slScrTransparent(0);
    slSynch();
    extern const void* TransList;
    memset((void*)TransList, 0x00, 0xf0 * 4 * 3);
#else
/*
** load floor/ceiling textures
*/
#if defined(USE_FLOORCEILINGTEX) && !defined(USE_MULTIFLATS)
    GetFlatTextures ();
#endif

/*
** have the caching manager load and purge stuff to make sure all marks
** are in memory
*/
    CA_LoadAllSounds ();
#endif
}


/* ========================================================================== */


/*
===================
=
= DrawPlayBorderSides
=
= To fix window overwrites
=
===================
*/
void DrawPlayBorderSides(void)
{
	const int sw = screenWidth;
	const int sh = screenHeight;
	const int vw = viewwidth;
	const int vh = viewheight;
	const int px = scaleFactor; /* size of one "pixel" */

	const int h  = sh - px * STATUSLINES;
	const int xl = sw / 2 - vw / 2;
	const int yl = (h - vh) / 2;

    if(viewsize == 21) return;

    if(xl != 0)
    {
        VL_BarScaledCoord (0,            0, xl - px,     h, bordercol);                 /* left side */
        VL_BarScaledCoord (xl + vw + px, 0, xl - px * 2, h, bordercol);                 /* right side */
    }

    if(yl != 0)
    {
        VL_BarScaledCoord (0, 0,            sw, yl - px, bordercol);                    /* upper side */
        VL_BarScaledCoord (0, yl + vh + px, sw, yl - px, bordercol);                    /* lower side */
    }

    if(xl != 0)
    {
        /* Paint game view border lines */
        VL_BarScaledCoord (xl - px, yl - px, vw + px, px,          0);                      /* upper border */
        VL_BarScaledCoord (xl,      yl + vh, vw + px, px,          bordercol - 2);          /* lower border */
        VL_BarScaledCoord (xl - px, yl - px, px,      vh + px,     0);                      /* left border */
        VL_BarScaledCoord (xl + vw, yl - px, px,      vh + px * 2, bordercol - 2);          /* right border */
        VL_BarScaledCoord (xl - px, yl + vh, px,      px,          bordercol - 3);          /* lower left highlight */
    }
    else
    {
        /* Just paint a lower border line */
        VL_BarScaledCoord (0, yl+vh, vw, px, bordercol-2);       /* lower border */
    }
}


/*
===================
=
= DrawStatusBorder
=
===================
*/

void DrawStatusBorder (unsigned char color)
{
#ifndef SEGA_SATURN
    int statusborderw = (screenWidth-scaleFactor*320)/2;

    VL_BarScaledCoord (0,0,screenWidth,screenHeight-scaleFactor*(STATUSLINES-3),color);
    VL_BarScaledCoord (0,screenHeight-scaleFactor*(STATUSLINES-3),
        statusborderw+scaleFactor*8,scaleFactor*(STATUSLINES-4),color);
    VL_BarScaledCoord (0,screenHeight-scaleFactor*2,screenWidth,scaleFactor*2,color);
    VL_BarScaledCoord (screenWidth-statusborderw-scaleFactor*8, screenHeight-scaleFactor*(STATUSLINES-3),
        statusborderw+scaleFactor*8,scaleFactor*(STATUSLINES-4),color);

    VL_BarScaledCoord (statusborderw+scaleFactor*9, screenHeight-scaleFactor*3,
        scaleFactor*97, scaleFactor*1, color-1);
    VL_BarScaledCoord (statusborderw+scaleFactor*106, screenHeight-scaleFactor*3,
        scaleFactor*161, scaleFactor*1, color-2);
    VL_BarScaledCoord (statusborderw+scaleFactor*267, screenHeight-scaleFactor*3,
        scaleFactor*44, scaleFactor*1, color-3);
    VL_BarScaledCoord (screenWidth-statusborderw-scaleFactor*9, screenHeight-scaleFactor*(STATUSLINES-4),
        scaleFactor*1, scaleFactor*20, color-2);
    VL_BarScaledCoord (screenWidth-statusborderw-scaleFactor*9, screenHeight-scaleFactor*(STATUSLINES/2-4),
        scaleFactor*1, scaleFactor*14, color-3);
#endif
}

/*
===================
=
= DrawPlayBorder
=
===================
*/

void DrawPlayBorder (void)
{
	const int px = scaleFactor; /* size of one "pixel" */
	const int xl = screenWidth/2-viewwidth/2;
    const int yl = (screenHeight-px*STATUSLINES-viewheight)/2;
    
	if (bordercol != VIEWCOLOR)
        DrawStatusBorder(bordercol);
    else
    {
#ifdef SEGA_SATURN
        const int statusborderw = (screenWidth - px * SATURN_WIDTH) / 2;

#if SATURN_WIDTH == 352
        VL_BarScaledCoord(0, screenHeight - px * STATUSLINES,
            8 + statusborderw + px * 8, px * STATUSLINES, bordercol);
        VL_BarScaledCoord(screenWidth - 8 - statusborderw - px * 8, screenHeight - px * STATUSLINES,
            8 + statusborderw + px * 8, px * STATUSLINES, bordercol
#else
        VL_BarScaledCoord(0, screenHeight - px * STATUSLINES,
            statusborderw + px * 8, px * STATUSLINES, bordercol);
        VL_BarScaledCoord(screenWidth - statusborderw - px * 8, screenHeight - px * STATUSLINES,
            statusborderw + px * 8, px * STATUSLINES, bordercol);
#endif
#else
        const int statusborderw = (screenWidth-px*320)/2;

        VL_BarScaledCoord (0, screenHeight-px*STATUSLINES,
            statusborderw+px*8, px*STATUSLINES, bordercol);
        VL_BarScaledCoord (screenWidth-statusborderw-px*8, screenHeight-px*STATUSLINES,
            statusborderw+px*8, px*STATUSLINES, bordercol);
#endif
    }

    if((unsigned) viewheight == screenHeight) return;

    VL_BarScaledCoord (0,0,screenWidth,screenHeight-px*STATUSLINES,bordercol);


    VL_BarScaledCoord (xl,yl,viewwidth,viewheight,0);

    if(xl != 0)
    {
#ifdef SEGA_SATURN
        SPRITE* sys_clip = (SPRITE*)SpriteVRAM;
        (*sys_clip).XC = (xl + viewwidth) - 1;
        (*sys_clip).YC = (yl + viewheight) - 1;

        slWindow(xl, yl, (xl + viewwidth) - 1, (yl + viewheight) - 1, 300, screenWidth / 2, (yl * 2 + viewheight) / 2);
#endif

        /* Paint game view border lines */
        VL_BarScaledCoord(xl-px, yl-px, viewwidth+px, px, 0);                      /* upper border */
        VL_BarScaledCoord(xl, yl+viewheight, viewwidth+px, px, bordercol-2);       /* lower border */
        VL_BarScaledCoord(xl-px, yl-px, px, viewheight+px, 0);                     /* left border */
        VL_BarScaledCoord(xl+viewwidth, yl-px, px, viewheight+2*px, bordercol-2);  /* right border */
        VL_BarScaledCoord(xl-px, yl+viewheight, px, px, bordercol-3);              /* lower left highlight */
    }
    else
    {
#ifdef SEGA_SATURN
        SPRITE* sys_clip = (SPRITE*)SpriteVRAM;
        (*sys_clip).XC = SATURN_WIDTH - 1;
        (*sys_clip).YC = 239;

        slWindow(0, 0, SATURN_WIDTH - 1, 239, 300, screenWidth / 2, screenHeight / 2);
#endif
        /* Just paint a lower border line */
        VL_BarScaledCoord(0, yl+viewheight, viewwidth, px, bordercol-2);       /* lower border */
    }
}


/*
===================
=
= DrawPlayScreen
=
===================
*/

void DrawPlayScreen (void)
{
#ifdef SEGA_SATURN
    VWB_DrawPicScaledCoord(SATURN_ADJUST + (screenWidth - scaleFactor * SATURN_WIDTH) / 2, screenHeight - scaleFactor * STATUSLINES, STATUSBARPIC);
#else
    VWB_DrawPicScaledCoord ((screenWidth-scaleFactor*320)/2,screenHeight-scaleFactor*STATUSLINES,STATUSBARPIC);
#endif
    DrawPlayBorder ();
    DrawFace ();
    DrawHealth ();
    DrawLives ();
    DrawLevel ();
    DrawAmmo ();
    DrawKeys ();
    DrawWeapon ();
    DrawScore ();
}

#ifndef SEGA_SATURN
void ShowActStatus()
{
    /* Draw status bar without borders */
    unsigned char *source = grsegs[STATUSBARPIC];
    int	picnum = STATUSBARPIC - STARTPICS;
    int width = pictable[picnum].width;
    int height = pictable[picnum].height;
    int destx = (screenWidth-scaleFactor*320)/2 + 9 * scaleFactor;
    int desty = screenHeight - (height - 4) * scaleFactor;

    VL_MemToScreenScaledCoord2(source, width, height, 9, 4, destx, desty, width - 18, height - 7);

    ingame = false;
    DrawFace ();
    DrawHealth ();
    DrawLives ();
    DrawLevel ();
    DrawAmmo ();
    DrawKeys ();
    DrawWeapon ();
    DrawScore ();
    ingame = true;
}


/* ========================================================================== */

/*
==================
=
= StartDemoRecord
=
==================
*/

char    demoname[13] = "DEMO?.";

#ifndef REMDEBUG
#define MAXDEMOSIZE     8192

void StartDemoRecord (int levelnumber)
{
    demobuffer = SafeMalloc(MAXDEMOSIZE);
    demoptr = (char *) demobuffer;
    lastdemoptr = demoptr+MAXDEMOSIZE;

    *demoptr = levelnumber;
    demoptr += 4;                           /* leave space for length */
    demorecord = true;
}


/*
==================
=
= FinishDemoRecord
=
==================
*/

void FinishDemoRecord (void)
{
    int        length;

    demorecord = false;

    length = (int) (demoptr - (char *)demobuffer);

    demoptr = ((char *)demobuffer)+1;
    demoptr[0] = (char) length;
    demoptr[1] = (char) (length >> 8);
    demoptr[2] = 0;

    VL_FadeIn (0, 255, gamepal, 30);
    CenterWindow(24,3);
    PrintY+=6;
    fontnumber=0;
    SETFONTCOLOR(0,15);
    US_Print(" Demo number (0-9): ");
    VL_UpdateScreen(screenBuffer);

    if (US_LineInput (px,py,str,NULL,true,1,0))
    {
        int level = atoi (str);
        if (level>=0 && level<=9)
        {
            demoname[4] = (char)('0'+level);
            CA_WriteFile (demoname,demobuffer,length);
        }
    }

    free(demobuffer);
}

/* ========================================================================== */

/*
==================
=
= RecordDemo
=
= Fades the screen out, then starts a demo.  Exits with the screen faded
=
==================
*/

void RecordDemo (void)
{
    int level,esc,maps;

    CenterWindow(26,3);
    PrintY+=6;
    fontnumber=0;
    SETFONTCOLOR(0,15);
#ifndef SPEAR
#ifdef UPLOAD
    US_Print("  Demo which level(1-10): "); maps = 10;
#else
    US_Print("  Demo which level(1-60): "); maps = 60;
#endif
#else
    US_Print("  Demo which level(1-21): "); maps = 21;
#endif
    VL_UpdateScreen(screenBuffer);
    VL_FadeIn (0, 255, gamepal, 30);
    esc = !US_LineInput (px,py,str,NULL,true,2,0);
    if (esc)
        return;

    level = atoi (str);
    level--;

    if (level >= maps || level < 0)
        return;

    VL_FadeOut (0, 255, 0, 0, 0, 30);

#ifndef SPEAR
    NewGame (gd_hard,level/10);
    gamestate.mapon = level%10;
#else
    NewGame (gd_hard,0);
    gamestate.mapon = level;
#endif

    StartDemoRecord (level);

    DrawPlayScreen ();
    VL_FadeIn (0, 255, gamepal, 30);

    startgame = false;

    SetupGameLevel ();
    StartMusic ();

    if(usedoublebuffering) VL_UpdateScreen(screenBuffer);
    fizzlein = true;

    PlayLoop ();


    StopMusic ();
    VL_FadeOut (0, 255, 0, 0, 0, 30);
    SD_StopDigitized ();

    FinishDemoRecord ();
}
#else
void FinishDemoRecord (void) {return;}
void RecordDemo (void) {return;}
#endif
#endif


/* ========================================================================== */

/*
==================
=
= PlayDemo
=
= Fades the screen out, then starts a demo.  Exits with the screen unfaded
=
==================
*/

void PlayDemo (int demonumber)
{
    int length;
#ifdef DEMOSEXTERN
/* debug: load chunk */
#ifndef SPEARDEMO
   /* Wolf3s: Count better the num of demos using NUMDEMOS By wayneca. */
    int dems[NUMDEMOS]={T_DEMO0,T_DEMO1,T_DEMO2,T_DEMO3};
#else
    int dems[NUMDEMOS]={T_DEMO0};
#endif

    demoptr = (char *) grsegs[dems[demonumber]];
#else
    demoname[4] = '0'+demonumber;
    CA_LoadFile (demoname,&demobuffer);
    demoptr = (char *)demobuffer;
#endif

    NewGame (1,0);
    gamestate.mapon = *demoptr++;
    gamestate.difficulty = gd_hard;
    length = READWORD((unsigned char *)demoptr);
    /* 
    ** TODO: Seems like the original demo format supports 16 MB demos 
    **       But T_DEM00 and T_DEM01 of Wolf have a 0xd8 as third length size...
    */
    demoptr += 3;
    lastdemoptr = demoptr-4+length;

    VL_FadeOut (0, 255, 0, 0, 0, 30);

    SETFONTCOLOR(0,15);
    DrawPlayScreen ();

    startgame = false;
    demoplayback = true;

    SetupGameLevel ();
    StartMusic ();

    PlayLoop ();

#ifndef DEMOSEXTERN
    free (demobuffer);
#endif

    demoplayback = false;

    StopMusic ();
    SD_StopDigitized ();
}

/* ========================================================================== */

/*
==================
=
= Died
=
==================
*/

#define DEATHROTATE 2

void Died (void)
{
    float   fangle;
 
    int     iangle,curangle,clockwise,counter,change;

    if (screenfaded)
    {
        ThreeDRefresh ();
        VL_FadeIn (0, 255, gamepal, 30);
    }

    gamestate.weapon = (weapontype) -1;                     /* take away weapon */
    SD_PlaySound (PLAYERDEATHSND);

    /*
    ** swing around to face attacker
    */
    if(killerobj)
    {
        fixed     dx, dy;
        dx = killerobj->x - player->x;
        dy = player->y - killerobj->y;
#ifdef SEGA_SATURN
        fangle = atan2fix(dy, dx);
#else
        fangle = (float) atan2((float) dy, (float) dx);     /* returns -pi to pi */
#endif
        if (fangle<0)
            fangle = (float) (M_PI*2+fangle);

#ifdef SEGA_SATURN
        iangle = atan2fix(dy, dx);
#else
        iangle = (int) (fangle/(M_PI*2)*ANGLES);
#endif
    }
    else
    {
        iangle = player->angle + ANGLES / 2;
        if(iangle >= ANGLES) iangle -= ANGLES;
    }

    if (player->angle > iangle)
    {
        counter = player->angle - iangle;
        clockwise = ANGLES-player->angle + iangle;
    }
    else
    {
        clockwise = iangle - player->angle;
        counter = player->angle + ANGLES-iangle;
    }

    curangle = player->angle;

    if (clockwise<counter)
    {
        /*
        ** rotate clockwise
        */
        if (curangle>iangle)
            curangle -= ANGLES;
        do
        {
            change = (int)tics*DEATHROTATE;
            if (curangle + change > iangle)
                change = iangle-curangle;

            curangle += change;
            player->angle += change;
            if (player->angle >= ANGLES)
                player->angle -= ANGLES;

            ThreeDRefresh ();
            CalcTics ();
        } while (curangle != iangle);
    }
    else
    {
        /*
        ** rotate counterclockwise
        */
        if (curangle<iangle)
            curangle += ANGLES;
        do
        {
            change = -(int)tics*DEATHROTATE;
            if (curangle + change < iangle)
                change = iangle-curangle;

            curangle += change;
            player->angle += change;
            if (player->angle < 0)
                player->angle += ANGLES;

            ThreeDRefresh ();
            CalcTics ();
        } while (curangle != iangle);
    }

    /*
    ** fade to red
    */
    FinishPaletteShifts ();

    if(usedoublebuffering) VL_UpdateScreen(screenBuffer);

    VL_BarScaledCoord (viewscreenx,viewscreeny,viewwidth,viewheight,4);

    IN_ClearKeysDown ();

    FizzleFade(screenBuffer,viewscreenx,viewscreeny,viewwidth,viewheight,70,false);

    IN_UserInput(100);
    SD_WaitSoundDone ();
    SD_StopDigitized();

    gamestate.lives--;

    if (gamestate.lives > -1)
    {
        gamestate.health = 100;
        gamestate.weapon = gamestate.bestweapon
            = gamestate.chosenweapon = wp_pistol;
        gamestate.ammo = STARTAMMO;
        gamestate.keys = 0;
        pwallstate = pwallpos = 0;
        gamestate.attackframe = gamestate.attackcount =
            gamestate.weaponframe = 0;
#ifdef AUTOMAP
        memset(automap, 0, sizeof(automap));
#endif

        if(viewsize != 21)
        {
            DrawKeys ();
            DrawWeapon ();
            DrawAmmo ();
            DrawHealth ();
            DrawFace ();
            DrawLives ();
        }
    }
}

/* ========================================================================== */

/*
===================
=
= GameLoop
=
===================
*/

#ifdef SEGA_SATURN
void heapWalk();
#endif

void GameLoop (void)
{
    boolean died;
#ifdef MYPROFILE
    clock_t start,end;
#endif

/* 
** gamestate.mapon = 8;
** gamestate.mapon = 3;
** gamestate.episode=3;
** GiveWeapon (gamestate.bestweapon+2);
** gamestate.ammo = 99;
** gamestate.keys = 3;
** vbt dernier niveau
*/

restartgame:
    SD_StopDigitized ();
    SETFONTCOLOR(0,15);
    VL_FadeOut (0, 255, 0, 0, 0, 30);
#ifdef AUTOINTER
    ClearMScreen();
    IntermissionScreens(); /* Intermission Text - Shown when starting new game */
    SD_StopDigitized();
#endif
    DrawPlayScreen ();
    died = false;
    do
    {
#ifndef SEGA_SATURN
        if (!loadedgame)
#endif
            gamestate.score = gamestate.oldscore;
        if(!died || viewsize != 21) DrawScore();

        startgame = false;
#ifndef SEGA_SATURN        
        if (!loadedgame)
#endif            
            SetupGameLevel ();
#ifdef MAPCONTROLLEDPLSETT
        if (!loadedgame) {
            if (tilemap[63][0]) {
                ResetPlayer();
            }
        }
#endif

#ifdef SPEAR
        if (gamestate.mapon == 20)      /* give them the key allways */
        {
            gamestate.keys |= 1;
            DrawKeys ();
        }
#endif
#ifdef MAPCONTROLPARTIME
        gamestate.partime = 2.0;
        if (tilemap[61][0] >= 1 && tilemap[61][0] <= 20)
        {
            gamestate.partime = tilemap[62][0];
            if (tilemap[62][0] >= 1 && tilemap[62][0] <= 3)
            {
                switch (tilemap[62][0])
                {
                case 1: gamestate.partime += .25; break;
                case 2: gamestate.partime += .5; break;
                case 3: gamestate.partime += .75; break;
                }
            }
        }
        gamestate.partime = (gamestate.partime * 4200) / 70;
#endif

        DrawLevel ();                        /* ADDEDFIX 5 -  Chris Chokan */

        ingame = true;
#ifndef SEGA_SATURN
        if(loadedgame)
        {
            ContinueMusic(lastgamemusicoffset);
            loadedgame = false;
        }
        else StartMusic ();
#endif
        if (!died)
            PreloadGraphics ();             /* TODO: Let this do something useful! */
        else
        {
            died = false;
            fizzlein = true;
        }

/*        DrawLevel ();          */           /* ADDEDFIX 5 - moved up  Chris Chokan */

#ifdef SPEAR
startplayloop:
#endif
        PlayLoop ();

#ifdef SPEAR
        if (spearflag)
        {
            SD_StopSound();
            SD_PlaySound(GETSPEARSND);
            if (DigiMode != sds_Off)
            {
                Delay(150);
            }
            else
                SD_WaitSoundDone();

            SD_StopDigitized ();
            gamestate.oldscore = gamestate.score;
            gamestate.mapon = 20;
            SetupGameLevel ();
            StartMusic ();
            player->x = spearx;
            player->y = speary;
            player->angle = spearangle;
            spearflag = false;
            Thrust (0,0);
#ifdef AUTOMAP
            memset(automap, 0, sizeof(automap));
#endif
            goto startplayloop;
        }
#endif

        StopMusic ();
        ingame = false;

        if (demorecord && playstate != ex_warped)
            FinishDemoRecord ();

        if (startgame || loadedgame)
            goto restartgame;

        switch (playstate)
        {
            case ex_completed:
            case ex_secretlevel:
                if(viewsize == 21) DrawPlayScreen();
                gamestate.keys = 0;
#ifdef AUTOMAP
                memset(automap, 0, sizeof(automap));
#endif
                DrawKeys ();
                VL_FadeOut (0, 255, 0, 0, 0, 30);

                SD_StopDigitized ();

                LevelCompleted ();              /* do the intermission */
#ifdef SEGA_SATURN
                gamestate.keys = 3;
#endif
                if(viewsize == 21) DrawPlayScreen();

#ifdef SPEARDEMO
                if (gamestate.mapon == 1)
                {
                    died = true;                    /* don't "get psyched!" */

                    VL_FadeOut (0, 255, 0, 0, 0, 30);

                    SD_StopDigitized ();

                    CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                    strcpy(MainMenu[viewscores].string,STR_VS);
#endif
                    MainMenu[viewscores].routine = (int(*)(int))CP_ViewScores;
                    return;
                }
#endif

#ifdef JAPDEMO
                if (gamestate.mapon == 3)
                {
                    died = true;                    /* don't "get psyched!" */

                    VL_FadeOut ( 0, 255, 0, 0, 0, 30);

                    SD_StopDigitized  ();

                    CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                    strcpy(MainMenu[viewscores].string,STR_VS);
#endif
                    MainMenu[viewscores].routine = CP_ViewScores;
                    return;
                }
#endif

                gamestate.oldscore = gamestate.score;

#ifndef SPEAR
                /*
                ** COMING BACK FROM SECRET LEVEL
                */
                if (gamestate.mapon == 9)
                    gamestate.mapon = ElevatorBackTo[gamestate.episode];    /* back from secret */
                else
                    /*
                    ** GOING TO SECRET LEVEL
                    */
                    if (playstate == ex_secretlevel)
                        gamestate.mapon = 9;
#else

#define FROMSECRET1             3
#define FROMSECRET2             11

                /*
                ** GOING TO SECRET LEVEL
                */
                if (playstate == ex_secretlevel)
                    switch(gamestate.mapon)
                {
                    case FROMSECRET1: gamestate.mapon = 18; break;
                    case FROMSECRET2: gamestate.mapon = 19; break;
                }
                else
                    /*
                    ** COMING BACK FROM SECRET LEVEL
                    */
                    if (gamestate.mapon == 18 || gamestate.mapon == 19)
                        switch(gamestate.mapon)
                    {
                        case 18: gamestate.mapon = FROMSECRET1+1; break;
                        case 19: gamestate.mapon = FROMSECRET2+1; break;
                    }
#endif
                    else
                        /*
                        ** GOING TO NEXT LEVEL
                        */
                        gamestate.mapon++;
#ifdef AUTOINTER
                IntermissionScreens(); /* Intermission Screen If file exists */
                SD_StopDigitized();
                DrawPlayScreen();
#endif
                break;

            case ex_died:
                Died ();
                died = true;                    /* don't "get psyched!" */

                if (gamestate.lives > -1)
#ifdef AUTOINTER
                {
                    VL_FadeOut(0, 255, 0, 0, 0, 30);
                    ClearMScreen();
                    SD_StopDigitized();
                    IntermissionScreens(); /* Intermission Text */
                    SD_StopDigitized();
                    DrawPlayScreen();
#endif
                    break;                          /* more lives left */
#ifdef AUTOINTER
                }
#endif

                VL_FadeOut (0, 255, 0, 0, 0, 30);
                if(screenHeight % 200 != 0)
                    VL_ClearScreen(0);

                SD_StopDigitized ();

                CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                strcpy(MainMenu[viewscores].string,STR_VS);
#endif
                MainMenu[viewscores].routine = (int(*)(int))CP_ViewScores;
                return;

            case ex_victorious:
                if(viewsize == 21) DrawPlayScreen();
#ifndef SPEAR
                VL_FadeOut (0, 255, 0, 0, 0, 30);
#else
                VL_FadeOut (0,255,0,17,17,300);
#endif
                SD_StopDigitized ();

                Victory ();

                SD_StopDigitized ();

                CheckHighScore (gamestate.score,gamestate.mapon+1);
#ifndef JAPAN
                strcpy(MainMenu[viewscores].string,STR_VS);
#endif
                MainMenu[viewscores].routine = (int(*)(int))CP_ViewScores;
                return;

            default:
                if(viewsize == 21) DrawPlayScreen();
#ifndef SEGA_SATURN
                SD_StopDigitized ();
#endif
                break;
        }
    } while (1);
}
