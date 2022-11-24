#ifdef PS2
#include "ps2_main.h"

void PS2_Started()
{
    PS2_Load_RPC_Patches();
	PS2_Load_Modules();
    PS2_Load_Memory_Card();
}

void PS2_Shutdown()
{
    PS2_Unload_Modules();
    PS2_Unload_Memory_Card();
}
#endif