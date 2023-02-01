// WL_HOOKER.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_HOOKER)
#include "wl_shade.h"

#define HOOKER_COST 3

#define HOOKER_INSTATE(ob, x) ((ob)->state == &s_hooker[x])
#define HOOKER_SETSTATE(ob, x) NewState(ob, &s_hooker[x])

static statetype s_hooker[] =
{
    {false,SPR_GHOOKER_WAIT,3,(statefunc)Hooker_Think,NULL,&s_hooker[0]},
    {false,SPR_GHOOKER_WELCOME,3,(statefunc)Hooker_Think,NULL,&s_hooker[1]},
    {false,SPR_GHOOKER_FLASH1,8,NULL,NULL,&s_hooker[3]},
    {false,SPR_GHOOKER_FLASH2,8,NULL,NULL,&s_hooker[4]},
    {false,SPR_GHOOKER_FLASH3,8,NULL,NULL,&s_hooker[5]},
    {false,SPR_GHOOKER_FLASH4,8,NULL,NULL,&s_hooker[6]},
    {false,SPR_GHOOKER_FLASH2,8,NULL,NULL,&s_hooker[7]},
    {false,SPR_GHOOKER_FLASH3,8,NULL,NULL,&s_hooker[8]},
    {false,SPR_GHOOKER_FLASH4,8,NULL,NULL,&s_hooker[9]},
    {false,SPR_GHOOKER_FLASH5,8,NULL,NULL,&s_hooker[1]},
    {false,SPR_GHOOKER_WAIT,3,NULL,NULL,&s_hooker[10]},
};

statetype *Hooker_States(void)
{
    return s_hooker;
}

void Hooker_Spawn(int x, int y)
{
    SpawnStand(en_anyactor, x, y, 0);
    newobj->flags &= ~FL_SHOOTABLE;
    newobj->flags |= FL_BLOCKING;
}

static boolean Hooker_AvailableToUse(objtype *ob)
{
    int dist;
    hookerobjdata *hookerObjData = &ob->u.hooker;

    dist = Object_TileDist(player, ob);
    return (dist <= 3 && gamestate.coins >= HOOKER_COST &&
        hookerObjData->pimpAuthorized);
}

void Hooker_Think(objtype *ob)
{
    int dist;
    hookerobjdata *hookerObjData = &ob->u.hooker;

    if (HOOKER_INSTATE(ob, 0))
    {
        if (Hooker_AvailableToUse(ob))
        {
            HOOKER_SETSTATE(ob, 1);
        }
        else
        {
            if (Object_CheckUsed(ob))
            {
                if (!hookerObjData->pimpAuthorized)
                {
                    SD_PlaySound (SEEMYPIMPSND);
                    LogDiscScreensNoLev("hooker_seemypimp");
                }
                else if (gamestate.coins < HOOKER_COST)
                {
                    SD_PlaySound (NOTENOUGHCOINSSND);
                    LogDiscScreensNoLev("hooker_nocoins");
                }
            }
        }
    }
    else if (HOOKER_INSTATE(ob, 1))
    {
        if (Hooker_AvailableToUse(ob))
        {
            dist = Object_TileDist(player, ob);

            if (dist <= 2 && Object_InShootDelta(ob) && 
                !hookerObjData->flashedTits)
            {
                hookerObjData->flashedTits = true;
                HOOKER_SETSTATE(ob, 2);
            }
            else if (Object_CheckUsed(ob))
            {
                UseCoins(HOOKER_COST);
                SD_PlaySound (USEHOOKERSND);
                Object_LogDiscScreens(ob, NULL, true);
                HOOKER_SETSTATE(ob, 10);
            }
        }
        else
        {
            HOOKER_SETSTATE(ob, 0);
        }
    }
}
#endif