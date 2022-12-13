//PS2_MAIN.C
#ifdef PS2
#include "ps2_main.h"
#include "ps2_data_loader.h"
#include <ps2_printf.h>

void PS2_Started()
{
#ifdef RESET_IOP
    ps2_printf("Reset IOP", 4);
    resetIOP();
#else
    ps2_printf("Init RPC", 4);
    initRPC();
#endif
    ps2_printf("Load SBV Patches...", 4);
    PS2_Load_RPC_Patches();
    ps2_printf("Loading modules...", 4);
	PS2_Load_Modules();
    //ps2_printf("Loading MC...", 4);
    //PS2_Load_Memory_Card();
    ps2_printf("Loading the Loader", 4);
    PS2_SDL_LoaderStart();
}

void PS2_Shutdown()
{
    ps2_printf("Undloadind SBV Patches", 4);
    PS2_Unload_RPC_Patches();
    ps2_printf("Unloading MC...", 4);
    PS2_Unload_Memory_Card();
    ps2_printf("Unloading modules...", 4);
    PS2_Unload_Modules();
}

#endif