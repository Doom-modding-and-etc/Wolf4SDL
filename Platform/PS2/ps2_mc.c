//PS2_MC.C
#ifdef PS2
#include "ps2_mc.h"
#include "wl_def.h"
#include <stdio.h>

bool PS2_Init_Memory_Card_Type()
{
    int ret;
    bool success = true; //Initialize the memory;

#ifdef USE_MC
    ret = mcInit(MC_TYPE_MC);
    if(!ret)
    {
        ps2_printf("Failed to initialize server\n", 3);
        return false;
    }
    else
    {
        ps2_printf("Initialized server.\n", 2);
        return true;
    }
#else
    ret = mcInit(MC_TYPE_XMC);
    if(!ret)
    {
       ps2_printf("Failed to initialize server\n", 3);
       return success = false;
    }
    else
    {
        ps2_printf("Initialized server with sucess\n", 2);
        return true;
    }
#endif
    ret = mcInit(MC_TYPE_PSX);
    if(!ret)
    {
        ps2_printf("Failed to detect PSX Memory Card\n", 3);
        return success = false;
    }
    else
    {
        ps2_printf("Detected PSX Memory Card, Initializing...", 3);
        return true;
    }

    ret = mcInit(MC_TYPE_PS2);
    if(!ret)
    {
        ps2_printf("Failed to detect PS2 Memory Card\n", 3);
        return success = false;
    }
    else
    {
        ps2_printf("Detected PS2 Memory Card, Initializing...\n", 3);
        return success = true;
    }

    ret = mcInit(MC_TYPE_POCKET);
    if(!ret)
    {
        ps2_printf("Failed to detect POCKET Memory Card\n", 3);
        return success = false;
    }
    else
    {
        ps2_printf("Detected POCKET Memory Card, Initializing...\n", 3);
        return success = true;
    }   
    
    ret = mcInit(MC_TYPE_NONE);
    if(!ret)
    {
       ps2_printf("Unknown type of Memory Card Detected", 3);
       return success = true;
    }
    else
    {
        ps2_printf("There´s no memory card inserted", 3);
        PS2_Unload_Memory_Card();
        return success = false;
    }
    return success;
}

void PS2_Get_Memory_Card0_Info()
{
    bool success = true;
    int ret;
    int type, free, info;
    ret = mcGetInfo(0, 0, &type, &free, &info);
    if(!ret)
    {
        ps2_printf("Failed to get information of the memory card on slot 1", 3);      
        return success = false;
    }
    else
    {
        mcSync(0, NULL, &ret);
        ps2_printf("mcGetInfo returned %d\n", 4, ret);
	    ps2_printf("Type: %d Free: %d Format: %d\n\n", 4, type, free, info);//Cosmito
        return success = true;
    }
}

void PS2_Get_Memory_Card1_Info()
{
    bool success;
    int ret;
    int type, free, info;  
    ret = mcGetInfo(1, 1, &type, &free, &info);
    if(!ret)
    {
        ps2_printf("Failed to get information of the memory card on slot 2", 3);      
        return success = false;
    }
    else
    {
        mcSync(0, NULL, &ret);
        ps2_printf("mcGetInfo returned %d\n", 3, ret);
	    ps2_printf("Type: %d Free: %d Format: %d\n\n", 3, type, free, info);//Cosmito
        return success = true;
    }
}

void PS2_SaveFile_Memory_Card0(const char* filename)
{
    sceMcTblGetDir dir;
    int ret, fd;
    bool success;
    PS2_Init_Memory_Card_Type(); //Get the what type of memory card;

    mcGetDir(0, 0, "mc0:data", NULL, NULL, NULL); //
    fd = open(filename, O_RDWR);
    //TBD: Add write here:
    close(fd);
    mcClose(0);
}

void PS2_SaveFile_Memory_Card1(const char *filename)
{
    sceMcTblGetDir dir;
    int ret, fd;
    PS2_Init_Memory_Card_Type(); //Get the what type of memory card;
    mcGetDir(0, 0, "mc1:data/", NULL, NULL, NULL); 
 

    fd = open(filename, O_RDWR);
    //TBD: Add write here:
    close(fd);
    mcClose(0);
}

void PS2_Load_Memory_Card()
{
    PS2_Init_Memory_Card_Type();
    PS2_Get_Memory_Card0_Info();
    PS2_Get_Memory_Card1_Info();
}

void PS2_Unload_Memory_Card()
{
    mcClose(0);
}

#endif
