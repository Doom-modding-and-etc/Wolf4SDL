/*
**	ID Engine
**	ID_HAPTIC.h - Header file for Input Manager
**	v2.3
**	By André Guilherme & Andrey Kuleshov
** 
**	This module handles dealing with the various haptic input devices
**	it depends of: Input Manager(aka: ID_IN.h)
*/

#include "wl_def.h"
#include "id_haptic.h"

#if defined(SDL_MAJOR_VERSION) && (SDL_MAJOR_VERSION == 2) && defined(HAPTIC_SUPPORT)

boolean hapticEnabled = false;
static SDL_Haptic* haptic = NULL;
static SDL_HapticEffect strongEffect;
static SDL_HapticEffect weakEffect;
static int hapticEffect = -1;
static const Uint32 strongLength = 300;
static const Uint32 weakLength = 150;
#define SDL_HAPTIC_MAX_MAGNITUDE 0x7FFF
extern SDL_Joystick* Joystick;

void HAPTIC_Startup(void)
{
    haptic = SDL_HapticOpenFromJoystick(Joystick);
    if (haptic) {
        unsigned int flags = SDL_HapticQuery(haptic);
        if ((flags & SDL_HAPTIC_LEFTRIGHT) == 0) {
            fprintf(stderr, "Only XInput force feedback is supported!\n");
            SDL_HapticClose(haptic);
            haptic = NULL;
        }
        strongEffect.leftright.type = SDL_HAPTIC_LEFTRIGHT;
        strongEffect.leftright.length = strongLength;
        strongEffect.leftright.large_magnitude = SDL_HAPTIC_MAX_MAGNITUDE;
        strongEffect.leftright.small_magnitude = 0;
        weakEffect.leftright.type = SDL_HAPTIC_LEFTRIGHT;
        weakEffect.leftright.length = weakLength;
        weakEffect.leftright.large_magnitude = 0;
        weakEffect.leftright.small_magnitude = SDL_HAPTIC_MAX_MAGNITUDE;
        hapticEnabled = true;
    }
    else 
    {
        fprintf(stderr, "Haptic not found\n");
    }
}

void HAPTIC_Shutdown(void)
{
    SDL_HapticClose(haptic);
}

boolean HAPTIC_Present(void)
{
    return haptic != NULL;
}

static void HAPTIC_RunEffect(SDL_HapticEffect* effect)
{
    if (!haptic || !hapticEnabled)
        return;
    if (hapticEffect >= 0)
        SDL_HapticDestroyEffect(haptic, hapticEffect);
    hapticEffect = SDL_HapticNewEffect(haptic, effect);
    SDL_HapticRunEffect(haptic, hapticEffect, 1);
}

void HAPTIC_WeakRumble(void)
{
    HAPTIC_RunEffect(&weakEffect);
}

void HAPTIC_StrongRumble(void)
{
    HAPTIC_RunEffect(&strongEffect);
}

void HAPTIC_StopRumble(void)
{
    if (!haptic)
        return;
    if (hapticEffect >= 0)
        SDL_HapticDestroyEffect(haptic, hapticEffect);
    hapticEffect = -1;
}

#endif