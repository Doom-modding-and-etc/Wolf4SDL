// WL_AI.C

#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_AI)
#include "wl_shade.h"

const double gunLoudness = 3.0;

typedef struct WL_AI_Enemy_s
{
	AI_Enemy_t aiEnemy;
	WL_AI_t *ai;
	objtype *ob;
	objtype *obIterator;
} WL_AI_Enemy_t;

static bool WL_AI_ActorRaisesSuspicion(WL_AI_Enemy_t *enemy,
	objtype *ob)
{
	if (ob == player && CheckSight(enemy->ob) && !notargetmode)
	{
		return true;
	}

	return false;
}

static bool WL_AI_EnemyDelegateActorIsPlayer(void *obj)
{
	WL_AI_Enemy_t *enemy;
	enemy = (WL_AI_Enemy_t *)obj;
	return enemy->obIterator == player;
}

static int WL_AI_EnemyDelegateActorDist(void *obj)
{
	WL_AI_Enemy_t *enemy;
	enemy = (WL_AI_Enemy_t *)obj;
	return Object_TileDist(enemy->obIterator, enemy->ob);
}

static double WL_AI_EnemyDelegateActorBrightness(void *obj)
{
	objtype *ob;
	WL_AI_Enemy_t *enemy;
	lwlib_TPoint3f spriteLitLevel;

	enemy = (WL_AI_Enemy_t *)obj;
	ob = enemy->obIterator;

	spriteLitLevel = LT_GetShadedSpriteLitLevel(ob->x, ob->y);
	return lwlib_vec3f_max(spriteLitLevel);
}

static void WL_AI_EnemyDelegateRewindActors(void *obj)
{
	objtype *ob;
	WL_AI_Enemy_t *enemy;

	enemy = (WL_AI_Enemy_t *)obj;

	ob = &objlist[0];
	while (ob != NULL && !WL_AI_ActorRaisesSuspicion(enemy, ob))
	{
		ob = ob->next;
	}

	enemy->obIterator = ob;
}

static bool WL_AI_EnemyDelegateMoreActors(void *obj)
{
	WL_AI_Enemy_t *enemy;
	enemy = (WL_AI_Enemy_t *)obj;
	return enemy->obIterator != NULL;
}

static void WL_AI_EnemyDelegateNextActor(void *obj)
{
	objtype *ob;
	WL_AI_Enemy_t *enemy;

	enemy = (WL_AI_Enemy_t *)obj;
	ob = enemy->obIterator;

	ob = ob->next;
	while (ob != NULL && !WL_AI_ActorRaisesSuspicion(enemy, ob))
	{
		ob = ob->next;
	}

	enemy->obIterator = ob;
}

static int WL_AI_EnemyDelegateActorSpot(void *obj)
{
	objtype *ob;
	WL_AI_Enemy_t *enemy;

	enemy = (WL_AI_Enemy_t *)obj;
	ob = enemy->obIterator;

	return SPOT_FROMTILE(ob->tilex, ob->tiley);
}

static void WL_AI_EnemyDelegateSuspicionLevelChanged(void *obj,
	int prevSuspicionLevel)
{
	objtype *ob;
	WL_AI_Enemy_t *enemy;

	enemy = (WL_AI_Enemy_t *)obj;
	ob = enemy->ob;

	if (prevSuspicionLevel == SUSPICION_PASSIVE)
	{
		FirstSighting(ob);
	}
	else if (prevSuspicionLevel == SUSPICION_ALERTED ||
		prevSuspicionLevel == SUSPICION_ALARMED)
	{
		if (ob->aiEnemy.suspicionLevel == SUSPICION_PASSIVE)
		{
			LoseSighting(ob);
		}
		else
		{
			FirstSighting(ob);
		}
	}
}

static void WL_AI_InitEnemyDelegateAPI(WL_AI_t *ai,
	WL_AI_Enemy_t *enemy)
{
	lwlib_DeclDelegateAPI(AI_Enemy_DelegateAPI_t, &enemy->aiEnemy);

	lwlib_InitDelegate(actorIsPlayer, enemy, 
		WL_AI_EnemyDelegateActorIsPlayer);

	lwlib_InitDelegate(actorDist, enemy,
		WL_AI_EnemyDelegateActorDist);

	lwlib_InitDelegate(actorBrightness, enemy,
		WL_AI_EnemyDelegateActorBrightness);

	lwlib_InitDelegate(rewindActors, enemy,
		WL_AI_EnemyDelegateRewindActors);

	lwlib_InitDelegate(moreActors, enemy,
		WL_AI_EnemyDelegateMoreActors);

	lwlib_InitDelegate(nextActor, enemy,
		WL_AI_EnemyDelegateNextActor);

	lwlib_InitDelegate(actorSpot, enemy,
		WL_AI_EnemyDelegateActorSpot);

	lwlib_InitDelegate(suspicionLevelChanged, enemy,
		WL_AI_EnemyDelegateSuspicionLevelChanged);
}

