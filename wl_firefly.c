// WL_FIREFLY.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_FIREFLY)
#include "wl_shade.h"

statetype s_flameproj              = {true,SPR_CLOCKPROJ_1,3,(statefunc)FireFly_ProjectileThink,NULL,&s_flameproj};

statetype s_flameprojbreak1        = {false,SPR_CLOCKPROJ_BREAK1,6,NULL,NULL,&s_flameprojbreak2};
statetype s_flameprojbreak2        = {false,SPR_CLOCKPROJ_BREAK2,6,NULL,NULL,&s_flameprojbreak3};
statetype s_flameprojbreak3        = {false,SPR_CLOCKPROJ_BREAK3,6,NULL,NULL,&s_flameprojbreak4};
statetype s_flameprojbreak4        = {false,SPR_CLOCKPROJ_BREAK4,6,NULL,NULL,NULL};

void FireFly_Spawn(int x, int y)
{
    SpawnTrans(x, y);
    newobj->obclass = anyactorobj;
}

void FireFly_Launch(objtype *ob, objtype *proj)
{
    PlaySoundLocActor (FIREFLYSHOOTSND, proj);
    proj->state = &s_flameproj;
}

void FireFly_ProjectileThink (objtype *proj)
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
        PlaySoundLocActor(FLAMEPROJBREAKSND,proj);
        proj->state = &s_flameprojbreak1;
        return;
    }

    proj->x += movex;
    proj->y += movey;

    deltax = LABS(proj->x - player->x);
    deltay = LABS(proj->y - player->y);

    if (deltax < PROJECTILESIZE && deltay < PROJECTILESIZE)
    {
        PlaySoundLocActor(FLAMEPROJHITSND,proj);
        damage = (US_RndT() >> 3);
        TakeDamage (damage, Object_ProjShooter(proj));
        proj->state = NULL;
        return;
    }

    proj->tilex = (short)(proj->x >> TILESHIFT);
    proj->tiley = (short)(proj->y >> TILESHIFT);
}
#endif