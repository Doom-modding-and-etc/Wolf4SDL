// WL_PHYSICS.C

#include <stdio.h>
#include <math.h>
#include "wl_def.h"
#if defined(LWLIB) && defined(WOLFRAD) && defined(USE_PHYSICS)
Phys_Vec3_t Phys_Vec3(fixed x, fixed y, fixed z)
{
    Phys_Vec3_t vec3;
    vec3.v[0] = x;
    vec3.v[1] = y;
    vec3.v[2] = z;
    return vec3;
}

Phys_Vec3_t Phys_Vec3Zero(void)
{
    Phys_Vec3_t vec3;
    memset(&vec3, 0, sizeof(vec3));
    return vec3;
}

Phys_Vec3_t Phys_Vec3Add(Phys_Vec3_t a, Phys_Vec3_t b)
{
    int i;
    Phys_Vec3_t c;
    
    for (i = 0; i < 3; i++)
    {
        c.v[i] = a.v[i] + b.v[i];
    }

    return c;
}

Phys_Vec3_t Phys_Vec3Rot2D(Phys_Vec3_t pt, fixed angle)
{
    // x' = x cos(t) - y sin(t)
    // y' = x sin(t) + y cos(t)
    fixed cost, sint;
    cost = costable[angle];
    sint = sintable[angle];
    return Phys_Vec3(
        fixedpt_mul(pt.v[0], cost) - fixedpt_mul(pt.v[1], sint),
        fixedpt_mul(pt.v[0], sint) + fixedpt_mul(pt.v[1], cost),
        pt.v[2]
        );
}

void Phys_ParticleMove(Phys_Particle_t *particle)
{
    int i;
    fixed t, a;

    t = TICS2SECFP(tics);

    for (i = 0; i < 3; i++)
    {
        particle->pos.v[i] += fixedpt_mul(particle->vel.v[i], t);

        a = fixedpt_mul(particle->force.v[i],
            particle->inv_mass);
        particle->vel.v[i] += fixedpt_mul(a, t);
    }

    if (particle->rejitter)
    {
        for (i = 0; i < 3; i++)
        {
            particle->jitterPos.v[i] = 
                (
                    US_RndT() * 
                    (particle->jitterAmplitude.v[i] * 2) / 255
                ) -
                particle->jitterAmplitude.v[i];
        }

        particle->rejitter = false;
    }
}

void Phys_ParticleReset(Phys_Particle_t *particle)
{
    memset(particle, 0, sizeof(Phys_Particle_t));
}

static void Phys_ParticleProcessSpringForce(
    Phys_Particle_t *particle, Phys_ForceGenerator_t forceGenerator)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        particle->force.v[i] += 
            fixedpt_mul(particle->pos.v[i], -forceGenerator.spring.constant.v[i]);
    }
}

static void Phys_ParticleProcessDampingForce(
    Phys_Particle_t *particle, Phys_ForceGenerator_t forceGenerator)
{
    int i;

    for (i = 0; i < 3; i++)
    {
        particle->force.v[i] += 
            fixedpt_mul(particle->vel.v[i], -forceGenerator.damping.constant.v[i]);
    }
}

static void Phys_ParticleProcessJitterForce(
    Phys_Particle_t *particle, Phys_ForceGenerator_t forceGenerator)
{
    int i;
    int idleTics, totalTics;

    totalTics = 0;
    idleTics = forceGenerator.jitter.idleTics;

    particle->jitterTics += tics;
    while (particle->jitterTics >= idleTics)
    {
        totalTics += idleTics;
        particle->jitterTics -= idleTics;
    }

    if (totalTics > 0)
    {
        for (i = 0; i < 3; i++)
        {
            particle->jitterAmplitude.v[i] -= 
                fixedpt_mul(
                    forceGenerator.jitter.amplitudeFallRate.v[i],
                    TICS2SECFP(totalTics)
                    );
            if (particle->jitterAmplitude.v[i] < 0)
            {
                particle->jitterAmplitude.v[i] = 0;
            }
        }

        particle->rejitter = true;
    }
}

static void Phys_ParticleProcessConstantForce(
    Phys_Particle_t *particle, Phys_ForceGenerator_t forceGenerator)
{
    particle->force = Phys_Vec3Add(particle->force,
        forceGenerator.constant.force);
}

static void Phys_ParticleProcessForceGenerator(Phys_Particle_t *particle,
    Phys_ForceGenerator_t forceGenerator)
{
    switch (forceGenerator.id)
    {
    case PHYS_FORCEGENID_SPRING:
        Phys_ParticleProcessSpringForce(particle, 
            forceGenerator);
        break;
    case PHYS_FORCEGENID_DAMPING:
        Phys_ParticleProcessDampingForce(particle, 
            forceGenerator);
        break;
    case PHYS_FORCEGENID_JITTER:
        Phys_ParticleProcessJitterForce(particle, 
            forceGenerator);
        break;
    case PHYS_FORCEGENID_CONSTANT:
        Phys_ParticleProcessConstantForce(particle, 
            forceGenerator);
        break;
    }
}

void Phys_ParticleUpdateForce(Phys_Particle_t *particle)
{
    int i;

    particle->force = Phys_Vec3Zero();

    for (i = 0; i < particle->numForceGenerators; i++)
    {
        Phys_ParticleProcessForceGenerator(particle,
            particle->forceGenerators[i]);
    }
}
#endif