static AI_Enemy_t *WL_AI_GetEnemyForActor(WL_AI_t *ai, objtype *ob)
{
	WL_AI_Enemy_t *enemy;
	enemy = (WL_AI_Enemy_t *)lwlib_IntMapElem(&ai->enemyMap,
		ob - objlist);
	return &enemy->aiEnemy;
}

static void WL_AI_EnemyLoop(WL_AI_t *ai, objtype *ob, int tics)
{
	AI_Enemy_Loop(WL_AI_GetEnemyForActor(ai, ob), tics);
}

void WL_AI_FreeEnemyMap(WL_AI_t *ai)
{
	WL_AI_Enemy_t *enemy;
	lwlib_IntMapIterDecl(it);

	lwlib_IntMapIterLoop(ai->enemyMap, it)
	{
		enemy = (WL_AI_Enemy_t *)it->val;
		free(enemy);
	}
	lwlib_IntMapClear(&ai->enemyMap);
}

void WL_AI_InitEnemyMap(WL_AI_t *ai)
{
	objtype *ob;
	WL_AI_Enemy_t *enemy;

	WL_AI_FreeEnemyMap(ai);

	for (ob = player; ob != NULL; ob = ob->next)
	{
		assert(ob->id > 0);
		if (ob->isAiEnemy)
		{
			enemy = lwlib_CallocSingle(WL_AI_Enemy_t);
			enemy->ob = ob;
			WL_AI_InitEnemyDelegateAPI(ai, enemy);
			enemy->aiEnemy.suspicionMeter = &ob->aiEnemy.suspicionMeter;
			enemy->aiEnemy.suspicionLevel = &ob->aiEnemy.suspicionLevel;
			enemy->aiEnemy.killed = &ob->aiEnemy.killed;
			enemy->aiEnemy.suspicionSpot = &ob->aiEnemy.suspicionSpot;
			enemy->aiEnemy.suspicionTargetIsPlayer = 
				&ob->aiEnemy.suspicionTargetIsPlayer;
			lwlib_IntMapInsert(&ai->enemyMap, ob->id - 1,
				(lwlib_Word_t)enemy);
		}
	}
}

void WL_AI_Update(WL_AI_t *ai)
{
	WL_AI_Enemy_t *enemy;
	lwlib_IntMapIterDecl(it);

	lwlib_IntMapIterLoop(ai->enemyMap, it)
	{
		enemy = (WL_AI_Enemy_t *)it->val;
		WL_AI_EnemyLoop(ai, enemy->ob, tics);
	}
}

void WL_AI_DoDamage(WL_AI_t *ai, objtype *ob, int damage, int attackerSpot)
{
	AI_Enemy_DoDamage(WL_AI_GetEnemyForActor(ai, ob), damage,
		attackerSpot);
}

void WL_AI_DoKill(WL_AI_t *ai, objtype *ob)
{
	AI_Enemy_DoKill(WL_AI_GetEnemyForActor(ai, ob));
}

void WL_AI_MakeNoise(WL_AI_t *ai, double loudness, int noiseSpot, 
	int noiseFlags)
{
	int dist;
	int noiseTileX, noiseTileY;
	objtype *ob;
	WL_AI_Enemy_t *enemy;
	lwlib_IntMapIterDecl(it);
	double enemyLoudness;
	bool capEnable;

	capEnable = (noiseFlags & suspicionNotCapped) ? 
		false : true;

	lwlib_IntMapIterLoop(ai->enemyMap, it)
	{
		enemy = (WL_AI_Enemy_t *)it->val;
		ob = enemy->ob;

		if (areabyplayer[ob->areanumber])
		{
			if (noiseFlags & distanceFalloff)
			{
				noiseTileX = SPOT_TOTILEX(noiseSpot);
				noiseTileY = SPOT_TOTILEY(noiseSpot);
				dist = TILEDIST(noiseTileX, noiseTileY, ob->tilex, ob->tiley);

				enemyLoudness = loudness / dist;
			}
			else
			{
				enemyLoudness = loudness;
			}

			AI_Enemy_DoNoise(WL_AI_GetEnemyForActor(ai, ob), 
				enemyLoudness, noiseSpot, capEnable);
		}
	}
}

bool WL_AI_IsSuspicious(WL_AI_t *ai, objtype *ob)
{
	return (ob->isAiEnemy && 
		ob->aiEnemy.suspicionLevel == SUSPICION_ALERTED);
}

bool WL_AI_SuspicionTargetIsPlayer(WL_AI_t *ai, objtype *ob)
{
	return ob->aiEnemy.suspicionTargetIsPlayer;
}
#endif