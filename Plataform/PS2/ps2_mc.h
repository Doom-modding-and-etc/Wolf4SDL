#ifdef PS2
#include <libmc.h>
#include <stdbool.h>

extern bool PS2_Init_Memory_Card_Type();
extern bool PS2_Get_Memory_Card0_Info(); 
extern bool PS2_Get_Memory_Card1_Info();
//TODO:
//extern void PS2_Save_Memory0_Card();
//extern void PS2_Save_Memory1_Card();
extern void PS2_Load_Memory_Card();
extern void PS2_Unload_Memory_Card();
#endif