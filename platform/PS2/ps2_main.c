/* PS2_MAIN.C */
#include "ps2_mc.h"
#include "ps2_modules.h"
#include "ps2_data_loader.h"
#include "ps2_main.h"
#include "ps2_data_loader.h"
#include <ps2_printf.h>

void PS2_Started()
{
#ifdef IOP
    ps2_printf_XY("Reset IOP", 4, 20, 20);
    resetIOP();
#endif
    ps2_printf_XY("Load SBV Patches...", 4, 20, 20);
    PS2_Load_RPC_Patches();
    ps2_printf_XY("Loading modules...", 4, 20, 20);
	PS2_Load_Modules();
    /* ps2_printf("Loading MC...", 4);
    ** PS2_Load_Memory_Card();
    ** ps2_printf_XY("Loading the Loader", 4, 20, 20);
    ** PS2_SDL_LoaderStart();
    */
}

void PS2_Shutdown()
{
    ps2_printf_XY("Unloading SBV Patches", 4, 20, 20);
    PS2_Unload_RPC_Patches();
    ps2_printf_XY("Unloading MC...", 4, 20, 20);
    PS2_Unload_Memory_Card();
    ps2_printf_XY("Unloading modules...", 4, 20, 20);
    PS2_Unload_Modules();
}
