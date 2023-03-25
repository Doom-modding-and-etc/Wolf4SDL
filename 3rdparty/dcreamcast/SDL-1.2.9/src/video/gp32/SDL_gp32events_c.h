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

    based on SDL_dcevents.h by

    BERO
    bero@geocities.co.jp
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gp32events_c.h,v 1.0 2003/05/06 16:50:56 slouken Exp $";
#endif

#include "SDL_gp32video.h"

/* Variables and functions exported by SDL_sysevents.c to other parts 
   of the native video subsystem (SDL_sysvideo.c)
*/
extern void SDL_gp32_InitOSKeymap(_THIS);
extern void SDL_gp32_PumpEvents(_THIS);

/* end of SDL_gp32events_c.h ... */

