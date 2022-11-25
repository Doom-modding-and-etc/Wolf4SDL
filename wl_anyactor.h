#include "version.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_ACTOR)
#define ANYACTOR(tile) (*AnyActor_Get(tile))
#define ANYACTOR_ISA(ob, code) (ob->tile == code)
#define ANYACTOR_MAXTILE 2048

#define ANYACTOR_SWITCH_HOOK(obc, obt, fn, ...) \
    obclass = obc; \
    if (ANYACTOR(obt).fn != NULL) \
    { \
        ANYACTOR(obt).fn(__VA_ARGS__); \
    } \
    else \
    { \
        if (ANYACTOR(obt).actorBaseObClass != nothing) \
        { \
            obclass = ANYACTOR(obt).actorBaseObClass; \
        } \
    } \
    switch (obclass)

typedef enum AnyActor_Tile_s
{
    ANYACTOR_TILE_NONE = 0,
} AnyActor_Tile_t;

typedef struct AnyActor_s
{
    statetype* (*actorStates)(void);
    void (*actorDeathScream)(objtype* ob);
    void (*actorLaunch)(objtype* ob, objtype* proj);
    void (*actorDoAttackState)(objtype* ob);
    bool (*actorIsBetterShot)(objtype* ob);
    void (*actorPlayShootSound)(objtype* ob);
    void (*actorBiteTakeDamage)(objtype* ob);
    void (*actorDoShockState)(objtype* ob);
    void (*actorSpawn)(int x, int y);
    void (*actorKill)(objtype* ob);
    bool (*actorDamageActor)(objtype* ob, int damage, int dmgtype);
    void (*actorDoPainState)(objtype* ob);
    void (*actorDoChaseState)(objtype* ob);
    void (*actorReactionTime)(objtype* ob);
    void (*actorNewHealthBar)(objtype* ob);
    const char* actorLogFileAlias;
    int actorSprOffset;
    bool actorIsHobo;
    classtype actorBaseObClass;
} AnyActor_t;

extern void AnyActor_Init(void);

extern bool AnyActor_IsReg(int tile);

extern AnyActor_t* AnyActor_Get(int tile);

extern void AnyActor_ReactionTime(objtype* ob);
#endif