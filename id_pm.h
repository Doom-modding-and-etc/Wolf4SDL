// ID_PM.H

#ifndef __ID_PM_H_
#define __ID_PM_H_

#define PMPageSize             (TEXTURESIZE * TEXTURESIZE)

#define PM_GetSpritePage(v)    PM_GetPage (PMSpriteStart + (v))
#ifndef SEGA_SATURN
#define PM_GetSoundPage(v)     PM_GetPage (PMSoundStart + (v))
#endif
#ifndef SEGA_SATURN
extern unsigned short ChunksInFile;
#endif
extern unsigned short PMSpriteStart;
#ifndef SEGA_SATURN
extern unsigned short PMSoundStart;
extern boolean PMSoundInfoPagePadded;
#endif 
extern unsigned short *pageLengths;
extern unsigned char **PMPages;

void     PM_Startup (void);
void     PM_Shutdown (void);
#ifndef SEGA_SATURN
unsigned int PM_GetPageSize (int page);
#endif
unsigned char     *PM_GetPage (int page);
#ifndef SEGA_SATURN
unsigned char     *PM_GetPageEnd (void);
#endif
#ifdef SEGA_SATURN
unsigned char* PM_DecodeSprites(unsigned int start, unsigned int endi, unsigned char* ptr, unsigned int* pageOffsets, word* pageLengths, unsigned char* Chunks);
extern static inline unsigned char* PM_GetTexture(int wallpic);
extern static inline unsigned short* PM_GetSprite(int shapenum);
#endif

#endif
