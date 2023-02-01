// WL_CONARTIST.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_CONARTIST)
#include "wl_shade.h"
#include "3rdparty/fixedptc.h"

#define CONARTIST_COST 20

#define CONARTIST_INSTATE(ob, x) ((ob)->state == &s_conartist[x])
#define CONARTIST_SETSTATE(ob, x) NewState(ob, &s_conartist[x])

static statetype s_conartist[] =
{
    {false,SPR_CONARTIST_WAIT,3,(statefunc)ConArtist_Think,NULL,&s_conartist[0]},
    {false,SPR_CONARTIST_WELCOME,3,(statefunc)ConArtist_Think,NULL,&s_conartist[1]},
    {false,SPR_CONARTIST_WAIT,3,NULL,NULL,&s_conartist[2]},
};

statetype *ConArtist_States(void)
{
    return s_conartist;
}

void ConArtist_Spawn(int x, int y)
{
    SpawnStand(en_anyactor, x, y, 0);
    newobj->flags &= ~FL_SHOOTABLE;
    newobj->flags |= FL_BLOCKING;
}

static boolean ConArtist_AvailableToUse(objtype *ob)
{
    int dist;
    dist = Object_TileDist(player, ob);
    return (dist <= 3 && gamestate.coins >= CONARTIST_COST);
}

void ConArtist_Think(objtype *ob)
{
    int dist;

    if (CONARTIST_INSTATE(ob, 0))
    {
        if (ConArtist_AvailableToUse(ob))
        {
            CONARTIST_SETSTATE(ob, 1);
        }
        else
        {
            if (Object_CheckUsed(ob))
            {
                if (gamestate.coins < CONARTIST_COST)
                {
                    SD_PlaySound (NOTENOUGHCOINSSND);
                    LogDiscScreensNoLev("conartist_nocoins");
                }
            }
        }
    }
    else if (CONARTIST_INSTATE(ob, 1))
    {
        if (ConArtist_AvailableToUse(ob))
        {
            if (Object_CheckUsed(ob))
            {
                UseCoins(CONARTIST_COST);
                LogDiscScreensNoLev("conartist");
                GiveKey(key_bonus);
                CONARTIST_SETSTATE(ob, 2);
            }
        }
        else
        {
            CONARTIST_SETSTATE(ob, 0);
        }
    }
}
#endif