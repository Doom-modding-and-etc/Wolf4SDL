/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002  Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    CHUI
    sdl_gp32@yahoo.es

    based on SDL_dcvideo.c by

    BERO
    bero@geocities.co.jp
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gp32video.c,v 1.0 2003/03/06 16:50:56 slouken Exp $";
#endif

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@linuxgames.com). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DC" by Sam Lantinga.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include "SDL_mouse.h"
#include "SDL_sysvideo.h"
#include "SDL_pixels_c.h"
#include "SDL_events_c.h"

#include "SDL_gp32video.h"
#include "SDL_gp32events_c.h"

#ifndef GP32_LIBC
#include <gpfont.h>
#include <gpgraphic.h>
#include <gpmem.h>
#include <gpmm.h>
#include <gpsockdef.h>
#include <gpos_def.h>
#include <gpdef.h>
#include <gpstdio.h>
#include <gpstdlib.h>
#else
#include <x_gp32.h>
#endif

#include "SDL_gp32blit.h"

/* Initialization/Query functions */
static int SDL_gp32_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **SDL_gp32_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *SDL_gp32_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int SDL_gp32_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void SDL_gp32_VideoQuit(_THIS);

/* Hardware surface functions */
static int SDL_gp32_AllocHWSurface(_THIS, SDL_Surface *surface);
static int SDL_gp32_LockHWSurface(_THIS, SDL_Surface *surface);
static void SDL_gp32_UnlockHWSurface(_THIS, SDL_Surface *surface);
static void SDL_gp32_FreeHWSurface(_THIS, SDL_Surface *surface);
static int SDL_gp32_FlipHWSurface(_THIS, SDL_Surface *surface);

/* etc. */
static void SDL_gp32_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

static int SDL_gp32_BitsPerPixel;

/* OpenGL */
//static void *SDL_gp32_GL_GetProcAddress(_THIS, const char *proc);
//static int SDL_gp32_GL_LoadLibrary(_THIS, const char *path);
//static int SDL_gp32_GL_GetAttribute(_THIS, SDL_GLattr attrib, int* value);
//static void SDL_gp32_GL_SwapBuffers(_THIS);

/* GP32 driver bootstrap functions */

#ifndef GP32_LIBC 
static GPDRAWSURFACE SDL_gp32_SDL_Draw[2];
#else
static void *SDL_gp32_SDL_Draw[2];
#endif

static int SDL_gp32_Available(void)
{
	return 1;
}

static void SDL_gp32_DeleteDevice(SDL_VideoDevice *device)
{
	free(device->hidden);
	free(device);
}



static SDL_VideoDevice *SDL_gp32_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;
//	 Initialize all variables that we clean on shutdown 

	device = (SDL_VideoDevice *)malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			free(device);
		}
		return(0);
	}
	memset(device->hidden, 0, (sizeof *device->hidden));

	// Set the function pointers
	device->VideoInit = SDL_gp32_VideoInit;
	device->ListModes = SDL_gp32_ListModes;
	device->SetVideoMode = SDL_gp32_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = SDL_gp32_SetColors;
	device->UpdateRects = SDL_gp32_UpdateRects;
	device->VideoQuit = SDL_gp32_VideoQuit;
	device->AllocHWSurface = SDL_gp32_AllocHWSurface;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = SDL_gp32_LockHWSurface;
	device->UnlockHWSurface = SDL_gp32_UnlockHWSurface;
	device->FlipHWSurface = SDL_gp32_FlipHWSurface;
	device->FreeHWSurface = SDL_gp32_FreeHWSurface;

	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = SDL_gp32_InitOSKeymap;
	device->PumpEvents = SDL_gp32_PumpEvents;

	device->free = SDL_gp32_DeleteDevice;
	return device;
}

VideoBootStrap GP32_bootstrap = {
	"gp32video", "Game Park 32 Video",
	SDL_gp32_Available, SDL_gp32_CreateDevice
};


