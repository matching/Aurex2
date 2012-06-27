/*
 * lcdfunc.cpp - LCD Functions emulation
 *
 * $Id: lcdfunc.cpp,v 1.33 2007/07/28 17:36:52 fumi Exp $
 *
 * Copyright (C) 2004, 2005, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "lcdfunc.h"
#include "cmlcd.h"
#include "cmlcdfunc.h"
#include "blinkmgr.h"
#include "reversemgr.h"
#include "rupaddress.h"
#include "rommmp.h"
#include "romgaiji.h"
#include "ruputerexception.h"

#include "log.h"

#define is_prop_font(c) ((c) >= 5 && (c) <= 7)

#define GV_LCDSIZE_X 320
#define GV_LCDSIZE_Y 240
#define is_in_gv(x, y) ((x) >= 0 && (y) >= 0 && \
						(x) < GV_LCDSIZE_X && (y) <= GV_LCDSIZE_Y)

struct font_size_t
{
	int width;
	int height;
};

static struct font_size_t font_size[]={
{6, 12},
{6, 10},
{6, 8},
{6, 6},
{5, 7},
{6, 12}, /* prop */
{5, 10}, /* prop */
{4, 8},  /* prop */
{3, 5},
{3, 7},
{3, 9},
{4, 9},
{5, 7},
{0, 0},
{0, 0},
{0, 0},
{5, 9},
{5, 9},
{0, 0},
{5, 10},
{6, 11},
{0, 0},
{0, 0},
{0, 0},
{6, 13},
{6, 13},
{6, 13},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{7, 13},
{7, 13},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{9, 16},
{9, 16},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{11, 17},
{11, 17},
{11, 17},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{0, 0},
{13, 19},
{13, 19},
{13, 19},
{13, 19},
{13, 24},
};




/*
 * cmlcd に対しての描画のみ許す
 */

LcdFunc::LcdFunc(CMLcd *_cmlcd, MemoryForInside *_mem)
{
	cmlcd=_cmlcd;
	mem=_mem;
}

LcdFunc::~LcdFunc(void)
{
}


int LcdFuncOnlyShort::getShort(void)
{
	return mem->read16_s(2);
}


int LcdFuncRect::getX1(void)
{
	return mem->read16_s(2);
}

int LcdFuncRect::getY1(void)
{
	return mem->read16_s(4);
}

int LcdFuncRect::getX2(void)
{
	return mem->read16_s(6);
}
int LcdFuncRect::getY2(void)
{
	return mem->read16_s(8);
}

int LcdFuncPoint::getX(void)
{
	return mem->read16_s(2);
}

int LcdFuncPoint::getY(void)
{
	return mem->read16_s(4);
}


/* str 文字列を scosii コードにする
 * 漢字かどうかは考えない。
 * 変換は、以下の様に行われる。
 *
 * 3Bh    ；    − ←変更
 * 3Ch    ＜    Ａ ←変更
 * 3Dh    ＝    Ｍ ←変更
 * 3Eh    ＞    Ｐ ←変更
 * 3Fh    ？    □ ←変更
 */
void LcdFuncMoji::convertSCOSII(char *str)
{
	for(;;)
	{
		switch(*str)
		{
		case ';':
			*str='-';
			break;
		case '<':
			*str='A';
			break;
		case '=':
			*str='M';
			break;
		case '>':
			*str='P';
			break;
		case '?':
			*str=' ';
			break;
		case 0:
			return;
		}

		str++;
	}
}

int LcdFuncMoji::isSCOSII(int size)
{
	return size >= 8 && size <= 61;
}



/*********************************************************************************************/

unsigned long LcdFuncScreen::callLcdFunc(void)
{
/* Ignore argment
 * A aurex supports only GV-MODE
 */
	return 0;
}
unsigned long LcdFuncGetscreen::callLcdFunc(void)
{
	return 1;
}

unsigned long LcdFuncCls::callLcdFunc(void)
{
	cmlcd->getCMLcdFunc()->clearRect(0, 0, 319, 239);
	return 0;
}

