/*************************************************************************
** Batman: No Man's Land
** Copyright (C) 2012 by LinuxWolf - Team RayCAST
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**************************************************************************
** LinuxWolf Library for Batman: No Man's Land
*************************************************************************/

#include "lw_bres.h"
#include "wl_def.h"

/*
=======================================================================

                          Line Drawing Algorithm

=======================================================================
*/

static lwlib_Bres_t LineDraw_Bres;

static bool lwlib_Bres_HandleDrawDot(lwlib_Bres_t bres, int x, int y);

/*
 * C implementation of Bresenham's line drawing algorithm.
 *
 * Compiled with Borland C++
 *
 * By Michael Abrash
 */

/*
 * Draws a dot at (X0,Y0).
 */
static bool DrawDot(unsigned int X0, unsigned int Y0)
{
	return lwlib_Bres_HandleDrawDot(LineDraw_Bres, (int)X0, (int)Y0);
}

/*
 * Draws a line in octant 0 or 3 ( |DeltaX| >= DeltaY ).
 */
static bool Octant0(unsigned int X0, unsigned int Y0, 
    unsigned int DeltaX, unsigned int DeltaY, int XDirection)
{
   int DeltaYx2;
   int DeltaYx2MinusDeltaXx2;
   int ErrorTerm;

   /* Set up initial error term and values used inside drawing loop */
   DeltaYx2 = DeltaY * 2;
   DeltaYx2MinusDeltaXx2 = DeltaYx2 - (int) ( DeltaX * 2 );
   ErrorTerm = DeltaYx2 - (int) DeltaX;

   /* Draw the line */
   if (DrawDot(X0, Y0))              /* draw the first pixel */
   {
      return true;
   }
   while ( DeltaX-- ) {
      /* See if it's time to advance the Y coordinate */
      if ( ErrorTerm >= 0 ) {
         /* Advance the Y coordinate & adjust the error term
            back down */
         Y0++;
         ErrorTerm += DeltaYx2MinusDeltaXx2;
      } else {
         /* Add to the error term */
         ErrorTerm += DeltaYx2;
      }
      X0 += XDirection;          /* advance the X coordinate */
      if (DrawDot(X0, Y0))      /* draw a pixel */
      {
         return true;
      }
   }

   return false;
}

/*
 * Draws a line in octant 1 or 2 ( |DeltaX| < DeltaY ).
 */
static bool Octant1(unsigned int X0, unsigned int Y0, 
    unsigned int DeltaX, unsigned int DeltaY, int XDirection)
{
   int DeltaXx2;
   int DeltaXx2MinusDeltaYx2;
   int ErrorTerm;

   /* Set up initial error term and values used inside drawing loop */
   DeltaXx2 = DeltaX * 2;
   DeltaXx2MinusDeltaYx2 = DeltaXx2 - (int) ( DeltaY * 2 );
   ErrorTerm = DeltaXx2 - (int) DeltaY;

   if (DrawDot(X0, Y0))           /* draw the first pixel */
   {
      return true;
   }
   while ( DeltaY-- ) {
      /* See if it's time to advance the X coordinate */
      if ( ErrorTerm >= 0 ) {
         /* Advance the X coordinate & adjust the error term
            back down */
         X0 += XDirection;
         ErrorTerm += DeltaXx2MinusDeltaYx2;
      } else {
         /* Add to the error term */
         ErrorTerm += DeltaXx2;
      }
      Y0++;                   /* advance the Y coordinate */
      if (DrawDot(X0, Y0))   /* draw a pixel */
      {
         return true;
      }
   }

   return false;
}

/*
 * Draws a line.
 */
static bool DrawLine(int X0, int Y0, int X1, int Y1)
{
   int DeltaX, DeltaY;
   int Temp;

   /* Save half the line-drawing cases by swapping Y0 with Y1
      and X0 with X1 if Y0 is greater than Y1. As a result, DeltaY
      is always > 0, and only the octant 0-3 cases need to be
      handled. */
   if ( Y0 > Y1 ) {
      Temp = Y0;
      Y0 = Y1;
      Y1 = Temp;
      Temp = X0;
      X0 = X1;
      X1 = Temp;
   }

   /* Handle as four separate cases, for the four octants in which
      Y1 is greater than Y0 */
   DeltaX = X1 - X0;    /* calculate the length of the line
                           in each coordinate */
   DeltaY = Y1 - Y0;
   if ( DeltaX > 0 ) {
      if ( DeltaX > DeltaY ) {
         return Octant0(X0, Y0, DeltaX, DeltaY, 1);
      } else {
         return Octant1(X0, Y0, DeltaX, DeltaY, 1);
      }
   } else {
      DeltaX = -DeltaX;             /* absolute value of DeltaX */
      if ( DeltaX > DeltaY ) {
         return Octant0(X0, Y0, DeltaX, DeltaY, -1);
      } else {
         return Octant1(X0, Y0, DeltaX, DeltaY, -1);
      }
   }
}

/*
=======================================================================

                 Miscellaneous Mathematical Functions

=======================================================================
*/

static int TwoCellInt(int x1, int y1, int x2, int y2)
{
	return (x1 << (mapshift * 0)) + (y1 << (mapshift * 1)) + 
		(x2 << (mapshift * 2)) + (y2 << (mapshift * 3));
}

/*
=======================================================================

                 Bresenham-Based 2D Visibility Algorithm

=======================================================================
*/

typedef struct lwlib_Bres_Priv_s
{
	uint8_t visdata[LWLIB_MB(2)];
	uint8_t blocks[64][64];
	lwlib_TPoint3i neigh[3];
	lwlib_TPoint3i target;
} lwlib_Bres_Priv_t;

