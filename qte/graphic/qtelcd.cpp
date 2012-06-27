/*
 * qtelcd.cpp - Qt/E graphic
 *
 * $Id: qtelcd.cpp,v 1.20 2006/02/20 08:20:02 fumi Exp $
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


#include "qtelcd.h"
#include "qtelcdfont.h"

#include "cmlcd.h"
#include "cmlcdfunc.h"
#include "cmlcdpix.h"
#include "rupaddress.h"



#include <stdio.h>
#include <qdirectpainter_qws.h>
#include <qwidget.h>
#include <qmenubar.h>


QTELcd::QTELcd(RupAddress *systemAddress, QWidget *_parent, CMLcd *cmlcd, int topHeight, int bottomHeight, int toolbarwidth) : SDLcd(cmlcd)
{
	parent = _parent;

	width = cmlcd->getRealWidth102();
	height = cmlcd->getRealHeight64();

	this->window_width = max(width, toolbarwidth);
	this->window_height = height + topHeight + bottomHeight;

	parent->setGeometry(10, 20, window_width, window_height);

	this->topHeight = topHeight;

	this->systemAddress = systemAddress;
}

QTELcd::~QTELcd()
{
//	delete qdp;
}

int QTELcd::getWindowWidth(void)
{
	return window_width;
}

int QTELcd::getWindowHeight(void)
{
	return window_height;
}



void QTELcd::updateWindow(void)
{
	int x, y;
	int pos_x, pos_y;
	int linestep;
	QRect rect;
	unsigned short *fb;
	QDirectPainter *qdp;

	if ( ! parent->isActiveWindow() )
		return;

	qdp = new QDirectPainter( parent );
	fb = (unsigned short *)qdp->frameBuffer();

	rect = parent->geometry();
	linestep = qdp->lineStep()/2;

	pos_x = rect.left();
	pos_y = rect.top() + topHeight;

	switch(qdp->transformOrientation())
	{
	case 0:
		for(y=0; y < height; y++)
		{
			unsigned char const *p;

			p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);

			for(x=0; x < width; x++, p++)
			{
				unsigned short c5, c6, color;

				c5 = *p >> 3;
				c6 = *p >> 2;
				color=(c5 << 11) | (c6 << 5) | c5;

				*(fb+( y + pos_y )*linestep+(x + pos_x )) = ~color;
			}
		}
		break;
	case 3:
		for(y=0; y < height; y++)
		{
			unsigned char const *p;

			p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);

			for(x=0; x < width; x++, p++)
			{
				unsigned short c5, c6, color;

				c5 = *p >> 3;
				c6 = *p >> 2;
				color=(c5 << 11) | (c6 << 5) | c5;

				*(fb+(480 - y - pos_y) + linestep*( x + pos_x)) = ~color;
			}
		}

		break;
	}
	delete qdp;
}

bool QTELcd::hasFontAa(void)
{
	return true;
}


SDLcdFont *QTELcd::createSDLcdFont(int width, int height, int prop, int gap, bool latin)
{
	return new QTELcdFont(parent, width, height, prop, gap, cmlcd->getRealWidth320(), false);
}

SDLcdFont *QTELcd::createSDLcdFontAa(int width, int height, int prop, int gap, bool latin)
{
	return new QTELcdFont(parent, width, height, prop, gap, cmlcd->getRealWidth320(), true);
}

