// ID_PM.H

#ifndef __ID_PM_H_
#define __ID_PM_H_

#ifdef _XBOX
#include <xtl.h>
#endif

#define PMPageSize             (TEXTURESIZE * TEXTURESIZE)

#define PM_GetSpritePage(v)    PM_GetPage (PMSpriteStart + (v))
#ifndef SEGA_SATURN
#define PM_GetSoundPage(v)     PM_GetPage (PMSoundStart + (v))
#endif
#ifndef SEGA_SATURN
extern word ChunksInFile;
#endif
extern word PMSpriteStart;
#ifndef SEGA_SATURN
extern word PMSoundStart;
extern boolean PMSoundInfoPagePadded;
#endif 
extern word *pageLengths;
extern byte **PMPages;

void     PM_Startup (void);
void     PM_Shutdown (void);
#ifndef SEGA_SATURN
uint32_t PM_GetPageSize (int page);
#endif
byte     *PM_GetPage (int page);
#ifndef SEGA_SATURN
byte     *PM_GetPageEnd (void);
#endif
#ifdef SEGA_SATURN
uint8_t* PM_DecodeSprites(unsigned int start, unsigned int endi, uint8_t* ptr, uint32_t* pageOffsets, word* pageLengths, Uint8* Chunks);
extern static inline byte* PM_GetTexture(int wallpic);
extern static inline uint16_t* PM_GetSprite(int shapenum);
#endif

#endif
