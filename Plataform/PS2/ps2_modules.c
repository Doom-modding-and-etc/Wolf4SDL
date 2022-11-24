#include "ps2_modules.h"
#ifdef PS2

#ifdef RESET_IOP
void resetIOP()
{
	#ifdef RESET_IOP  
    SifInitRpc(0);
    while (!SifIopReset("", 0)){};
    while (!SifIopSync()){};
    SifInitRpc(0);
    #endif
}
#endif

void PS2_Load_Modules(void)
{
#ifdef RESET_IOP
	resetIOP();
#endif
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

bool PS2_Load_RPC_Patches()
{
	int ret;
	bool success = true;

	ret = sbv_patch_enable_lmb();
	if(ret <= 0)
	{
		printf("Failed to enable lmb patch");
		return success = false;
	}

    ret = sbv_patch_disable_prefix_check();
    if(ret <= 0)
	{
		printf("Failed to disable the prefix check");
		return success = false;
	}

    ret = sbv_patch_fileio();
	if(ret <= 0)
	{
		printf("Failed to patch the file io");
		return success = false;
	}
  return success;
}

#endif