unsigned long LcdFuncLcdon::callLcdFunc(void)
{
	return 0;
}

unsigned long LcdFuncGetlcdon::callLcdFunc(void)
{
	return 1;
}

unsigned long LcdFuncMojisize::callLcdFunc(void)
{
	return 0;
}

unsigned long LcdFuncGetbufsiz::callLcdFunc(void)
{
	int width, height;
	long size;

	width=abs(getX2()-getX1())+1;
	height=abs(getY2()-getY1())+1;

	size=((height+7)/8)*width;
	size+=4;

	return size;
}

unsigned long LcdFuncLcdfreeze::callLcdFunc(void)
{
	int flag;

	flag = getShort();

#if 0
	fprintf(stderr, "\
!!------------------LcdFreeze--------------------\n\
!!  flag : %d\n\
!!---------------------------------------------\n", flag);
	fflush(stderr);
#endif

	return 0;
}
unsigned long LcdFuncLcdvol::callLcdFunc(void)
{
	return 0;
}
unsigned long LcdFuncGetlcdvol::callLcdFunc(void)
{
	return 0;
}

unsigned long LcdFuncGv_place::callLcdFunc(void)
{
	int x, y;

	x=getX();
	y=getY();

#ifdef DEBUG
	fprintf(stderr, "gv place (%d, %d)\n", x, y);
	fflush(stderr);
#endif

	if(is_in_gv(x, y))
	{
		systemAddress->set_lcdPlace(x, y);
//		cmlcd->setUpdatePlace(x, y);
		return 0;
	}

	return 0xffffff;
}
unsigned long LcdFuncGv_kput::callLcdFunc(void)
{
	unsigned long addr;
	char *p;
	addr=mem->read24(6);

	p=(char *)Memory::getMemoryPointer(addr);

	int size;
	int gap;
	int logic;

	size=mem->read16_u(10);
	gap=mem->read16_u(12);
	logic=mem->read16_u(14);

	if(moji.isSCOSII(size))
		moji.convertSCOSII(p);

#ifdef DEBUG
	fprintf(stderr, "\
!!------------------gv_kput--------------------\n\
!!  output string : (%s)\n\
!!  (x, y)        : (%d, %d)\n\
!!  font code     : %d\n\
!!  gap           : %d\n\
!!  logic         : %d\n\
!!---------------------------------------------\n",
			p,
			getX(), getY(),
			size,
			gap,
			logic);
	fflush(stderr);
#endif
	if(size > 60)
		return 1;

	cmlcd->getCMLcdFunc()->drawString(getX(), getY(),
									  font_size[size].width, font_size[size].height,
									  p,
									  is_prop_font(size),
									  gap,
									  logic);
	return 0;
}
unsigned long LcdFuncGv_sput::callLcdFunc(void)
{
	char str[4];
	unsigned short code;
	int logic;

	code=mem->read16_u(6);
	str[0]=mem->read8_u(6);
	str[1]=mem->read8_u(7);
	str[2]=0;
	logic=mem->read16_u(8);

	{
		RomGaiji romgaiji;
		unsigned char *p;

		p=romgaiji.getGaijiData(code);
		if(p)
		{
			cmlcd->getCMLcdFunc()->putMMPDataWithoutWH(getX(), getY(),
													  12, 12,
													  p,
													  logic);

			Log::getInstance()->output(Log::Info, "put GAIJI code %04x\n", code);

#ifdef DEBUG
			fprintf(stderr, "!! gaiji code is %x\n", code);
			fflush(stderr);
#endif
			return 0;
		}
	}


	cmlcd->getCMLcdFunc()->drawString(getX(), getY(),
									 6, 12, // width, height
									 str,
									 0, // non proportional font,
									 0,
									 logic);
	return 0;
}

