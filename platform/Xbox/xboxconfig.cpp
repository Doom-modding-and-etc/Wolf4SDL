#include "xboxconfig.h"
#include <stdlib.h>

#include "SDL.h"
#include "xboxjoy.h"

#include "SDL_config_lib.h"
#include "SDL_config_auto_tests.h"


/*  ----------------------------------------- */

CFG_File config;
int result;

/*  ----------------------------------------- */

void XboxLoadConfig (void)
 {
  result = CFG_OpenFile("D:\\xbox.cfg", &config );
  Uint32 flags = SDL_INIT_VIDEO;

  /* CFG_OpenFile returns value, that allows you to check if everything went right, or do we have problems */
  if ( CFG_ERROR == result )
   {
    result = CFG_OpenFile((0), &config );
   }

  xpos = CFG_ReadInt("xpos", 0);
  ypos = CFG_ReadInt("ypos", 0);
  xstretch = CFG_ReadInt("xstretch", 0);
  ystretch = CFG_ReadInt("ystretch", 0);

  flickerfilter = CFG_ReadInt("flickerfilter", 1);
  if (flickerfilter>5) flickerfilter=5;
	else if (flickerfilter<0) flickerfilter=0;

  m720p = CFG_ReadInt("720p", 1);
  if (m720p) flags |= SDL_INIT_VIDEO720;
  SDL_Init(flags);
 }

 void XboxSaveConfig (void)
 {
	CFG_WriteInt("xpos", xpos);
	CFG_WriteInt("ypos", ypos);
	CFG_WriteInt("xstretch", xstretch);
	CFG_WriteInt("ystretch", ystretch);
	CFG_WriteInt("flickerfilter", flickerfilter);
	CFG_WriteInt("720p", m720p);

	CFG_SaveFile("D:\\xbox.cfg", CFG_SORT_ORIGINAL, CFG_COMPRESS_OUTPUT);

  /* Close selected file (we would pass to it 'config' variable instead of 0, if it wasn't selected) */
	CFG_CloseFile(0);

 }