// WL_SLEEPHOBO.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_SLEEPHOBO)
#include "wl_shade.h"

extern  statetype s_dogchase1;

static statetype s_sleephobo[] =
{
    {false,SPR_BEGGER_SLEEP,3,(statefunc)SleepHobo_Think,NULL,&s_sleephobo[0]},
    {false,SPR_BEGGER_WAKE,10,NULL,(statefunc)SleepHobo_Awake,NULL},
};

statetype *SleepHobo_States(void)
{
    return s_sleephobo;
}

void SleepHobo_Spawn(int x, int y)
{
    SpawnStand(en_anyactor, x, y, 0);
    newobj->flags &= ~FL_SHOOTABLE;
    newobj->flags |= FL_BLOCKING;
}

void SleepHobo_Think(objtype *ob)
{
    if (Hobo_CheckWake(ob))
    {
        Object_NewState(ob, 1);
    }
}

void SleepHobo_Awake(objtype *ob)
{
    PlaySoundLocActor(HOBOAWAKESND, ob);

    SpawnNewObj (ob->tilex, ob->tiley, &s_dogchase1);
    newobj->obclass = dogobj;
    newobj->starthitpoints = newobj->hitpoints =
        starthitpoints[gamestate.difficulty][en_dog];
    newobj->dir = nodir;
    newobj->speed = SPDDOG * 3;

    newobj->flags |= FL_SHOOTABLE;
    newobj->flags |= FL_ATTACKMODE|FL_FIRSTATTACK;

    T_Aim(newobj);
}
#endif