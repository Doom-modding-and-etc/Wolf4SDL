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

    based on SDL_dcevents.c by

    BERO
    bero@geocities.co.jp
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gp32events.c,v 1.0 2003/05/06 16:50:56 slouken Exp $";
#endif

/* Being a null driver, there's no event stream. We just define stubs for
   most of the API. */

#include "SDL.h"
#include "SDL_sysevents.h"
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

const static unsigned short sdlSDL_gp32_key[]= {
		SDLK_RETURN,
		SDLK_SPACE,
		SDLK_LEFT,
		SDLK_RIGHT,
		SDLK_UP,
		SDLK_DOWN,
		SDLK_LCTRL,
		SDLK_LALT,
		SDLK_TAB,
		SDLK_BACKSPACE
	};

const static int sdlSDL_gp32_keygp32[]= {
#ifndef GP32_LIBC
		GPC_VK_START,
		GPC_VK_SELECT,
		GPC_VK_LEFT,
		GPC_VK_RIGHT,
		GPC_VK_UP,
		GPC_VK_DOWN,
		GPC_VK_FA,
		GPC_VK_FB,
		GPC_VK_FR,
		GPC_VK_FL
#else
		BUTTONMASK_START,
		BUTTONMASK_SELECT,
		BUTTONMASK_JOYLEFT,
		BUTTONMASK_JOYRIGHT,
		BUTTONMASK_JOYUP,
		BUTTONMASK_JOYDOWN,
		BUTTONMASK_A,
		BUTTONMASK_B,
		BUTTONMASK_RIGHT,
		BUTTONMASK_LEFT
#endif
	};

static int sdlSDL_gp32_key_old_status[]= {
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED,
		SDL_RELEASED	
	};

void SDL_gp32_PumpEvents(_THIS)
{
	int i;
	SDL_keysym keysym;
	int key;
#ifndef GP32_LIBC
	key=GpKeyGet();
#else
	x_gp32_ReadKeys();
	key=x_gp32_nKeys;
#endif
	for(i=0;i<10;i++)
	{
		if (sdlSDL_gp32_key_old_status[i]==SDL_RELEASED)
		{
			if (key&sdlSDL_gp32_keygp32[i])
			{
				keysym.sym=sdlSDL_gp32_key[i];
				SDL_PrivateKeyboard(SDL_PRESSED,&keysym);
				sdlSDL_gp32_key_old_status[i]=SDL_PRESSED;
			}
		}
		else
		{
			if (!(key&sdlSDL_gp32_keygp32[i]))
			{
				keysym.sym=sdlSDL_gp32_key[i];
				SDL_PrivateKeyboard(SDL_RELEASED,&keysym);
				sdlSDL_gp32_key_old_status[i]=SDL_RELEASED;
			}
		}
		
	}
}

void SDL_gp32_InitOSKeymap(_THIS)
{
	/* do nothing. */
}

/* end of SDL_gp32events.c ... */

