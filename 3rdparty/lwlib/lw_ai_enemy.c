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

#include "ai_enemy.h"
#include "lw_vec.h"

#define STATE *(*enemy)

// suspicion meter increase per unit brightness per unit time
#define SUSPICION_METER_ROI 0.1

// suspicion meter decrease per unit time
#define SUSPICION_METER_ROD 0.006

// max suspicion meter before suspicion level turn over
#define SUSPICION_METER_MAX 1.0

// distance is multiplied by this constant and maximum brightness
#define SUSPICION_BRIGHTNESS_FACTOR 0.075

#define SUSPICION_CDATA (*AI_Enemy_SuspicionConstData(enemy))

#define MAX_BRIGHTNESS 1.0

#define DAMAGE_SUSPICION_FACTOR 0.05

#define NOISE_SUSPICION_FACTOR 1.0
#define AUDIBLE_NOISE_LOUDNESS 0.1

#define deln lwlib_CallDelegateNoArgs
#define del lwlib_CallDelegate

typedef struct SuspicionConstData_s
{
	int level;
	double roi;
	double rod;
	double damageSuspicionFactor;
	double noiseSuspicionFactor;
} SuspicionConstData_t;

static const int passive = AI_ENEMY_SUSPICIONLEVEL_PASSIVE;
static const int alerted = AI_ENEMY_SUSPICIONLEVEL_ALERTED;
static const int alarmed = AI_ENEMY_SUSPICIONLEVEL_ALARMED;

static const SuspicionConstData_t suspicionConstData[] =
{
	{
		passive, // level
		SUSPICION_METER_ROI, // roi
		SUSPICION_METER_ROD / 4, // rod
		DAMAGE_SUSPICION_FACTOR, // damageSuspicionFactor
		NOISE_SUSPICION_FACTOR, // noiseSuspicionFactor
	}, // AI_ENEMY_SUSPICIONLEVEL_PASSIVE
	{
		alerted, // level
		SUSPICION_METER_ROI, // roi
		SUSPICION_METER_ROD / 8, // rod
		DAMAGE_SUSPICION_FACTOR, // damageSuspicionFactor
		NOISE_SUSPICION_FACTOR, // noiseSuspicionFactor
	}, // AI_ENEMY_SUSPICIONLEVEL_ALERTED
	{
		alarmed, // level
		SUSPICION_METER_ROI, // roi
		SUSPICION_METER_ROD / 2, // rod
		DAMAGE_SUSPICION_FACTOR, // damageSuspicionFactor
		NOISE_SUSPICION_FACTOR, // noiseSuspicionFactor
	}, // AI_ENEMY_SUSPICIONLEVEL_ALARMED
};

static const SuspicionConstData_t *AI_Enemy_SuspicionConstData(
	AI_Enemy_t *enemy)
{
	const int suspicionLevel = STATE.suspicionLevel;
	assert(suspicionLevel >= 0 && suspicionLevel < 
		lwlib_CountArray(suspicionConstData));
	return &suspicionConstData[suspicionLevel];
}

static void AI_Enemy_SetSuspicionSpot(AI_Enemy_t *enemy,
	int suspicionSpot, bool isPlayer)
{
	if (STATE.suspicionSpot && STATE.suspicionTargetIsPlayer &&
		!isPlayer)
	{
		return;
	}
	STATE.suspicionSpot = suspicionSpot;
	STATE.suspicionTargetIsPlayer = isPlayer;
}

static void AI_Enemy_SetSuspicionLevel(AI_Enemy_t *enemy,
	int suspicionLevel)
{
	int prevSuspicionLevel;
	lwlib_DeclDelegateAPI(AI_Enemy_DelegateAPI_t, enemy);

	if (suspicionLevel != STATE.suspicionLevel)
	{
		prevSuspicionLevel = STATE.suspicionLevel;
		STATE.suspicionLevel = suspicionLevel;
		del(suspicionLevelChanged, prevSuspicionLevel);
	}

	if (STATE.suspicionLevel != alerted)
	{
		AI_Enemy_SetSuspicionSpot(enemy, 0, true);
	}
}

static double AI_Enemy_SuspiciousBrightness(AI_Enemy_t *enemy,
	int dist)
{
	double b;
	b = dist * MAX_BRIGHTNESS * SUSPICION_BRIGHTNESS_FACTOR;
	b = lwlib_MIN(b, MAX_BRIGHTNESS);
	return b;
}

