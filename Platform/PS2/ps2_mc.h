//PS2_MC.H
#include "wl_def.h"
extern boolean PS2_Init_Memory_Card_Type();
extern void PS2_Get_Memory_Card0_Info(); 
extern void PS2_Get_Memory_Card1_Info();
//todo: cleanup...
extern void PS2_SaveFile_Memory_Card0(const char *filename);
extern void PS2_SaveFile_Memory_Card1(const char *filename);
//extern void PS2_Load_SaveFile_Memory_Card0();
//extern void PS2_Load_SaveFile_Memory_Card1();
extern void PS2_Load_Memory_Card();
extern void PS2_Unload_Memory_Card();
