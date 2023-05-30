/* ID_VL.C */

#include <string.h>
#include "wl_def.h"
#ifdef CRT
#include "id_crt.h"
#endif


#if defined(_arch_dreamcast)
boolean usedoublebuffering = false;
unsigned int screenWidth = 640;
unsigned int screenHeight = 400;
#elif defined(GP2X)
boolean usedoublebuffering = true;
unsigned int screenWidth = 320;
unsigned int screenHeight = 240;
#if defined(GP2X_940)
unsigned int     screenBits = 8;
#else
unsigned int      screenBits = 16;
#endif
#elif defined(PS2)
boolean usedoublebuffering = true;
unsigned int screenWidth = 640;
unsigned int screenHeight = 448;
unsigned int screenBits = 8;
#elif defined(N3DS)
boolean usedoublebuffering = true;
unsigned int screenWidth = 400;
unsigned int screenHeight = 240;
unsigned int screenBits = 32;      /* use "best" color depth according to libSDL */  /* ADDEDFIX 0 */
#else
boolean usedoublebuffering = true;
#if defined(SCALE2X) 
unsigned int screenWidth = 320;
unsigned int screenHeight = 200;
static unsigned int scaledScreenWidth = 640;
static unsigned int scaledScreenHeight = 405;
#else
#ifdef SEGA_SATURN
unsigned int screenWidth = SATURN_WIDTH;
unsigned int screenHeight = SATURN_HEIGHT;
#else
unsigned int screenWidth = 640;
unsigned int screenHeight = 405;
#endif
#endif
unsigned int screenBits = 8;      /* use "best" color depth according to libSDL */
#endif

SDL_Surface *screen = NULL;
#ifdef SAVE_GAME_SCREENSHOT
SDL_Surface* lastGameSurface = NULL;
#endif
SDL_Surface* screenBuffer = NULL;

unsigned int screenPitch;
unsigned int bufferPitch;

#if SDL_MAJOR_VERSION == 2
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
#if defined(SCALE2X) 
SDL_Texture* upscaledTexture;
#endif
#endif

int      scaleFactor;

boolean	 screenfaded;
#ifndef SEGA_SATURN
unsigned bordercolor;

unsigned int *ylookup;

SDL_Color palette1[256], palette2[256];
#endif
SDL_Color curpal[256];


static size_t rndbits_y;
static size_t rndmask;