int SDL_gp32_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	/* Determine the screen depth (use default 8-bit depth) */
	/* we change this during the SDL_SetVideoMode implementation... */

	SDL_gp32_BitsPerPixel = 0;
	vformat->BitsPerPixel = 8;
	vformat->Rmask = 0; //0x000000e0;
	vformat->Gmask = 0; //0x0000001c;
	vformat->Bmask = 0; //0x00000003;
	vformat->Amask = 0;
	/* We're done! */
	return(0);
}

const static SDL_Rect
	__GP32_RECT_320x240__ = {0,0,320,240},
	__GP32_RECT_240x160__ = {0,0,240,160},
	__GP32_RECT_160x144__ = {0,0,160,144};

const static SDL_Rect *__gp32_vid_modes__[] = {
	&__GP32_RECT_320x240__,
	&__GP32_RECT_240x160__,
	&__GP32_RECT_160x144__,
	NULL
};

SDL_Rect *__gp32_vid_mode__=NULL;

SDL_Rect **SDL_gp32_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
	switch(format->BitsPerPixel) {
	case 8:
	case 16:
		return (SDL_Rect **)&__gp32_vid_modes__;
	}
	return NULL;
}


SDL_Surface *SDL_gp32_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	Uint32 Rmask=0, Gmask=0, Bmask=0;
	int i=0;

	__gp32_vid_mode__=NULL;
	while(__gp32_vid_modes__[i]!=NULL)
	{
		if ((__gp32_vid_modes__[i]->w==width)&&(__gp32_vid_modes__[i]->h==height))
		{
			__gp32_vid_mode__=(SDL_Rect *)__gp32_vid_modes__[i];
			break;
		}
		i++;
	}

	if (__gp32_vid_mode__==NULL)
	{
		SDL_SetError("Couldn't find requested mode in list");
		return(NULL);
	}

	switch(bpp) {
	case 8:
		SDL_gp32_BitsPerPixel=8;
		Rmask = 0; //0x000000e0;
		Gmask = 0; //0x0000001c;
		Bmask = 0; //0x00000003;
		break;
	case 16:
		SDL_gp32_BitsPerPixel=16;
		// 5-6-5 
		Rmask = 0x0000f800;
		Gmask = 0x000007c0;
		Bmask = 0x0000003e;
		break;
	default:
		SDL_SetError("Couldn't find requested mode in list");
		return(NULL);
	}


	if ( ! SDL_ReallocFormat(current, bpp, Rmask, Gmask, Bmask, 0) ) {
		return(NULL);
	}
	// Set up the new mode framebuffer 
	current->flags = (SDL_FULLSCREEN|SDL_HWSURFACE|SDL_PREALLOC|flags);
	current->w = width;
	current->h = height;
	current->pitch = width * ((bpp+7) /8);
#ifndef GP32_LIBC
	GpGraphicModeSet(bpp,NULL);
	GpLcdSurfaceGet(&SDL_gp32_SDL_Draw[0], 0);
	GpLcdSurfaceGet(&SDL_gp32_SDL_Draw[1], 1);
	current->pixels = SDL_gp32_SDL_Draw[1].ptbuffer;

	GpSurfaceSet(&SDL_gp32_SDL_Draw[0]);
        GpLcdEnable();
#else
//	x_gp32_SetGraphicsMode(bpp); 
	x_gp32_initFramebuffer(SCREEN_BUFFER_1,bpp,85); //50);
	SDL_gp32_SDL_Draw[0]=SCREEN_BUFFER_1;
	SDL_gp32_SDL_Draw[1]=SCREEN_BUFFER_3;
	current->pixels=SDL_gp32_SDL_Draw[1];
#endif
	// We're done 
	return(current);
}

#ifndef GP32_LIBC
GPDRAWSURFACE *SDL_gp32_getDrawSurface(void)
{
	return &SDL_gp32_SDL_Draw[0];
}
#endif

/* We don't actually allow hardware surfaces other than the main one */
static int SDL_gp32_AllocHWSurface(_THIS, SDL_Surface *surface)
{
	return(-1);
}
static void SDL_gp32_FreeHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

/* We need to wait for vertical retrace on page flipped displays */
static int SDL_gp32_LockHWSurface(_THIS, SDL_Surface *surface)
{
	return(0);
}

