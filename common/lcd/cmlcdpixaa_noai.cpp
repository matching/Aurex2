/*
 * cmlcdpixaa_noai.cpp - a pixel control class for anti aliases ( no afterimage )
 *
 * $Id: cmlcdpixaa_noai.cpp,v 1.3 2006/02/19 13:04:40 fumi Exp $
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

#include "cmlcdpix.h"
#include "cmlcdpixaa_noai.h"
#include "aapixel.h"

#include "lcdblrvmgr.h"

CMLcdPixAA_Noai::CMLcdPixAA_Noai(int expansion) : CMLcdPixAA(expansion)
{
// do nothing
}


CMLcdPixAA_Noai::~CMLcdPixAA_Noai(void)
{
}

void CMLcdPixAA_Noai::updatePixel(int start_x, int start_y, enum UpdateRegion updateRegion)
{
	int x, y;
	int end_y, end_x;
	int place_x, place_y;

	start_x = convertToReal( start_x );
	start_y = convertToReal( start_y );

	if(updateRegion == PART)
	{
		end_x = getRealWidth102();
		end_y = getRealHeight64();
	}
	else
	{
		end_y = getRealHeight240();
		end_x = getRealWidth320();
	}

	for( y = 0; y < end_y; y++ )
	{
		place_y = start_y + y;

		for( x = 0; x < end_x; x++ )
		{
			place_x = start_x + x;
			switch(LcdBlRvMgr::getPixelStatus( convertFromReal( place_x ), convertFromReal( place_y )))
			{
			case ON:
				fgPixelData[y][x] = pixelData[place_y][place_x];
				break;

			case OFF:
				fgPixelData[y][x] = 0;
				break;

			case REVERSE:
				fgPixelData[y][x] = 255 - pixelData[place_y][place_x];
				break;
			}
		}
	}
}
