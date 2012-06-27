/*
 * cmlcdfuncaa.cpp - common drawing module for Lcd
 *
 * $Id: cmlcdfuncaa.cpp,v 1.8 2006/02/19 13:04:39 fumi Exp $
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
#include <assert.h>

#include "cmlcdfunc.h"
#include "cmlcdfuncaa.h"

#include "cmlcdpix.h"

#ifdef DOT_ANTI_ALIASES
#include "cmlcdpixaa.h"
#include "cmlcdpixaa_noai.h"
#endif

#include "cmlcdpixdefault.h"
#include "cmlcdpixdefault_noai.h"

#include "sdlcd.h"
#include "sdlcdfont.h"
#include "ruputer.h"
#include "memory.h"


/** 共通LCD描画ルーチンコンストラクタ
 */
CMLcdFuncAa::CMLcdFuncAa(Ruputer *ruputer, int expansion, bool after_image, int ai_value) : CMLcdFuncDefault(ruputer, expansion, after_image, ai_value)
{
#ifdef DOT_ANTI_ALIASES

	delete pix;

	if( after_image )
		pix = new CMLcdPixAA(expansion, ai_value);
	else
		pix = new CMLcdPixAA_Noai(expansion);
#endif
}

/** 共通LCD描画ルーチン デストラクタ
 */
CMLcdFuncAa::~CMLcdFuncAa(void)
{
}

/** LCDの点の明暗を設定する（public）
 * @param x x 座標
 * @param y y 座標
 * @param color 0:白 to 255:黒
 * @param logic  gv_line 系の logic は下位 2 bitに格納され、
 * gv_kput 系の logic は 2bit 左シフトして格納される
 */
void CMLcdFuncAa::setPointReal(int x, int y, int color, int logic)
{
	int c;

	switch(logic)
	{
	case 0:	/* 上書き */
		pix->psetReal(x, y, color);
		break;
	case 1: /* xor */
		c = pix->pgetReal(x, y);
		c=c ^ color;
		pix->psetReal(x, y, c);
		break;
	case 4: /* string or */
	case 2:	/* or */
		c = pix->pgetReal(x, y);
//		c=c | color;
		c += c + color;
		if( c > 255 )
			c = 255;
		pix->psetReal(x, y, c);
		break;
	case 3:	/* and */
		c = pix->pgetReal(x, y);
		c=c & color;
		pix->psetReal(x, y, c);
		break;

	case 8: /* 反転上書き */
		color = 255 - color;
		pix->psetReal(x, y, color);
		break;

	case 0xc: /* 反転 OR */
		c = pix->pgetReal(x, y);
		c = ( 255 - color ) + c;
		if( c > 255 )
			c = 255;
		pix->psetReal(x, y, c);
		break;
	}
}

/** 線を引く
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 * @param style 8bit で点線を表現。0xff で*ただ*の線
 * @param logic 上書き等の設定
 */
void CMLcdFuncAa::drawLine(int x1, int y1, int x2, int y2, unsigned char ucstyle, int logic)
{
	unsigned long style;

	style=((unsigned long)ucstyle << 24)
		| ((unsigned long)ucstyle << 16)
		| ((unsigned long)ucstyle <<  8)
		|  (unsigned long)ucstyle;

	drawLine(x1, y1, x2, y2, style, logic);
}

/** 線を引く style unsinged long 版
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 * @param style 32bit で点線を表現。0xffffffffUL で*ただ*の線
 * @param logic 上書き等の設定
 *
 * Todo
 * 線の太さを考慮したアンチエイリアス 線にした方がいいかも
 */
