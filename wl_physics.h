#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_PHYSICS)
#define PHYS_MAX_FORCEGENS 5

typedef struct Phys_Vec3_s
{
    fixed v[3];
} Phys_Vec3_t;

typedef enum Phys_ForceGeneratorId_e
{
    PHYS_FORCEGENID_SPRING,
    PHYS_FORCEGENID_DAMPING,
    PHYS_FORCEGENID_JITTER,
    PHYS_FORCEGENID_CONSTANT,
} Phys_ForceGeneratorId_t;

typedef struct Phys_ForceGenerator_s
{
    Phys_ForceGeneratorId_t id;
    struct
    {
        Phys_Vec3_t constant;
    } spring;
    struct
    {
        Phys_Vec3_t constant;
    } damping;
    struct
    {
        Phys_Vec3_t initialAmplitude;
        int32_t idleTics;
        Phys_Vec3_t amplitudeFallRate;
    } jitter;
    struct
    {
        Phys_Vec3_t force;
    } constant;
} Phys_ForceGenerator_t;

typedef struct Phys_Particle_s
{
    Phys_Vec3_t pos;
    Phys_Vec3_t vel;
    fixed inv_mass;
    Phys_Vec3_t force;
    boolean rejitter;
    Phys_Vec3_t jitterPos;
    Phys_Vec3_t jitterAmplitude;
    int32_t jitterTics;
    Phys_ForceGenerator_t forceGenerators[PHYS_MAX_FORCEGENS];
    int numForceGenerators;
} Phys_Particle_t;

extern Phys_Vec3_t Phys_Vec3(fixed x, fixed y, fixed z);

extern Phys_Vec3_t Phys_Vec3Zero(void);

extern Phys_Vec3_t Phys_Vec3Add(Phys_Vec3_t a, Phys_Vec3_t b);

extern Phys_Vec3_t Phys_Vec3Rot2D(Phys_Vec3_t pt, fixed angle);

extern void Phys_ParticleMove(Phys_Particle_t* particle);

extern void Phys_ParticleReset(Phys_Particle_t* particle);

extern void Phys_ParticleUpdateForce(Phys_Particle_t* particle);
#endif