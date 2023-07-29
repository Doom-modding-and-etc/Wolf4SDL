#include "version.h"

#include "wl_def.h"
#include "wl_shade.h"

typedef struct {
    unsigned char destRed, destGreen, destBlue;   /* values between 0 and 255 */
    unsigned char fogStrength;
} shadedef_t;

shadedef_t shadeDefs[] = {
    {   0,   0,   0, LSHADE_NOSHADING },
    {   0,   0,   0, LSHADE_NORMAL },
    {   0,   0,   0, LSHADE_FOG },
    {  40,  40,  40, LSHADE_NORMAL },
    {  60,  60,  60, LSHADE_FOG }
};

unsigned char shadetable[SHADE_COUNT][256];
int LSHADE_flag;
#ifndef MAPCONTROLLEDSHADE

static wlinline int GetShadeDefID()
{
    int shadeID;

    if (use_extra_features)
    {
        shadeID = ffDataTopLeft & 0x00ff;
    }
    else 
    { 
        switch (gamestate.episode * 10 + gamestate.mapon)
        {
        case  0: shadeID = 4; break;
        case  1:
        case  2:
        case  6: shadeID = 1; break;
        case  3: shadeID = 0; break;
        case  5: shadeID = 2; break;
        default: shadeID = 3; break;
        }
    }
    wlassert(shadeID >= 0 && shadeID < lengthof(shadeDefs));
    return shadeID;
}

#endif

/*
** Returns the palette index of the nearest matching color of the
** given RGB color in given palette
*/
unsigned char GetColor(unsigned char red, unsigned char green, unsigned char blue, SDL_Color *palette)
{
    int col;
    unsigned char mincol = 0;
    
    SDL_Color *palPtr = palette;

    for(col = 0; col < 256; col++, palPtr++)
    {
		double mindist = 200000.F, curdist, DRed, DGreen, DBlue;
        DRed   = (double) (red   - palPtr->r);
        DGreen = (double) (green - palPtr->g);
        DBlue  = (double) (blue  - palPtr->b);
        curdist = DRed * DRed + DGreen * DGreen + DBlue * DBlue;
        if(curdist < mindist)
        {
            mindist = curdist;
            mincol = (unsigned char) col;
        }
    }
    return mincol;
}

/* 
** Fade all colors in 32 steps down to the destination-RGB
** (use gray for fogging, black for standard shading)
*/
void GenerateShadeTable(unsigned char destRed, unsigned char destGreen, unsigned char destBlue,
                        SDL_Color *palette, int fog)
{
    int i,shade;

    SDL_Color *palPtr = palette;

    /* Set the fog-flag */
    LSHADE_flag=fog;

    /* Color loop */
    for(i = 0; i < 256; i++, palPtr++)
    {
		double curRed, curGreen, curBlue, redStep, greenStep, blueStep;
        /* Get original palette color */
        curRed   = palPtr->r;
        curGreen = palPtr->g;
        curBlue  = palPtr->b;

        /* Calculate increment per step */
        redStep   = ((double) destRed   - curRed)   / (SHADE_COUNT + 8);
        greenStep = ((double) destGreen - curGreen) / (SHADE_COUNT + 8);
        blueStep  = ((double) destBlue  - curBlue)  / (SHADE_COUNT + 8);

        /* Calc color for each shade of the current color */
        for (shade = 0; shade < SHADE_COUNT; shade++)
        {
            shadetable[shade][i] = GetColor((unsigned char) curRed, (unsigned char) curGreen, (unsigned char) curBlue, palette);

            /* Inc to next shade */
            curRed   += redStep;
            curGreen += greenStep;
            curBlue  += blueStep;
        }
    }
}

void NoShading()
{
    int i,shade;
    for(shade = 0; shade < SHADE_COUNT; shade++)
        for(i = 0; i < 256; i++)
            shadetable[shade][i] = i;
}

void InitLevelShadeTable()
{
#ifdef MAPCONTROLLEDSHADE
    int rlevel, glevel, blevel;
    int fstrength;
    fstrength = tilemap[3][0];
    rlevel = tilemap[4][0];
    glevel = tilemap[5][0];
    blevel = tilemap[6][0];
    if (fstrength == 0) {
        NoShading();
    }
    else if (fstrength == 1) {
        GenerateShadeTable(rlevel, glevel, blevel, gamepal, LSHADE_NORMAL);
    }
    else if (fstrength >= 2) {
        GenerateShadeTable(rlevel, glevel, blevel, gamepal, LSHADE_FOG);
    }
#else
    shadedef_t *shadeDef = &shadeDefs[GetShadeDefID()];
    if(shadeDef->fogStrength == LSHADE_NOSHADING)
        NoShading();
    else
        GenerateShadeTable(shadeDef->destRed, shadeDef->destGreen, shadeDef->destBlue, gamepal, shadeDef->fogStrength);
#endif
}

int GetShade(int scale)
{
    int shade;
#ifdef MAPCONTROLLEDSHADE
    if (tilemap[2][0]) return 0; /* Turns shading or fog off */
#endif
    shade = (scale >> 1) / (((viewwidth * 3) >> 8) + 1 + LSHADE_flag);  /* TODO: reconsider this... */
    if(shade > 32) shade = 32;
    else if(shade < 1) shade = 1;
    shade = 32 - shade;

    return shade;
}