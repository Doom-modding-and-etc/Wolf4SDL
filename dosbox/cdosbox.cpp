
#include "cdosbox.h"
#ifdef USE_DOSBOX
#include "dbopl.h"

DBOPL::Operator op;
DBOPL::Operator::State state;
DBOPL::Channel channel;
DBOPL::Chip chips;

//Operator
 
//Private:
//Public:
void Operator_UpdateAttenuation()
{
	op.UpdateAttenuation();
}

void Operator_UpdateFrequency()
{
	op.UpdateFrequency();
}

const bool Operator_Silent()
{
	return op.Silent();
}

void Operator_KeyOn(Bit8u mask)
{
	op.KeyOn(mask);
}

void Operator_KeyOff(Bit8u mask)
{
	op.KeyOff(mask);
}

Bit32s Operator_RateForward(Bit32u add)
{
	op.RateForward(add);
}

Bitu Operator_ForwardWave()
{
	op.ForwardWave();
}

Bitu Operator_ForwardVolume()
{
	op.ForwardVolume();
}

Bits Operator_GetSample(Bits modulation)
{
	op.GetSample(modulation);
}

Bits Operator_GetWave(Bitu index, Bitu vol)
{
	op.GetWave(index, vol);
}

//Channel:
//Private:
struct Operator* Channel_Op(Bitu index)
{
	channel.Op(index);
}

//Chip:
//Private:
Bit32u Chip_ForwardLFO(Bit32u samples)
{
	return chips.ForwardLFO(samples);
}
Bit32u Chip_ForwardNoise()
{
	return chips.ForwardNoise();
}
void Chip_WriteBD(Bit8u val)
{
	chips.WriteBD(val);
}
void Chip_WriteReg(Bit32u reg, Bit8u val)
{
	chips.WriteReg(reg, val);
}
Bit32u Chip_WriteAddr(Bit32u port, Bit8u val)
{
	return chips.WriteAddr(port, val);
}
void Chip_GenerateBlock2(Bitu samples, Bit32s* output)
{
	chips.GenerateBlock2(samples, output);
}
void Chip_GenerateBlock3(Bitu samples, Bit32s* output)
{
	chips.GenerateBlock3(samples, output);
}
void Chip_Generate(Bit32u samples)
{
	chips.Generate(samples);
}
void Chip_Setup(Bit32u r)
{
	chips.Setup(r);
}

#endif