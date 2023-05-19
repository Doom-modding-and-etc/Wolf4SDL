#include <xtl.h>

#include "xboxjoy.h"

int xpos = 0;
int ypos = 0;
int xstretch = 0;
int ystretch = 0;
int flickerfilter = 1;
int m720p = 1;
extern LPDIRECT3DDEVICE8 D3D_Device;

void XboxSetScreen (void)
{
if (flickerfilter>5) flickerfilter=5;
 else if (flickerfilter<0) flickerfilter=0;
IDirect3DDevice8_SetFlickerFilter(D3D_Device, flickerfilter);
SDL_XBOX_SetScreenPosition(xpos, ypos);
SDL_XBOX_SetScreenStretch(xstretch, ystretch);
SDL_ShowCursor(SDL_DISABLE);
}

void XboxInitVideo (int flags)
{
	SDL_XBOX_PresentationInterval(flags);
}