static bool lwlib_Bres_GetVisBit(lwlib_Bres_t bres, int x1, int y1,
	int x2, int y2)
{
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);
	const int x = TwoCellInt(x1, y1, x2, y2);
	return (priv->visdata[x / 8] & (1 << (x & 7))) != 0;
}

static void lwlib_Bres_SetVisBit(lwlib_Bres_t bres, int x1, int y1,
	int x2, int y2, bool blocked)
{
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);
	const int x = TwoCellInt(x1, y1, x2, y2);
	priv->visdata[x / 8] &= ~(1 << (x & 7));
	if (blocked)
	{
		priv->visdata[x / 8] |= (1 << (x & 7));
	}
}

static bool lwlib_Bres_IsNeighBlocked(lwlib_Bres_t bres, int x, int y,
	int index)
{
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);
	const int xn = x + lwlib_X(priv->neigh[index]);
	const int yn = y + lwlib_Y(priv->neigh[index]);
	return priv->blocks[xn][yn];
}

static void lwlib_Bres_InitBlocks(lwlib_Bres_t bres)
{
	int x, y;
	byte maptile;
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);

	for (y = 0; y < 64; y++)
	{
		for (x = 0; x < 64; x++)
		{
			maptile = tilemap[x][64 - 1 - y];
			if (maptile != 0 && (maptile & 0x80) == 0 && 
				MAPSPOT(x, 64 - 1 - y, 1) != PUSHABLETILE)
			{
				priv->blocks[x][y] = 1;
			}
			else
			{
				priv->blocks[x][y] = 0;
			}
		}
	}
}

static bool lwlib_Bres_HandleDrawDot(lwlib_Bres_t bres, int x, int y)
{
	int xn, yn;
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);

	// continue line drawing if too close to target
	if (abs(x - lwlib_X(priv->target)) < 2 && 
		abs(y - lwlib_Y(priv->target)) < 2)
	{
		return false;
	}

	#define BLOCKED(n) lwlib_Bres_IsNeighBlocked(bres, x, y, n)
	if (BLOCKED(2) && (BLOCKED(0) || BLOCKED(1)))
	{
		return true;
	}
	#undef BLOCKED

	return false;
}

static void lwlib_Bres_ComputeVis(lwlib_Bres_t bres,
	lwlib_TPoint3i start, lwlib_TPoint3i end)
{
	int s;
	int dx, dy;
	int sx, sy;
	bool blocked;
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);

	if (priv->blocks[lwlib_X(start)][lwlib_Y(start)] || 
		priv->blocks[lwlib_X(end)][lwlib_Y(end)])
	{
		blocked = true;
	}
	else
	{
		LineDraw_Bres = bres;
		priv->target = end;

		dx = lwlib_X(end) - lwlib_X(start);
		dy = lwlib_Y(end) - lwlib_Y(start);
		sx = lwlib_SignInt(dx);
		sy = lwlib_SignInt(dy);

		priv->neigh[1] = lwlib_vec3i(sx, sy, 0);
		if (sx * dx == sy * dy || dx == 0 || dy == 0)
		{
			priv->neigh[0] = priv->neigh[2] = priv->neigh[1];
		}
		else
		{
			s = sx * dx > sy * dy ? 1 : 0;
			priv->neigh[0] = lwlib_vec3i(sx * (1 - s), sy * s, 0);
			priv->neigh[2] = lwlib_vec3i(sx * s, sy * (1 - s), 0);
		}

		blocked = DrawLine(lwlib_X(start), lwlib_Y(start), lwlib_X(end),
			lwlib_Y(end));
	}
	
	lwlib_Bres_SetVisBit(bres, lwlib_X(start), lwlib_Y(start),
		lwlib_X(end), lwlib_Y(end), blocked);
}


lwlib_Bres_t lwlib_Bres_New(void)
{
	lwlib_Bres_t bres;

	lwlib_Zero(bres);
	bres.priv = lwlib_CallocSingle(lwlib_Bres_Priv_t);

	lwlib_GetPriv(lwlib_Bres_Priv_t, bres);

	return bres;
}

lwlib_Bres_t lwlib_Bres_Free(lwlib_Bres_t bres)
{
	lwlib_GetPriv(lwlib_Bres_Priv_t, bres);
	if (priv)
	{
		free(priv);
	}
	lwlib_Zero(bres);
	return bres;
}

void lwlib_Bres_ComputeVisAll(lwlib_Bres_t bres)
{
	lwlib_TPoint3i start, end;
	lwlib_GetPrivAssert(lwlib_Bres_Priv_t, bres);

	lwlib_Bres_InitBlocks(bres);

	start = lwlib_vec3i_zero(); 
	do
	{
		end = lwlib_vec3i_zero(); 
		do
		{
			lwlib_Bres_ComputeVis(bres, start, end);

			end = lwlib_vec3i_bounded_inc(end, 
				lwlib_vec3i(64, 64, 1));
		} while (!lwlib_vec3i_equal(end, lwlib_vec3i_zero()));

		start = lwlib_vec3i_bounded_inc(start, 
			lwlib_vec3i(64, 64, 1));

		if (WolfRad_Cancelled(wolfRad))
		{
			return;
		}
	} while (!lwlib_vec3i_equal(start, lwlib_vec3i_zero()));
}

bool lwlib_Bres_VisBlocked(lwlib_Bres_t bres, lwlib_TPoint3i start,
	lwlib_TPoint3i end)
{
	return lwlib_Bres_GetVisBit(bres, lwlib_X(start), lwlib_Y(start),
		lwlib_X(end), lwlib_Y(end));
}
