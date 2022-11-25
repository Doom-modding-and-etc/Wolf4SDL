// WL_PIMP.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_PIMP)
#include "wl_shade.h"

#define PIMP_COST 2

#define PIMP_INSTATE(ob, x) ((ob)->state == &s_pimp[x])
#define PIMP_SETSTATE(ob, x) NewState(ob, &s_pimp[x])

static statetype s_pimp[] =
{
    {false,SPR_GPIMP,3,(statefunc)Pimp_Think,NULL,&s_pimp[0]},
    {false,SPR_GPIMP,3,NULL,NULL,&s_pimp[1]},
};

statetype *Pimp_States(void)
{
    return s_pimp;
}

void Pimp_Spawn(int x, int y)
{
    SpawnStand(en_anyactor, x, y, 0);
    newobj->flags &= ~FL_SHOOTABLE;
    newobj->flags |= FL_BLOCKING;
}

static bool Pimp_OwnsWhore(objtype *ob, objtype *whore)
{
    return 
        (
            ANYACTOR_ISA(ob, ANYACTOR_TILE_GPIMP) &&
            ANYACTOR_ISA(whore, ANYACTOR_TILE_GHOOKER)
        ) ||
        (
            ANYACTOR_ISA(ob, ANYACTOR_TILE_RPIMP) &&
            ANYACTOR_ISA(whore, ANYACTOR_TILE_RHOOKER)
        );
}

static void Pimp_AuthorizeSluts(objtype *ob)
{
    objtype *check;
    hookerobjdata *hookerObjData;

    for (check = player->next; check; check = check->next)
    {
        hookerObjData = &check->u.hooker;
        if (Pimp_OwnsWhore(ob, check))
        {
            hookerObjData->pimpAuthorized = true;
        }
    }
}

void Pimp_Think(objtype *ob)
{
    int dist;

    if (PIMP_INSTATE(ob, 0))
    {
        dist = Object_TileDist(player, ob);

        if (Object_CheckUsed(ob))
        {
            if (gamestate.coins >= PIMP_COST)
            {
                UseCoins(PIMP_COST);
                Pimp_AuthorizeSluts(ob);
                SD_PlaySound (SLUTSAUTHORIZEDSND);
                Object_LogDiscScreens(ob, NULL, false);
                PIMP_SETSTATE(ob, 1);
            }
            else
            {
                SD_PlaySound (NOTENOUGHCOINSSND);
                LogDiscScreensNoLev("pimp_nocoins");
            }
        }
    }
}
#endif