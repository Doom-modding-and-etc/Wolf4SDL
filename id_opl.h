#ifndef	__ID_OPL_H_
#define	__ID_OPL_H_

#ifdef __cplusplus
extern "C"
{
#endif

int FMOPL_Init(int numChips, int clock, int rate);
int FMOPL_Write(int which, int a, int v);
void FMOPL_UpdateOne(int which, short* buffer, int length);

int DBOPL_Init(int numChips, int clock, int rate);
void DBOPL_Write(int which, unsigned int reg, unsigned char val);
void DBOPL_UpdateOne(int which, short* stream, int length);

int NKOPL3_Init(int numChips, int clock, int rate);
void NKOPL3_Write(int which, unsigned int reg, unsigned char val);
void NKOPL3_UpdateOne(int which, short* stream, int length);


#ifdef __cplusplus
}
#endif

#endif