#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];
#ifdef SEGA_SATURN
#define WRGB(r, g, b) {(r)*255/63, (g)*255/63, 0x100(b)*255/63, 0}
#else
#define WRGB(r, g, b) {(r)*255/63, (g)*255/63, (b)*255/63, 0}
#endif
SDL_Color gamepal[] =
{
#ifdef HEADER
#ifdef SPEAR
    #include "sodpal.inc"
#else
    #include "wolfpal.inc"
#endif
#else
#ifdef SPEAR
WRGB(0,  0,  0),WRGB(0,  0, 42),WRGB(0, 42,  0),WRGB(0, 42, 42),WRGB(42,  0,  0),
WRGB(42,  0, 42),WRGB(42, 21,  0),WRGB(42, 42, 42),WRGB(21, 21, 21),WRGB(21, 21, 63),
WRGB(21, 63, 21),WRGB(21, 63, 63),WRGB(63, 21, 21),WRGB(63, 21, 63),WRGB(63, 63, 21),
WRGB(63, 63, 63),WRGB(59, 59, 59),WRGB(55, 55, 55),WRGB(52, 52, 52),WRGB(48, 48, 48),
WRGB(45, 45, 45),WRGB(42, 42, 42),WRGB(38, 38, 38),WRGB(35, 35, 35),WRGB(31, 31, 31),
WRGB(28, 28, 28),WRGB(25, 25, 25),WRGB(21, 21, 21),WRGB(18, 18, 18),WRGB(14, 14, 14),
WRGB(11, 11, 11),WRGB(8,  8,  8),WRGB(63,  0,  0),WRGB(59,  0,  0),WRGB(56,  0,  0),
WRGB(53,  0,  0),WRGB(50,  0,  0),WRGB(47,  0,  0),WRGB(44,  0,  0),WRGB(41,  0,  0),
WRGB(38,  0,  0),WRGB(34,  0,  0),WRGB(31,  0,  0),WRGB(28,  0,  0),WRGB(25,  0,  0),
WRGB(22,  0,  0),WRGB(19,  0,  0),WRGB(16,  0,  0),WRGB(63, 54, 54),WRGB(63, 46, 46),
WRGB(63, 39, 39),WRGB(63, 31, 31),WRGB(63, 23, 23),WRGB(63, 16, 16),WRGB(63,  8,  8),
WRGB(63,  0,  0),WRGB(63, 42, 23),WRGB(63, 38, 16),WRGB(63, 34,  8),WRGB(63, 30,  0),
WRGB(57, 27,  0),WRGB(51, 24,  0),WRGB(45, 21,  0),WRGB(39, 19,  0),WRGB(63, 63, 54),
WRGB(63, 63, 46),WRGB(63, 63, 39),WRGB(63, 63, 31),WRGB(63, 62, 23),WRGB(63, 61, 16),
WRGB(63, 61,  8),WRGB(63, 61,  0),WRGB(57, 54,  0),WRGB(51, 49,  0),WRGB(45, 43,  0),
WRGB(39, 39,  0),WRGB(33, 33,  0),WRGB(28, 27,  0),WRGB(22, 21,  0),WRGB(16, 16,  0),
WRGB(52, 63, 23),WRGB(49, 63, 16),WRGB(45, 63,  8),WRGB(40, 63,  0),WRGB(36, 57,  0),
WRGB(32, 51,  0),WRGB(29, 45,  0),WRGB(24, 39,  0),WRGB(54, 63, 54),WRGB(47, 63, 46),
WRGB(39, 63, 39),WRGB(32, 63, 31),WRGB(24, 63, 23),WRGB(16, 63, 16),WRGB(8, 63,  8),
WRGB(0, 63,  0),WRGB(0, 63,  0),WRGB(0, 59,  0),WRGB(0, 56,  0),WRGB(0, 53,  0),
WRGB(1, 50,  0),WRGB(1, 47,  0),WRGB(1, 44,  0),WRGB(1, 41,  0),WRGB(1, 38,  0),
WRGB(1, 34,  0),WRGB(1, 31,  0),WRGB(1, 28,  0),WRGB(1, 25,  0),WRGB(1, 22,  0),
WRGB(1, 19,  0),WRGB(1, 16,  0),WRGB(54, 63, 63),WRGB(46, 63, 63),WRGB(39, 63, 63),
WRGB(31, 63, 62),WRGB(23, 63, 63),WRGB(16, 63, 63),WRGB(8, 63, 63),WRGB(0, 63, 63),
WRGB(0, 57, 57),WRGB(0, 51, 51),WRGB(0, 45, 45),WRGB(0, 39, 39),WRGB(0, 33, 33),
WRGB(0, 28, 28),WRGB(0, 22, 22),WRGB(0, 16, 16),WRGB(23, 47, 63),WRGB(16, 44, 63),
WRGB(8, 42, 63),WRGB(0, 39, 63),WRGB(0, 35, 57),WRGB(0, 31, 51),WRGB(0, 27, 45),
WRGB(0, 23, 39),WRGB(54, 54, 63),WRGB(46, 47, 63),WRGB(39, 39, 63),WRGB(31, 32, 63),
WRGB(23, 24, 63),WRGB(16, 16, 63),WRGB(8,  9, 63),WRGB(0,  1, 63),WRGB(0,  0, 63),
WRGB(0,  0, 59),WRGB(0,  0, 56),WRGB(0,  0, 53),WRGB(0,  0, 50),WRGB(0,  0, 47),
WRGB(0,  0, 44),WRGB(0,  0, 41),WRGB(0,  0, 38),WRGB(0,  0, 34),WRGB(0,  0, 31),
WRGB(0,  0, 28),WRGB(0,  0, 25),WRGB(0,  0, 22),WRGB(0,  0, 19),WRGB(0,  0, 16),
WRGB(10, 10, 10),WRGB(63, 56, 13),WRGB(63, 53,  9),WRGB(63, 51,  6),WRGB(63, 48,  2),
WRGB(63, 45,  0),WRGB(0, 14,  0),WRGB(0, 10,  0),WRGB(38,  0, 57),WRGB(32,  0, 51),
WRGB(29,  0, 45),WRGB(24,  0, 39),WRGB(20,  0, 33),WRGB(17,  0, 28),WRGB(13,  0, 22),
WRGB(10,  0, 16),WRGB(63, 54, 63),WRGB(63, 46, 63),WRGB(63, 39, 63),WRGB(63, 31, 63),
WRGB(63, 23, 63),WRGB(63, 16, 63),WRGB(63,  8, 63),WRGB(63,  0, 63),WRGB(56,  0, 57),
WRGB(50,  0, 51),WRGB(45,  0, 45),WRGB(39,  0, 39),WRGB(33,  0, 33),WRGB(27,  0, 28),
WRGB(22,  0, 22),WRGB(16,  0, 16),WRGB(63, 58, 55),WRGB(63, 56, 52),WRGB(63, 54, 49),
WRGB(63, 53, 47),WRGB(63, 51, 44),WRGB(63, 49, 41),WRGB(63, 47, 39),WRGB(63, 46, 36),
WRGB(63, 44, 32),WRGB(63, 41, 28),WRGB(63, 39, 24),WRGB(60, 37, 23),WRGB(58, 35, 22),
WRGB(55, 34, 21),WRGB(52, 32, 20),WRGB(50, 31, 19),WRGB(47, 30, 18),WRGB(45, 28, 17),
WRGB(42, 26, 16),WRGB(40, 25, 15),WRGB(39, 24, 14),WRGB(36, 23, 13),WRGB(34, 22, 12),
WRGB(32, 20, 11),WRGB(29, 19, 10),WRGB(27, 18,  9),WRGB(23, 16,  8),WRGB(21, 15,  7),
WRGB(18, 14,  6),WRGB(16, 12,  6),WRGB(14, 11,  5),WRGB(10,  8,  3),WRGB(24,  0, 25),
WRGB(0, 25, 25),WRGB(0, 24, 24),WRGB(0,  0,  7),WRGB(0,  0, 11),WRGB(12,  9,  4),
WRGB(18,  0, 18),WRGB(20,  0, 20),WRGB(0,  0, 13),WRGB(7,  7,  7),WRGB(19, 19, 19),
WRGB(23, 23, 23),WRGB(16, 16, 16),WRGB(12, 12, 12),WRGB(13, 13, 13),WRGB(54, 61, 61),
WRGB(46, 58, 58),WRGB(39, 55, 55),WRGB(29, 50, 50),WRGB(18, 48, 48),WRGB(8, 45, 45),
WRGB(8, 44, 44),WRGB(0, 41, 41),WRGB(0, 38, 38),WRGB(0, 35, 35),WRGB(0, 33, 33),
WRGB(0, 31, 31),WRGB(0, 30, 30),WRGB(0, 29, 29),WRGB(0, 28, 28),WRGB(0, 27, 27),
WRGB(38,  0, 34)
#else
WRGB(0,  0,  0),WRGB(0,  0, 42),WRGB(0, 42,  0),WRGB(0, 42, 42),WRGB(42,  0,  0),
WRGB(42,  0, 42),WRGB(42, 21,  0),WRGB(42, 42, 42),WRGB(21, 21, 21),WRGB(21, 21, 63),
WRGB(21, 63, 21),WRGB(21, 63, 63),WRGB(63, 21, 21),WRGB(63, 21, 63),WRGB(63, 63, 21),
WRGB(63, 63, 63),WRGB(59, 59, 59),WRGB(55, 55, 55),WRGB(52, 52, 52),WRGB(48, 48, 48),
WRGB(45, 45, 45),WRGB(42, 42, 42),WRGB(38, 38, 38),WRGB(35, 35, 35),WRGB(31, 31, 31),
WRGB(28, 28, 28),WRGB(25, 25, 25),WRGB(21, 21, 21),WRGB(18, 18, 18),WRGB(14, 14, 14),
WRGB(11, 11, 11),WRGB(8,  8,  8),WRGB(63,  0,  0),WRGB(59,  0,  0),WRGB(56,  0,  0),
WRGB(53,  0,  0),WRGB(50,  0,  0),WRGB(47,  0,  0),WRGB(44,  0,  0),WRGB(41,  0,  0),
WRGB(38,  0,  0),WRGB(34,  0,  0),WRGB(31,  0,  0),WRGB(28,  0,  0),WRGB(25,  0,  0),
WRGB(22,  0,  0),WRGB(19,  0,  0),WRGB(16,  0,  0),WRGB(63, 54, 54),WRGB(63, 46, 46),
WRGB(63, 39, 39),WRGB(63, 31, 31),WRGB(63, 23, 23),WRGB(63, 16, 16),WRGB(63,  8,  8),
WRGB(63,  0,  0),WRGB(63, 42, 23),WRGB(63, 38, 16),WRGB(63, 34,  8),WRGB(63, 30,  0),
WRGB(57, 27,  0),WRGB(51, 24,  0),WRGB(45, 21,  0),WRGB(39, 19,  0),WRGB(63, 63, 54),
WRGB(63, 63, 46),WRGB(63, 63, 39),WRGB(63, 63, 31),WRGB(63, 62, 23),WRGB(63, 61, 16),
WRGB(63, 61,  8),WRGB(63, 61,  0),WRGB(57, 54,  0),WRGB(51, 49,  0),WRGB(45, 43,  0),
WRGB(39, 39,  0),WRGB(33, 33,  0),WRGB(28, 27,  0),WRGB(22, 21,  0),WRGB(16, 16,  0),
WRGB(52, 63, 23),WRGB(49, 63, 16),WRGB(45, 63,  8),WRGB(40, 63,  0),WRGB(36, 57,  0),
WRGB(32, 51,  0),WRGB(29, 45,  0),WRGB(24, 39,  0),WRGB(54, 63, 54),WRGB(47, 63, 46),
WRGB(39, 63, 39),WRGB(32, 63, 31),WRGB(24, 63, 23),WRGB(16, 63, 16),WRGB(8, 63,  8),
WRGB(0, 63,  0),WRGB(0, 63,  0),WRGB(0, 59,  0),WRGB(0, 56,  0),WRGB(0, 53,  0),
WRGB(1, 50,  0),WRGB(1, 47,  0),WRGB(1, 44,  0),WRGB(1, 41,  0),WRGB(1, 38,  0),
WRGB(1, 34,  0),WRGB(1, 31,  0),WRGB(1, 28,  0),WRGB(1, 25,  0),WRGB(1, 22,  0),
WRGB(1, 19,  0),WRGB(1, 16,  0),WRGB(54, 63, 63),WRGB(46, 63, 63),WRGB(39, 63, 63),
WRGB(31, 63, 62),WRGB(23, 63, 63),WRGB(16, 63, 63),WRGB(8, 63, 63),WRGB(0, 63, 63),
WRGB(0, 57, 57),WRGB(0, 51, 51),WRGB(0, 45, 45),WRGB(0, 39, 39),WRGB(0, 33, 33),
WRGB(0, 28, 28),WRGB(0, 22, 22),WRGB(0, 16, 16),WRGB(23, 47, 63),WRGB(16, 44, 63),
WRGB(8, 42, 63),WRGB(0, 39, 63),WRGB(0, 35, 57),WRGB(0, 31, 51),WRGB(0, 27, 45),
WRGB(0, 23, 39),WRGB(54, 54, 63),WRGB(46, 47, 63),WRGB(39, 39, 63),WRGB(31, 32, 63),
WRGB(23, 24, 63),WRGB(16, 16, 63),WRGB(8,  9, 63),WRGB(0,  1, 63),WRGB(0,  0, 63),
WRGB(0,  0, 59),WRGB(0,  0, 56),WRGB(0,  0, 53),WRGB(0,  0, 50),WRGB(0,  0, 47),
WRGB(0,  0, 44),WRGB(0,  0, 41),WRGB(0,  0, 38),WRGB(0,  0, 34),WRGB(0,  0, 31),
WRGB(0,  0, 28),WRGB(0,  0, 25),WRGB(0,  0, 22),WRGB(0,  0, 19),WRGB(0,  0, 16),
WRGB(10, 10, 10),WRGB(63, 56, 13),WRGB(63, 53,  9),WRGB(63, 51,  6),WRGB(63, 48,  2),
WRGB(63, 45,  0),WRGB(45,  8, 63),WRGB(42,  0, 63),WRGB(38,  0, 57),WRGB(32,  0, 51),
WRGB(29,  0, 45),WRGB(24,  0, 39),WRGB(20,  0, 33),WRGB(17,  0, 28),WRGB(13,  0, 22),
WRGB(10,  0, 16),WRGB(63, 54, 63),WRGB(63, 46, 63),WRGB(63, 39, 63),WRGB(63, 31, 63),
WRGB(63, 23, 63),WRGB(63, 16, 63),WRGB(63,  8, 63),WRGB(63,  0, 63),WRGB(56,  0, 57),
WRGB(50,  0, 51),WRGB(45,  0, 45),WRGB(39,  0, 39),WRGB(33,  0, 33),WRGB(27,  0, 28),
WRGB(22,  0, 22),WRGB(16,  0, 16),WRGB(63, 58, 55),WRGB(63, 56, 52),WRGB(63, 54, 49),
WRGB(63, 53, 47),WRGB(63, 51, 44),WRGB(63, 49, 41),WRGB(63, 47, 39),WRGB(63, 46, 36),
WRGB(63, 44, 32),WRGB(63, 41, 28),WRGB(63, 39, 24),WRGB(60, 37, 23),WRGB(58, 35, 22),
WRGB(55, 34, 21),WRGB(52, 32, 20),WRGB(50, 31, 19),WRGB(47, 30, 18),WRGB(45, 28, 17),
WRGB(42, 26, 16),WRGB(40, 25, 15),WRGB(39, 24, 14),WRGB(36, 23, 13),WRGB(34, 22, 12),
WRGB(32, 20, 11),WRGB(29, 19, 10),WRGB(27, 18,  9),WRGB(23, 16,  8),WRGB(21, 15,  7),
WRGB(18, 14,  6),WRGB(16, 12,  6),WRGB(14, 11,  5),WRGB(10,  8,  3),WRGB(24,  0, 25),
WRGB(0, 25, 25),WRGB(0, 24, 24),WRGB(0,  0,  7),WRGB(0,  0, 11),WRGB(12,  9,  4),
WRGB(18,  0, 18),WRGB(20,  0, 20),WRGB(0,  0, 13),WRGB(7,  7,  7),WRGB(19, 19, 19),
WRGB(23, 23, 23),WRGB(16, 16, 16),WRGB(12, 12, 12),WRGB(13, 13, 13),WRGB(54, 61, 61),
WRGB(46, 58, 58),WRGB(39, 55, 55),WRGB(29, 50, 50),WRGB(18, 48, 48),WRGB(8, 45, 45),
WRGB(8, 44, 44),WRGB(0, 41, 41),WRGB(0, 38, 38),WRGB(0, 35, 35),WRGB(0, 33, 33),
WRGB(0, 31, 31),WRGB(0, 30, 30),WRGB(0, 29, 29),WRGB(0, 28, 28),WRGB(0, 27, 27),
WRGB(38,  0, 34)
#endif
#endif
};

