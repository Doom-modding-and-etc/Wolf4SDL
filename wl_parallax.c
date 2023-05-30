/* WL_PARALLAX.C */

#include "version.h"

#ifdef USE_PARALLAX

#include "wl_def.h"

#ifdef MAPCONTROLLEDSKY
static int GetParallaxStartTexture()
{
    int startTex = tilemap[7][0];
    return startTex;
}

#else

#ifdef USE_FEATUREFLAGS

/* The lower left tile of every map determines the start texture of the parallax sky. */
int GetParallaxStartTexture (void)
{
    int startTex = ffDataBottomLeft;

    wlassert(startTex >= 0 && startTex < PMSpriteStart);

    return startTex;
}

#else

int GetParallaxStartTexture (void)
{
    int startTex;

    switch (gamestate.episode * 10 + gamestate.mapon)
    {
        case  0: startTex = 20; break;
        default: startTex =  0; break;
    }

    wlassert(startTex >= 0 && startTex < PMSpriteStart);

    return startTex;
}

#endif

#endif
/*
====================
=
= DrawParallax
=
====================
*/

void DrawParallax (void)
{
    int     x,y;
    unsigned char *dest,*skysource;
    unsigned short    texture;
    short angle;
    short skypage,curskypage;
    short lastskypage;
    short xtex;

    skypage = GetParallaxStartTexture();
    skypage += 16 - 1;
    lastskypage = -1;

    for (x = 0; x < viewwidth; x++)
    {
        short toppix = centery - (wallheight[x] >> 3);

        if (toppix <= 0)
            continue;                /* nothing to draw */

        angle = pixelangle[x] + midangle;

        if (angle < 0)
            angle += FINEANGLES;
        else if (angle >= FINEANGLES)
            angle -= FINEANGLES;

        xtex = ((angle * 16) << TEXTURESHIFT) / FINEANGLES;
        curskypage = xtex >> TEXTURESHIFT;

        if (lastskypage != curskypage)
        {
            lastskypage = curskypage;
            skysource = PM_GetPage(skypage - curskypage);
        }

        texture = TEXTUREMASK - ((xtex & (TEXTURESIZE - 1)) << TEXTURESHIFT);

        for (y = 0, dest = &vbuf[x]; y < toppix; y++, dest += bufferPitch)
            *dest = skysource[texture + ((y << TEXTURESHIFT) / centery)];
    }
}

#endif
