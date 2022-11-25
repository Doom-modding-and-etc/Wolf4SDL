/*************************************************************************
** Wolfenstein Media Library
** Copyright (C) 2009-2010 by Wolfy
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
** Media Images Module
*************************************************************************/
#ifndef LWLIB_IMG_H
#define LWLIB_IMG_H

#ifdef LWLIB
typedef struct lwlib_Img_s
{
	int w, h;
	int bpp;
	unsigned char *data;
} lwlib_Img_t;
#endif
#endif
