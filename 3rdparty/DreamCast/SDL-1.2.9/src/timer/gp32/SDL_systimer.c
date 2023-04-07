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

    based on dc/SDL_systimer.c

    BERO
    bero@geocities.co.jp
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_systimer.c,v 1.0 2003/05/06 16:50:56 slouken Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/signal.h>

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
#define GP32_TIMER_IDX	0

#else
#include <x_gp32.h>
#endif

#include "SDL_timer.h"
#include "SDL_error.h"
#include "SDL_timer_c.h"



Uint32 SDL_GetTicks(void)
{
#ifndef GP32_LIBC
	return GpTickCountGet();
#else
//	return (x_gp32_timer_counter*1000)>>6;
	return (x_gp32_timer_counter<<4);
#endif
}

void SDL_Delay(Uint32 ms)
{
#ifndef GP32_LIBC
	GpThreadSleep(ms);
#else
	Uint32 step=SDL_GetTicks()+ms;
	while (step>SDL_GetTicks())
		x_gp32_NOP();
#endif
}

#ifdef GP32_LIBC
void _sdl_gp32_TimerInt(void) __attribute__ ((interrupt ("IRQ")));
#endif

void _sdl_gp32_TimerInt(void)
{
#ifdef GP32_LIBC
	x_gp32_timer_counter++;
#endif
	if (SDL_timer_running)
		SDL_ThreadedTimerCheck();
}


int SDL_SYS_TimerInit(void)
{
#ifndef GP32_LIBC
	if(GPOS_ERR_ALREADY_USED == GpTimerOptSet(GP32_TIMER_IDX,100,0,_sdl_gp32_TimerInt))
		GpTimerKill(GP32_TIMER_IDX); // return -1;
	GpTimerSet(GP32_TIMER_IDX);
	return(SDL_SetTimerThreaded(1));
#else
	x_gp32_DisableIRQ();
	x_gp32_InstallSWIIRQ(8,_sdl_gp32_TimerInt);
	x_gp32_EnableIRQ();
	return 0;
#endif
}

extern void x_gp32_timer_ISR(void);

void SDL_SYS_TimerQuit(void)
{
#ifndef GP32_LIBC
	GpTimerKill(GP32_TIMER_IDX);
#else
	x_gp32_DisableIRQ();
	x_gp32_InstallSWIIRQ(8,x_gp32_timer_ISR);
	x_gp32_EnableIRQ();
#endif
}

int SDL_SYS_StartTimer(void)
{
#ifndef GP32_LIBC
	GpTimerResume(GP32_TIMER_IDX);
#endif
	return 0;
}

void SDL_SYS_StopTimer(void)
{
#ifndef GP32_LIBC
	GpTimerPause(GP32_TIMER_IDX);
#endif
}

void SDL_StartTicks(void)
{
}
