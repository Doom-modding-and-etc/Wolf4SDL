#include "ps2_modules.h"
#ifdef PS2

#ifdef RESET_IOP
void resetIOP()
{
    SifInitRpc(0);
    while (!SifIopReset("", 0)){};
    while (!SifIopSync()){};
    SifInitRpc(0);
}
#else
void initRPC()
{
  SifInitRpc(0);
  while (!SifIopReset(NULL, 0)) {};
  while (!SifIopSync()){};
  SifInitRpc(0);
}
#endif

void PS2_Load_RPC_Patches()
{
	sbv_patch_enable_lmb();
    sbv_patch_disable_prefix_check();
	sbv_patch_fileio();
}

void PS2_Load_Modules(void)
{
#ifdef RESET_IOP
	resetIOP();
#else
	initRPC();
#endif
	PS2_Load_RPC_Patches();
	init_fileXio_driver();
	init_memcard_driver(true);
	init_usb_driver(true);
	init_cdfs_driver();
	init_joystick_driver(true);
	init_audio_driver();
	init_poweroff_driver();
	init_hdd_driver(true, true);
}

void PS2_Unload_Modules()
{
	deinit_poweroff_driver();
	deinit_audio_driver();
	deinit_joystick_driver(false);
	deinit_usb_driver(false);
	deinit_cdfs_driver();
	deinit_memcard_driver(true);
	deinit_hdd_driver(false);
	deinit_fileXio_driver();
}
#endif