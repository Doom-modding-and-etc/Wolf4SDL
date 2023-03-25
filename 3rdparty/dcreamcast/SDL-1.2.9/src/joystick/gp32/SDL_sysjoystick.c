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

    based on dc/SDL_sysjoystick.c

    BERO
    bero@geocities.co.jp
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_sysjoystick.c,v 1.0 2003/05/06 16:50:56 slouken Exp $";
#endif


#include <stdlib.h>
#include <stdio.h>		/* For the definition of NULL */

#include "SDL_error.h"
#include "SDL_joystick.h"
#include "SDL_sysjoystick.h"
#include "SDL_joystick_c.h"

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
#include <gpmem.h>
#else
#include <x_gp32.h>
#define GPC_VK_UP BUTTONMASK_JOYUP
#define GPC_VK_DOWN BUTTONMASK_JOYDOWN
#define GPC_VK_LEFT BUTTONMASK_JOYLEFT
#define GPC_VK_RIGHT BUTTONMASK_JOYRIGHT
#define GPC_VK_FA BUTTONMASK_A
#define GPC_VK_FB BUTTONMASK_B
#define GPC_VK_FL BUTTONMASK_LEFT
#define GPC_VK_FR BUTTONMASK_RIGHT
#define GPC_VK_START BUTTONMASK_START
#define GPC_VK_SELECT BUTTONMASK_SELECT
#endif

extern int sdlgp32_key_old_status[];

#define MAX_JOYSTICKS	8	/* only 2 are supported in the multimedia API */
#define MAX_AXES	2	/* each joystick can have up to 2 axes */
#define MAX_BUTTONS	6	/* and 6 buttons                      */
#define	MAX_HATS	1

#define	JOYNAMELEN	8

struct joystick_hwdata
{
	int prev_buttons;
};



/* Function to scan the system for joysticks.
 * This function should set SDL_numjoysticks to the number of available
 * joysticks.  Joystick 0 should be the system default joystick.
 * It should return 0, or -1 on an unrecoverable fatal error.
 */
int SDL_SYS_JoystickInit(void)
{
	return 1;
}

/* Function to get the device-dependent name of a joystick */
const char *SDL_SYS_JoystickName(int index)
{
	return "GP32 Joy";
}

/* Function to open a joystick for use.
   The joystick to open is specified by the index field of the joystick.
   This should fill the nbuttons and naxes fields of the joystick structure.
   It returns 0, or -1 if there is an error.
 */


int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	joystick->hwdata = (struct joystick_hwdata *) malloc(sizeof(*joystick->hwdata));
	if (joystick->hwdata == NULL)
	{
		SDL_OutOfMemory();
		return -1;
	}
	joystick->hwdata->prev_buttons=0;

	// fill nbuttons, naxes, and nhats fields
	joystick->nbuttons = MAX_BUTTONS;
	joystick->naxes = MAX_AXES;
	joystick->nhats = MAX_HATS;

	return(0);
}


/* Function to update the state of a joystick - called as a device poll.
 * This function shouldn't update the joystick structure directly,
 * but instead should call SDL_PrivateJoystick*() to deliver events
 * and update joystick device state.
 */

void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	int buttons,prev_buttons,changed;
#ifndef GP32_LIBC	
	buttons = GpKeyGet();
#else
	x_gp32_ReadKeys();
	buttons = x_gp32_nKeys;
#endif

	prev_buttons = joystick->hwdata->prev_buttons;
	changed = buttons^prev_buttons;
	if ((changed)&(GPC_VK_UP|GPC_VK_DOWN|GPC_VK_LEFT|GPC_VK_RIGHT)) {
		int hat = SDL_HAT_CENTERED;
		if (buttons&GPC_VK_UP) hat|=SDL_HAT_UP;
		else if (buttons&GPC_VK_DOWN) hat|=SDL_HAT_DOWN;
		if (buttons&GPC_VK_LEFT) hat|=SDL_HAT_LEFT;
		else if (buttons&GPC_VK_RIGHT) hat|=SDL_HAT_RIGHT;
		SDL_PrivateJoystickHat(joystick, 0, hat);
	}
	if (changed & GPC_VK_FA)
		SDL_PrivateJoystickButton(joystick,0,(buttons & GPC_VK_FA)?SDL_PRESSED:SDL_RELEASED);
	if (changed & GPC_VK_FB)
		SDL_PrivateJoystickButton(joystick,1,(buttons & GPC_VK_FB)?SDL_PRESSED:SDL_RELEASED);
	if (changed & GPC_VK_FL)
		SDL_PrivateJoystickButton(joystick,2,(buttons & GPC_VK_FL)?SDL_PRESSED:SDL_RELEASED);
	if (changed & GPC_VK_FR)
		SDL_PrivateJoystickButton(joystick,3,(buttons & GPC_VK_FR)?SDL_PRESSED:SDL_RELEASED);
	if (changed & GPC_VK_START)
		SDL_PrivateJoystickButton(joystick,4,(buttons & GPC_VK_START)?SDL_PRESSED:SDL_RELEASED);
	if (changed & GPC_VK_SELECT)
		SDL_PrivateJoystickButton(joystick,5,(buttons & GPC_VK_SELECT)?SDL_PRESSED:SDL_RELEASED);

	joystick->hwdata->prev_buttons= buttons;
}

/* Function to close a joystick after use */
void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	if (joystick->hwdata != NULL) {
		// free system specific hardware data
		free(joystick->hwdata);
	}
}

/* Function to perform any system-specific joystick related cleanup */
void SDL_SYS_JoystickQuit(void)
{
	return;
}