static void SDL_gp32_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
	return;
}

static int SDL_gp32_FlipHWSurface(_THIS, SDL_Surface *surface)
{
	switch(surface->w)
	{
	 case 320:
		if (surface->pitch == 320)
#ifndef GP32_LIBC
			SDL_gp32blit(SDL_gp32_SDL_Draw[1].ptbuffer,SDL_gp32_SDL_Draw[0].ptbuffer);
#else
			SDL_gp32blit(SDL_gp32_SDL_Draw[1],SDL_gp32_SDL_Draw[0]);
#endif
		else
#ifndef GP32_LIBC
			SDL_gp32blitW((unsigned short *)SDL_gp32_SDL_Draw[1].ptbuffer,(unsigned short *)SDL_gp32_SDL_Draw[0].ptbuffer);
#else
			SDL_gp32blitW((unsigned short *)SDL_gp32_SDL_Draw[1],(unsigned short *)SDL_gp32_SDL_Draw[0]);
#endif
		break;
	 case 240:
		if (surface->pitch == 240)
#ifndef GP32_LIBC
			SDL_gp32blit240x160(SDL_gp32_SDL_Draw[1].ptbuffer,SDL_gp32_SDL_Draw[0].ptbuffer);
#else
			SDL_gp32blit240x160(SDL_gp32_SDL_Draw[1],SDL_gp32_SDL_Draw[0]);
#endif
		else
#ifndef GP32_LIBC
			SDL_gp32blitW240x160((unsigned short *)SDL_gp32_SDL_Draw[1].ptbuffer,(unsigned short *)SDL_gp32_SDL_Draw[0].ptbuffer);
#else
			SDL_gp32blitW240x160((unsigned short *)SDL_gp32_SDL_Draw[1],(unsigned short *)SDL_gp32_SDL_Draw[0]);
#endif

		break;
	case 160:
		if (surface->pitch == 160)
#ifndef GP32_LIBC
			SDL_gp32blit160x144(SDL_gp32_SDL_Draw[1].ptbuffer,SDL_gp32_SDL_Draw[0].ptbuffer);
#else
			SDL_gp32blit160x144(SDL_gp32_SDL_Draw[1],SDL_gp32_SDL_Draw[0]);
#endif
		else
#ifndef GP32_LIBC
			SDL_gp32blitW160x144((unsigned short *)SDL_gp32_SDL_Draw[1].ptbuffer,(unsigned short *)SDL_gp32_SDL_Draw[0].ptbuffer);
#else
			SDL_gp32blitW160x144((unsigned short *)SDL_gp32_SDL_Draw[1],(unsigned short *)SDL_gp32_SDL_Draw[0]);
#endif
		break;
	}

	return(0);
}

