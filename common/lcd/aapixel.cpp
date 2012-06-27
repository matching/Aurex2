/*
 * aapixel.cpp - a pixel class for anti aliases
 *
 * $Id: aapixel.cpp,v 1.3 2005/01/13 08:40:42 fumi Exp $
 *
 * Copyright (C) 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
 * All rights reserved.
 *
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "aapixel.h"


// ¿þ¤Î¹­¤¬¤ê ¡ÊÇÜÎ¨¡Ë
#define SUSO 2
//#define SUSO 1
//

AAPixel::AAPixel(int expansion)
{
	width = expansion * SUSO;
	height = width;

// ³ÈÂç¤Ê¤·
	if( expansion == 1 )
	{
		all_width = 1;
		all_height = 1;

		offset_top = 0;
		offset_left = 0;
	}
	else if( expansion & 1 )
	{
// ´ñ¿ô
		all_width = expansion * SUSO - 1;
		all_height = all_width;

		offset_top = expansion / 2;
		offset_left = offset_top;
	}
	else
	{
// ¶ö¿ô
		all_width = expansion * SUSO;
		all_height = all_width;

		offset_top = expansion / 2;
		offset_left = offset_top;
	}

	int size;
	size = all_width * all_height;
	pixel = new unsigned char[size];
	memset(pixel, 0, size);

	setPixelData(expansion);
}

AAPixel::~AAPixel(void)
{
	delete[] pixel;
}

void AAPixel::setPixelData(int expansion)
{
	int x, y;

	for( y = 0; y < all_height; y++ )
		for( x = 0; x < all_width; x++ )
			pixel[y * all_width + x] = getPixelValue(x, y, expansion);

//#ifdef DEBUG
#if 1
	fprintf(stderr, "\
!! AAPixel::setPixelData --------------------------------------------------------\n\
width :%d\n\
height:%d\n\
all-width :%d\n\
all-height:%d\n\
offset-top :%d\n\
offset-left:%d\n", width, height, all_width, all_height, offset_top, offset_left);

	for( y = 0; y < all_height; y++ )
	{
		for( x = 0; x < all_width; x++ )
			fprintf(stderr, "% 4d ", pixel[y * all_width + x]);
		fputs("\n", stderr);
	}
	fflush(stderr);
#endif

}

unsigned char AAPixel::getPixelValue(int x, int y, int expansion)
{
	double v;

	if( expansion == 1 )
	{
		return 255;
	}
	else if( expansion & 1 )
	{
		if( x >= all_width / 2 + 1)
			x = all_width - x - 1;

		if( y >= all_height / 2 + 1)
			y = all_height - y - 1;

		double max_len = all_height / 2;
		max_len *= max_len;
		max_len *= 2;
		max_len = sqrt(max_len);

		double len = sqrt( (double)(x * x) + (y * y) );

		double tmp = exp(-(max_len - len)/max_len);
//		v = 255 * pow(tmp, 1.4);
		v = 255 * tmp * tmp;
	}
	else
	{
		if( x >= all_width / 2 )
			x = all_width - 1 - x;

		if( y >= all_height / 2 )
			y = all_height - 1 - y;

		double max_len = all_height / 2 - 1;
		max_len *= max_len;
		max_len *= 2;
		max_len = sqrt(max_len);

//		double b = 200;
//		double a = (double)(b) / (all_width / 2 - 1);
		double len;

		len = sqrt( (double)(x * x) + (y * y) );
//		v = - a * (max_len - len) + b;
//		double tmp = sin((len/max_len) * 3.141592 / 2);
//		v = 224 * tmp * tmp;

		double tmp = exp(-(max_len - len)/max_len);
//		v = 224 * tmp * tmp;
//		v = 224 * tmp * tmp * tmp * tmp;

		v = 224 * tmp * tmp * tmp;

//		v = 255;


	}

	if ( v > 255.0 )
		return 255;
	if ( v <= 0.0 )
		return 0;

	return (unsigned char)v;
}

int AAPixel::getWidth(void)
{
	return width;
}
int AAPixel::getHeight(void)
{
	return height;
}
int AAPixel::getAllWidth(void)
{
	return all_width;
}

int AAPixel::getAllHeight(void)
{
	return all_height;
}
int AAPixel::getOffsetTop(void)
{
	return offset_top;
}
int AAPixel::getOffsetLeft(void)
{
	return offset_left;
}

unsigned char const *AAPixel::getPixelLine(int y)
{
	return pixel + y * all_width;
}
