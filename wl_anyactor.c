// WL_ANYACTOR.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_ACTOR)
#include "wl_shade.h"

static AnyActor_t m_table[] =
{
    {
        NULL, // actorStates
        NULL, // actorDeathScream
        NULL, // actorLaunch
        NULL, // actorDoAttackState
        NULL, // actorIsBetterShot
        NULL, // actorPlayShootSound
        NULL, // actorBiteTakeDamage
        NULL, // actorDoShockState
        NULL, // actorSpawn
        NULL, // actorKill
        NULL, // actorDamageActor
        NULL, // actorDoPainState
        NULL, // actorDoChaseState
        NULL, // actorReactionTime
        NULL, // actorNewHealthBar
        NULL, // actorLogFileAlias
        0, // actorSprOffset
        false, // actorIsHobo
        nothing, // actorBaseObClass
    }, // ANYACTOR_TILE_NONE
};

static unsigned char m_tileToTableIndex[ANYACTOR_MAXTILE];

void AnyActor_Init(void)
{
    int i;

    i = 0;
    #define NEXT(tile) \
        m_tileToTableIndex[tile] = i++;
    NEXT(ANYACTOR_TILE_NONE);
    #undef NEXT
}

boolean AnyActor_IsReg(int tile)
{
    return tile > 0 && tile < ANYACTOR_MAXTILE &&
        m_tileToTableIndex[tile] != 0;
}

AnyActor_t *AnyActor_Get(int tile)
{
    return AnyActor_IsReg(tile) ?
        &m_table[m_tileToTableIndex[tile]] : 
        &m_table[0];
}
#endif