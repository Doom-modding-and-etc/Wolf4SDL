//PS2_DATA_LOADER.H
#include "../../wl_def.h"

#include "ps2_mc.h"

/* General Structure for the ps2 dualshock joystick. */
/*
typedef enum
{
    NONE = 0,
    START, //1
    SELECT, //2
    L1, // 3
    L2, // 4
    R1, // 5
    R2, //6
    lStick, //7
    rStick, //8
    dPAD_UP, //9
    dPAD_DOWN, //10
    dAD_LEFT, //11
    dPAD_RIGHT //12
    btX //13
    btO, //14
    btSquare, //15
    btTriangle, //16
    btDefault, //17
    btMax, //18
} Control_Presses;
*/
extern boolean PS2_SDL_CreateWindow();
extern void PS2_SDL_Shutdown();
extern void PS2_SDL_LoaderStart(void);
extern SDL_Surface* PS2_SDL_LoadBMP(const char* filename);
extern boolean PS2_SDL_LoadBitmapImgs();

extern SDL_Surface *loader_surface;
extern SDL_Surface *loader_image;
extern SDL_Surface *loader_icn;
/* extern SDL_Surface *loader_image_btPresses[btMax]; */
extern SDL_Window *loader_window;
