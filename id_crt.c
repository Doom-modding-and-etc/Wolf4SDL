// File: id_crt.c
// Project: Wolf4SDL
// Author: André Guilherme
// Creation date: 2022-07-11 
// Description: 
// This file fixes the crt bug
// using 4:3 and Makes support of screenshot.
// Note: This file is heavyly modified of the following repo: 
// https://github.com/fabiensanglard/Chocolate-Wolfenstein-3D/blob/master/crt.h
// Credits for the following pepole: Fabien sanglard and zZeck
// and the original file creation date: 2014-08-26.


#include "wl_def.h"
#ifdef CRT
#include <SDL.h>

// Win32
#if SDL_MAJOR_VERSION == 1
#if defined(XBOX)
#include <fakeglx09.h>
#elif defined(_MSC_VER)
#include <Windows.h>
#include <GL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#else
#include <SDL_opengl.h>
#endif
#endif

#include "id_crt.h"

static int width;
static int height;

unsigned char coloredFrameBuffer[320 * 200 * 3];

#if SDL_MAJOR_VERSION == 1
unsigned int crtTexture;
#endif

void CRT_Init(int _width) 
{
    width = _width;
    height = _width * 3 / 4;

#if SDL_MAJOR_VERSION == 1  
    //Alloc the OpenGL texture where the screen will be uploaded each frame.
    glGenTextures(1, &crtTexture);
    glBindTexture(GL_TEXTURE_2D, crtTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,         // target
        0,                     // level, 0 = base, no minimap,
        GL_RGB,                // internalformat
        screenWidth,                   // width
        screenHeight,                   // height
        0,                     // border, always 0 in OpenGL ES
        GL_RGB,                // format
        GL_UNSIGNED_BYTE,      // type
        0
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapBuffers(); 
#elif SDL_MAJOR_VERSION == 2
    texture = SDL_CreateTexture(renderer, 0, 0, width, height);
    SDL_GetTextureColorMod(texture, (unsigned char *)0xFF, (unsigned char *)0xFF, (unsigned char *)0xFF);
    SDL_UpdateTexture(texture, NULL, screen->pixels, screenWidth * sizeof(Uint32));
#ifdef SCALE2X
    // Render the intermediate texture into the up-scaled texture using 'nearest' integer scaling.
    SDL_SetRenderTarget(renderer, upscaledTexture);
#endif    
    SDL_RenderCopy(renderer, texture, NULL, NULL);
#ifdef SCALE2X
    // Finally render this up-scaled texture to the window using linear scaling.
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, upscaledTexture, NULL, NULL);
#endif
    SDL_RenderPresent(renderer);
#endif
}

void CRT_DAC(void) 
{
#if SDL_MAJOR_VERSION == 1
    // Grab the screen framebuffer from SDL
    int i;
    //Convert palette based framebuffer to RGB for OpenGL
    unsigned char* pixelPointer = coloredFrameBuffer;
    
    screen = screenBuffer;

    for (i = 0; i < 320 * 200; i++) {
        unsigned char paletteIndex;
        paletteIndex = ((unsigned char*)screen->pixels)[i];
        *pixelPointer++ = gamepal[paletteIndex].r;
        *pixelPointer++ = gamepal[paletteIndex].g;
        *pixelPointer++ = gamepal[paletteIndex].b;
    }

    //Upload texture
    glBindTexture(GL_TEXTURE_2D, crtTexture);
    glTexSubImage2D(GL_TEXTURE_2D,
        0,
        0,
        0,
        screenWidth,
        screenHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        coloredFrameBuffer);

    //Draw a quad with the texture
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3i(0, 0, 0);
    glTexCoord2f(0, 0); glVertex3i(0, height, 0);
    glTexCoord2f(1, 0); glVertex3i(width, height, 0);
    glTexCoord2f(1, 1); glVertex3i(width, 0, 0);
    glEnd();

    //Flip buffer
    SDL_GL_SwapBuffers();
#elif SDL_MAJOR_VERSION == 2
    texture = SDL_CreateTextureFromSurface(renderer, screenBuffer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_GetKeyboardState(NULL);
#endif
}

void CRT_FreeScreenshot(SDL_Surface* surface1, SDL_Surface *surface2)
{
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
}

#if SDL_MAJOR_VERSION == 2
void CRT_DestroyTexture(SDL_Texture* texture1, SDL_Texture* texture2) 
{
    SDL_DestroyTexture(texture);
#ifdef SCALE_2X
    SDL_DestroyTexture(upscaledTexture);
#endif
}
#endif

void CRT_BlitImage(SDL_Surface* img1src, SDL_Surface* img1dst, SDL_Surface* img2src, SDL_Surface* img2dst)
{
    SDL_BlitSurface(img1src, NULL, img1dst, NULL);
    SDL_BlitSurface(img2src, NULL, img2dst, NULL);
}

void CRT_Screenshot(void) 
{
    const char* filename = "screenshot.bmp";
    int aspectWidth = 640;
    int aspectHeight = 440;
    SDL_Surface* correctAspect;
    SDL_Surface* incorrectAspect;

    printf("Screenshot.\n");


    correctAspect = SDL_CreateRGBSurface(0, aspectWidth, aspectHeight, 32, 0, 0, 0, 0);
    incorrectAspect = SDL_CreateRGBSurface(0, screenBuffer->w, screenBuffer->h, 32, 0, 0, 0, 0);
    CRT_BlitImage(screenBuffer, incorrectAspect, incorrectAspect, correctAspect);
    SDL_SaveBMP(correctAspect, filename);

    CRT_FreeScreenshot(correctAspect, incorrectAspect);
}
#endif