/* XOR masks for the pseudo-random number sequence starting with n=17 bits */
static const size_t rndmasks[] = {
    /* n    XNOR from (starting at 1, not 0 as usual) */
0x00012000,     /* 17   17,14 */
0x00020400,     /* 18   18,11 */
0x00040023,     /* 19   19,6,2,1 */
0x00090000,     /* 20   20,17 */
0x00140000,     /* 21   21,19 */
0x00300000,     /* 22   22,21 */
0x00420000,     /* 23   23,18 */
0x00e10000,     /* 24   24,23,22,17 */
0x01200000,     /* 25   25,22      (this is enough for 8191x4095) */
};


#ifndef SEGA_SATURN
CASSERT(lengthof(gamepal) == 256)
#endif

/* ========================================================================== */

/* Returns the number of bits needed to represent the given value */
static size_t log2_ceil(unsigned int x)
{
    int n = 0;
    unsigned int v = 1;
    
    while (v < x)
    {
        n++;
        v <<= 1;
    }
    return n;
}

/*
=======================
=
= VL_Startup
=
=======================
*/

void VL_Startup()
{
    size_t rndbits_x = log2_ceil(screenWidth);
    size_t rndbits;
    rndbits_y = log2_ceil(screenHeight);

    rndbits = rndbits_x + rndbits_y;
    if (rndbits < 17)
        rndbits = 17;       /* no problem, just a bit slower */
    else if (rndbits > 25)
        rndbits = 25;       /* fizzle fade will not fill whole screen */

    rndmask = rndmasks[rndbits - 17];
}

