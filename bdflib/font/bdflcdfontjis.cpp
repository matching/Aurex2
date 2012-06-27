/*
 * bdflcdfontjis.cpp -  a class to draw by jis bdf
 *
 * $Id: bdflcdfontjis.cpp,v 1.6 2006/01/25 09:43:14 fumi Exp $
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

#include "bdffont.h"
#include "bdflcdfont.h"
#include "bdflcdfontjis.h"
#include "cessjis.h"
#include "bdffontsets.h"

BdfLcdFontJis::BdfLcdFontJis(int width, int height, int gap, BdfFontSets *fontsets) : BdfLcdFont(width, height, gap, fontsets)
{
	this->sjis=new CesSjis();
}


BdfLcdFontJis::~BdfLcdFontJis(void)
{
	delete sjis;
}

unsigned char **BdfLcdFontJis::drawString(char const *str, int *resultwidth)
{
	unsigned char **buf;
	int i;

	buf=new unsigned char *[height];
	for(i=0; i < height; i++)
	{
		buf[i]=new unsigned char[320];
		memset(buf[i], 0, 320);
	}

	unsigned char str2[1024]="", *p;
	sjis->convToEuc(str, (char *)str2);
	p=str2;


	int x=0;
	while(x < 320 && *p)
	{
		BdfFont *bf;
		if(*p >= 0x80)
		{
			unsigned short code;
			code=((int)*p << 8) | *(p+1);
			bf=fontsets->getFontData(code);
			p+=2;
		}
		else
		{
			bf=fontsets->getFontData(*p);
			p++;
		}

		if(bf == NULL)
			continue;

#if DEBUG
		{
			fprintf(stderr, "code %c, %04x, data:", *(p-1), *(p-1));
			int i;
			for(i=0; i < bf->data_len; i++)
				fprintf(stderr, "%02x, ", bf->fontdata[i]);
			fprintf(stderr, "\n");
		}
#endif

		int i, j, k;
		unsigned char *q;
		q=bf->fontdata;
		for(i=0; i < height; i++)
		{
			for(j=0; j < (bf->width+7)/8; j++, q++)
			{
				for(k=7; k >= 0; k--)
				{
					if(*q & (1 << k))
						buf[i][x+j*8+(7-k)]=0;
					else
						buf[i][x+j*8+(7-k)]=1;
				}
			}
		}
		x+=bf->width;
		x+=gap;
	}
	if(resultwidth)
		*resultwidth=x;
	return buf;
}

void BdfLcdFontJis::freeBuffer(unsigned char **buf)
{
	int i;
	for(i=0; i < height; i++)
		delete[] buf[i];
	delete[] buf;
}

