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

    based on dc/SDL_syscdrom.c by

    BERO
    bero@geocities.co.jp
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_syscdrom.c,v 1.0 2003/05/06 16:50:56 slouken Exp $";
#endif

/* Functions for system-level CD-ROM audio control */

#include <stdlib.h>
#include <stdio.h>

#include "SDL_error.h"
#include "SDL_cdrom.h"
#include "SDL_syscdrom.h"


int  SDL_SYS_CDInit(void)
{
	return(-1);
}

char *SDL_SYS_CDName(int drive)
{
	return NULL;
}

int SDL_SYS_CDOpen(int drive)
{
	return(drive);
}

int SDL_SYS_CDGetTOC(SDL_CD *cdrom)
{
	return -1;
}

/* Get CD-ROM status */
CDstatus SDL_SYS_CDStatus(SDL_CD *cdrom, int *position)
{
	return	CD_ERROR;
}

/* Start play */
int SDL_SYS_CDPlay(SDL_CD *cdrom, int start, int length)
{
	return -1;
}

/* Pause play */
int SDL_SYS_CDPause(SDL_CD *cdrom)
{
	return -1;
}

/* Resume play */
int SDL_SYS_CDResume(SDL_CD *cdrom)
{
	return -1;
}

/* Stop play */
int SDL_SYS_CDStop(SDL_CD *cdrom)
{
	return -1;
}

/* Eject the CD-ROM */
int SDL_SYS_CDEject(SDL_CD *cdrom)
{
	return -1;
}

/* Close the CD-ROM handle */
void SDL_SYS_CDClose(SDL_CD *cdrom)
{
}

void SDL_SYS_CDQuit(void)
{

}
