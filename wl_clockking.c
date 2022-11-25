// WL_CLOCKKING.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_CLOCKKING)
#include "wl_shade.h"
#include "3rdparty/fixedptc.h"


#define CLOCKKING_DEFENSE_PERIOD 250

static const int clockking_timetokill[gd_max] =
{
    150, // gd_baby
    120, // gd_easy
    90, // gd_medium
    60, // gd_hard
};

static const int clockking_defensechance[gd_max] =
{
    20, // gd_baby
    30, // gd_easy
    40, // gd_medium
    50, // gd_hard
};

statetype s_clock              = {true,SPR_CLOCKPROJ_1,3,(statefunc)ClockKing_ProjectileThink,NULL,&s_clock};

statetype s_break1             = {false,SPR_CLOCKPROJ_BREAK1,6,NULL,NULL,&s_break2};
statetype s_break2             = {false,SPR_CLOCKPROJ_BREAK2,6,NULL,NULL,&s_break3};
statetype s_break3             = {false,SPR_CLOCKPROJ_BREAK3,6,NULL,NULL,&s_break4};
statetype s_break4             = {false,SPR_CLOCKPROJ_BREAK4,6,NULL,NULL,NULL};

statetype s_clockkingdefense   = {false,SPR_CLOCKKING_DEFENSE,CLOCKKING_DEFENSE_PERIOD,NULL,(statefunc)ClockKing_EndDefense,&s_willchase1s};

void ClockKing_Spawn(int x, int y)
{
    SpawnWill(x, y);
    newobj->obclass = anyactorobj;
}

void ClockKing_Launch(objtype *ob, objtype *proj)
{
    PlaySoundLocActor (CLOCKKINGSHOOTSND, proj);
    proj->state = &s_clock;
}

void ClockKing_ProjectileThink (objtype *proj)
{
    int32_t deltax,deltay,movex,movey;
    int     damage;
    int32_t speed;

    speed = (int32_t)proj->speed*tics;

    movex = FixedMul(speed,costable[proj->angle]);
    movey = -FixedMul(speed,sintable[proj->angle]);

    if (movex>0x10000l)
        movex = 0x10000l;
    if (movey>0x10000l)
        movey = 0x10000l;

    if (!ProjectileTryMove (proj, movex, movey))
    {
        PlaySoundLocActor(CLOCKPROJHITSND,proj);
        proj->state = &s_break1;
        return;
    }

    proj->x += movex;
    proj->y += movey;

    deltax = LABS(proj->x - player->x);
    deltay = LABS(proj->y - player->y);

    if (deltax < PROJECTILESIZE && deltay < PROJECTILESIZE)
    {
        damage = (US_RndT() >> 3);
        TakeDamage (damage, Object_ProjShooter(proj));
        TimeWarp_SetEnabled(true);
        proj->state = NULL;
        SD_PlaySound(TIMEWARPFASTSND);
        return;
    }

    proj->tilex = (short)(proj->x >> TILESHIFT);
    proj->tiley = (short)(proj->y >> TILESHIFT);
}

void ClockKing_DoChaseState(objtype *ob)
{
    SD_PlaySound(WILHELMSIGHTSND);
    NewState (ob,&s_willchase1);
    ob->speed = 2048;
    gamestate.clockking_timer = 
        SECS2TICS(clockking_timetokill[gamestate.difficulty]);
}

void ClockKing_UpdateTimer(void)
{
    if (gamestate.clockking_timer > 0)
    {
        gamestate.clockking_timer -= tics;
        if (gamestate.clockking_timer <= 0)
        {
            gamestate.clockking_timer = 0;
            Suicide();
        }
    }
}

void ClockKing_Kill(objtype *ob)
{
    GivePoints (5000);
    NewState (ob,&s_willdie1);
    PlaceItemType (bo_key1,ob->tilex,ob->tiley);
    TimeWarp_SetEnabled(false);
    gamestate.clockking_timer = 0;
}

void ClockKing_DoAttackState(objtype *ob)
{
    int rnd;
    int starthp;
    bool doshoot;

    doshoot = true;

    starthp = starthitpoints[gamestate.difficulty][en_will];
    if (ob->hitpoints < starthp / 3)
    {
        rnd = US_RndT();
        if (rnd < clockking_defensechance[gamestate.difficulty])
        {
            ClockKing_StartDefense(ob);
            doshoot = false;
        }
    }

    if (doshoot)
    {
        NewState (ob,&s_willshoot1);
    }
}

bool ClockKing_DamageActor(objtype *ob, int damage, int dmgtype)
{
    return ob->state == &s_clockkingdefense;
}

void ClockKing_StartDefense(objtype *ob)
{
    int lightId;
    LT_Light_t *light;

    lightId = LT_SpawnLightPredef(ob->x, ob->y, LT_LIGHT_PREDEF_HIGH);

    light = LT_GetLightById(lightId);
    light->state.cfg.lifeTics = CLOCKKING_DEFENSE_PERIOD;
    light->state.cfg.think = 
        (LT_Light_AnonThink_t)LT_LightThinkLifeTimed;

    NewState (ob,&s_clockkingdefense);

    HealthMeter_SetBarFlashing(ob->healthMeterBarId, true);
}

void ClockKing_EndDefense(objtype *ob)
{
    HealthMeter_SetBarFlashing(ob->healthMeterBarId, false);
}
#endif