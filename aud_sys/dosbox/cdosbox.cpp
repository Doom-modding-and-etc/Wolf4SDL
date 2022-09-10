#include "cdosbox.h"

#ifdef USE_DOSBOX
#include "dbopl.h"
using namespace DBOPL;
struct Operator op;
Operator::State state;
struct Channel channel;
struct Chip chips;


template<Operator::State Yes>
void Operator_TemplateVolumeWrapper()
{
	op.TemplateVolume(Yes);
}

void Operator_SetState(Bit8u s)
{
	op.SetState(s);
}

//Public:
void Operator_UpdateAttenuation()
{
	op.UpdateAttenuation();
}

void Operator_UpdateFrequency()
{
	op.UpdateFrequency();
}

const boolean Operator_Silent()
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
	return op.RateForward(add);
}

Bitu Operator_ForwardWave()
{
	return op.ForwardWave();
}

Bitu Operator_ForwardVolume()
{
	return op.ForwardVolume();
}

Bits Operator_GetSample(Bits modulation)
{
	return op.GetSample(modulation);
}

Bits Operator_GetWave(Bitu index, Bitu vol)
{
	return op.GetWave(index, vol);
}

//Channel:
//Private:
struct COperator* Channel_Op(Bitu index)
{
	channel.Op(index);
}

Bits Operator_TemplateVolume()
{

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

void Chip_Setup(Bit32u r)
{
	chips.Setup(r);
}

#endif