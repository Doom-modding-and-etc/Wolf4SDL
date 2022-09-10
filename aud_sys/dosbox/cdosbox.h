#include "../../version.h"
#ifdef USE_DOSBOX
#include <stdio.h>
#include "../wl_def.h"
#if defined(_arch_dreamcast)
#	include "dc/dc_main.h"
#elif !defined(_WIN32)
#	include <stdint.h>
#endif
#include <SDL.h>

typedef uint32_t	Bitu;
typedef int32_t		Bits;
typedef uint32_t	Bit32u;
typedef int32_t		Bit32s;
typedef uint16_t	Bit16u;
typedef int16_t		Bit16s;
typedef uint8_t		Bit8u;
typedef int8_t		Bit8s;

//Use 8 handlers based on a small logatirmic wavetabe and an exponential table for volume
#define WAVE_HANDLER	10
//Use a logarithmic wavetable with an exponential table for volume
#define WAVE_TABLELOG	11
//Use a linear wavetable with a multiply table for volume
#define WAVE_TABLEMUL	12

//Select the type of wave generator routine
#define DBOPL_WAVE WAVE_TABLEMUL

#if (DBOPL_WAVE == WAVE_HANDLER)
typedef Bits(DB_FASTCALL* WaveHandler) (Bitu i, Bitu volume);
#endif

typedef Bits (*Operator_VolumeHandler) ();
typedef struct CChannel* (*Channel_SynthHandler) (struct CChip* chip, Bit32u samples, Bit32s output);


struct COperator
{
	Operator_VolumeHandler volHandler;
#if (DBOPL_WAVE == WAVE_HANDLER)
	WaveHandler waveHandler;	//Routine that generate a wave
#else
	Bit16s* waveBase;
	Bit32u waveMask;
	Bit32u waveStart;
#endif
	Bit32u waveIndex;			//WAVE_BITS shifted counter of the frequency index
	Bit32u waveAdd;				//The base frequency without vibrato
	Bit32u waveCurrent;			//waveAdd + vibratao

	Bit32u chanData;			//Frequency/octave and derived data coming from whatever channel controls this
	Bit32u freqMul;				//Scale channel frequency with this, TODO maybe remove?
	Bit32u vibrato;				//Scaled up vibrato strength
	Bit32s sustainLevel;		//When stopping at sustain level stop here
	Bit32s totalLevel;			//totalLevel is added to every generated volume
	Bit32u currentLevel;		//totalLevel + tremolo
	Bit32s volume;				//The currently active volume

	Bit32u attackAdd;			//Timers for the different states of the envelope
	Bit32u decayAdd;
	Bit32u releaseAdd;
	Bit32u rateIndex;			//Current position of the evenlope

	Bit8u rateZero;				//Bits for the different states of the envelope having no changes
	Bit8u keyOn;				//Bitmask of different values that can generate keyon
	//Registers, also used to check for changes
	Bit8u reg20, reg40, reg60, reg80, regE0;
	//Active part of the envelope we're in
	Bit8u state;
	//0xff when tremolo is enabled
	Bit8u tremoloMask;
	//Strength of the vibrato
	Bit8u vibStrength;
	//Keep track of the calculated KSR so we can check for changes
	Bit8u ksr;
};

struct CChannel
{
	struct COperator op[2];

	Channel_SynthHandler synthHandler;
	Bit32u chanData;		//Frequency/octave and derived values
	Bit32s old[2];			//Old data for feedback

	Bit8u feedback;			//Feedback shift
	Bit8u regB0;			//Register values to check for changes
	Bit8u regC0;
	//This should correspond with reg104, bit 6 indicates a Percussion channel, bit 7 indicates a silent channel
	Bit8u fourMask;
	Bit8s maskLeft;		//Sign extended values for both channel's panning
	Bit8s maskRight;
};

struct CChip
{
	//This is used as the base counter for vibrato and tremolo
	Bit32u lfoCounter;
	Bit32u lfoAdd;


