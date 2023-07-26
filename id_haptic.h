/*
**	ID Engine
**	ID_HAPTIC.h - Header file for Input Manager
**	v2.3
**	By André Guilherme & Andrey Kuleshov
*/


#ifndef ID_HAPTIC_H
#define ID_HAPTIC_H

#if defined(SDL_MAJOR_VERSION) && (SDL_MAJOR_VERSION == 2) || (SDL_MAJOR_VERSION == 3) && defined(HAPTIC_SUPPORT)

extern boolean hapticEnabled;

void HAPTIC_Startup(void);
void HAPTIC_Shutdown(void);
boolean HAPTIC_Present(void);
void HAPTIC_WeakRumble(void);
void HAPTIC_StrongRumble(void);
void HAPTIC_StopRumble(void);

#endif

#endif