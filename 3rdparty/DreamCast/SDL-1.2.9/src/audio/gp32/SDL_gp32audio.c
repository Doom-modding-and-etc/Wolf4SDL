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

    CHUI <sdl_gp32@yahoo.es>
    based on SDL_dcaudio.c by BERO <bero@geocities.co.jp>

*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_gp32audio.c,v 1.1 2003/05/06 16:50:56 slouken Exp $";
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "SDL_audio.h"
#include "SDL_error.h"
#include "SDL_audiomem.h"
#include "SDL_audio_c.h"
#include "SDL_timer.h"
#include "SDL_audiodev_c.h"
#include "SDL_gp32audio.h"

#ifdef GP32_LIBC
#include <x_gp32.h>
#include "SDL_dmaaudio.h"
#else
#include <gpfont.h>
#include <gpgraphic.h>
#include <gpmem.h>
#include <gpmm.h>
#include <gpsockdef.h>
#include <gpos_def.h>
#include <gpdef.h>
#include <gpstdio.h>
#include <gpstdlib.h>
#define GP32_TIMER_AUDIO_IDX 1
volatile unsigned int SDL_gp32_Audio_frame;
#endif

static SDL_AudioDevice *SDL_gp32_AudioDevice;
static int SDL_gp32_LockAudio_BOOL;

/* Audio driver functions */
static int GP32AUD_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void GP32AUD_WaitAudio(_THIS);
static void GP32AUD_PlayAudio(_THIS);
static Uint8 *GP32AUD_GetAudioBuf(_THIS);
static void GP32AUD_CloseAudio(_THIS);
static void GP32AUD_LockAudio(_THIS);
static void GP32AUD_UnlockAudio(_THIS);

/* Audio driver bootstrap functions */
static int GP32AUD_Available(void)
{
	return 1;
}

static void GP32AUD_DeleteDevice(SDL_AudioDevice *device)
{
	free(device->hidden);
	free(device);
}

static SDL_AudioDevice *GP32AUD_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;

	// Initialize all variables that we clean on shutdown
	this = (SDL_AudioDevice *)malloc(sizeof(SDL_AudioDevice));
	if ( this ) {
		memset(this, 0, (sizeof *this));
		this->hidden = (struct SDL_PrivateAudioData *)
				malloc((sizeof *this->hidden));
	}
	if ( (this == NULL) || (this->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( this ) {
			free(this);
		}
		return(0);
	}
	memset(this->hidden, 0, (sizeof *this->hidden));

	// Set the function pointers
	this->OpenAudio = GP32AUD_OpenAudio;
	this->WaitAudio = GP32AUD_WaitAudio;
	this->PlayAudio = GP32AUD_PlayAudio;
	this->GetAudioBuf = GP32AUD_GetAudioBuf;
	this->CloseAudio = GP32AUD_CloseAudio;
	this->LockAudio = GP32AUD_LockAudio;
	this->UnlockAudio = GP32AUD_UnlockAudio;

	this->free = GP32AUD_DeleteDevice;

	return this;
}

AudioBootStrap GP32AUD_bootstrap = {
	"gp32audio", "GP32 audio",
	GP32AUD_Available, GP32AUD_CreateDevice
};

/* This function waits until it is possible to write a full sound buffer */
static void GP32AUD_WaitAudio(_THIS)
{
}

static void GP32AUD_PlayAudio(_THIS)
{
}

static Uint8 *GP32AUD_GetAudioBuf(_THIS)
{
	return((Uint8 *)this->hidden->mixbuf);
}

static void GP32AUD_CloseAudio(_THIS)
{
#ifndef GP32_LIBC
        GpTimerKill(GP32_TIMER_AUDIO_IDX);
        GpPcmStop();
#else
	sdl_gp32_dma_audio_stop();
	if ( this->hidden->mixbuf != NULL ) {
		SDL_FreeAudioMem(this->hidden->mixbuf);
		this->hidden->mixbuf = NULL;
	}
#endif
}

