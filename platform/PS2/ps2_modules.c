#include "ps2_modules.h"

#include <kernel.h>
#include <stdio.h>
#include <ps2_all_drivers.h>


#ifdef RESET_IOP
void resetIOP()
{
    SifInitRpc(0);
#ifdef RESET_IOP	
    while (!SifIopReset("", 0)){};
#else
    while (!SifIopReset(NULL, 0)) {};
#endif
    while (!SifIopSync()){};
    SifInitRpc(0);
}
#endif

void PS2_Load_RPC_Patches()
{
	sbv_patch_enable_lmb();
	sbv_patch_fileio();
}

void PS2_Unload_RPC_Patches(void)
{
	sbv_patch_disable_prefix_check();
}

void PS2_Load_Modules(void)
{
	init_memcard_driver(true);
#ifdef USB	
	init_usb_driver(true);
#endif
#ifdef CDFS
	init_cdfs_driver();
#endif
	init_joystick_driver(true);
	init_audio_driver();
	init_poweroff_driver();
#ifdef HDD
	init_hdd_driver(true, true);
#endif
}

void PS2_Unload_Modules()
{
	deinit_poweroff_driver();
	deinit_audio_driver();
	deinit_joystick_driver(false);
#ifdef USB	
	deinit_usb_driver(false);
#endif
#ifdef CDFS
	deinit_cdfs_driver();
#endif
	deinit_memcard_driver(true);
#ifdef HDD
	deinit_hdd_driver(false);
#endif
}
