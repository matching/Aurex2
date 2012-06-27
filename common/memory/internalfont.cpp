/*
 * internalfont.cpp - internal font emulation
 *
 * $Id: internalfont.cpp,v 1.9 2006/01/25 09:43:15 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <stdlib.h>
#include <string.h>

#include "address.h"
#include "sdlcd.h"
#include "sdlcdfont.h"
#include "internalfont.h"
#include "ruputer.h"
#include "memory.h"

/** 内蔵フォントをエミュレートするクラスコンストラクタ
 */
InternalFont::InternalFont(Ruputer *ruputer)
{
	this->ruputer = ruputer;
	fontbaseaddr=0;
}
/** デストラクタ
 */
InternalFont::~InternalFont(void)
{
}

/** 内蔵フォントのアドレスか否かを判断する
 * @param addr アドレス
 * @retval true 内蔵フォントのアドレス
 * @retval false 上記以外
 */
bool InternalFont::isInternalFontAddress(unsigned long addr)
{
	if(addr >= A_FONT_BASE_HAN && addr <= A_FONT_END)
		return true;
	return false;
}

bool InternalFont::read8(unsigned long addr, unsigned char *c)
{
	if(!isInternalFontAddress(addr))
		return false;

	if(addr < A_FONT_BASE_ZEN)
		*c=hankaku8(addr);
	else
		*c=zenkaku8(addr);

	return true;
}

bool InternalFont::read16(unsigned long addr, unsigned short *c)
{
	if(!isInternalFontAddress(addr))
		return false;

	if(addr < A_FONT_BASE_ZEN)
		*c=hankaku16(addr);
	else
		*c=zenkaku16(addr);

	return true;
}

bool InternalFont::read24(unsigned long addr, unsigned long *c)
{
	if(!isInternalFontAddress(addr))
		return false;

	if(addr < A_FONT_BASE_ZEN)
		*c=hankaku24(addr);
	else
		*c=zenkaku24(addr);

	return true;
}


int InternalFont::getHankakuAddress(unsigned long addr)
{
	unsigned char str[8];
	unsigned long base;

	str[0]=(addr-A_FONT_BASE_HAN)/12+0x20;
	str[1]=0;
	base=A_FONT_BASE_HAN+((int)str[0]-0x20)*12;

	if(base == fontbaseaddr)
		return addr-base;

	fontbaseaddr=base;

	createInternalFont((char *)str, 1);
	return addr-base;
}

int InternalFont::getZenkakuAddress(unsigned long addr)
{
	unsigned char str[8];
	unsigned long base;

	int no;
	no=(addr-A_FONT_BASE_ZEN)/24;
	base=A_FONT_BASE_ZEN+no*24;
	if(base == fontbaseaddr)
		return addr-base;

	fontbaseaddr=base;

#ifdef DEBUG
	fprintf(stderr, "no is %d\n", no);
	fflush(stderr);
#endif

//no=(low-0x81)*0xbd+(high-0x40)
	str[0]=no/0xbd+0x81;
	str[1]=no-((int)str[0]-0x81)*0xbd+0x40;
	str[2]=0;

	createInternalFont((char *)str, 2);
	return addr-base;
}


unsigned char InternalFont::hankaku8(unsigned long addr)
{
	int offset;

	offset=getHankakuAddress(addr);

	return buffer[offset];
}

unsigned short InternalFont::hankaku16(unsigned long addr)
{
	int offset;

	offset=getHankakuAddress(addr);
	MemoryForInside m(buffer);

	return m.read16_u(offset);
}

unsigned long InternalFont::hankaku24(unsigned long addr)
{
	int offset;

	offset=getHankakuAddress(addr);
	MemoryForInside m(buffer);

	return m.read24(offset);
}

unsigned char InternalFont::zenkaku8(unsigned long addr)
{
	int offset;

	offset=getZenkakuAddress(addr);

	return buffer[offset];
}
unsigned short InternalFont::zenkaku16(unsigned long addr)
{
	int offset;

	offset=getZenkakuAddress(addr);
	MemoryForInside m(buffer);

	return m.read16_u(offset);
}

unsigned long InternalFont::zenkaku24(unsigned long addr)
{
	int offset;

	offset=getZenkakuAddress(addr);
	MemoryForInside m(buffer);

	return m.read24(offset);
}

void InternalFont::createInternalFont(char *str, int n)
{
	SDLcdFont *font;

	unsigned char **p;

	font=ruputer->getSDLcd()->createSDLcdFont(6, 12, 0, 0, false);
	if(font == NULL)
		return;

#ifdef DEBUG
	fprintf(stderr, "!! internal font %d %s\n", n, str);
	fflush(stderr);
#endif

	p=font->drawString(str, NULL);

	unsigned char *buf;
	buf=buffer;
	memset(buf, 0, sizeof(buffer));
	int y, x;
	int i;
	for(y=0; y < 12; y+=8)
	{
		*buf=0;
		for(x=0; x < 6*n; x++, buf++)
		{
			for(i=0; i < 8 && y+i < 12; i++)
			{
				if(p[y+i][x] == 0)
					*buf|=1 << i;
			}
		}
	}

#ifdef DEBUG
	for(i=0; i < 24; i++)
		fprintf(stderr, "0x%02x ", buffer[i]);
	fputc('\n', stderr);
#endif

	font->freeBuffer( p );

	delete font;
}
