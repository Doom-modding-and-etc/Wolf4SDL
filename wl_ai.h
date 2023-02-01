#include "version.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_AI)
typedef enum 
{
    distanceFalloff = 0x01,
    suspicionNotCapped = 0x02,
}WL_AI_NoiseFlags;

extern const double gunLoudness;

typedef struct WL_AI_s
{
    lwlib_IntMap_t enemyMap;
} WL_AI_t;

extern WL_AI_t ai;

void WL_AI_InitEnemyMap(WL_AI_t* ai);

void WL_AI_FreeEnemyMap(WL_AI_t* ai);

void WL_AI_Update(WL_AI_t* ai);

void WL_AI_DoDamage(WL_AI_t* ai, objtype* ob, int damage,
    int attackerSpot);

void WL_AI_DoKill(WL_AI_t* ai, objtype* ob);

void WL_AI_MakeNoise(WL_AI_t* ai, double loudness, int noiseSpot,
    int noiseFlags);

boolean WL_AI_IsSuspicious(WL_AI_t* ai, objtype* ob);

boolean WL_AI_SuspicionTargetIsPlayer(WL_AI_t* ai, objtype* ob);
#endif