/* -*-mode:c++; coding:shift_jis-unix;-*-
 *
 * winlcdfontaa.cpp -
 *
 * $Id: winlcdfontaasjis.cpp,v 1.4 2006/02/22 17:40:39 fumi Exp $
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
#include <windows.h>
#include <assert.h>

#include "winlcdfont.h"
#include "sdlcdfont.h"
#include "cmlcd.h"
#include "cessjis.h"

WINLcdFontAaSjis::WINLcdFontAaSjis(HDC hDC_wnd, int width, int height, int prop, int gap) : WINLcdFontAa(hDC_wnd, width, height, prop, gap, "ÇlÇr ÉSÉVÉbÉN", "ÇlÇr ÇoÉSÉVÉbÉN", SHIFTJIS_CHARSET)
{
	sjis=new CesSjis();
}

WINLcdFontAaSjis::~WINLcdFontAaSjis(void)
{
	delete sjis;
}

unsigned char **WINLcdFontAaSjis::drawString(char const *str, int *resultwidth)
{
	GLYPHMETRICS glyph;
	TEXTMETRIC textmetric;

	int len;
	int nbuf;
	int i;
	unsigned int x, y;
	int dst_x;
	unsigned char **buf;
	unsigned char *glyphbuf;

	MAT2 mat2 = {{ 0, 1, }, { 0, 0, },
				 { 0, 0, }, { 0, 1, }};														//

	SIZE textSize;

	len = strlen(str);

	textSize.cx = fwidth * 2 * len;

	buf = new unsigned char *[fheight];
	for(i=0; i < fheight; i++)
	{
		buf[i] = new unsigned char[ textSize.cx ];
		memset(buf[i], 0, textSize.cx);
	}
	dst_x = 0;

	GetTextMetrics(hDC, &textmetric);

	int n;
	unsigned short achar;
	while(*str)
	{
		n = sjis->checkChar(str, &achar);
		str += n;

		nbuf = GetGlyphOutline(hDC, achar, GGO_GRAY8_BITMAP, &glyph, 0, NULL, &mat2);

		if( achar != ' ')
		{
			glyphbuf = new unsigned char[ nbuf ];

			GetGlyphOutline(hDC, achar, GGO_GRAY8_BITMAP, &glyph, nbuf, glyphbuf, &mat2);
			int bytePerLine= ( glyph.gmBlackBoxX + 3 ) & 0xfffc;

			for(y = 0; y < glyph.gmBlackBoxY; y++)
			{
				for(x = 0; x < glyph.gmBlackBoxX; x++)
				{
					int yi = y + textmetric.tmAscent - glyph.gmptGlyphOrigin.y;
					int xi = dst_x + x + glyph.gmptGlyphOrigin.x;

					assert( textSize.cx > xi );

					if( yi < fheight)
						buf[yi][xi] = (int)*(glyphbuf + y * bytePerLine + x) * 255 / 64;
				}
			}

			delete[] glyphbuf;

		}
		dst_x += glyph.gmCellIncX;
	}


	*resultwidth = dst_x;

	return buf;
}
