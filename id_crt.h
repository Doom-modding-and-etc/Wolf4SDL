// File: id_crt.h
// Project: Wolf4SDL
// Author: André Guilherme 
// Original author: Fabien sanglard 
// Creation date: 2022-07-11 
// Original Creation date: 2014-08-26.
// Description: 
// Header file for the whole contents
// Note: This file is heavly modified on: 
// https://github.com/fabiensanglard/Chocolate-Wolfenstein-3D/blob/master/crt.h

#ifndef id_crt_h
#define id_crt_h

#include <SDL.h>
extern SDL_Color curpal[256];
/*
 * CRT aspect ratio is 4:3, height will be infered.
 */
void CRT_Init(int width);
/*
 *   Trigger the Digital To Analogic convertion
 */
void CRT_DAC(void);
void CRT_Screenshot(void);

void CRT_FreeScreenshot(SDL_Surface *surface1, SDL_Surface *surface2);

#endif