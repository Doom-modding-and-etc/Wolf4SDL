// ID_VH.H

#ifndef __ID_VH_H_
#define __ID_VH_H_

#define WHITE			15			// graphics mode independent colors
#define BLACK			0
#define FIRSTCOLOR		1
#define SECONDCOLOR		12
#define F_WHITE			15
#define F_BLACK			0
#define F_FIRSTCOLOR	1
#define F_SECONDCOLOR	12

//===========================================================================

#define MAXSHIFTS	1

typedef struct
{
	short width,height;
} pictabletype;


typedef struct
{
	short height;
	short location[256];
	char width[256];
} fontstruct;


//===========================================================================


extern	pictabletype	*pictable;
extern	pictabletype	*picmtable;

extern  unsigned char            fontcolor,backcolor;
extern	int             fontnumber;
extern	int             px,py;

#define SETFONTCOLOR(f,b) fontcolor=f;backcolor=b;

//
// wolfenstein EGA compatbility stuff
//


#define VW_Shutdown		    VL_Shutdown
#define VW_Bar			    VL_Bar
#define VW_Plot			    VL_Plot
#define VW_Hlin(x,z,y,c)	VL_Hlin(x,y,(z)-(x)+1,c)
#define VW_Vlin(y,z,x,c)	VL_Vlin(x,y,(z)-(y)+1,c)
#define VW_DrawPic		    VH_DrawPic
#define VW_WaitVBL		    VL_WaitVBL
#define VW_FadeIn()		    VL_FadeIn(0,255,gamepal,30);
#define VW_FadeOut()	    VL_FadeOut(0,255,0,0,0,30);
#define VW_ScreenToScreen	VL_ScreenToScreen
void	VW_MeasurePropString(const char* string, unsigned short* width, unsigned short* height);

void    VH_Startup();
boolean FizzleFade(SDL_Surface* source, int x1, int y1,
	unsigned width, unsigned height, unsigned frames, boolean abortable);

//
// mode independent routines
// coordinates in pixels, rounded to best screen res
// regions marked in double buffer
//

void VWB_DrawPropString	 (const char *string);

void VWB_DrawTile8 (int x, int y, int tile);
void VWB_DrawPic (int x, int y, int chunknum);
void VWB_DrawPicScaledCoord (int x, int y, int chunknum);
void VWB_Bar (int x, int y, int width, int height, int color);
#define VWB_BarScaledCoord VL_BarScaledCoord
void VWB_Plot (int x, int y, int color);
#define VWB_PlotScaledCoord VW_Plot
void VWB_Hlin (int x1, int x2, int y, int color);
void VWB_Vlin (int y1, int y2, int x, int color);
#define VWB_HlinScaledCoord VW_Hlin
#define VWB_VlinScaledCoord VW_Vlin

void VH_UpdateScreen (SDL_Surface *surface);
#ifndef SEGA_SATURN
#define VW_UpdateScreen()   VH_UpdateScreen (screenBuffer)
#endif
#if SDL_MAJOR_VERSION == 2 || SDL_MAJOR_VERSION == 3
void VH_RenderTextures();
#endif


#endif