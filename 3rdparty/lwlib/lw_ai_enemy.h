/*************************************************************************
** Batman: No Man's Land
** Copyright (C) 2012 by LinuxWolf - Team RayCAST
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
** LinuxWolf Library for Batman: No Man's Land
*************************************************************************/

#ifndef AI_ENEMY_H
#define AI_ENEMY_H

#include "lw_misc.h"

struct AI_Enemy_s;

lwlib_DefineDelegate(AI_Enemy_DelegateActorIsPlayer_t,
	// ret
	bool,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateActorDist_t,
	// ret
	int,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateActorBrightness_t,
	// ret
	double,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateRewindActors_t,
	// ret
	void,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateMoreActors_t,
	// ret
	bool,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateNextActor_t,
	// ret
	void,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateActorSpot_t,
	// ret
	int,
	// params
	void *obj
	);
lwlib_DefineDelegate(AI_Enemy_DelegateSuspicionLevelChanged_t,
	// ret
	void,
	// params
	void *obj, int prevSuspicionLevel
	);

typedef struct AI_Enemy_DelegateAPI_s
{
	AI_Enemy_DelegateActorIsPlayer_t actorIsPlayer;
	AI_Enemy_DelegateActorDist_t actorDist;
	AI_Enemy_DelegateActorBrightness_t actorBrightness;
	AI_Enemy_DelegateRewindActors_t rewindActors;
	AI_Enemy_DelegateMoreActors_t moreActors;
	AI_Enemy_DelegateNextActor_t nextActor;
	AI_Enemy_DelegateActorSpot_t actorSpot;
	AI_Enemy_DelegateSuspicionLevelChanged_t suspicionLevelChanged;
} AI_Enemy_DelegateAPI_t;

typedef enum AI_Enemy_SuspicionLevel_e
{
	AI_ENEMY_SUSPICIONLEVEL_PASSIVE,
	AI_ENEMY_SUSPICIONLEVEL_ALERTED,
	AI_ENEMY_SUSPICIONLEVEL_ALARMED,
} AI_Enemy_SuspicionLevel_t;

typedef struct AI_Enemy_s
{
	AI_Enemy_DelegateAPI_t delegateAPI;
	double *suspicionMeter;
	int *suspicionLevel;
	bool *killed;
	int *suspicionSpot;
	bool *suspicionTargetIsPlayer;
} AI_Enemy_t;

void AI_Enemy_Loop(AI_Enemy_t *enemy, int tics);

void AI_Enemy_DoDamage(AI_Enemy_t *enemy, int damage, int attackerSpot);

void AI_Enemy_DoNoise(AI_Enemy_t *enemy, double loudness, int noiseSpot,
	bool capEnable);

void AI_Enemy_DoKill(AI_Enemy_t *enemy);

#endif
