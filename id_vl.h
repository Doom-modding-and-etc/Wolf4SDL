/* ID_VL.H */

#ifndef __ID_VL_H_
#define __ID_VL_H_

/* wolf compatbility */
#include "wl_def.h"
void Quit (const char *error,...);

/* =========================================================================== */

#define CHARWIDTH		2
#define TILEWIDTH		4

/* =========================================================================== */

extern SDL_Surface *screen, *screenBuffer;
#ifdef SAVE_GAME_SCREENSHOT
extern SDL_Surface* lastGameSurface;
#endif
#if SDL_MAJOR_VERSION == 2
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
#if defined(SCALE2X)
extern SDL_Texture* upscaledTexture;
#endif
#endif

#ifndef SEGA_SATURN 
extern  boolean fullscreen, usedoublebuffering;
#endif

extern unsigned int screenWidth, screenHeight, screenBits,
#ifndef SEGA_SATURN
screenPitch, bufferPitch;   
#endif
extern  int      scaleFactor;

extern	boolean  screenfaded;
#ifndef SEGA_SATURN
extern	unsigned bordercolor;
#endif

extern  unsigned int *ylookup;

extern SDL_Color gamepal[256];

/* =========================================================================== */

/*
** VGA hardware routines
*/

#ifdef SEGA_SATURN
#define VL_WaitVBL(a)	wait_vblank(a*1)  /* SDL_Delay((a)*8) */
#else
#define VL_WaitVBL(a)        SDL_Delay(((unsigned int)a)*8)
#endif
#define VL_ClearScreen(c)    SDL_FillRect(screenBuffer,NULL,(c))


void VL_DePlaneVGA (unsigned char *source, int width, int height);
void VL_SetVGAPlaneMode (void);

void VL_Startup();
void VL_UpdateScreen(SDL_Surface* surface);
#if SDL_MAJOR_VERSION == 2
void VL_RenderTextures();
#endif

void	VL_MeasurePropString(const char* string, unsigned short* width, unsigned short* height);

boolean FizzleFade(SDL_Surface* source, int x1, int y1,
	unsigned int width, unsigned int height, unsigned int frames, boolean abortable);

void VL_Shutdown (void);

void VL_ConvertPalette(unsigned char *srcpal, SDL_Color *destpal, int numColors);
void VL_FillPalette (unsigned char red, unsigned char green, unsigned char blue);
void VL_SetPalette  (SDL_Color *palette, boolean forceupdate);
void VL_GetPalette  (SDL_Color *palette);
void VL_FadeOut     (int start, int end, unsigned char red, unsigned char green, unsigned char blue, int steps);
void VL_FadeIn      (int start, int end, SDL_Color *palette, int steps);

unsigned char *VL_LockSurface(SDL_Surface *surface);
#ifndef SEGA_SATURN
void VL_UnlockSurface(SDL_Surface *surface);
#else
#define VL_UnlockSurface(surface) SDL_UnlockSurface(surface)
#endif
#ifdef SAVE_GAME_SCREENSHOT
unsigned char VL_GetPixel(SDL_Surface* surface, int x, int y);
void VL_SetSaveGameSlot();
#else
unsigned char VL_GetPixel        (int x, int y);
#endif
void VL_Plot            (int x, int y, int color);
void VL_Hlin            (unsigned x, unsigned y, unsigned width, int color);
void VL_Vlin            (int x, int y, int height, int color);
void VL_BarScaledCoord  (int scx, int scy, int scwidth, int scheight, int color);
extern wlinline void VL_Bar             (int x, int y, int width, int height, int color);

void VL_ScreenToScreen          (SDL_Surface *source, SDL_Surface *dest);
void VL_MemToScreenScaledCoord  (unsigned char *source, int width, int height, int scx, int scy);
void VL_MemToScreenScaledCoord2  (unsigned char *source, int origwidth, int origheight, int srcx, int srcy,
                                    int destx, int desty, int width, int height);

extern wlinline void VL_MemToScreen (unsigned char *source, int width, int height, int x, int y);

#define VW_Hlin(x,z,y,c)	VL_Hlin(x,y,(z)-(x)+1,c)
#define VW_Vlin(y,z,x,c)	VL_Vlin(x,y,(z)-(y)+1,c)

#endif