static void SDL_gp32_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	Uint16 x,y,h,w;

	if (__gp32_vid_mode__!=NULL)
	switch (__gp32_vid_mode__->h)
	{
	 case 240:
	  if (SDL_gp32_BitsPerPixel==8)
	  {
		while(numrects>=0)
		{
			x=rects[numrects].x;
			y=rects[numrects].y;
			if ((x<320)&&(y<240))
			{
				h=rects[numrects].h;
				w=rects[numrects].w;
				if (320-(x+w)<0)
					w=320-x;
				if (240-(y+h)<0)
					h=240-y;
				{
#ifndef GP32_LIBC
					register Uint8 *s=SDL_gp32_SDL_Draw[1].ptbuffer+x+y*320;
					register Uint8 *d=SDL_gp32_SDL_Draw[0].ptbuffer+239-y+x*240;
#else
					register Uint8 *s=SDL_gp32_SDL_Draw[1]+x+y*320;
					register Uint8 *d=SDL_gp32_SDL_Draw[0]+239-y+x*240;
#endif
					register Uint32 off_s=320-w;
					register Uint32 off_d=(240*w)+1;
					for(;h>0;h--,s+=off_s,d-=off_d)
					{
						register int i;
						for(i=w;i>0;i--,s++,d+=240)
							*d=*s;
					}
				}
			}
			numrects--;
		}
	  }
	  else
	  {
		while(numrects>=0)
		{
			x=rects[numrects].x;
			y=rects[numrects].y;
			if ((x<320)&&(y<240))
			{
				h=rects[numrects].h;
				w=rects[numrects].w;
				if (320-(x+w)<0)
					w=320-x;
				if (240-(y+h)<0)
					h=240-y;
				{
#ifndef GP32_LIBC
					register Uint16 *s=(Uint16 *)(SDL_gp32_SDL_Draw[1].ptbuffer+(x<<1)+(y*640));
					register Uint16 *d=(Uint16 *)(SDL_gp32_SDL_Draw[0].ptbuffer+((239-y)<<1)+(x*480));
#else
					register Uint16 *s=(Uint16 *)(SDL_gp32_SDL_Draw[1]+(x<<1)+(y*640));
					register Uint16 *d=(Uint16 *)(SDL_gp32_SDL_Draw[0]+((239-y)<<1)+(x*480));
#endif
					register Uint32 off_s=320-w;
					register Uint32 off_d=(240*w)+1;
					for(;h>0;h--,s+=off_s,d-=off_d)
					{
						register int i;
						for(i=w;i>0;i--,s++,d+=240)
							*d=*s;
					}
				}
			}
			numrects--;
		}
	  }
	  break;

	 case 160:
	  if (SDL_gp32_BitsPerPixel==8)
#ifndef GP32_LIBC
		SDL_gp32blit240x160(SDL_gp32_SDL_Draw[1].ptbuffer,SDL_gp32_SDL_Draw[0].ptbuffer);
#else
		SDL_gp32blit240x160(SDL_gp32_SDL_Draw[1],SDL_gp32_SDL_Draw[0]);
#endif
	  else
#ifndef GP32_LIBC
		SDL_gp32blitW240x160((unsigned short *)SDL_gp32_SDL_Draw[1].ptbuffer,(unsigned short *)SDL_gp32_SDL_Draw[0].ptbuffer);
#else
		SDL_gp32blitW240x160((unsigned short *)SDL_gp32_SDL_Draw[1],(unsigned short *)SDL_gp32_SDL_Draw[0]);
#endif
	  break;

	 case 144:
	  if (SDL_gp32_BitsPerPixel==8)
#ifndef GP32_LIBC
		SDL_gp32blit160x144(SDL_gp32_SDL_Draw[1].ptbuffer,SDL_gp32_SDL_Draw[0].ptbuffer);
#else
		SDL_gp32blit160x144(SDL_gp32_SDL_Draw[1],SDL_gp32_SDL_Draw[0]);
#endif
	  else
#ifndef GP32_LIBC
		SDL_gp32blitW160x144((unsigned short *)SDL_gp32_SDL_Draw[1].ptbuffer,(unsigned short *)SDL_gp32_SDL_Draw[0].ptbuffer);
#else
		SDL_gp32blitW160x144((unsigned short *)SDL_gp32_SDL_Draw[1],(unsigned short *)SDL_gp32_SDL_Draw[0]);
#endif
	  break;

	}
}


#define RGB2INT(r,g,b) (((((r>>3))&0x1f)<<11)|((((g>>3))&0x1f)<<6)|((((b>>3))&0x1f)<<1))
static int SDL_gp32_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i,j=firstcolor+ncolors;
#ifndef GP32_LIBC
	for(i=firstcolor;i<j;i++)
	{
		unsigned long *palette=(unsigned long *)0x14A00400;
	 	palette[i]=RGB2INT(colors[i].r,colors[i].g,colors[i].b);
	}
#else
	unsigned int *pLcdReg=(unsigned int *)LCD_PALETTE;
	x_gp32_XArmDisableInterrupt();
//	while (((rLCDCON5>>19)&3)!=3);
	for(i=firstcolor,pLcdReg=&pLcdReg[firstcolor];i<j;i++,pLcdReg++)
		*pLcdReg=RGB2INT(colors[i].r,colors[i].g,colors[i].b);
	x_gp32_XArmEnableInterrupt();
#endif
	return 1; 
}

/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
static void SDL_gp32_VideoQuit(_THIS)
{
}
