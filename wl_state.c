// WL_STATE.C

#include "wl_def.h"

#if defined(EMBEDDED) && defined(SEGA_SATURN)
extern statetype gamestates[MAXSTATES];
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


static const dirtype opposite[9] =
    {west,southwest,south,southeast,east,northeast,north,northwest,nodir};

static const dirtype diagonal[9][9] =
{
    /* east */  {nodir,nodir,northeast,nodir,nodir,nodir,southeast,nodir,nodir},
                {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
    /* north */ {northeast,nodir,nodir,nodir,northwest,nodir,nodir,nodir,nodir},
                {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
    /* west */  {nodir,nodir,northwest,nodir,nodir,nodir,southwest,nodir,nodir},
                {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
    /* south */ {southeast,nodir,nodir,nodir,southwest,nodir,nodir,nodir,nodir},
                {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir},
                {nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir,nodir}
};



#if defined(EMBEDDED) && defined(SEGA_SATURN)
void SpawnNewObj(unsigned tilex, unsigned tiley, int state);
#else
void SpawnNewObj(unsigned tilex, unsigned tiley, statetype* state);
#endif
void    NewState (objtype *ob, statetype *state);

boolean TryWalk (objtype *ob);
void    MoveObj (objtype *ob, int move);

void    KillActor (objtype *ob);
void    DamageActor (objtype *ob, unsigned damage);

boolean CheckLine (objtype *ob);
void    FirstSighting (objtype *ob);
boolean CheckSight (objtype *ob);

/*
=============================================================================
                                LOCAL VARIABLES
=============================================================================
*/



//===========================================================================


/*
===================
=
= SpawnNewObj
=
= Spawns a new actor at the given TILE coordinates, with the given state, and
= the given size in GLOBAL units.
=
= newobj = a pointer to an initialized new actor
=
===================
*/
#if defined(EMBEDDED) && defined(SEGA_SATURN)
void	SpawnNewObj(unsigned tilex, unsigned tiley, int state){ /* stateenum */
GetNewActor();

newobj->state = state;
if (gamestates[state].tictime)
newobj->ticcount = US_RndT() % gamestates[state].tictime;
else
newobj->ticcount = 0;

newobj->tilex = tilex;
newobj->tiley = tiley;
newobj->x = ((long)tilex << TILESHIFT) + TILEGLOBAL / 2;
newobj->y = ((long)tiley << TILESHIFT) + TILEGLOBAL / 2;
newobj->dir = nodir;

actorat[tilex][tiley] = newobj->id | 0x8000;
newobj->areanumber =
*(mapsegs[0] + farmapylookup[newobj->tiley] + newobj->tilex) - AREATILE;

}
#else
void SpawnNewObj (unsigned tilex, unsigned tiley, statetype *state)
{
    GetNewActor ();
    newobj->state = state;
    if (state->tictime)
        newobj->ticcount = DEMOCHOOSE_ORIG_SDL(
                US_RndT () % state->tictime,
                US_RndT () % state->tictime + 1);     // Chris' moonwalk bugfix ;D
    else
        newobj->ticcount = 0;

    newobj->tilex = (short) tilex;
    newobj->tiley = (short) tiley;
    newobj->x = ((fixed)tilex<<TILESHIFT)+TILEGLOBAL/2;
    newobj->y = ((fixed)tiley<<TILESHIFT)+TILEGLOBAL/2;
    newobj->dir = nodir;

    actorat[tilex][tiley] = newobj;
    newobj->areanumber = MAPSPOT(tilex,tiley,0) - AREATILE;
}
#endif

/*
===================
=
= NewState
=
= Changes ob to a new state, setting ticcount to the max for that state
=
===================
*/

#if defined(EMBEDDED) && defined(SEGA_SATURN)
void NewState(objtype* ob, int state)
#else
void NewState(objtype* ob, statetype* state)
#endif
{
    ob->state = state;
#if defined(EMBEDDED) && defined(SEGA_SATURN)
    ob->ticcount = gamestates[state].tictime;
#else	
    ob->ticcount = state->tictime;
#endif
}



/*
=============================================================================
                        ENEMY TILE WORLD MOVEMENT CODE
=============================================================================
*/


/*
==================================
=
= TryWalk
=
= Attempts to move ob in its current (ob->dir) direction.
=
= If blocked by either a wall or an actor returns FALSE
=
= If move is either clear or blocked only by a door, returns TRUE and sets
=
= ob->tilex         = new destination
= ob->tiley
= ob->areanumber    = the floor tile number (0-(NUMAREAS-1)) of destination
= ob->distance      = TILEGLOBAl, or -doornumber if a door is blocking the way
=
= If a door is in the way, an OpenDoor call is made to start it opening.
= The actor code should wait until
=       doorobjlist[-ob->distance].action = dr_open, meaning the door has been
=       fully opened
=
==================================
*/
#if defined(EMBEDDED) && defined(SEGA_SATURN)
#define CHECKDIAG(x,y)								\
{                                               \
	if (any_actor_at(x, y))                                       \
	{                                               \
		if (!obj_actor_at(x, y))                               \
			return false;                           \
		if (objlist[get_actor_at(x, y)].flags & FL_SHOOTABLE)  \
			return false;                           \
	}                                               \
}

#else
#define CHECKDIAG(x,y)                              \
{                                                   \
    temp=(uintptr_t)actorat[x][y];                  \
    if (temp)                                       \
    {                                               \
        if (temp<BIT_ALLTILES)                      \
            return false;                           \
        if (((objtype *)temp)->flags&FL_SHOOTABLE)  \
            return false;                           \
    }                                               \
}
#endif
#ifdef PLAYDEMOLIKEORIGINAL
    #define DOORCHECK                                   \
            if(DEMOCOND_ORIG)                           \
                doornum = temp&63;                      \
            else                                        \
            {                                           \
                doornum = (int) temp & ~BIT_DOOR;       \
                if (ob->obclass != ghostobj             \
                    && ob->obclass != spectreobj)       \
                {                                       \
                    OpenDoor(doornum);                  \
                    ob->distance = -doornum - 1;        \
                    return true;                        \
                }                                       \
            }
#else
    #define DOORCHECK                                   \
            doornum = (int) temp & ~BIT_DOOR;           \
            if (ob->obclass != ghostobj                 \
                && ob->obclass != spectreobj)           \
            {                                           \
                OpenDoor(doornum);                      \
                ob->distance = -doornum - 1;            \
                return true;                            \
            }
#endif

#if defined(EMBEDDED) && defined(SEGA_SATURN)
#define CHECKSIDE(x,y)								\
{                                               \
	if (any_actor_at(x, y))                                       \
	{                                               \
		if (wall_actor_at(x, y))                               \
			return false;                           \
		if (!obj_actor_at(x, y))                               \
			doornum = get_actor_at(x, y);                      \
		else if (objlist[get_actor_at(x, y)].flags & FL_SHOOTABLE) \
			return false;                           \
	}                                               \
}

#else
#define CHECKSIDE(x,y)                                  \
{                                                       \
    temp=(uintptr_t)actorat[x][y];                      \
    if (temp)                                           \
    {                                                   \
        if (temp<BIT_DOOR)                              \
            return false;                               \
        if (temp<BIT_ALLTILES)                          \
        {                                               \
            DOORCHECK                                   \
        }                                               \
        else if (((objtype *)temp)->flags&FL_SHOOTABLE) \
            return false;                               \
    }                                                   \
}
#endif

boolean TryWalk (objtype *ob)
{
    int       doornum = -1;
    uintptr_t temp;

    if (ob->obclass == inertobj)
    {
        switch (ob->dir)
        {
            case north:
                ob->tiley--;
                break;

            case northeast:
                ob->tilex++;
                ob->tiley--;
                break;

            case east:
                ob->tilex++;
                break;

            case southeast:
                ob->tilex++;
                ob->tiley++;
                break;

            case south:
                ob->tiley++;
                break;

            case southwest:
                ob->tilex--;
                ob->tiley++;
                break;

            case west:
                ob->tilex--;
                break;

            case northwest:
                ob->tilex--;
                ob->tiley--;
                break;
        }
    }
    else
    {
        switch (ob->dir)
        {
            case north:
                if (ob->obclass == dogobj || ob->obclass == fakeobj)
                {
                    CHECKDIAG(ob->tilex,ob->tiley-1);
                }
                else
                {
                    CHECKSIDE(ob->tilex,ob->tiley-1);
                }
                ob->tiley--;
                break;

            case northeast:
                CHECKDIAG(ob->tilex+1,ob->tiley-1);
                CHECKDIAG(ob->tilex+1,ob->tiley);
                CHECKDIAG(ob->tilex,ob->tiley-1);
                ob->tilex++;
                ob->tiley--;
                break;

            case east:
                if (ob->obclass == dogobj || ob->obclass == fakeobj)
                {
                    CHECKDIAG(ob->tilex+1,ob->tiley);
                }
                else
                {
                    CHECKSIDE(ob->tilex+1,ob->tiley);
                }
                ob->tilex++;
                break;

            case southeast:
                CHECKDIAG(ob->tilex+1,ob->tiley+1);
                CHECKDIAG(ob->tilex+1,ob->tiley);
                CHECKDIAG(ob->tilex,ob->tiley+1);
                ob->tilex++;
                ob->tiley++;
                break;

            case south:
                if (ob->obclass == dogobj || ob->obclass == fakeobj)
                {
                    CHECKDIAG(ob->tilex,ob->tiley+1);
                }
                else
                {
                    CHECKSIDE(ob->tilex,ob->tiley+1);
                }
                ob->tiley++;
                break;

            case southwest:
                CHECKDIAG(ob->tilex-1,ob->tiley+1);
                CHECKDIAG(ob->tilex-1,ob->tiley);
                CHECKDIAG(ob->tilex,ob->tiley+1);
                ob->tilex--;
                ob->tiley++;
                break;

            case west:
                if (ob->obclass == dogobj || ob->obclass == fakeobj)
                {
                    CHECKDIAG(ob->tilex-1,ob->tiley);
                }
                else
                {
                    CHECKSIDE(ob->tilex-1,ob->tiley);
                }
                ob->tilex--;
                break;

            case northwest:
                CHECKDIAG(ob->tilex-1,ob->tiley-1);
                CHECKDIAG(ob->tilex-1,ob->tiley);
                CHECKDIAG(ob->tilex,ob->tiley-1);
                ob->tilex--;
                ob->tiley--;
                break;

            case nodir:
                return false;

            default:
                Quit ("Walk: Bad dir");
        }
    }

#ifdef PLAYDEMOLIKEORIGINAL
    if (doornum != -1)
    {
        OpenDoor(doornum);
        ob->distance = -doornum-1;
        return true;
    }
#endif
#if defined(EMBEDDED) && defined(SEGA_SATURN)
    ob->areanumber = *(mapsegs[0] + farmapylookup[tiley] + tilex) - AREATILE;
#else
    ob->areanumber = MAPSPOT(ob->tilex,ob->tiley,0) - AREATILE;
#endif
    ob->distance = TILEGLOBAL;
    return true;
}


/*
==================================
=
= SelectDodgeDir
=
= Attempts to choose and initiate a movement for ob that sends it towards
= the player while dodging
=
= If there is no possible move (ob is totally surrounded)
=
= ob->dir           =       nodir
=
= Otherwise
=
= ob->dir           = new direction to follow
= ob->distance      = TILEGLOBAL or -doornumber
= ob->tilex         = new destination
= ob->tiley
= ob->areanumber    = the floor tile number (0-(NUMAREAS-1)) of destination
=
==================================
*/

void SelectDodgeDir (objtype *ob)
{
    int         deltax,deltay,i;
    unsigned    absdx,absdy;
    dirtype     dirtry[5];
    dirtype     turnaround,tdir;

    if (ob->flags & FL_FIRSTATTACK)
    {
        //
        // turning around is only ok the very first time after noticing the
        // player
        //
        turnaround = nodir;
        ob->flags &= ~FL_FIRSTATTACK;
    }
    else
        turnaround=opposite[ob->dir];

    deltax = player->tilex - ob->tilex;
    deltay = player->tiley - ob->tiley;

    //
    // arrange 5 direction choices in order of preference
    // the four cardinal directions plus the diagonal straight towards
    // the player
    //

    if (deltax>0)
    {
        dirtry[1]= east;
        dirtry[3]= west;
    }
    else
    {
        dirtry[1]= west;
        dirtry[3]= east;
    }

    if (deltay>0)
    {
        dirtry[2]= south;
        dirtry[4]= north;
    }
    else
    {
        dirtry[2]= north;
        dirtry[4]= south;
    }

    //
    // randomize a bit for dodging
    //
    absdx = abs(deltax);
    absdy = abs(deltay);

    if (absdx > absdy)
    {
        tdir = dirtry[1];
        dirtry[1] = dirtry[2];
        dirtry[2] = tdir;
        tdir = dirtry[3];
        dirtry[3] = dirtry[4];
        dirtry[4] = tdir;
    }

    if (US_RndT() < 128)
    {
        tdir = dirtry[1];
        dirtry[1] = dirtry[2];
        dirtry[2] = tdir;
        tdir = dirtry[3];
        dirtry[3] = dirtry[4];
        dirtry[4] = tdir;
    }

    dirtry[0] = diagonal [ dirtry[1] ] [ dirtry[2] ];

    //
    // try the directions until one works
    //
    for (i=0;i<5;i++)
    {
        if ( dirtry[i] == nodir || dirtry[i] == turnaround)
            continue;

        ob->dir = dirtry[i];
        if (TryWalk(ob))
            return;
    }

    //
    // turn around only as a last resort
    //
    if (turnaround != nodir)
    {
        ob->dir = turnaround;

        if (TryWalk(ob))
            return;
    }

    ob->dir = nodir;
}


/*
============================
=
= SelectChaseDir
=
= As SelectDodgeDir, but doesn't try to dodge
=
============================
*/

void SelectChaseDir (objtype *ob)
{
    int     deltax,deltay;
    dirtype d[3];
    dirtype tdir, olddir, turnaround;


    olddir=ob->dir;
    turnaround=opposite[olddir];

    deltax=player->tilex - ob->tilex;
    deltay=player->tiley - ob->tiley;

    d[1]=nodir;
    d[2]=nodir;

    if (deltax>0)
        d[1]= east;
    else if (deltax<0)
        d[1]= west;
    if (deltay>0)
        d[2]=south;
    else if (deltay<0)
        d[2]=north;

    if (abs(deltay)>abs(deltax))
    {
        tdir=d[1];
        d[1]=d[2];
        d[2]=tdir;
    }

    if (d[1]==turnaround)
        d[1]=nodir;
    if (d[2]==turnaround)
        d[2]=nodir;


    if (d[1]!=nodir)
    {
        ob->dir=d[1];
        if (TryWalk(ob))
            return;     /*either moved forward or attacked*/
    }

    if (d[2]!=nodir)
    {
        ob->dir=d[2];
        if (TryWalk(ob))
            return;
    }

    /* there is no direct path to the player, so pick another direction */

    if (olddir!=nodir)
    {
        ob->dir=olddir;
        if (TryWalk(ob))
            return;
    }

    if (US_RndT()>128)      /*randomly determine direction of search*/
    {
        for (tdir=north; tdir<=west; tdir=(dirtype)(tdir+1))
        {
            if (tdir!=turnaround)
            {
                ob->dir=tdir;
                if ( TryWalk(ob) )
                    return;
            }
        }
    }
    else
    {
        for (tdir=west; tdir>=north; tdir=(dirtype)(tdir-1))
        {
            if (tdir!=turnaround)
            {
                ob->dir=tdir;
                if ( TryWalk(ob) )
                    return;
            }
        }
    }

    if (turnaround !=  nodir)
    {
        ob->dir=turnaround;
        if (ob->dir != nodir)
        {
            if ( TryWalk(ob) )
                return;
        }
    }

    ob->dir = nodir;                // can't move
}


/*
============================
=
= SelectRunDir
=
= Run Away from player
=
============================
*/

void SelectRunDir (objtype *ob)
{
    int deltax,deltay;
    dirtype d[3];
    dirtype tdir;


    deltax=player->tilex - ob->tilex;
    deltay=player->tiley - ob->tiley;

    if (deltax<0)
        d[1]= east;
    else
        d[1]= west;
    if (deltay<0)
        d[2]=south;
    else
        d[2]=north;

    if (abs(deltay)>abs(deltax))
    {
        tdir=d[1];
        d[1]=d[2];
        d[2]=tdir;
    }

    ob->dir=d[1];
    if (TryWalk(ob))
        return;     /*either moved forward or attacked*/

    ob->dir=d[2];
    if (TryWalk(ob))
        return;

    /* there is no direct path to the player, so pick another direction */

    if (US_RndT()>128)      /*randomly determine direction of search*/
    {
        for (tdir=north; tdir<=west; tdir=(dirtype)(tdir+1))
        {
            ob->dir=tdir;
            if ( TryWalk(ob) )
                return;
        }
    }
    else
    {
        for (tdir=west; tdir>=north; tdir=(dirtype)(tdir-1))
        {
            ob->dir=tdir;
            if ( TryWalk(ob) )
                return;
        }
    }

    ob->dir = nodir;                // can't move
}


/*
=================
=
= MoveObj
=
= Moves ob be move global units in ob->dir direction
= Actors are not allowed to move inside the player
= Does NOT check to see if the move is tile map valid
=
= ob->x                 = adjusted for new position
= ob->y
=
=================
*/

void MoveObj (objtype *ob, int move)
{
    int    deltax,deltay;

    switch (ob->dir)
    {
        case north:
            ob->y -= move;
            break;
        case northeast:
            ob->x += move;
            ob->y -= move;
            break;
        case east:
            ob->x += move;
            break;
        case southeast:
            ob->x += move;
            ob->y += move;
            break;
        case south:
            ob->y += move;
            break;
        case southwest:
            ob->x -= move;
            ob->y += move;
            break;
        case west:
            ob->x -= move;
            break;
        case northwest:
            ob->x -= move;
            ob->y -= move;
            break;

        case nodir:
            return;

        default:
            Quit ("MoveObj: bad dir!");
    }

    //
    // check to make sure it's not on top of player
    //
#ifdef SEGA_SATURN
    if (getareabyplayer(ob->areanumber))
#else
    if (ob->areanumber >= NUMAREAS || areabyplayer[ob->areanumber])
#endif
    {
        deltax = ob->x - player->x;
        if (deltax < -MINACTORDIST || deltax > MINACTORDIST)
            goto moveok;
        deltay = ob->y - player->y;
        if (deltay < -MINACTORDIST || deltay > MINACTORDIST)
            goto moveok;

        if (ob->hidden && spotvis[player->tilex][player->tiley])
            goto moveok;         // move closer until he meets CheckLine

        if (ob->obclass == ghostobj || ob->obclass == spectreobj)
            TakeDamage ((int)tics*2,ob);

        //
        // back up
        //
        switch (ob->dir)
        {
            case north:
                ob->y += move;
                break;
            case northeast:
                ob->x -= move;
                ob->y += move;
                break;
            case east:
                ob->x -= move;
                break;
            case southeast:
                ob->x -= move;
                ob->y -= move;
                break;
            case south:
                ob->y -= move;
                break;
            case southwest:
                ob->x += move;
                ob->y -= move;
                break;
            case west:
                ob->x += move;
                break;
            case northwest:
                ob->x += move;
                ob->y += move;
                break;

            case nodir:
                return;
        }
        return;
    }
moveok:
    ob->distance -=move;
}

/*
=============================================================================
                                STUFF
=============================================================================
*/
#ifndef SEGA_SATURN
/*
===============
=
= DropItem
=
= Tries to drop a bonus item somewhere in the tiles surrounding the
= given tilex/tiley
=
===============
*/

void DropItem (wl_stat_t itemtype, int tilex, int tiley)
{
    int     x,y,xl,xh,yl,yh;

    //
    // find a free spot to put it in
    //
    if (!actorat[tilex][tiley])
    {
        PlaceItemType (itemtype, tilex,tiley);
        return;
    }

    xl = tilex-1;
    xh = tilex+1;
    yl = tiley-1;
    yh = tiley+1;

    for (x=xl ; x<= xh ; x++)
    {
        for (y=yl ; y<= yh ; y++)
        {
            if (!actorat[x][y])
            {
                PlaceItemType (itemtype, x,y);
                return;
            }
        }
    }
}
#endif


/*
===============
=
= KillActor
=
===============
*/
#if defined(EMBEDDED) && defined(SEGA_SATURN)
void KillActor(objtype* ob)
{
    int	tilex, tiley;
    int	points;
    int	newstate;

    tilex = ob->tilex = ob->x >> TILESHIFT;		// drop item on center
    tiley = ob->tiley = ob->y >> TILESHIFT;

    switch (ob->obclass)
    {
    case guardobj:
        points = 100;
        newstate = s_grddie1;
        PlaceItemType(bo_clip2, tilex, tiley);
        break;

    case officerobj:
        points = 400;
        newstate = s_ofcdie1;
        PlaceItemType(bo_clip2, tilex, tiley);
        break;

    case mutantobj:
        points = 700;
        newstate = s_mutdie1;
        PlaceItemType(bo_clip2, tilex, tiley);
        break;

    case ssobj:
        points = 500;
        newstate = s_ssdie1;
        if (gamestate.bestweapon < wp_machinegun)
            PlaceItemType(bo_machinegun, tilex, tiley);
        else
            PlaceItemType(bo_clip2, tilex, tiley);
        break;

    case dogobj:
        points = 200;
        newstate = s_dogdie1;
        break;

#ifndef SPEAR
    case bossobj:
        points = 5000;
        newstate = s_bossdie1;
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case gretelobj:
        points = 5000;
        newstate = s_greteldie1;
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case giftobj:
        points = 5000;
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        newstate = s_giftdie1;
        break;

    case fatobj:
        points = 5000;
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        newstate = s_fatdie1;
        break;

    case schabbobj:
        points = 5000;
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        newstate = s_schabbdie1;
        A_DeathScream(ob);
        break;
    case fakeobj:
        points = 2000;
        newstate = s_fakedie1;
        break;

    case mechahitlerobj:
        points = 5000;
        newstate = s_mechadie1;
        break;
    case realhitlerobj:
        points = 5000;
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        newstate = s_hitlerdie1;
        A_DeathScream(ob);
        break;
#else
    case spectreobj:
        points = 200;
        newstate = s_spectredie1;
        break;

    case angelobj:
        points = 5000;
        newstate = s_angeldie1;
        break;

    case transobj:
        points = 5000;
        newstate = s_transdie0;
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case uberobj:
        points = 5000;
        newstate = s_uberdie0;
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case willobj:
        points = 5000;
        newstate = s_willdie1;
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case deathobj:
        points = 5000;
        newstate = s_deathdie1;
        PlaceItemType(bo_key1, tilex, tiley);
        break;
#endif
    default:
        points = 0;
        newstate = ob->state;
        break;
    }

    GivePoints(points);
    NewState(ob, newstate);
    //#ifdef ENABLE_STATS
    gamestate.killcount++;
    //#endif
    ob->flags &= ~FL_SHOOTABLE;
    //	clear_actor(ob->tilex, ob->tiley);
    actorat[ob->tilex][ob->tiley] = NULL;
    ob->flags |= FL_NONMARK;
}
#else
void KillActor (objtype *ob)
{
    int     tilex,tiley;

    tilex = ob->x >> TILESHIFT;         // drop item on center
    tiley = ob->y >> TILESHIFT;

    switch (ob->obclass)
    {
        case guardobj:
            GivePoints (100);
            NewState (ob,&s_grddie1);
            PlaceItemType (bo_clip2,tilex,tiley);
            break;

        case officerobj:
            GivePoints (400);
            NewState (ob,&s_ofcdie1);
            PlaceItemType (bo_clip2,tilex,tiley);
            break;

        case mutantobj:
            GivePoints (700);
            NewState (ob,&s_mutdie1);
            PlaceItemType (bo_clip2,tilex,tiley);
            break;

        case ssobj:
            GivePoints (500);
            NewState (ob,&s_ssdie1);
            if (gamestate.bestweapon < wp_machinegun)
                PlaceItemType (bo_machinegun,tilex,tiley);
            else
                PlaceItemType (bo_clip2,tilex,tiley);
            break;

        case dogobj:
            GivePoints (200);
            NewState (ob,&s_dogdie1);
            break;

#ifndef SPEAR
        case bossobj:
            GivePoints (5000);
            NewState (ob,&s_bossdie1);
            PlaceItemType (bo_key1,tilex,tiley);
            break;

        case gretelobj:
            GivePoints (5000);
            NewState (ob,&s_greteldie1);
            PlaceItemType (bo_key1,tilex,tiley);
            break;

        case giftobj:
            GivePoints (5000);
            gamestate.killx = player->x;
            gamestate.killy = player->y;
            NewState (ob,&s_giftdie1);
            break;

        case fatobj:
            GivePoints (5000);
            gamestate.killx = player->x;
            gamestate.killy = player->y;
            NewState (ob,&s_fatdie1);
            break;

        case schabbobj:
            GivePoints (5000);
            gamestate.killx = player->x;
            gamestate.killy = player->y;
            NewState (ob,&s_schabbdie1);
            break;
        case fakeobj:
            GivePoints (2000);
            NewState (ob,&s_fakedie1);
            break;

        case mechahitlerobj:
            GivePoints (5000);
            NewState (ob,&s_mechadie1);
            break;
        case realhitlerobj:
            GivePoints (5000);
            gamestate.killx = player->x;
            gamestate.killy = player->y;
            NewState (ob,&s_hitlerdie1);
            break;
#else
        case spectreobj:
            if (ob->flags&FL_BONUS)
            {
                GivePoints (200);       // Get points once for each
                ob->flags &= ~FL_BONUS;
            }
            NewState (ob,&s_spectredie1);
            break;

        case angelobj:
            GivePoints (5000);
            NewState (ob,&s_angeldie1);
            break;

        case transobj:
            GivePoints (5000);
            NewState (ob,&s_transdie0);
            PlaceItemType (bo_key1,tilex,tiley);
            break;

        case uberobj:
            GivePoints (5000);
            NewState (ob,&s_uberdie0);
            PlaceItemType (bo_key1,tilex,tiley);
            break;

        case willobj:
            GivePoints (5000);
            NewState (ob,&s_willdie1);
            PlaceItemType (bo_key1,tilex,tiley);
            break;

        case deathobj:
            GivePoints (5000);
            NewState (ob,&s_deathdie1);
            PlaceItemType (bo_key1,tilex,tiley);
            break;
#endif
    }

    gamestate.killcount++;
    ob->flags &= ~FL_SHOOTABLE;
    actorat[ob->tilex][ob->tiley] = NULL;
    ob->flags |= FL_NONMARK;
}
#endif


/*
===================
=
= DamageActor
=
= Called when the player successfully hits an enemy.
=
= Does damage points to enemy ob, either putting it into a stun frame or
= killing it.
=
===================
*/

void DamageActor (objtype *ob, unsigned damage)
{
    madenoise = true;

    //
    // do double damage if shooting a non attack mode actor
    //
    if ( !(ob->flags & FL_ATTACKMODE) )
        damage <<= 1;

    ob->hitpoints -= (short)damage;

    if (ob->hitpoints<=0)
        KillActor (ob);
    else
    {
        if (! (ob->flags & FL_ATTACKMODE) )
            FirstSighting (ob);             // put into combat mode

        switch (ob->obclass)                // dogs only have one hit point
        {
            case guardobj:
                if (ob->hitpoints&1)
#if defined(EMBEDDED) && defined(SEGA_SATURN)
                    NewState(ob, s_grdpain);
                else
                    NewState(ob, s_grdpain1);
#else
                    NewState (ob,&s_grdpain);
                else
                    NewState (ob,&s_grdpain1);
#endif
                break;

            case officerobj:
                if (ob->hitpoints&1)
#if defined(EMBEDDED) && defined(SEGA_SATURN)
                    NewState(ob, s_ofcpain);
                else
                    NewState(ob, s_ofcpain1);
#else
                    NewState (ob,&s_ofcpain);
                else
                    NewState (ob,&s_ofcpain1);
#endif     
                break;

            case mutantobj:
                if (ob->hitpoints&1)
#if defined(EMBEDDED) && defined(SEGA_SATURN)
                    NewState(ob, s_mutpain);
                else
                    NewState(ob, s_mutpain1);
#else
                    NewState (ob,&s_mutpain);
                else
                    NewState (ob,&s_mutpain1);
#endif
                break;

            case ssobj:
                if (ob->hitpoints&1)
#if defined(EMBEDDED) && defined(SEGA_SATURN)
                    NewState(ob, s_sspain);
                else
                    NewState(ob, s_sspain1);
#else
                    NewState (ob,&s_sspain);
                else
                    NewState (ob,&s_sspain1);
#endif
                break;
        }
    }
}

/*
=============================================================================
                                CHECKSIGHT
=============================================================================
*/


/*
=====================
=
= CheckLine
=
= Returns true if a straight line between the player and ob is unobstructed
=
=====================
*/

boolean CheckLine (objtype *ob)
{
    int         x1,y1,xt1,yt1,x2,y2,xt2,yt2;
    int         x,y;
    int         xdist,ydist,xstep,ystep;
    int         partial,delta;
    int     ltemp;
    int         xfrac,yfrac,deltafrac;
    unsigned    value,intercept;

    x1 = ob->x >> UNSIGNEDSHIFT;            // 1/256 tile precision
    y1 = ob->y >> UNSIGNEDSHIFT;
    xt1 = x1 >> 8;
    yt1 = y1 >> 8;

    x2 = plux;
    y2 = pluy;
    xt2 = player->tilex;
    yt2 = player->tiley;

    xdist = abs(xt2-xt1);

    if (xdist > 0)
    {
        if (xt2 > xt1)
        {
            partial = 256-(x1&0xff);
            xstep = 1;
        }
        else
        {
            partial = x1&0xff;
            xstep = -1;
        }

        deltafrac = abs(x2-x1);
        delta = y2-y1;
        ltemp = ((int)delta<<8)/deltafrac;
        if (ltemp > 0x7fffl)
            ystep = 0x7fff;
        else if (ltemp < -0x7fffl)
            ystep = -0x7fff;
        else
            ystep = ltemp;
        yfrac = y1 + (((int)ystep*partial) >>8);

        x = xt1+xstep;
        xt2 += xstep;
        do
        {
            y = yfrac>>8;
            yfrac += ystep;

            value = (unsigned)tilemap[x][y];
            x += xstep;

            if (!value)
                continue;

            if (value<BIT_DOOR || value>BIT_ALLTILES)
                return false;

            //
            // see if the door is open enough
            //
            value &= ~BIT_DOOR;
            intercept = yfrac-ystep/2;

#ifdef BLAKEDOORS
            if (intercept<ldoorposition[value] || intercept>rdoorposition[value])
#else
            if (intercept > doorposition[value])
#endif
                return false;

        } while (x != xt2);
    }

    ydist = abs(yt2-yt1);

    if (ydist > 0)
    {
        if (yt2 > yt1)
        {
            partial = 256-(y1&0xff);
            ystep = 1;
        }
        else
        {
            partial = y1&0xff;
            ystep = -1;
        }

        deltafrac = abs(y2-y1);
        delta = x2-x1;
        ltemp = ((int)delta<<8)/deltafrac;
        if (ltemp > 0x7fffl)
            xstep = 0x7fff;
        else if (ltemp < -0x7fffl)
            xstep = -0x7fff;
        else
            xstep = ltemp;
        xfrac = x1 + (((int)xstep*partial) >>8);

        y = yt1 + ystep;
        yt2 += ystep;
        do
        {
            x = xfrac>>8;
            xfrac += xstep;

            value = (unsigned)tilemap[x][y];
            y += ystep;

            if (!value)
                continue;

            if (value<BIT_DOOR || value>BIT_ALLTILES)
                return false;

            //
            // see if the door is open enough
            //
            value &= ~BIT_DOOR;
            intercept = xfrac-xstep/2;

#ifdef BLAKEDOORS
            if (intercept<ldoorposition[value] || intercept>rdoorposition[value])
#else
            if (intercept > doorposition[value])
#endif
                return false;
        } while (y != yt2);
    }

    return true;
}


/*
================
=
= CheckSight
=
= Checks a straight line between player and current object
=
= If the sight is ok, check alertness and angle to see if they notice
=
= returns true if the player has been spotted
=
================
*/

#define MINSIGHT        0x18000l

boolean CheckSight (objtype *ob)
{
    int deltax,deltay;

    //
    // don't bother tracing a line if the area isn't connected to the player's
    //
    if (ob->areanumber < NUMAREAS && !areabyplayer[ob->areanumber])
        return false;

    //
    // if the player is real close, sight is automatic
    //
    deltax = player->x - ob->x;
    deltay = player->y - ob->y;

    if (deltax > -MINSIGHT && deltax < MINSIGHT
        && deltay > -MINSIGHT && deltay < MINSIGHT)
        return true;

    //
    // see if they are looking in the right direction
    //
    switch (ob->dir)
    {
        case north:
            if (deltay > 0)
                return false;
            break;

        case east:
            if (deltax < 0)
                return false;
            break;

        case south:
            if (deltay < 0)
                return false;
            break;

        case west:
            if (deltax > 0)
                return false;
            break;

        // check diagonal moving guards fix

        case northwest:
            if (DEMOCOND_SDL && deltay > -deltax)
                return false;
            break;

        case northeast:
            if (DEMOCOND_SDL && deltay > deltax)
                return false;
            break;

        case southwest:
            if (DEMOCOND_SDL && deltax > deltay)
                return false;
            break;

        case southeast:
            if (DEMOCOND_SDL && -deltax > deltay)
                return false;
            break;
    }

    //
    // trace a line to check for blocking tiles (corners)
    //
    return CheckLine (ob);
}


/*
===============
=
= FirstSighting
=
= Puts an actor into attack mode and possibly reverses the direction
= if the player is behind it
=
===============
*/

void FirstSighting (objtype *ob)
{
    //
    // react to the player
    //
    switch (ob->obclass)
    {
        case guardobj:
            PlaySoundLocActor(HALTSND,ob);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_grdchase1);
#else
            NewState(ob, &s_grdchase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case officerobj:
            PlaySoundLocActor(SPIONSND,ob);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_ofcchase1);
#else
            NewState(ob, &s_ofcchase1);
#endif
            ob->speed *= 5;                 // go faster when chasing player
            break;

        case mutantobj:
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_mutchase1);
#else
            NewState(ob, &s_mutchase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case ssobj:
            PlaySoundLocActor(SCHUTZADSND,ob);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_sschase1);
#else
            NewState(ob, &s_sschase1);
#endif
            ob->speed *= 4;                 // go faster when chasing player
            break;

        case dogobj:
            PlaySoundLocActor(DOGBARKSND,ob);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_dogchase1);
#else
            NewState(ob, &s_dogchase1);
#endif
            ob->speed *= 2;                 // go faster when chasing player
            break;

#ifndef SPEAR
        case bossobj:
            SD_PlaySound(GUTENTAGSND);
#if defined(BOSS_MUSIC) && defined(VIEASM)
            ChangeGameMusic(ULTIMATE_MUS);
#endif

#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_bosschase1);
#else
            NewState(ob, &s_bosschase1);
#endif
            ob->speed = SPDPATROL*3;        // go faster when chasing player
            break;

#ifndef APOGEE_1_0
        case gretelobj:
            SD_PlaySound(KEINSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_gretelchase1);
#else
            NewState(ob, &s_gretelchase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case giftobj:
            SD_PlaySound(EINESND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_giftchase1);
#else			
            NewState(ob, &s_giftchase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case fatobj:
            SD_PlaySound(ERLAUBENSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_fatchase1);
#else
            NewState(ob, &s_fatchase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;
#endif

        case schabbobj:
            SD_PlaySound(SCHABBSHASND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_schabbchase1);
#else
            NewState(ob, &s_schabbchase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case fakeobj:
            SD_PlaySound(TOT_HUNDSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_fakechase1);
#else
            NewState(ob, &s_fakechase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case mechahitlerobj:
            SD_PlaySound(DIESND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_mechachase1);
#else
            NewState(ob, &s_mechachase1);
#endif
            ob->speed *= 3;                 // go faster when chasing player
            break;

        case realhitlerobj:
            SD_PlaySound(DIESND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_hitlerchase1);
#else
            NewState(ob, &s_hitlerchase1);
#endif
            ob->speed *= 5;                 // go faster when chasing player
            break;

        case ghostobj:
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_blinkychase1);
#else
            NewState(ob, &s_blinkychase1);
#endif
            ob->speed *= 2;                 // go faster when chasing player
            break;
#else
        case spectreobj:
            SD_PlaySound(GHOSTSIGHTSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_spectrechase1);
#else
            NewState(ob, &s_spectrechase1);
#endif
            ob->speed = 800;                        // go faster when chasing player
            break;

        case angelobj:
            SD_PlaySound(ANGELSIGHTSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_angelchase1);
#else
            NewState(ob, &s_angelchase1);
#endif
            ob->speed = 1536;                       // go faster when chasing player
            break;

        case transobj:
            SD_PlaySound(TRANSSIGHTSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_transchase1);
#else
            NewState(ob, &s_transchase1);
#endif
            ob->speed = 1536;                       // go faster when chasing player
            break;

        case uberobj:
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_uberchase1);
#else
            NewState(ob, &s_uberchase1);
#endif
            ob->speed = 3000;                       // go faster when chasing player
            break;

        case willobj:
            SD_PlaySound(WILHELMSIGHTSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_willchase1);
#else
            NewState(ob, &s_willchase1);
#endif
            ob->speed = 2048;                       // go faster when chasing player
            break;

        case deathobj:
            SD_PlaySound(KNIGHTSIGHTSND);
#if defined(EMBEDDED) && defined(SEGA_SATURN)
            NewState(ob, s_deathchase1);
#else
            NewState(ob, &s_deathchase1);
#endif
            ob->speed = 2048;                       // go faster when chasing player
            break;
#endif
    }

    if (ob->distance < 0)
        ob->distance = 0;       // ignore the door opening command

    ob->flags |= FL_ATTACKMODE|FL_FIRSTATTACK;
    ob->active = (activetype)true;	// wake up the guards! Wolf3s: HUH?
}



/*
===============
=
= SightPlayer
=
= Called by actors that ARE NOT chasing the player.  If the player
= is detected (by sight, noise, or proximity), the actor is put into
= it's combat frame and true is returned.
=
= Incorporates a random reaction delay
=
===============
*/

boolean SightPlayer (objtype *ob)
{
    if (ob->flags & FL_ATTACKMODE)
        Quit ("An actor in ATTACKMODE called SightPlayer!");

    if (ob->temp2)
    {
        //
        // count down reaction time
        //
        ob->temp2 -= (short) tics;
        if (ob->temp2 > 0)
            return false;
        ob->temp2 = 0;                                  // time to react
    }
    else
    {
        if (ob->areanumber < NUMAREAS && !areabyplayer[ob->areanumber])
            return false;

        if (ob->flags & FL_AMBUSH)
        {
            if (!CheckSight (ob))
                return false;
            ob->flags &= ~FL_AMBUSH;
        }
        else
        {
            if (!madenoise && !CheckSight (ob))
                return false;
        }


        switch (ob->obclass)
        {
            case guardobj:
                ob->temp2 = 1+US_RndT()/4;
                break;
            case officerobj:
                ob->temp2 = 2;
                break;
            case mutantobj:
                ob->temp2 = 1+US_RndT()/6;
                break;
            case ssobj:
                ob->temp2 = 1+US_RndT()/6;
                break;
            case dogobj:
                ob->temp2 = 1+US_RndT()/8;
                break;

            case bossobj:
            case schabbobj:
            case fakeobj:
            case mechahitlerobj:
            case realhitlerobj:
            case gretelobj:
            case giftobj:
            case fatobj:
            case spectreobj:
            case angelobj:
            case transobj:
            case uberobj:
            case willobj:
            case deathobj:
                ob->temp2 = 1;
                break;
        }
        return false;
    }

    FirstSighting (ob);

    return true;
}
//WL_STATE_C