void CMLcdFuncAa::drawLine(int x1, int y1, int x2, int y2, unsigned long style, int logic)
{
	int i, j;
	int x, y;
	int dx, dy;
	int sx, sy;
	int a, e;
	int color[32];

	x1 = pix->convertToReal( x1 );
	y1 = pix->convertToReal( y1 );
	x2 = pix->convertToReal( x2 );
	y2 = pix->convertToReal( y2 );

#ifdef DEBUG
	fprintf(stderr, "drawLine (%d, %d)-(%d, %d), styel %x, logic %x\n", x1, y1, x2, y2, style, logic);
	fflush(stderr);
#endif

	for(i=0; i < 32; i++, style>>=1)
		color[i]=(style & 1 ? 255 : 0);

/* シグナム int */
#define sgni(x) ((x) > 0 ? 1 : -1)

	dx=abs(x2-x1);
	dy=abs(y2-y1);
	sx=sgni(x2-x1);
	sy=sgni(y2-y1);

	if( dx == 0 )
	{
		y = y1;
		for( i = 0 ; i <= dy; i++ )
		{
			for(j = 0; j < expansion; j++)
				setPointReal(x1 + j, y, color[i & 0x1f], logic);
			y += sy;
		}
	}
	else if( dy == 0 )
	{
		x = x1;
		for( i = 0 ; i <= dx; i++ )
		{
			for(j = 0; j < expansion; j++)
				setPointReal(x, y1 + j, color[i & 0x1f], logic);
			x += sx;
		}
	}
	else if(dx > dy)
	{
		x=x1;
		y=y1;
		e=-dx;
		a=2*dy;
		for(i=0; i <= dx; i++)
		{
			int c;

			c = - color[i & 0x1f] * e / 2 / dx;

			setPointReal(x, y,      c, logic);
			if( c == 0 )
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x, y + sy * j,   0, logic);

				setPointReal(x, y + sy * j, 0, logic);
			}
			else
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x, y + sy * j,   255, logic);

				setPointReal(x, y + sy * j, 255 - c, logic);
			}

			e+=a;
			if(e >= 0)
			{
				y+=sy;
				e-=2*dx;
			}

			x+=sx;
		}
	}
	else
	{
		x=x1;
		y=y1;
		e=-dy;
		a=2*dx;

		for(i=0; i <= dy; i++)
		{
			int c;

			c = - color[i & 0x1f] * e / 2 / dy;

			setPointReal(x, y,      c, logic);

			if( c == 0 )
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x + sx * j, y,   0, logic);

				setPointReal(x + sx * j, y, 0, logic);
			}
			else
			{
				for(j = 1; j < expansion; j++)
					setPointReal(x + sx * j, y,   255, logic);

				setPointReal(x + sx * j, y, 255 - c, logic);
			}

			e+=a;
			if(e >= 0)
			{
				x+=sx;
				e-=2*dy;
			}

			y+=sy;
		}
	}
}


/** 四角を描く
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 * @param style 8bit で点線を表現できる。0xff で*ただ*の線
 * @param logic 上書き等の設定
 */
void CMLcdFuncAa::drawRect(int x1, int y1, int x2, int y2, unsigned char style, int logic)
{
	unsigned long ulstyle;

	ulstyle=((unsigned long)style << 24) | ((unsigned long)style << 16) | ((unsigned long)style << 8) | style;

	drawLine(x1, y1, x2, y1, ulstyle, logic);
	drawLine(x2, y1, x2, y2, ulstyle, logic);
	drawLine(x2, y2, x1, y2, ulstyle, logic);
	drawLine(x1, y2, x1, y1, ulstyle, logic);
}

/** 塗り潰し四角を描く
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 * @param style 32bit で
 * @param logic 上書き等の設定
 */
void CMLcdFuncAa::fillRect(int x1, int y1, int x2, int y2, unsigned long style, int logic)
{
	int ymin, ymax;
	int y;

	ymin=min(y1, y2);
	ymax=max(y1, y2);
	for(y=ymin; y < ymax; y++)
		drawLine(x1, y, x2, y, style, logic);
}



/** 文字を描く
 * @param x x 座標（文字の左上の座標）
 * @param y y 座標
 * @param fontwidth 文字幅
 * @param fontheight 文字高
 * @param str 文字列へのポインタ
 * @param prop_flag プロポーショナルフォントか否か（未使用）
 * @param gap 文字間のギャップ（ドット）
 * @param logic 上書き等の設定
 */
void CMLcdFuncAa::drawStringSub(int x, int y, int fontwidth, int fontheight, char const *str, int prop_flag, int gap, int logic, int latin)
{
	SDLcdFont *font;
	int width;
	unsigned char **p;

	SDLcd *sdlcd = ruputer->getSDLcd();

	if( sdlcd->hasFontAa() )
	{
		font = sdlcd->createSDLcdFontAa( pix->convertToReal( fontwidth ), pix->convertToReal( fontheight ), prop_flag, gap, latin);
		if( font == NULL )
			return;

		p = font->drawString(str, &width);
		if( p == NULL )
		{
			delete font;
			return;
		}

		subDrawStringReal(p, pix->convertToReal( x ), pix->convertToReal( y ), width, pix->convertToReal( fontheight ), logic);
		font->freeBuffer(p);

		delete font;
	}
	else
	{
		CMLcdFuncDefault::drawStringSub(x, y, fontwidth, fontheight, str, prop_flag, gap, logic, latin);
	}
}


/** 文字をレンダリングしたバッファから、LCDバッファへコピーする
 * @param buf 1文字をレンダリングしたバッファ
 * @param x x 座標（文字の左上の座標）
 * @param y y 座標
 * @param w 描画した幅
 * @param h 文字高
 * @param logic 上書き等の設定
 */
