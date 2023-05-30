#include "wl_def.h"

pictabletype	*pictable;

int	    px,py;
unsigned char	fontcolor,backcolor;
int	    fontnumber;

/* ========================================================================== */

void VWB_DrawPropString(const char* string)
{
#ifdef SEGA_SATURN
    fontstruct* font;
    int	height;
    unsigned char* dest;
    unsigned char	    ch;

    unsigned char* vbuf = LOCK();

    font = (fontstruct*)grsegs[STARTFONT + fontnumber];
    font->height = SWAP_BYTES_16(font->height);
    height = font->height;/* SWAP_BYTES_16(font->height);*/ /* font->height; */ /* font->height; */ /* font->height; */ /* SWAP_BYTES_16(font->height); */

    dest = vbuf + scaleFactor * (py * curPitch + px);

    while ((ch = (unsigned char)*string++) != 0)
    {
        int width, step;
        unsigned char* source;
        width = step = font->width[ch];
        source = ((unsigned char*)font) + SWAP_BYTES_16(font->location[ch]);

        while (width--)
        {
            int i;
            for (i = 0; i < height; i++)
            {
                if (source[i * step])
                {
                    unsigned sy, sx;
                    for (sy = 0; sy < scaleFactor; sy++)
                        for (sx = 0; sx < scaleFactor; sx++)
                            dest[(scaleFactor * i + sy) * curPitch + sx] = fontcolor;
                }
            }

            source++;
            px++;
            dest += scaleFactor;
        }
    }

    font->height = SWAP_BYTES_16(font->height);

    UNLOCK();
#else
	fontstruct  *font;
	int		     height;
	unsigned char	   *dest;
	unsigned char 	    ch;
	int i;
	unsigned sx, sy;

	dest = VL_LockSurface(screenBuffer);
	if(dest == NULL) return;

	font = (fontstruct *) grsegs[STARTFONT+fontnumber];
	height = font->height;
	dest += scaleFactor * (ylookup[py] + px);

	while ((ch = (unsigned char)*string++)!=0)
	{
        int step;
        int width = step = font->width[ch];
        unsigned char* source = ((unsigned char *)font)+font->location[ch];
		while (width--)
		{
			for(i=0; i<height; i++)
			{
				if(source[i*step])
				{
					for(sy=0; sy<(unsigned int)scaleFactor; sy++)
						for(sx=0; sx<(unsigned int)scaleFactor; sx++)
							dest[ylookup[scaleFactor*i+sy]+sx]=fontcolor;
				}
			}

			source++;
			px++;
			dest+=scaleFactor;
		}
	}

	VL_UnlockSurface(screenBuffer);
#endif
}

/*
=============================================================================

				Double buffer management routines

=============================================================================
*/

void VWB_DrawTile8 (int x, int y, int tile)
{
#ifdef SEGA_SATURN
    LatchDrawChar(x, y, tile);
#else
	VL_MemToScreen (grsegs[STARTTILE8]+tile*64,8,8,x,y);
#endif
}

void VWB_DrawPic (int x, int y, int chunknum)
{
	int	picnum = chunknum - STARTPICS;
	unsigned width,height;

	x &= ~7;

	width = pictable[picnum].width;
	height = pictable[picnum].height;

#ifdef SEGA_SATURN
    VL_MemToScreenScaledCoord(grsegs[chunknum], width, height, scaleFactor * x, scaleFactor * y);
#else
	VL_MemToScreen (grsegs[chunknum],width,height,x,y);
#endif
}

void VWB_DrawPicScaledCoord (int scx, int scy, int chunknum)
{
	int	picnum = chunknum - STARTPICS;
	unsigned width,height;

	width = pictable[picnum].width;
	height = pictable[picnum].height;

    VL_MemToScreenScaledCoord (grsegs[chunknum],width,height,scx,scy);
}


void VWB_Bar (int x, int y, int width, int height, int color)
{
    VL_Bar (x,y,width,height,color);
}

#ifndef SEGA_SATURN
void VWB_Plot (int x, int y, int color)
{
    if(scaleFactor == 1)
        VL_Plot(x,y,color);
    else
        VWB_Bar(x, y, 1, 1, color);
}
#endif

void VWB_Hlin (int x1, int x2, int y, int color)
{
    if(scaleFactor == 1)
        VW_Hlin(x1, x2, y, color);
    else
        VL_Bar(x1, y, x2-x1+1, 1, color);
}

void VWB_Vlin (int y1, int y2, int x, int color)
{
    if (scaleFactor == 1)
        VW_Vlin(y1, y2, x, color);
    else
        VL_Bar(x, y1, 1, y2-y1+1, color);
}


/*
=============================================================================

						WOLFENSTEIN STUFF

=============================================================================
*/

#ifdef SEGA_SATURN
/*
===================
=
= LoadLatchMem
=
===================
*/

void LoadLatchMem(void)
{
    int	i, width, height, start, end;
    unsigned char* src;
    SDL_Surface* surf; /* ,*surf1; */
#if 0
    /*
    ** tile 8s
    */
    surf = SDL_CreateRGBSurface(SDL_HWSURFACE, 8 * 8,
        ((NUMTILE8 + 7) / 8) * 8, 8, 0, 0, 0, 0);
    if (surf == NULL)
    {
        Quit("Unable to create surface for tiles!");
    }
    SDL_SetColors(surf, gamepal, 0, 256);

    latchpics[0] = surf;

    CA_CacheGrChunk(STARTTILE8);
    src = grsegs[STARTTILE8];
    for (i = 0; i < NUMTILE8; i++)
    {
        VL_MemToLatch(src, 8, 8, surf, (i & 7) * 8, (i >> 3) * 8);
        src += 64;
    }
    UNCACHEGRCHUNK(STARTTILE8);

    latchpics[1] = surf;
#endif	
    /*
    ** pics
    */
    start = LATCHPICS_LUMP_START;
    end = LATCHPICS_LUMP_END;

    for (i = start; i <= end; i++)
    {
        width = pictable[i - STARTPICS].width;
        height = pictable[i - STARTPICS].height;

        surf = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 8, 0, 0, 0, 0);
        if (surf == NULL)
        {
            Quit("Unable to create surface for picture!");
        }
        /*       SDL_SetColors(surf, gamepal, 0, 256);          */
        latchpics[2 + i - start] = surf;
        CA_CacheGrChunk(i);
        VL_MemToLatch(grsegs[i], width, height, surf, 0, 0);
        UNCACHEGRCHUNK(i);
        /* vbt 26/07/2020 free remis	
        ** vbt 15/08/2020 pas de free c'est les images de la barre de statut	
        ** free(surf);
        ** surf=NULL;
        */
    }
    /* 
    ** vbt 26/07/2020 free remis 
    ** vbt 15/08/2020 utilisation de lowworkram 
    ** free(surf1);
    ** surf1=NULL;
    */
}
#endif /* ID_VH_C */
