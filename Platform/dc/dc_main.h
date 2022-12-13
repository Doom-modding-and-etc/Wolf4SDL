//Wolf4SDL\DC
//dc_main.h
//2009 - Cyle Terry

#ifndef __DC_MAIN_H_
#define __DC_MAIN_H_


//dc_cd.c
int DC_CheckDrive();

//dc_main.c
void DC_Init();
void DC_CheckArguments();
int  DC_CheckForMaps(char *path);
#ifdef  SPEAR
#ifndef SPEARDEMO
int  DC_SetMission(char *path);
#endif
#endif

//dc_maple.c
int  DC_ButtonPress(int button);
int  DC_MousePresent();
void DC_WaitButtonPress(int button);
void DC_WaitButtonRelease(int button);

//dc_video.c
void DC_VideoInit();
void DC_DrawString(int x, int y, char *str);
void DC_CLS();
void DC_Flip();

//dc_vmu.c
extern void DiskFlopAnim(int x, int y);
       void DC_StatusDrawLCD(int index);
       void DC_StatusClearLCD();
       void DC_SaveToVMU(char *fname, char *str);
       int  DC_LoadFromVMU(char *fname);
#endif