void CMLcdFuncAa::subDrawStringReal(unsigned char **buf, int x, int y, int w, int h, int logic)
{
	int i, j;

	int logic2;

	logic2 = logic << 2;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
			setPointReal(x + i, y + j, buf[j][i], logic2);
		}
	}
}



/** 反転
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 */
void CMLcdFuncAa::reverseRect(int x1, int y1, int x2, int y2)
{
	int i, j;
	int dx, dy;
	int x, y;

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

	dx=abs(x1-x2);
	dy=abs(y1-y2);
	x=min(x1, x2);
	y=min(y1, y2);

#if 1
	dx = pix->convertToReal( dx );
	dy = pix->convertToReal( dy );
	x  = pix->convertToReal( x );
	y  = pix->convertToReal( y );

	dx += expansion - 1;
	dy += expansion - 1;
#endif

	for(i=0; i <= dy; i++)
	{
		for(j=0; j <= dx; j++)
		{
#if 1
			pix->psetReal(x + j, y + i, 255 - pix->pgetReal(x + j, y + i));

#else
			pix->pset(x + j, y + i, pix->pget(x + j, y + i) ^ 1);
#endif
		}
	}
}

/** 消去
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 */
void CMLcdFuncAa::clearRect(int x1, int y1, int x2, int y2)
{
	int i, j;
	int dx, dy;
	int x, y;

	dx=abs(x1-x2);
	dy=abs(y1-y2);
	x=min(x1, x2);
	y=min(y1, y2);

#if 1
	dx = pix->convertToReal( dx );
	dy = pix->convertToReal( dy );
	x  = pix->convertToReal( x );
	y  = pix->convertToReal( y );

	dx += expansion - 1;
	dy += expansion - 1;
#endif

	for(i=0; i <= dy; i++)
	{
		for(j=0; j <= dx; j++)
		{
#if 1
			pix->psetReal(x + j, y + i, 0);
#else
			pix->pset(x + j, y + i, 0);
	//			now[y+i][x+j]=0;
#endif
		}
	}
}


/** 楕円を描く
 * 奥村晴彦著・『Ｃ言語による最新アルゴリズム事典』より
 * 奥村先生ありがとうございます。
 *
 */
void CMLcdFuncAa::drawEllipse(int xc, int yc, int rx, int ry, int logic)
{
	int x, x1, y, y1, r;

	xc = pix->convertToReal( xc );
	yc = pix->convertToReal( yc );
	rx = pix->convertToReal( rx );
	ry = pix->convertToReal( ry );

#ifdef DEBUG
	fprintf(stderr, "\
!!  CMLcdFuncAa::drawEllipse start\n\
  xc: %d\n\
  yc: %d\n\
  rx: %d\n\
  ry: %d\n\
!!  CMLcdFuncAa::drawEllipse end\n", xc, yc, rx, ry);
	fflush(stderr);
#endif

	if (rx > ry) {
		x = r = rx;  y = 0;
		while (x >= y) {
			x1 = (int)((long)x * ry / rx);
			y1 = (int)((long)y * ry / rx);
			setPointReal(xc + x, yc + y1, 255, logic);
			setPointReal(xc + x, yc - y1, 255, logic);
			setPointReal(xc - x, yc + y1, 255, logic);
			setPointReal(xc - x, yc - y1, 255, logic);
			setPointReal(xc + y, yc + x1, 255, logic);
			setPointReal(xc + y, yc - x1, 255, logic);
			setPointReal(xc - y, yc + x1, 255, logic);
			setPointReal(xc - y, yc - x1, 255, logic);
			if ((r -= (y++ << 1) - 1) < 0)
				r += (x-- - 1) << 1;
		}
	} else {
		x = r = ry;  y = 0;
		while (x >= y) {
			x1 = (int)((long)x * rx / ry);
			y1 = (int)((long)y * rx / ry);
			setPointReal(xc + x1, yc + y, 255, logic);
			setPointReal(xc + x1, yc - y, 255, logic);
			setPointReal(xc - x1, yc + y, 255, logic);
			setPointReal(xc - x1, yc - y, 255, logic);
			setPointReal(xc + y1, yc + x, 255, logic);
			setPointReal(xc + y1, yc - x, 255, logic);
			setPointReal(xc - y1, yc + x, 255, logic);
			setPointReal(xc - y1, yc - x, 255, logic);
			if ((r -= (y++ << 1) - 1) < 0)
				r += (x-- - 1) << 1;
		}
	}
}

unsigned char const *CMLcdFuncAa::getPixelArray(int x, int y)
{
	return pix->getPixelArray(x, y);
}
