/* WL_PLANE.C */

#include "wl_def.h"
#include "wl_shade.h"

unsigned char *ceilingsource,*floorsource;

void GetFlatTextures (void)
{
    if (use_extra_features)
    {
        ceilingsource = PM_GetPage(ffDataBottomRight >> 8);
        floorsource = PM_GetPage(ffDataBottomRight & 0xff);
    }
    else 
    {
        ceilingsource = PM_GetPage(0);
        floorsource = PM_GetPage(1);
    }
}

/*
===================
=
= DrawSpan
=
= Height ranges from 0 (infinity) to [centery] (nearest)
=
= With multi-textured floors and ceilings stored in lower and upper bytes of
= according tilex/tiley in the third mapplane, respectively
=
===================
*/
void DrawSpan (short x1, short x2, short height)
{
    unsigned char      tilex,tiley,lasttilex,lasttiley;
    unsigned char      *dest;
    unsigned char      *shade;
    unsigned short      texture,spot;
    unsigned int  rowofs;
    short   ceilingpage,floorpage,lastceilingpage,lastfloorpage;
    short   count,prestep;
    fixed     basedist,stepscale;
    fixed     xfrac,yfrac;
    fixed     xstep,ystep;

    count = x2 - x1;

    if (!count)
        return;                                                 /* nothing to draw */

    if (use_shading)
    {
        shade = shadetable[GetShade(height << 3)];
    }
    dest = vbuf + ylookup[centery - 1 - height] + x1;
    rowofs = ylookup[(height << 1) + 1];                        /* toprow to bottomrow delta */

    prestep = centerx - x1 + 1;
    basedist = FixedDiv(scale,height + 1) >> 1;                 /* distance to row projection */
    stepscale = basedist / scale;

    xstep = FixedMul(stepscale,viewsin);
    ystep = -FixedMul(stepscale,viewcos);

    xfrac = (viewx + FixedMul(basedist,viewcos)) - (xstep * prestep);
    yfrac = -(viewy - FixedMul(basedist,viewsin)) - (ystep * prestep);
    
    if (use_multiflats)
    {
        /*
        ** draw two spans simultaneously
        */
        lastceilingpage = lastfloorpage = -1;
        lasttilex = lasttiley = 0;

        /*
        ** Beware - This loop is SLOW, and WILL cause framedrops on slower machines and/or on higher resolutions.
        ** I can't stress it enough: this is one of the worst loops in history! No amount of optimisation will
        ** change that! This loop SUCKS!
        **
        ** Someday flats rendering will be re-written, but until then, YOU HAVE BEEN WARNED.
        */
        while (count--)
        {
            /*
            ** get tile coords of texture
            */
            tilex = (xfrac >> TILESHIFT) & (mapwidth - 1);
            tiley = ~(yfrac >> TILESHIFT) & (mapheight - 1);

            /*
            ** get floor & ceiling textures if it's a new tile
            */
            if (lasttilex != tilex || lasttiley != tiley)
            {
                lasttilex = tilex;
                lasttiley = tiley;
                spot = MAPSPOT(tilex, tiley, 2);

                if (spot)
                {
                    ceilingpage = spot >> 8;
                    floorpage = spot & 0xff;
                }
            }

            if (spot)
            {
                texture = ((xfrac >> FIXED2TEXSHIFT) & TEXTUREMASK) + (~(yfrac >> (FIXED2TEXSHIFT + TEXTURESHIFT)) & (TEXTURESIZE - 1));

                /*
                ** write ceiling pixel
                */
                if (ceilingpage)
                {
                    if (lastceilingpage != ceilingpage)
                    {
                        lastceilingpage = ceilingpage;
                        ceilingsource = PM_GetPage(ceilingpage);
                    }
                    if (use_shading)
                    {
                        *dest = shade[ceilingsource[texture]];
                    }
                    else
                    {
                        *dest = ceilingsource[texture];
                    }

                }

                /*
                ** write floor pixel
                */
                if (floorpage)
                {
                    if (lastfloorpage != floorpage)
                    {
                        lastfloorpage = floorpage;
                        floorsource = PM_GetPage(floorpage);
                    }
                    if (use_shading)
                    {
                        dest[rowofs] = shade[floorsource[texture]];
                    }
                    else
                    {
                        dest[rowofs] = floorsource[texture];
                    }
                }
            }
        }
    }

    else
    {
        while (count--)
        {
            texture = ((xfrac >> FIXED2TEXSHIFT) & TEXTUREMASK) + (~(yfrac >> (FIXED2TEXSHIFT + TEXTURESHIFT)) & (TEXTURESIZE - 1));

            if (use_shading)
            {
                *dest = shade[ceilingsource[texture]];
                dest[rowofs] = shade[floorsource[texture]];
            }
            else
            {
                *dest = ceilingsource[texture];
                dest[rowofs] = floorsource[texture];
            }
            dest++;
            xfrac += xstep;
            yfrac += ystep;
        }
    }
        dest++;

        xfrac += xstep;
        yfrac += ystep;
}

/*
===================
=
= DrawPlanes
=
===================
*/

void DrawPlanes (void)
{
    int     x,y;
    short	height;

/*
** loop over all columns
*/
    y = centery;

    for (x = 0; x < viewwidth; x++)
    {
        height = wallheight[x] >> 3;

        if (height < y)
        {
            /*
            ** more starts
            */
            while (y > height)
                spanstart[--y] = x;
        }
        else if (height > y)
        {
            /*
            ** draw clipped spans
            */
            if (height > centery)
                height = centery;

            while (y < height)
            {
                if (y > 0)
                    DrawSpan (spanstart[y],x,y);

                y++;
            }
        }
    }

    /*
    ** draw spans
    */
    height = centery;

    while (y < height)
    {
        if (y > 0)
            DrawSpan (spanstart[y],viewwidth,y);

        y++;
    }
}