static void AI_Enemy_ContribSuspicion(AI_Enemy_t *enemy,
	double suspicionIncrease, bool capEnable)
{
	bool cappedSuspicion;
	double suspicionDecrease, suspicionLeft;

	if (suspicionIncrease > 0)
	{
		while (STATE.suspicionMeter + suspicionIncrease > 
			SUSPICION_METER_MAX)
		{
			suspicionLeft = SUSPICION_METER_MAX -
				STATE.suspicionMeter;

			cappedSuspicion =
				(
					STATE.suspicionLevel == alarmed ||
					(
						STATE.suspicionLevel == alerted &&
						capEnable
					)
				);
			if (cappedSuspicion)
			{
				suspicionIncrease = suspicionLeft;
				break;
			}

			suspicionIncrease -= suspicionLeft;
			AI_Enemy_SetSuspicionLevel(enemy, 
				STATE.suspicionLevel + 1);
			STATE.suspicionMeter = 0;
		}

		STATE.suspicionMeter += suspicionIncrease;
	}
	else if (suspicionIncrease < 0)
	{
		suspicionDecrease = -suspicionIncrease;

		while (STATE.suspicionMeter - suspicionDecrease < 0)
		{
			if (STATE.suspicionLevel == passive)
			{
				suspicionDecrease = STATE.suspicionMeter;
				break;
			}

			suspicionDecrease -= STATE.suspicionMeter;
			AI_Enemy_SetSuspicionLevel(enemy, 
				STATE.suspicionLevel - 1);
			STATE.suspicionMeter = SUSPICION_METER_MAX;
		}

		STATE.suspicionMeter -= suspicionDecrease;
	}
}

static bool AI_Enemy_CheckActors(AI_Enemy_t *enemy, int tics)
{
	int dist;
	bool foundThreat;
	double brightness, suspiciousBrightness;
	double suspicionIncrease;
	lwlib_DeclDelegateAPI(AI_Enemy_DelegateAPI_t, enemy);

	deln(rewindActors);
	foundThreat = false;

	while (deln(moreActors))
	{
		dist = deln(actorDist);

		brightness = deln(actorBrightness);
		suspiciousBrightness = AI_Enemy_SuspiciousBrightness(enemy,
			dist);

		if (brightness >= suspiciousBrightness)
		{
			AI_Enemy_SetSuspicionSpot(enemy, deln(actorSpot),
				deln(actorIsPlayer));

			suspicionIncrease = brightness * tics *
				SUSPICION_CDATA.roi / dist;

			AI_Enemy_ContribSuspicion(enemy, suspicionIncrease,
				!deln(actorIsPlayer));

			foundThreat = true;
		}

		deln(nextActor);
	}

	return foundThreat;
}

static void AI_Enemy_DoSuspicionDrop(AI_Enemy_t *enemy, int tics)
{
	double suspicionDecrease;
	suspicionDecrease = tics * SUSPICION_CDATA.rod;
	AI_Enemy_ContribSuspicion(enemy, -suspicionDecrease, true);
}

void AI_Enemy_Loop(AI_Enemy_t *enemy, int tics)
{
	lwlib_DeclDelegateAPI(AI_Enemy_DelegateAPI_t, enemy);

	if (STATE.killed)
	{
		return;
	}

	if (!AI_Enemy_CheckActors(enemy, tics))
	{
		AI_Enemy_DoSuspicionDrop(enemy, tics);
	}

	#if defined(AI_DEBUGGING)
	if (STATE.suspicionMeter != 0.0)
	{
		lwlib_LogPrintf("XXX: meter=%.3f\n",
			STATE.suspicionLevel + 
			(STATE.suspicionMeter / SUSPICION_METER_MAX));
	}
	#endif
}

void AI_Enemy_DoDamage(AI_Enemy_t *enemy, int damage, int attackerSpot)
{
	double suspicionIncrease;

	if (STATE.suspicionLevel == passive)
	{
		AI_Enemy_SetSuspicionLevel(enemy, alerted);
	}

	suspicionIncrease = SUSPICION_CDATA.damageSuspicionFactor * damage;
	AI_Enemy_ContribSuspicion(enemy, suspicionIncrease, true);

	AI_Enemy_SetSuspicionSpot(enemy, attackerSpot, false);
}

void AI_Enemy_DoNoise(AI_Enemy_t *enemy, double loudness, int noiseSpot,
	bool capEnable)
{
	double suspicionIncrease;

	if (loudness < AUDIBLE_NOISE_LOUDNESS)
	{
		return;
	}

	if (STATE.suspicionLevel == passive)
	{
		AI_Enemy_SetSuspicionLevel(enemy, alerted);
	}

	suspicionIncrease = SUSPICION_CDATA.noiseSuspicionFactor * loudness;
	AI_Enemy_ContribSuspicion(enemy, suspicionIncrease, capEnable);

	AI_Enemy_SetSuspicionSpot(enemy, noiseSpot, false);
}

void AI_Enemy_DoKill(AI_Enemy_t *enemy)
{
	STATE.killed = true;
}
