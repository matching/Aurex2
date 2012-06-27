/*
 * qtelcdfont.cpp - qt/e draw string wrapper routine
 *
 * $Id: qtelcdfont.cpp,v 1.9 2006/02/12 16:31:41 fumi Exp $
 *
 * Copyright (C) 2004, 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "qtelcdfont.h"
#include "cessjis.h"

#include <stdio.h>

#include <qstring.h>
#include <qpainter.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qeucjpcodec.h>
#include <qwidget.h>
#include <qnamespace.h>

QTELcdFont::QTELcdFont(QWidget *parent, int width, int height, int prop, int gap, int maxWidth, bool aaflag)
{
	sjis = new CesSjis();

	pixmap = new QPixmap(maxWidth, height, 16);

	this->width = width;
	this->height = height;
	this->gap = gap;

	this->aaflag = aaflag;

	this->parent = parent;
}
QTELcdFont::~QTELcdFont(void)
{
	delete pixmap;
	delete sjis;
}

unsigned char **QTELcdFont::drawString(char const *str, int *resultwidth)
{
	QPainter p( pixmap );

#ifdef ZAURUS
	QFont f( "lcfont", 12);
#else
	QFont f( "unifont", 12);
#endif
	int widthmax;

	char str2[1024];
	sjis->convToEuc(str, str2);

	f.setPixelSize( height );
	p.setPen( QColor(255, 255, 255) );
	p.setFont( f );

	if(gap == 0)
	{
		p.drawText(pixmap->rect(), Qt::AlignLeft + Qt::AlignVCenter, parent->tr(str2) );
		widthmax = strlen(str2) * width;
	}
	else
	{
		int x=0;
		char *s;
		char tmp[3] = "";

		QRect rect = pixmap->rect();

		s=str2;
		while(*s)
		{
			rect.setX(x);

			if(*s & 0x80)
			{
				if(*(s+1) == 0)
					break;
				tmp[0] = *s;
				tmp[1] = *(s+1);

				p.drawText(rect, Qt::AlignLeft + Qt::AlignVCenter, parent->tr(tmp));
				s += 2;
				x += width * 2 + gap;
			}
			else
			{
				tmp[0] = *s;
				tmp[1] = 0;
				p.drawText(rect, Qt::AlignLeft + Qt::AlignVCenter, parent->tr(tmp));
				s++;
				x += width + gap;
			}
		}
		widthmax = x;
	}

	*resultwidth = widthmax;
	image = pixmap->convertToImage();

	int i, j;
	unsigned char **buf = new unsigned char * [height];
	if( aaflag )
	{
		for(i=0; i < height; i++)
		{
			buf[i] = new unsigned char [widthmax];

			for( j = 0; j < widthmax; j++)
			{
				buf[i][j] = image.pixel(j, i) & 0xff;
			}
		}
	}
	else
	{
		for(i=0; i < height; i++)
		{
			buf[i] = new unsigned char [widthmax];
			for( j = 0; j < widthmax; j++)
			{
				buf[i][j] = (image.pixel(j, i) & 0xff ) >  0x7f ?  0x00 : 0xff ;
			}
		}
	}

	return buf;
}

void QTELcdFont::freeBuffer(unsigned char **buf)
{
	int i;
	for(i=0; i < height; i++)
	{
		delete[] buf[i];
	}
	delete[] buf;
}