unsigned long LcdFuncGv_aput::callLcdFunc(void)
{
	int w, h;
	int logic;
	unsigned long addr;
	unsigned char *p;

	w=mem->read16_s(6);
	h=mem->read16_s(8);
	addr=mem->read24(10);

	p=(unsigned char *)Memory::getMemoryPointer(addr);
	logic=mem->read16_u(14);


	cmlcd->getCMLcdFunc()->putMMPDataWithoutWH(getX(), getY(),
											  w, h,
											  p,
											  logic);

	return 0;
}



unsigned long LcdFuncGv_line::callLcdFunc(void)
{
	cmlcd->getCMLcdFunc()->drawLine(getX1(), getY1(), getX2(), getY2(), getStyle(), getLogic());
	return 0;
}

unsigned long LcdFuncGv_square::callLcdFunc(void)
{
#ifdef DEBUG
	fprintf(stderr, "!! gvsquiare (%d, %d)-(%d, %d), %x, %x\n", getX1(), getY1(), getX2(), getY2(), getStyle(), getLogic());
	fflush(stderr);
#endif
#if 0
	cmlcd->drawLine(getX1(), getY1(), getX2(), getY1(), getStyle(), getLogic());
	cmlcd->drawLine(getX2(), getY1(), getX2(), getY2(), getStyle(), getLogic());
	cmlcd->drawLine(getX2(), getY2(), getX1(), getY2(), getStyle(), getLogic());
	cmlcd->drawLine(getX1(), getY2(), getX1(), getY1(), getStyle(), getLogic());
#endif
	int logic;
	int paint_flag;

	logic=getLogic();
	paint_flag=(logic >> 3 ) & 1;
	logic &= 0x3;

	if(paint_flag == 0)
		cmlcd->getCMLcdFunc()->drawRect(getX1(), getY1(), getX2(), getY2(), getStyle(), logic);
	else
		cmlcd->getCMLcdFunc()->fillRect(getX1(), getY1(), getX2(), getY2(), getStyle32(), logic);

	return 0;
}



unsigned long LcdFuncGv_reverse::callLcdFunc(void)
{
/**/
	cmlcd->getCMLcdFunc()->reverseRect(getX1(), getY1(), getX2(), getY2());
	return 0;
}

unsigned long LcdFuncGv_clear::callLcdFunc(void)
{
	cmlcd->getCMLcdFunc()->clearRect(getX1(), getY1(), getX2(), getY2());
	return 0;
}

unsigned long LcdFuncGv_get::callLcdFunc(void)
{
	unsigned long addr;
	unsigned char *p;

	addr=mem->read24(10);
	p=(unsigned char *)Memory::getMemoryPointer(addr);

	cmlcd->getCMLcdFunc()->getMMPData(getX1(), getY1(), getX2(), getY2(), p);

	return 0;
}

unsigned long LcdFuncGv_put::callLcdFunc(void)
{
	unsigned long addr;
	unsigned char *p;


	addr=mem->read24(6);
	try {
		if(addr <= MEMORY_SIZE)
			p=(unsigned char *)Memory::getMemoryPointer(addr);
		else
		{
			RomMmp rommmp;
			p=rommmp.getMemoryPointer(addr);

			Log::getInstance()->output(Log::Info, "put MMP in ROM. address = 0x%06x\n", addr);
		}
		cmlcd->getCMLcdFunc()->putMMPData(getX(), getY(), p, mem->read16_u(10));
	}
	catch(MemoryBoundaryException &e)
	{
// do nothing
		Log::getInstance()->output(Log::Info, "put MMP output error (not defined) address = 0x%06x\n", addr);
	}

	return 0;
}



unsigned long LcdFuncBlink::callLcdFunc(void)
{
	return cmlcd->blinkmgr->add(getX1(), getY1(), getX2(), getY2(), getColor());
}

unsigned long LcdFuncBlstop::callLcdFunc(void)
{
	return cmlcd->blinkmgr->stop(getShort());
}

unsigned long LcdFuncBlstart::callLcdFunc(void)
{
	return cmlcd->blinkmgr->start(getShort());
}

unsigned long LcdFuncBlend::callLcdFunc(void)
{
	return cmlcd->blinkmgr->del(getShort());
}



