#ifdef PS2
#include <ps2_all_drivers.h>
#include <kernel.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef RESET_IOP
extern void resetIOP();
#endif

extern void PS2_Load_Modules(void);
extern void PS2_Unload_Modules(void);
extern bool PS2_Load_RPC_Patches();
#endif