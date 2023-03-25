// File: id_crt.h
// Project: Wolf4SDL
// Original author: Fabien sanglard 
// Modificaton author: André Guilherme 
// Original Creation date: 2014-08-26.
// Description: 
// Header file for the whole contents
// Note: This file is heavly modified on: 
// https://github.com/fabiensanglard/Chocolate-Wolfenstein-3D/blob/master/crt.h

#ifndef ID_CRT_H
#define ID_CRT_H
#include "version.h"

#ifdef CRT
#include <SDL.h>
// Win32
#if SDL_MAJOR_VERSION == 1
#if defined(_XBOX)
#include "xbox/fakeglx.h"
#elif defined(_WIN32)
#include <Windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#else
#include <SDL_opengl.h>
#endif
#endif

extern SDL_Color curpal[256];
/*
 * CRT aspect ratio is 4:3, height will be infered.
 */
void CRT_Init(int _width); //TODO: Add height support.
/*
 *   Trigger the Digital To Analogic convertion
 */
void CRT_DAC(void);

void CRT_Screenshot(void);

void CRT_FreeScreenshot(SDL_Surface *surface1, SDL_Surface *surface2);

#if !SDL_MAJOR_VERSION == 1 || SDL_MAJOR_VERSION == 2 || SDL_MAJOR_VERSION == 3
void CRT_DestroyTexture(SDL_Texture* texture1, SDL_Texture* texture2);
#endif

void CRT_BlitImage(SDL_Surface* img1src, SDL_Surface* img1dst, SDL_Surface* img2src, SDL_Surface* img2dst);

#endif
#endif