void sdl_gp32_audio_process(void)
{
#ifndef GP32_LIBC
        if ((!SDL_gp32_AudioDevice->paused)&&(!SDL_gp32_LockAudio_BOOL))
        {
                unsigned int *soundPos=SDL_gp32_AudioDevice->hidden->soundPos;
                void *buffer=SDL_gp32_AudioDevice->hidden->mixbuf;
                unsigned int frame=SDL_gp32_AudioDevice->hidden->frame;
                unsigned int shiftVal=SDL_gp32_AudioDevice->hidden->shiftVal;
                unsigned int samples=SDL_gp32_AudioDevice->spec.samples;
                unsigned int sampleshiftVal=(samples<<shiftVal);
                void *userdata=SDL_gp32_AudioDevice->spec.userdata;
                unsigned int t = (((unsigned int)(*soundPos) - (unsigned int)buffer)>>shiftVal) >= samples ? 1 : 0;
                if (t!=frame)
                {
                        unsigned int offs = ((frame==1) ? (sampleshiftVal) : 0);
                        memset(buffer+offs,0,sampleshiftVal);
                        SDL_gp32_AudioDevice->spec.callback(userdata, (Uint8*)((unsigned int)buffer+offs), samples<<shiftVal);
                        SDL_gp32_AudioDevice->hidden->frame = t;
                        {
                                register Uint16 *d=(buffer+offs);
                                register Uint32 max=(buffer+offs+sampleshiftVal);
                                do
                                {
                                        *d++^=0x8000; // 1
                                        *d++^=0x8000; // 2
                                        *d++^=0x8000; // 3
                                        *d++^=0x8000; // 4
                                        *d++^=0x8000; // 5
                                        *d++^=0x8000; // 6
                                        *d++^=0x8000; // 7
                                        *d++^=0x8000; // 8
                                        *d++^=0x8000; // 9
                                        *d++^=0x8000; // 10
                                        *d++^=0x8000; // 11
                                        *d++^=0x8000; // 12
                                        *d++^=0x8000; // 13
                                        *d++^=0x8000; // 14
                                        *d++^=0x8000; // 15
                                        *d++^=0x8000; // 16
                                        *d++^=0x8000; // 17
                                        *d++^=0x8000; // 18
                                        *d++^=0x8000; // 19
                                        *d++^=0x8000; // 20
                                        *d++^=0x8000; // 21
                                        *d++^=0x8000; // 22
                                        *d++^=0x8000; // 23
                                        *d++^=0x8000; // 24
                                        *d++^=0x8000; // 25
                                        *d++^=0x8000; // 26
                                        *d++^=0x8000; // 27
                                        *d++^=0x8000; // 28
                                        *d++^=0x8000; // 29
                                        *d++^=0x8000; // 30
                                        *d++^=0x8000; // 31
                                        *d++^=0x8000; // 32
                                }while((Uint32)d<max);
                        }
                }
        }

#else

	extern short *pOutBuffer[3];

	if ((!SDL_gp32_AudioDevice->paused)&&(!SDL_gp32_LockAudio_BOOL))
	{
		/* Start playing pre-prepared sound buffer */
		rDISRC2 = (int)pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];
		rDIDST2 = (1<<30)+   	  	/* destination on peripheral bus */
			  (1<<29)+       	/* fixed adress */
			  ((int)IISFIF); 	/* IIS FIFO txd */
		rDCON2 = 0x50D00000 + ((SDL_gp32_AudioDevice->hidden->buffersamples)<<1);
		rDMASKTRIG2 = 2;

		/* Flip buffer select */
		SDL_gp32_AudioDevice->hidden->flipper ^= 1;

		/* Clear the mix buffer. Assume that there is an even number of bytes. */
		{
			register int x;
			register short *pMB = (short *)SDL_gp32_AudioDevice->hidden->mixbuf;
			for (x=0; x<(SDL_gp32_AudioDevice->hidden->bufferbytes >> 1); x++)
				*pMB++ = 0;
		}

		/* Get new data */
		SDL_gp32_AudioDevice->spec.callback(SDL_gp32_AudioDevice->spec.userdata,
			(Uint8 *)SDL_gp32_AudioDevice->hidden->mixbuf,
			(int)SDL_gp32_AudioDevice->hidden->bufferbytes );


		switch (SDL_gp32_AudioDevice->hidden->channels)
		{
		case 1:		/* MONO */
			switch(SDL_gp32_AudioDevice->hidden->format)
			{
			case  AUDIO_U8:
				{
					register int x;
					register unsigned char *pSrc = (unsigned char *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=SDL_gp32_AudioDevice->hidden->buffersamples; x>0; x--)
					{
						register short temp = (((short)*pSrc)-128)<<8;
						pSrc++;
						*pDest++ = temp;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_S8:
				{
					register int x;
					register signed char *pSrc = (signed char *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=SDL_gp32_AudioDevice->hidden->buffersamples; x>0; x--)
					{
						register short temp = ((short)*pSrc)<<8;
						pSrc++;
						*pDest++ = temp;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_U16LSB:
				{
					register int x;
					register unsigned short *pSrc = (unsigned short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=SDL_gp32_AudioDevice->hidden->buffersamples; x>0; x--)
					{
						register short temp = ((short)*pSrc)^0x8000;
						pSrc++;
						*pDest++ = temp;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_S16LSB:
				{
					register int x;
					register short *pSrc = (short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=SDL_gp32_AudioDevice->hidden->buffersamples; x>0; x--)
					{
						register short temp = *pSrc;
						pSrc++;
						*pDest++ = temp;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_U16MSB:
				{
					register int x;
					register unsigned short *pSrc = (unsigned short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=SDL_gp32_AudioDevice->hidden->buffersamples; x>0; x--)
					{
						register short temp1 = ((short)*pSrc)^0x0080;
						register short temp2 = ((temp1>>8)&0xff) + (temp1 << 8);
						pSrc++;
						*pDest++ = temp2;
						*pDest++ = temp2;
					}
				}
				break;
			case  AUDIO_S16MSB:
				{
					register int x;
					register short *pSrc = (short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=SDL_gp32_AudioDevice->hidden->buffersamples; x>0; x--)
					{
						register short temp1 = *pSrc;
						register short temp2 = ((temp1>>8)&0xff) + (temp1 << 8);
						pSrc++;
						*pDest++ = temp2;
						*pDest++ = temp2;
					}
				}
				break;
			}
			break;
		case 2:		/* STEREO */
			switch(SDL_gp32_AudioDevice->hidden->format)
			{
			case  AUDIO_U8:
				{
					register int x;
					register unsigned char *pSrc = (unsigned char *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=(SDL_gp32_AudioDevice->hidden->buffersamples << 1); x>0; x--)
					{
						register short temp = (((short)*pSrc)-128)<<8;
						pSrc++;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_S8:
				{
					register int x;
					register signed char *pSrc = (signed char *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=(SDL_gp32_AudioDevice->hidden->buffersamples << 1); x>0; x--)
					{
						register short temp = ((short)*pSrc)<<8;
						pSrc++;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_U16LSB:
				{
					register int x;
					register unsigned short *pSrc = (unsigned short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=(SDL_gp32_AudioDevice->hidden->buffersamples << 1); x>0; x--)
					{
						register short temp = ((short)*pSrc)^0x8000;
						pSrc++;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_S16LSB:
				{
					register int x;
					register short *pSrc = (short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=(SDL_gp32_AudioDevice->hidden->buffersamples << 1); x>0; x--)
					{
						register short temp = *pSrc;
						pSrc++;
						*pDest++ = temp;
					}
				}
				break;
			case  AUDIO_U16MSB:
				{
					register int x;
					register unsigned short *pSrc = (unsigned short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=(SDL_gp32_AudioDevice->hidden->buffersamples << 1); x>0; x--)
					{
						register short temp1 = ((short)*pSrc)^0x0080;
						register short temp2 = ((temp1>>8)&0xff) + (temp1 << 8);
						pSrc++;
						*pDest++ = temp2;
					}
				}
				break;
			case  AUDIO_S16MSB:
				{
					register int x;
					register short *pSrc = (short *)SDL_gp32_AudioDevice->hidden->mixbuf;
					register short *pDest = pOutBuffer[SDL_gp32_AudioDevice->hidden->flipper];

					for (x=(SDL_gp32_AudioDevice->hidden->buffersamples << 1); x>0; x--)
					{
						register short temp1 = *pSrc;
						register short temp2 = ((temp1>>8)&0xff) + (temp1 << 8);
						pSrc++;
						*pDest++ = temp2;
					}
				}
				break;
			}
			break;
		}
	}
	else
	{
		/* Play silence */
		rDISRC2 = (int)pOutBuffer[2];
		rDIDST2 = (1<<30)+  	  	/* destination on peripheral bus */
			  (1<<29)+   		/* fixed adress */
		 	  ((int)IISFIF); 	/* IIS FIFO txd */
		rDCON2 = 0x50D00000 + ((SDL_gp32_AudioDevice->hidden->buffersamples)<<1);
		rDMASKTRIG2 = 2;
	}
#endif
}

static void GP32AUD_LockAudio(_THIS)
{
	SDL_gp32_LockAudio_BOOL=1;
}

static void GP32AUD_UnlockAudio(_THIS)
{
	SDL_gp32_LockAudio_BOOL=0;
}

static int GP32AUD_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
#ifndef GP32_LIBC
        int i;
        Uint16 *tmp_buf;
        PCM_SR gp_sr;
        PCM_BIT gp_bit;

        this->hidden->shiftVal=0;
        switch(spec->format&0xff) {
        case  8:
                spec->format = AUDIO_S8;
                gp_bit=PCM_8BIT;
                break;
        case 16:
                spec->format = AUDIO_S16LSB;
                gp_bit=PCM_16BIT;
                this->hidden->shiftVal++;
                break;
        default:
                SDL_SetError("Unsupported audio format");
                return(-1);
        }
        switch(spec->freq){
        case 11025:
                if (spec->channels==2)
                {
                        gp_sr=PCM_S11;
                        this->hidden->shiftVal++;
                }
                else
                        gp_sr=PCM_M11;
                break;
        case 22050:
                if (spec->channels==2)
                {
                        gp_sr=PCM_S22;
                        this->hidden->shiftVal++;
                }
                else
                        gp_sr=PCM_M22;
                break;
        case 44100:
                if (spec->channels==2)
                {
                        gp_sr=PCM_S44;
                        this->hidden->shiftVal++;
                }
                else
                        gp_sr=PCM_M44;
                break;
        default:
                SDL_SetError("Unsupported sample rate");
                return(-1);
        }
        this->hidden->samplesize = 1<<this->hidden->shiftVal;


        // Update the fragment size as size in bytes
        SDL_CalculateAudioSpec(spec);

        // Allocate mixing buffer
//      spec->size = spec->size - (spec->size%128) + 128;
        this->hidden->mixlen = spec->size<<1;
        this->hidden->mixbuf = (Uint8 *) SDL_AllocAudioMem(this->hidden->mixlen)
;
        if ( this->hidden->mixbuf == NULL ) {
                return(-1);
        }
        tmp_buf=this->hidden->mixbuf;
        for(i=0;i<spec->size;i++)
                tmp_buf[i]=0x8000;
        SDL_gp32_AudioDevice=this;
        SDL_gp32_LockAudio_BOOL=1;

        if(GPOS_ERR_ALREADY_USED == GpTimerOptSet(GP32_TIMER_AUDIO_IDX, (2*spec->freq)/spec->samples,0,sdl_gp32_audio_process))
                GpTimerKill(GP32_TIMER_AUDIO_IDX);
        GpTimerSet(GP32_TIMER_AUDIO_IDX);
        GpPcmInit(gp_sr, gp_bit);
        GpPcmPlay((unsigned short *)this->hidden->mixbuf, this->hidden->mixlen,
1);
        GpPcmLock((unsigned short *)this->hidden->mixbuf, (int*)&this->hidden->idx_buf, (unsigned int *) &this->hidden->soundPos);

        // We're ready to rock and roll. :-)

#else
	int samplesize = 1;

	switch(spec->format)
	{
	case  AUDIO_U8:
		spec->silence = 0x80;
		break;
	case  AUDIO_S8:
		spec->silence = 0;
		break;
	case  AUDIO_U16LSB:
		spec->silence = 0x8000;
		samplesize<<=1;
		break;
	case  AUDIO_S16LSB:
		spec->silence = 0;
		samplesize<<=1;
		break;
	case  AUDIO_U16MSB:
		spec->silence = 0x0080;
		samplesize<<=1;
		break;
	case  AUDIO_S16MSB:
		spec->silence = 0;
		samplesize<<=1;
		break;
	default:
		SDL_SetError("Unsupported audio format");
		return -1;
	}

	switch(spec->freq)
	{
	case 11025:
	case 22050:
	case 44100:
		break;
	default:
		SDL_SetError("Unsupported sample rate");
		return -1;
	}

	switch (spec->channels)
	{
	case 1:
		break;
	case 2:
		samplesize<<=1;
		break;
	default:
		SDL_SetError ("Unsupported number of channels");
		return -1;
	}

	spec->size = spec->samples * spec->channels * samplesize;
	this->hidden->buffersamples = spec->samples;
	this->hidden->bufferbytes = spec->size / spec->channels;
	this->hidden->channels = spec->channels;

	this->hidden->mixbuf = (Uint8 *) malloc(spec->size);
	if ( this->hidden->mixbuf == NULL )
	{
		SDL_OutOfMemory();
		return -1;
	}

	this->hidden->format = spec->format;
	this->hidden->flipper = 0;

	SDL_gp32_AudioDevice=this;
	SDL_gp32_LockAudio_BOOL=1;

	if (sdl_gp32_dma_audio_start(spec->freq,spec->samples,0))
	{
		SDL_SetError ("Failed to start audio engine");
		free (this->hidden->mixbuf);
	}
#endif
	return(0);
}