	Bit32u noiseCounter;
	Bit32u noiseAdd;
	Bit32u noiseValue;

	//Frequency scales for the different multiplications
	Bit32u freqMul[16];
	//Rates for decay and release for rate of this chip
	Bit32u linearRates[76];
	//Best match attack rates for the rate of this chip
	Bit32u attackRates[76];

	//18 channels with 2 operators each
	struct CChannel chan[18];

	Bit8u reg104;
	Bit8u reg08;
	Bit8u reg04;
	Bit8u regBD;
	Bit8u vibratoIndex;
	Bit8u tremoloIndex;
	Bit8s vibratoSign;
	Bit8u vibratoShift;
	Bit8u tremoloValue;
	Bit8u vibratoStrength;
	Bit8u tremoloStrength;
	//Mask for allowed wave forms
	Bit8u waveFormMask;
	//0 or -1 when enabled
	Bit8s opl3Active;

};
//Private:
//Operator:
extern void Operator_SetState(Bit8u s);
#ifdef WIP
void Operator_UpdateAttack(const struct Chip* chip);
void Operator_UpdateRelease(const struct Chip* chip);
void Operator_UpdaateDecay(const struct Chip* chip);
#endif
//Channel:
struct COperator* Channel_Op(Bitu index);
#ifdef WIP
DOSBOX_API void Channel_SetChanData(const struct Chip* chip, Bit32u data);
DOSBOX_API void Channel_UpdateFrequency(const struct Chip* chip, Bit8u fourOp);
DOSBOX_API void Channel_WriteA0(const struct Chip* chip, Bit8u val);
DOSBOX_API void Channel_WriteB0(const struct Chip* chip, Bit8u val);
DOSBOX_API void Channel_WriteC0(const struct Chip* chip, Bit8u val);
DOSBOX_API void Channel_ResetC0(const struct Chip* chip);
DOSBOX_API void Channel_GeneratePercussion(struct Chip* chip, Bit32s* output);
DOSBOX_API struct Channel* Channel_BlockTemplate(struct Chip* chip, Bit32u samples, Bit32s* output);
#endif
//Chip:
extern Bit32u Chip_ForwardLFO(Bit32u samples);
extern Bit32u Chip_ForwardNoise();
extern void Chip_WriteBD(Bit8u val);
extern void Chip_WriteReg(Bit32u reg, Bit8u val);
extern Bit32u Chip_WriteAddr(Bit32u port, Bit8u val);
extern void Chip_GenerateBlock2(Bitu samples, Bit32s* output);
extern void Chip_GenerateBlock3(Bitu samples, Bit32s* output);
extern void Chip_Setup(Bit32u r);


//Public:
extern void Operator_UpdateAttenuation();
#ifdef WIP
DOSBOX_API void Operator_UpdateRates(const struct CChip* chip);
#endif
extern void Operator_UpdateFrequency();
#ifdef WIP
DOSBOX_API void Operator_Write20(const struct Chip* chip, Bit8u val);
DOSBOX_API void Operator_Write40(const struct Chip* chip, Bit8u val);
DOSBOX_API void Operator_Write60(const struct Chip* chip, Bit8u val);
DOSBOX_API void Operator_Write80(const struct Chip* chip, Bit8u val);
DOSBOX_API void Operator_WriteE0(const struct Chip* chip, Bit8u val);
#endif
extern const boolean Operator_Silent();
#ifdef WIP
DOSBOX_API void Operator_Prepare(const struct Chip* chip);
#endif
extern void Operator_KeyOn(Bit8u mask);
extern void Operator_KeyOff(Bit8u mask);
extern Bits Operator_TemplateVolume();
extern Bit32s Operator_RateForward(Bit32u add);
extern Bitu Operator_ForwardWave();
extern Bitu Operator_ForwardVolume();
extern Bits Operator_GetSample(Bits modulation);
extern Bits Operator_GetWave(Bitu index, Bitu vol);
#endif