void VWL_MeasureString(const char* string, unsigned short* width, unsigned short* height, fontstruct* font)
{
#ifdef SEGA_SATURN
    * height = SWAP_BYTES_16(font->height);
#else
    * height = font->height;
#endif
    for (*width = 0; *string; string++)
        *width += font->width[*((unsigned char*)string)];	/* proportional width */

}

void VL_MeasurePropString(const char* string, unsigned short* width, unsigned short* height)
{
    VWL_MeasureString(string, width, height, (fontstruct*)grsegs[STARTFONT + fontnumber]);
}

/*
=======================
=
= VL_Shutdown
=
=======================
*/

void VL_Shutdown (void)
{
    SDL_FreeSurface (screenBuffer);
#if SDL_MAJOR_VERSION == 2
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
#endif
    free (ylookup);
    free (pixelangle);
    free (wallheight);
#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
    free (spanstart);
    spanstart = NULL;
#endif
    screenBuffer = NULL;
    ylookup = NULL;
    pixelangle = NULL;
    wallheight = NULL;
}

/*
===================
=
= FizzleFade
=
= returns true if aborted
=
= It uses maximum-length Linear Feedback Shift Registers (LFSR) counters.
= You can find a list of them with lengths from 3 to 168 at:
= http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
= Many thanks to Xilinx for this list!!!
=
===================
*/

boolean FizzleFade(SDL_Surface* source, int x1, int y1,
    unsigned int width, unsigned int height, unsigned int frames, boolean abortable)
{
#ifdef SEGA_SATURN
#if 1
    unsigned x, y, p, frame, pixperframe;
    int  rndval, lastrndval;
    int      first = 1;
    unsigned char* srcptr;
    unsigned char color;
    unsigned char* destptr;
    int i;
    SDL_Rect rect;

    lastrndval = 0;
    pixperframe = width * height / frames;

    IN_StartAck();

    frame = GetTimeCount();
    srcptr = (unsigned char*)source->pixels;
    color = (srcptr[x1 + (y1 * width)] ? 0 : 4);

    rect = { x1,y1,width,height };

    if (curSurface == screen)
    {
        curSurface = screenBuffer;
        VL_BarScaledCoord(x1, y1, width, height, srcptr[x1 + (y1 * width)]); /* vbt ajout */
        DrawStatusBar(); /* vbt : ajout */
    }
    else
    {
        VL_BarScaledCoord(x1, y1, width, height, color); /* vbt ajout */		
        DrawStatusBar(); /* vbt : ajout */
        curSurface = screen;
    }

    do
    {
        if (abortable && IN_CheckAck())
        {
#ifndef USE_SPRITES
            /*            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
            **            SDL_UpdateRect(screen, 0, 0, 0, 0);
            */
#endif

            /* xxx	VGAClearScreen(); /* vbt : maj du fond d'�cran * /
            ** curSurface = source;
            */
            VL_BarScaledCoord(x1, y1, width, height, color); /* vbt ajout */
            return true;
        }


        destptr = (unsigned char*)dest->pixels;

        rndval = lastrndval;

        /* 
        ** When using double buffering, we have to copy the pixels of the last AND the current frame.
        ** Only for the first frame, there is no "last frame"
        */
        for (i = first; i < 2; i++)
        {

            for (p = 0; p < pixperframe; p++)
            {
                /*
                ** seperate random value into x/y pair
                */

                x = rndval >> rndbits_y;
                y = rndval & ((1 << rndbits_y) - 1);

                /*
                ** advance to next random element
                */

                rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : rndmask);

                if (x >= width || y >= height)
                {
                    if (rndval == 0)     /* entire sequence has been completed */
                        goto finished;
                    p--;
                    continue;
                }

                /*
                ** copy one pixel
                */

                *(destptr + (y1 + y) * dest->pitch + x1 + x) = *(srcptr + (y1 + y) * source->pitch + x1 + x);

                if (rndval == 0)		/* entire sequence has been completed */
                    goto finished;
            }

            if (!i || first) lastrndval = rndval;
        }
        first = 0;

        SDL_BlitSurface(dest, &rect, source, &rect);
        frame++;

        Delay(frame - GetTimeCount());        /* don't go too fast */
    } while (1);


finished:
    SDL_BlitSurface(dest, &rect, source, &rect);
    return false;
