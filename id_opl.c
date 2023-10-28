
#ifdef USE_FBNEO_FMOPL
#include "aud_sys/fbneo/fmoplneo.h"
#elif defined(GP2X_940)
#include "gp2x/fmopl.h"
#else
#include "aud_sys/mame/fmopl.h"
#endif
#include "aud_sys/dosbox/dbopl.h"
#include "aud_sys/nukedopl3/opl3.h"
#include "wl_def.h"
#include "id_opl.h"
#include <stdio.h>

static Chip dboplChip;
static opl3_chip nkoplChip;

int FMOPL_Init(int numChips, int clock, int rate)
{
    if (YM3812Init(numChips, clock, rate))
    {
        printf("Unable to create virtual OPL!!\n");
    }
    return 0;
}

int FMOPL_Write(int which, int a, int v)
{
    return YM3812Write(which, a, v);
}

void FMOPL_UpdateOne(int which, short* buffer, int length)
{
    YM3812UpdateOne(which, buffer, length);
}

int DBOPL_Init(int numChips, int clock, int rate)
{ 
	DBOPL_InitTables();
	Chip__Chip(&dboplChip);
	Chip__Setup(&dboplChip, rate);
	return false;
}

void DBOPL_Write(int which, unsigned int reg, unsigned char val)
{
	Chip__WriteReg(&dboplChip, reg, val);
}

void DBOPL_UpdateOne(int which, short* stream, int length)
{
	int buffer[2048 * 2];
	int i;

	/*
	** length should be at least the max. samplesPerMusicTick
	** in Catacomb 3-D and Keen 4-6, which is param_samplerate / 700.
	** So 512 is sufficient for a sample rate of 358.4 kHz, which is
	** significantly higher than the OPL rate anyway.
	*/
	if (length > 2048)
		length = 2048;

	Chip__GenerateBlock2(&dboplChip, length, buffer);

	/*
	** GenerateBlock2 generates a number of "length" 32-bit mono samples
	** so we only need to convert them to 16-bit mono samples
	*/
	for (i = 0; i < length; i++)
	{
		/* Scale volume */
		int sample = 2 * buffer[i];
		if (sample > 32767) sample = 32767;
		else if (sample < -32768) sample = -32768;
#if defined(MIXER_SAMPLE_FORMAT_FLOAT)
		stream[i] = (float)sample / 32767.0f;
#elif defined (MIXER_SAMPLE_FORMAT_SINT16)
		stream[i] = sample;
#else
		stream[i * 2] = stream[i * 2 + 1] = (short)sample;
#endif
	}
}

int NKOPL3_Init(int numChips, int clock, int rate)
{
	OPL3_Reset(&nkoplChip, rate);
	return 1;
}

void NKOPL3_Write(int which, unsigned int reg, unsigned char val)
{
	OPL3_WriteReg(&nkoplChip, (unsigned short)reg, (unsigned char)val);
}

void NKOPL3_UpdateOne(int which, short* stream, int length)
{
	short buffer[2048 * 2];
	int i;

	/*
	** length should be at least the max. samplesPerMusicTick
	** in Catacomb 3-D and Keen 4-6, which is param_samplerate / 700.
	** So 512 is sufficient for a sample rate of 358.4 kHz, which is
	** significantly higher than the OPL rate anyway.
	*/
	if (length > 2048)
		length = 2048;

	OPL3_GenerateStream(&nkoplChip, buffer, length);

	/*
	** OPL3_GenerateStream generates a number of "length" 32-bit stereo samples
	** so we only need to convert them to 16-bit samples
	*/
	for (i = 0; i < length * 2; i++)  /* * 2 for left/right channel */
	{
		/* Multiply by 4 to match loudness of MAME emulator. */
		short sample = buffer[i] << 2;
		if (sample > 32767) sample = 32767;
		else if (sample < -32768) sample = -32768;
		stream[i] = sample;
	}
}