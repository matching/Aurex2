/*
 * lcdport.cpp - Lcd port module
 *
 * $Id: lcdport.cpp,v 1.11 2005/01/06 14:21:33 fumi Exp $
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
#include "rupaddress.h"

#include "cmlcd.h"
#include "cmlcdfunc.h"
#include "cmlcdpix.h"

#include "lcdport.h"
#include "memory.h"

#define is_lcd_ctrl_port(a) ((a) >= A_LCD_CTRL_PORT && (a) < A_LCD_DATA_PORT)
#define is_lcd_data_port(a) ((a) >= A_LCD_DATA_PORT && (a) < A_LCD_DATA_PORT+4)

LcdPort::LcdPort(CMLcd *cmlcd)
{
	this->cmlcd = cmlcd;

	x=0;
	y=0;

	adc=0;
	rmw=0;
	inv=0;

	read_x=0;
	rmw_prev_x=0;

}

bool LcdPort::isLcdPort(unsigned long addr)
{
	if(is_lcd_ctrl_port(addr))
		return true;
	if(is_lcd_data_port(addr))
		return true;
	return false;
}


bool LcdPort::writeLcdPort8(unsigned long addr, unsigned char c)
{
	if(!isLcdPort(addr))
		return false;

	if(is_lcd_ctrl_port(addr))
		writeLcdCtrlPort(c);
	else if(is_lcd_data_port(addr))
		writeLcdDataPort(c);

	return true;
}

bool LcdPort::writeLcdPort16(unsigned long addr, unsigned short c)
{
	if(!isLcdPort(addr))
		return false;
	int i;
	for(i=0; i < 2; i++)
	{
		if(is_lcd_ctrl_port(addr))
			writeLcdCtrlPort(c & 0xff);
		else if(is_lcd_data_port(addr))
			writeLcdDataPort(c & 0xff);
		addr++;
		c>>=8;
	}
	return true;

}

bool LcdPort::writeLcdPort24(unsigned long addr, unsigned long c)
{
	if(!isLcdPort(addr))
		return false;

	int i;
	for(i=0; i < 3; i++)
	{
		if(is_lcd_ctrl_port(addr))
			writeLcdCtrlPort(c & 0xff);
		else if(is_lcd_data_port(addr))
			writeLcdDataPort(c & 0xff);
		addr++;
		c>>=8;
	}
	return true;
}


void LcdPort::writeLcdCtrlPort(unsigned char data)
{
	switch(data)
	{
	case 0xb0:case 0xb1:case 0xb2:case 0xb3:
	case 0xb4:case 0xb5:case 0xb6:case 0xb7:
	case 0xb8:case 0xb9:case 0xba:case 0xbb:
	case 0xbc:case 0xbd:case 0xbe:case 0xbf:	/* set page address */
		y=(data & 0x0f) << 3;	/* 1 page が 8 dots */
		break;
	case 0x0:case 0x1:case 0x2:case 0x3:
	case 0x4:case 0x5:case 0x6:case 0x7:
	case 0x8:case 0x9:case 0xa:case 0xb:
	case 0xc:case 0xd:case 0xe:case 0xf:	/* column low 4 bit */
		x&=0xf0;
		x|=data & 0xf;	/* 下位 4 bit だけ書き換える */
		break;
	case 0x10:case 0x11:case 0x12:case 0x13:
	case 0x14:case 0x15:case 0x16:case 0x17:
	case 0x18:case 0x19:case 0x1a:case 0x1b:
	case 0x1c:case 0x1d:case 0x1e:case 0x1f:/* column high 4 bit */
		x&=0x0f;
		data-=0x4;
		x|=(data & 0xf) << 4;
		break;

#if 0
	case 0xa0:
		status.adc=0;
		break;
	case 0xa1:
		status.adc=1;
		break;
#endif

	case 0xa6:
		inv=0;
		break;
	case 0xa7:
		inv=1;
		break;
	case 0xe0:
		rmw_prev_x=x;
		rmw=1;
		break;
	case 0xee:
		x=rmw_prev_x;	/* return to "read modify write" column */
		rmw=0;
		break;
	}
}

void LcdPort::writeLcdDataPort(unsigned char data)
{
	CMLcdPix *pix = cmlcd->getCMLcdFunc()->getCMLcdPix();

	int i;

	for(i=0; i < 8; i++, data>>=1)
		pix->pset(x, y+i, (data & 1) ^ inv);
	x++;
}

bool LcdPort::readLcdPort8(unsigned long addr, unsigned char *c)
{
	if(!isLcdPort(addr))
		return false;

	if(is_lcd_data_port(addr))
		*c=readLcdDataPort();

	return true;
}

bool LcdPort::readLcdPort16(unsigned long addr, unsigned short *c)
{
	if(!isLcdPort(addr))
		return false;

	int i;
	*c=0;
	for(i=0; i < 2; i++)
	{
		if(is_lcd_data_port(addr))
			*c|=((int)readLcdDataPort() << (i*8));
		addr++;
	}

	return true;
}

bool LcdPort::readLcdPort24(unsigned long addr, unsigned long *c)
{
	if(!isLcdPort(addr))
		return false;

	int i;
	*c=0;
	for(i=0; i < 3; i++)
	{
		if(is_lcd_data_port(addr))
			*c|=((int)readLcdDataPort() << (i*8));
		addr++;
	}

	return true;
}

unsigned char LcdPort::readLcdDataPort(void)
{
	CMLcdPix *pix = cmlcd->getCMLcdFunc()->getCMLcdPix();

	int j;
	unsigned char c=0;

	for(j=0; j < 8; j++)
	{
		if(pix->pget(x, y+j) == 1)
			c|=1 << j;
	}
	if(rmw == 0)
		x++;

	return c;
}