#endif
#else
    size_t x, y, p, pixperframe, frame;
    size_t  rndval = 0, lastrndval = 0;
    size_t      i, first = 1;
    unsigned char* srcptr;
    pixperframe = width * height / frames;

    IN_StartAck();

    frame = GetTimeCount();
    srcptr = VL_LockSurface(source);
    if (srcptr == NULL) return false;

    do
    {
        unsigned char* destptr;
        IN_ProcessEvents();

        if (abortable && IN_CheckAck())
        {
            VL_UnlockSurface(source);
            VL_UpdateScreen(source);
            return true;
        }

        destptr = VL_LockSurface(screen);

        if (destptr != NULL)
        {
            rndval = lastrndval;

            /* When using double buffering, we have to copy the pixels of the last AND the current frame. */
            /* Only for the first frame, there is no "last frame" */
            for (i = first; i < 2; i++)
            {
                for (p = 0; p < pixperframe; p++)
                {
                    /*
                    ** seperate random value into x/y pair
                    */

                    x = rndval >> rndbits_y;
                    y = rndval & ((1 << rndbits_y) - 1);

                    /*
                    ** advance to next random element
                    */

                    rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : rndmask);

                    if (x >= width || y >= height)
                    {
                        if (rndval == 0)     /* entire sequence has been completed */
                            goto finished;
                        p--;
                        continue;
                    }

                    /*
                    ** copy one pixel
                    */

                    if (screenBits == 8)
                    {
                        *(destptr + (y1 + y) * screen->pitch + x1 + x)
                            = *(srcptr + (y1 + y) * source->pitch + x1 + x);
                    }
                    else
                    {
                        unsigned char col = *(srcptr + (y1 + y) * source->pitch + x1 + x);
                        unsigned int fullcol = SDL_MapRGB(screen->format, curpal[col].r, curpal[col].g, curpal[col].b);
                        memcpy(destptr + (y1 + y) * screen->pitch + (x1 + x) * screen->format->BytesPerPixel,
                            &fullcol, screen->format->BytesPerPixel);
                    }

                    if (rndval == 0)		/* entire sequence has been completed */
                        goto finished;
                }

                if (!i || first) lastrndval = rndval;
            }

            /* If there is no double buffering, we always use the "first frame" case */
            if (usedoublebuffering) first = 0;

            VL_UnlockSurface(screen);
            VL_UpdateScreen(screen);
        }
        else
        {
            /* No surface, so only enhance rndval */
            for (i = first; i < 2; i++)
            {
                for (p = 0; p < pixperframe; p++)
                {
                    rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : rndmask);
                    if (rndval == 0)
                        goto finished;
                }
            }
        }

        frame++;
        Delay((int)frame - (int)GetTimeCount());        /* don't go too fast */
    } while (1);

finished:
    VL_UnlockSurface(source);
    VL_UnlockSurface(screen);
    VL_UpdateScreen(source);
    return false;
#endif
}


/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode (void)
{
    unsigned int i;
#if SDL_MAJOR_VERSION == 2
    unsigned int a,r,g,b;
#endif
#ifdef SPEAR
    const char* title = "Spear of Destiny";
#else
    const char* title = "Wolfenstein 3D";
#endif

#if SDL_MAJOR_VERSION == 1
#ifdef SEGA_SATURN
    if (screenBits == -1)
    {
        const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo();
        screenBits = vidInfo->vfmt->BitsPerPixel;
    }

    if (screen == NULL)
    {
        screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBits,
            SDL_SWSURFACE | (screenBits == 8 ? SDL_HWPALETTE : 0));
    }

    if (!screen)
    {
        /*         
        **        printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth,
        **            screenHeight, screenBits, SDL_GetError());
        */
        SYS_Exit(1);
    }


    if (screenBuffer == NULL)
    {
        screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight, 8, 0, 0, 0, 0);
        if (!screenBuffer)
        {
            /*        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());          */
            SYS_Exit(1);
        }

        /*    
        **    SDL_ShowCursor(SDL_DISABLE);
        ** curSurface = screenBuffer;
        ** curPitch = screenBuffer->pitch;
        */
    }
        SDL_SetColors(screen, gamepal, 0, 256);
    memcpyl(curpal, gamepal, sizeof(SDL_Color) * 256);

    scaleFactor = screenWidth / SATURN_WIDTH;
    if (screenHeight / 200 < scaleFactor) scaleFactor = screenHeight / 200;
    if (pixelangle == NULL) SafeMalloc(pixelangle) = (short*)malloc(screenWidth * sizeof(short));
#else
#ifdef CRT
    /* 
    ** Fab's CRT Hack
    ** Adjust height so the screen is 4:3 aspect ratio
    */
    screenHeight = screenWidth * 3.0 / 4.0;
#endif  
    SDL_WM_SetCaption(title, NULL);

    if (screenBits == -1)
    {
        const SDL_VideoInfo *vidInfo = SDL_GetVideoInfo();
        screenBits = vidInfo->vfmt->BitsPerPixel;
    }

#if N3DS
    screen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_TOPSCR | SDL_CONSOLEBOTTOM | SDL_DOUBLEBUF);
#else
    screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBits,
        (usedoublebuffering ? SDL_HWSURFACE | SDL_DOUBLEBUF : 0) | (screenBits == 8 ? SDL_HWPALETTE : 0)
        | (fullscreen ? SDL_FULLSCREEN : 0) | (SDL_OPENGL ? true : false));
#endif

    if(!screen)
    {
        printf("Unable to set %ux%ux%ux video mode: %s\n", screenWidth,
            screenHeight, screenBits, SDL_GetError());
        exit(1);
    }
    if((screen->flags & SDL_DOUBLEBUF) != SDL_DOUBLEBUF)
        usedoublebuffering = false;

    SDL_SetColors(screen, gamepal, 0, 256);
    memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);

#ifdef CRT  
    /* Fab's and André´s CRT Hack */
    CRT_Init(screen);
#endif 
    screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth,
        screenHeight, 8, 0, 0, 0, 0);
    if(!screenBuffer)
    {
        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetColors(screenBuffer, gamepal, 0, 256);
#endif
#elif SDL_MAJOR_VERSION == 2
#ifdef CRT
    /* 
    ** Fab's CRT Hack:
    ** Adjust height so the screen is 4:3 aspect ratio
    */
    screenHeight = screenWidth * 3 / 4;
#endif     
#if defined(SCALE2X) 
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scaledScreenWidth, scaledScreenHeight,
        (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (SDL_WINDOW_OPENGL ? true : false));
#else
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
        (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (SDL_WINDOW_OPENGL ? true : false));
#endif
    SDL_PixelFormatEnumToMasks (SDL_PIXELFORMAT_ARGB8888, (int*)&screenBits, &r, &g, &b, &a);

    screen = SDL_CreateRGBSurface(0,screenWidth,screenHeight,screenBits,r,g,b,a);

    if(!screen)
    {
#if defined(SCALE2X) 
        printf("Unable to set %ux%ux%i video mode: %s\n", scaledScreenWidth, scaledScreenHeight, screenBits, SDL_GetError());
#else
        printf("Unable to set %ux%ux%i video mode: %s\n", screenWidth, screenHeight, screenBits, SDL_GetError());
#endif
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    SDL_SetPaletteColors(screen->format->palette, gamepal, 0, 256);
    memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);
    
#ifdef CRT  
    /* Fab's and André´s CRT Hack */
    CRT_Init(screen);
#endif
    screenBuffer = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 
    8, 0, 0, 0, 0);
    
    if(!screenBuffer)
    {
        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetPaletteColors(screenBuffer->format->palette, gamepal, 0, 256);

#if defined(SCALE2X) 
    /* Create the intermediate texture that we render the screen surface into. */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
#endif
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

    if (!texture)
    {
        printf("Unable to create screen texture: %s\n", SDL_GetError());
        exit(1);
    }

    /*
    ** Create the up-scaled texture that we render to the window. We use 'nearest' scaling here because depending on the
    ** window size, the texture may need to be scaled by a non-integer factor.
    */
#if defined(SCALE2X) 
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    upscaledTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, scaledScreenWidth, scaledScreenHeight);
    if (!upscaledTexture)
    {
        printf("Unable to create up-scaled texture: %s\n", SDL_GetError());
        exit(1);
    }