unsigned long LcdFuncReverse::callLcdFunc(void)
{
	return cmlcd->reversemgr->add(getX1(), getY1(), getX2(), getY2());
}

unsigned long LcdFuncRevstop::callLcdFunc(void)
{
	return cmlcd->reversemgr->stop(getShort());
}

unsigned long LcdFuncRevstart::callLcdFunc(void)
{
	return cmlcd->reversemgr->start(getShort());
}

unsigned long LcdFuncRevend::callLcdFunc(void)
{
	return cmlcd->reversemgr->del(getShort());
}




unsigned long LcdFuncGv_circle::callLcdFunc(void)
{
	int x, y;
	int xw, yw;

	int d1x, d1y;
	int d2x, d2y;

	int logic;

	unsigned long style;
	unsigned long addr;

	x=mem->read16_s(2);
	y=mem->read16_s(4);
	xw=mem->read16_s(6);
	yw=mem->read16_s(8);
	d1x=mem->read16_s(10);
	d1y=mem->read16_s(12);
	d2x=mem->read16_s(14);
	d2y=mem->read16_s(16);
	logic=mem->read16_s(18);
	style=mem->read32(20);
	addr=mem->read32(24);

#ifdef DEBUG
	fprintf(stderr, "\
!!------------------gv_circle--------------------\n\
!!  (x, y)        : (%d, %d)\n\
!!  (w, h)        : (%d, %d)\n\
!!  logic         : %d\n\
!!---------------------------------------------\n",
			x, y,
			xw, yw,
			logic);
	fflush(stderr);
#endif




	if(!(logic & 0x10))	/* 円 */
	{
		cmlcd->getCMLcdFunc()->drawEllipse(x, y, xw, yw, logic);
	}

#if 0
	else if((logic & 0x18) == 0x10)	/* 円弧 */
	{
		Arc(hDC0, x1, y1, x2, y2, p->d2x, p->d2y, p->d1x, p->d1y);
	}
	else /* 扇形 */
	{
		Pie(hDC0, x1, y1, x2, y2, p->d2x, p->d2y, p->d1x, p->d1y);
	}
	if(p->logic & 0x4)
		ExtFloodFill(hDC0, p->x, p->y, 0xffffff, FLOODFILLSURFACE);
#endif

	return 0;
}

unsigned long LcdFuncGv_pset::callLcdFunc(void)
{
	int logic = mem->read16_u(6);
	int color = (logic & 0x4 ? 1 : 0 );

	logic &= 0x3;

	cmlcd->getCMLcdFunc()->setPoint( getX(), getY(), color, logic );

	return 0;
}


unsigned long LcdFuncGv_point::callLcdFunc(void)
{
	return cmlcd->getCMLcdFunc()->getPoint( getX(), getY() );
}










unsigned long LcdFuncGv_kput2::callLcdFunc(void)
{
	unsigned long addr;
	char *p;
	addr=mem->read24(6);

	p=(char *)Memory::getMemoryPointer(addr);

	int size;
	int gap;
	int logic;
	int latin;

	size=mem->read16_u(10);
	gap=mem->read16_u(12);
	logic=mem->read16_u(14);
	latin=mem->read16_u(16);

	if(moji.isSCOSII(size))
		moji.convertSCOSII(p);

#ifdef DEBUG
	fprintf(stderr, "\
!!------------------gv_kput--------------------\n\
!!  output string : (%s)\n\
!!  (x, y)        : (%d, %d)\n\
!!  font code     : %d\n\
!!  gap           : %d\n\
!!  logic         : %d\n\
!!  latin         : %d\n\
!!---------------------------------------------\n",
			p,
			getX(), getY(),
			size,
			gap,
			logic,
			latin);
	fflush(stderr);
#endif
	if(size > 60)
		return 1;

	cmlcd->getCMLcdFunc()->drawStringSub(getX(), getY(),
										 font_size[size].width, font_size[size].height,
										 p,
										 is_prop_font(size),
										 gap,
										 logic,
										 latin);
	return 0;
}
