//PS2_MAIN.H

#ifdef PS2
#include "ps2_mc.h"
#include "ps2_modules.h"
#include "ps2_data_loader.h"
extern void PS2_Started(); //Hack: Load all the functions...
extern void PS2_Shutdown(); //Hack: Unload all the functions...
#endif