#endif
#endif

    SDL_ShowCursor(SDL_DISABLE);

    screenPitch = screen->pitch;
    bufferPitch = screenBuffer->pitch;

    scaleFactor = screenWidth/320;
    if(screenHeight/200 < (unsigned int)scaleFactor) scaleFactor = screenHeight/200;

    ylookup = wlcast_conversion(unsigned int*, SafeMalloc(screenHeight * sizeof(*ylookup)));
    pixelangle = wlcast_conversion(short*, SafeMalloc(screenWidth * sizeof(*pixelangle)));
    wallheight = wlcast_conversion(short*, SafeMalloc(screenWidth * sizeof(*wallheight)));
#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
    spanstart = wlcast_conversion(short*, SafeMalloc((screenHeight / 2) * sizeof(*spanstart)));
#endif

    for (i = 0; i < screenHeight; i++)
        ylookup[i] = i * bufferPitch;
}

#if SDL_MAJOR_VERSION == 2
void VL_RenderTextures()
{
    SDL_UpdateTexture(texture, NULL, screen->pixels, screenWidth * sizeof(unsigned int));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
#endif

#if !defined(SEGA_SATURN) && !defined(USE_SPRITE)
void VL_UpdateScreen(SDL_Surface* surface)
{
    SDL_BlitSurface(surface, NULL, screen, NULL);
#if SDL_MAJOR_VERSION == 1
    SDL_Flip(screen);
#elif SDL_MAJOR_VERSION == 2
    VL_RenderTextures();
#endif
}
#endif

/*
=============================================================================

						PALETTE OPS

		To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/

/*
=================
=
= VL_ConvertPalette
=
=================
*/

void VL_ConvertPalette(unsigned char *srcpal, SDL_Color *destpal, int numColors)
{
    int i;

    for(i=0; i<numColors; i++)
    {
#if N3DS
        destpal[i].r = *srcpal++;
        destpal[i].g = *srcpal++;
        destpal[i].b = *srcpal++;
#else
        destpal[i].r = *srcpal++ * 255 / 63;
        destpal[i].g = *srcpal++ * 255 / 63;
        destpal[i].b = *srcpal++ * 255 / 63;
#endif

    }
}

/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette (unsigned char red, unsigned char green, unsigned char blue)
{
    int i;
    SDL_Color pal[256];

    for(i=0; i<256; i++)
    {
        pal[i].r = red;
        pal[i].g = green;
        pal[i].b = blue;
    }

    VL_SetPalette(pal, true);
}

/* =========================================================================== */

/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor(int color, unsigned char red, unsigned char green, unsigned char blue)
{
#ifdef SEGA_SATURN
    memcpyl(curpal, palette, sizeof(SDL_Color) * 256);
#else
   
    SDL_Color col = 
    { 
        red,
        green,
        blue
    };
    
    curpal[color] = col;

    if(screenBits == 8)
#if SDL_MAJOR_VERSION == 1
        SDL_SetPalette(screen, SDL_PHYSPAL, &col, color, 1);
    else
    {
        SDL_SetPalette(screenBuffer, SDL_LOGPAL, &col, color, 1);

#ifdef CRT        
        CRT_Init(screen);
        CRT_DAC();
#else
        VL_UpdateScreen(screen);
#endif

#elif SDL_MAJOR_VERSION == 2
        SDL_SetPaletteColors(screen->format->palette, &col, color, 1);
    else
    {
        SDL_SetPaletteColors(screenBuffer->format->palette, &col, color, 1);
        SDL_BlitSurface(screenBuffer, NULL, screen, NULL);
#ifdef CRT        
        CRT_Init(screen);
        CRT_DAC();
#else
        VL_UpdateScreen(screen);
#endif
#endif
#endif
    }
}

/* =========================================================================== */

/* =========================================================================== */

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette(SDL_Color* palette, boolean forceupdate)
{
    memcpy(curpal, palette, sizeof(SDL_Color) * 256);

    if(screenBits == 8)
#if SDL_MAJOR_VERSION == 1
        SDL_SetPalette(screen, SDL_PHYSPAL, palette, 0, 256);
    else
    {
        SDL_SetPalette(screenBuffer, SDL_LOGPAL, palette, 0, 256);
        if(forceupdate)
        {
            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

	        VL_UpdateScreen(screen);
#else
        SDL_SetPaletteColors(screen->format->palette, palette, 0, 256);
    else
    {
        SDL_SetPaletteColors(screenBuffer->format->palette, palette, 0, 256);
        if (forceupdate)
        {
            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

            VL_UpdateScreen(screen);
#endif
        }
    }
}


/* =========================================================================== */

/*
=================
=
= VL_GetPalette
=
=================
*/

void VL_GetPalette (SDL_Color *palette)
{
    memcpy(palette, curpal, sizeof(SDL_Color) * 256);
}


/* =========================================================================== */

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut(int start, int end, unsigned char red, unsigned char green, unsigned char blue, int steps)
{
	int		    i,j,orig,delta;

    red = red * 255 / 63;
    green = green * 255 / 63;
    blue = blue * 255 / 63;

	VL_WaitVBL(1);
	VL_GetPalette(palette1);
	memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

/*
** fade through intermediate frames
*/
	for (i=0;i<steps;i++)
	{
        SDL_Color* origptr, * newptr;
		origptr = &palette1[start];
		newptr = &palette2[start];
		for (j=start;j<=end;j++)
		{
			orig = origptr->r;
			delta = red-orig;
			newptr->r = orig + delta * i / steps;
			orig = origptr->g;
			delta = green-orig;
			newptr->g = orig + delta * i / steps;
			orig = origptr->b;
			delta = blue-orig;
			newptr->b = orig + delta * i / steps;
			origptr++;
			newptr++;
		}

		if(!usedoublebuffering || screenBits == 8) VL_WaitVBL(1);
		VL_SetPalette (palette2, true);
	}

/*
** final color
*/
	VL_FillPalette (red,green,blue);

	screenfaded = true;
}


/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn (int start, int end, SDL_Color *palette, int steps)
{
	int j,delta;
    int i;
	VL_WaitVBL(1);
	VL_GetPalette(palette1);
	memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

/*
** fade through intermediate frames
*/
	for (i=0; i<steps; i++)
	{
		for (j=start;j<=end;j++)
		{
			delta = palette[j].r-palette1[j].r;
			palette2[j].r = palette1[j].r + delta * i / steps;
			delta = palette[j].g-palette1[j].g;
			palette2[j].g = palette1[j].g + delta * i / steps;
			delta = palette[j].b-palette1[j].b;
			palette2[j].b = palette1[j].b + delta * i / steps;
		}

		if(!usedoublebuffering || screenBits == 8) VL_WaitVBL(1);
		VL_SetPalette(palette2, true);
	}

/*
** final color
*/
	VL_SetPalette (palette, true);
	screenfaded = false;
}

/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

unsigned char *VL_LockSurface(SDL_Surface *surface)
{
#ifndef SEGA_SATURN
    if(SDL_MUSTLOCK(surface))
    {
        if(SDL_LockSurface(surface) < 0)
            return NULL;
    }
#endif
    return (unsigned char *) surface->pixels;
}

#ifndef SEGA_SATURN
void VL_UnlockSurface(SDL_Surface *surface)
{
    if(SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface);
    }
}

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot (int x, int y, int color)
{
    unsigned char *dest;

    wlassert(x >= 0 && (unsigned) x < screenWidth
            && y >= 0 && (unsigned) y < screenHeight
            && "VL_Plot: Pixel out of bounds!");

    dest = VL_LockSurface(screenBuffer);
    if(dest == NULL) return;

    dest[ylookup[y] + x] = color;

    VL_UnlockSurface(screenBuffer);
}
#endif 
/*
=================
=
= VL_GetPixel
=
=================
*/

#ifdef SAVE_GAME_SCREENSHOT
unsigned char VL_GetPixel(SDL_Surface* surface, int x, int y)
#else
unsigned char VL_GetPixel(int x, int y)
#endif
{
    unsigned char col;

    wlassert(x >= 0 && (unsigned) x < screenWidth
            && y >= 0 && (unsigned) y < screenHeight
            && "VL_GetPixel: Pixel out of bounds!");
#ifdef SEGA_SATURN
    return ((unsigned char*)surface->pixels)[y * pitch + x];
#else
#ifdef SAVE_GAME_SCREENSHOT
    if (!VL_LockSurface(surface))
        return 0;

    col = ((unsigned char*)screen->pixels)[ylookup[y] + x];

    VL_UnlockSurface(screen);
#else
    if (!VL_LockSurface(screenBuffer))
        return 0;

    col = ((unsigned char *) screenBuffer->pixels)[ylookup[y] + x];

    VL_UnlockSurface(screenBuffer);
#endif
    return col;
#endif
}

#ifdef SAVE_GAME_SCREENSHOT
/*
==============================
=
= SDL_DuplicateSurface
=
= Deep copies SDL_Surface
=
==============================
*/
SDL_Surface* SDL_DuplicateSurface(SDL_Surface* surf)
{
    SDL_Surface* cpy;
    size_t size;
    cpy = (SDL_Surface*)malloc(sizeof(SDL_Surface));
    memcpy((SDL_Surface*)cpy, (SDL_Surface*)surf, sizeof(SDL_Surface));
    cpy->format = (SDL_PixelFormat*)malloc(sizeof(SDL_PixelFormat));
    memcpy((SDL_PixelFormat*)cpy->format, (SDL_PixelFormat*)surf->format,
        sizeof(SDL_PixelFormat));
    size = surf->pitch * surf->h;
    cpy->pixels = malloc(size);
    memcpy((Uint8*)cpy->pixels, (Uint8*)surf->pixels, size * sizeof(Uint8));
    return cpy;
}

/*
==============================
=
= DrawPixel
=
= Draws the pixel onto the surface
=
==============================
*/
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16*)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}

/*
=================================
=
= SDL_ScaleSurface
=
= Creates a surface to scaled width, then scales it accordingly
= Width and Height can be any size, not multiples of 320x200
=
=================================
*/
SDL_Surface* SDL_ScaleSurface(SDL_Surface* Surface, Uint16 Width, Uint16 Height)
{
    SDL_Surface* _ret;
    double _stretch_factor_x, _stretch_factor_y;
    Sint32 y, x, o_y, o_x;

    if (!Surface || !Width || !Height)
        return 0;
    _ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, 8, 0, 0, 0, 0);
#if SDL_MAJOR_VERSION == 2
    SDL_SetPaletteColors(_ret, gamepal, 0, 256);
#else
    SDL_SetColors(_ret, gamepal, 0, 256);
#endif
    _stretch_factor_x = (double)Width  / (double)Surface->w;
    _stretch_factor_y = (double)Height / (double)Surface->h;

    for (y = 0; y < Surface->h; y++)
        for (x = 0; x < Surface->w; x++)
            for (o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for (o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    DrawPixel(_ret, (Sint32)_stretch_factor_x * x + o_x,
                        (Sint32)_stretch_factor_y * y + o_y, VL_GetPixel(Surface, x, y));
    return _ret;
}

/*
=================
=
= VL_SetSaveGameSlot
=
=================
*/
void VL_SetSaveGameSlot() {
    lastGameSurface = SDL_ScaleSurface(SDL_DuplicateSurface(screen), 128, 80);
}
#endif

/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin (unsigned x, unsigned y, unsigned width, int color)
{
    unsigned char *dest;

    wlassert(x >= 0 && x + width <= screenWidth
            && y >= 0 && y < screenHeight
            && "VL_Hlin: Destination rectangle out of bounds!");

    dest = VL_LockSurface(screenBuffer);
    if(dest == NULL) return;

    memset(dest + ylookup[y] + x, color, width);

#ifndef SEGA_SATURN
    VL_UnlockSurface(screenBuffer);
#endif
}


/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin (int x, int y, int height, int color)
{
    unsigned char *dest;

    wlassert(x >= 0 && (unsigned) x < screenWidth
			&& y >= 0 && (unsigned) y + height <= screenHeight
			&& "VL_Vlin: Destination rectangle out of bounds!");

	dest = VL_LockSurface(screenBuffer);
	if(dest == NULL) return;

	dest += ylookup[y] + x;

	while (height--)
	{
		*dest = color;
		dest += bufferPitch;
	}

#ifndef SEGA_SATURN
	VL_UnlockSurface(screenBuffer);
#endif
}


/*
=================
=
= VL_Bar
=
=================
*/

wlinline void VL_Bar (int x, int y, int width, int height, int color)
{
    VL_BarScaledCoord(scaleFactor*x, scaleFactor*y,scaleFactor*width, scaleFactor*height, color);
}

void VL_BarScaledCoord (int scx, int scy, int scwidth, int scheight, int color)
{
    unsigned char *dest;

#ifndef SEGA_SATURN
    wlassert(scx >= 0 && (unsigned) scx + scwidth <= screenWidth
			&& scy >= 0 && (unsigned) scy + scheight <= screenHeight
			&& "VL_BarScaledCoord: Destination rectangle out of bounds!");
#endif
	dest = VL_LockSurface(screenBuffer);
	if(dest == NULL) return;

	dest += ylookup[scy] + scx;

	while (scheight--)
	{
		memset(dest, color, scwidth);
		dest += bufferPitch;
	}
	VL_UnlockSurface(screenBuffer);
}

/*
============================================================================

							MEMORY OPS

============================================================================
*/


/*
===================
=
= VL_DePlaneVGA
=
= Unweave a VGA graphic to simplify drawing
=
===================
*/

void VL_DePlaneVGA (unsigned char *source, int width, int height)
{
    int  x,y,plane;
    unsigned short size,pwidth;
    unsigned char *temp,*dest,*srcline;

    size = width * height;

    if (width & 3)
        Quit ("DePlaneVGA: width not divisible by 4!");

    temp = wlcast_conversion(unsigned char*, SafeMalloc(size));

/*
** munge pic into the temp buffer
*/
    srcline = source;
    pwidth = width >> 2;

    for (plane = 0; plane < 4; plane++)
    {
        dest = temp;

        for (y = 0; y < height; y++)
        {
            for (x = 0; x < pwidth; x++)
                *(dest + (x << 2) + plane) = *srcline++;

            dest += width;
        }
    }

/*
** copy the temp buffer back into the original source
*/
    memcpy (source,temp,size);

    free (temp);
}


/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a block of data to the screen with scaling according to scaleFactor.
=
=================
*/

wlinline void VL_MemToScreen (unsigned char *source, int width, int height, int x, int y)
{
    VL_MemToScreenScaledCoord(source, width, height, scaleFactor*x, scaleFactor*y);
}

void VL_MemToScreenScaledCoord (unsigned char *source, int width, int height, int destx, int desty)
{
#ifdef SEGA_SATURN
    /*
    **    assert5(destx >= 0 && destx + width * scaleFactor <= screenWidth
    **            && desty >= 0 && desty + height * scaleFactor <= screenHeight
    **            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");
    */
    /*    VL_LockSurface(curSurface);     */
    unsigned char* vbuf = (unsigned char*)curSurface->pixels + (desty * curPitch) + destx;
    unsigned char w2 = width >> 2;
    unsigned int mul = w2 * height;

    /*	if(scaleFactor == 1)   */
    {
        for (unsigned int j = 0; j < height; j++)
        {
            for (unsigned int i = 0; i < width; i++)
            {
                vbuf[i] = source[(i >> 2) + (i & 3) * mul];
            }
            /* vbuf += curPitch; */
            source += w2;
        }
    }
    /*	else
        {
            for(unsigned int j=0,scj=0; j<height; j++, scj+=scaleFactor)
            {
                for(unsigned int i=0,sci=0; i<width; i++, sci+=scaleFactor)
                {
                    byte col = source[(j*w2+(i>>2))+(i&3)*mul];
                    for(unsigned m=0; m<scaleFactor; m++)
                    {
                        for(unsigned n=0; n<scaleFactor; n++)
                        {
                            vbuf[(scj+m)*curPitch+sci+n] = col;
                        }
                    }
                }
            }
        }*/
    VL_UnlockSurface(surface); /* vbt utile pour signon screen */

#else
    unsigned char *dest;
    int i, j, sci, scj;
    unsigned m, n;

    wlassert(destx >= 0 && destx + width * (int)scaleFactor <= (int)screenWidth
            && desty >= 0 && desty + height * (int)scaleFactor <= (int)screenHeight
            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    dest = VL_LockSurface(screenBuffer);
    if(dest == NULL) return;

    for(j = 0, scj = 0; j < height; j++, scj += scaleFactor)
    {
        for(i = 0, sci = 0; i < width; i++, sci += scaleFactor)
        {
            unsigned char col = source[(j * width) + i];
            for(m = 0; m < (unsigned int)scaleFactor; m++)
            {
                for(n = 0; n < (unsigned int)scaleFactor; n++)
                {
                    dest[ylookup[scj + m + desty] + sci + n + destx] = col;
                }
            }
        }
    }
    VL_UnlockSurface(screenBuffer);
#endif
}

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a part of a block of data to the screen.
= The block has the size origwidth*origheight.
= The part at (srcx, srcy) has the size width*height
= and will be painted to (destx, desty) with scaling according to scaleFactor.
=
=================
*/

void VL_MemToScreenScaledCoord2 (unsigned char *source, int origwidth, int origheight, int srcx, int srcy,
                                int destx, int desty, int width, int height)
{
    unsigned char *dest;
    int i, j, sci, scj;
    unsigned m, n;

    wlassert(destx >= 0 && destx + width * (int)scaleFactor <= (int)screenWidth
            && desty >= 0 && desty + height * (int)scaleFactor <= (int)screenHeight
            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    dest = VL_LockSurface(screenBuffer);
    if(dest == NULL) return;

    for(j = 0, scj = 0; j < height; j++, scj += scaleFactor)
    {
        for(i = 0, sci = 0; i < width; i++, sci += scaleFactor)
        {
            unsigned char col = source[((j + srcy) * origwidth) + (i + srcx)];

            for(m = 0; m < (unsigned int)scaleFactor; m++)
            {
                for(n = 0; n < (unsigned int)scaleFactor; n++)
                {
                    dest[ylookup[scj + m + desty] + sci + n + destx] = col;
                }
            }
        }
    }
    VL_UnlockSurface(screenBuffer);
}

/* ========================================================================== */

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ScreenToScreen (SDL_Surface *source, SDL_Surface *dest)
{
    SDL_BlitSurface(source, NULL, dest, NULL);
}
