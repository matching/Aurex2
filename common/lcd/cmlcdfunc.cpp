/*
 * cmlcdfunc.cpp - common drawing module for 
 *
 * $Id: cmlcdfunc.cpp,v 1.5 2006/01/22 15:35:28 fumi Exp $
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
#include <string.h>
#include <assert.h>

#include "cmlcdpix.h"
#include "cmlcdfunc.h"
#include "memory.h"

#include "option.h"


/** 共通LCD描画ルーチンコンストラクタ
 */
CMLcdFunc::CMLcdFunc(Ruputer *ruputer, int expansion)
{
	this->expansion = expansion;
	this->ruputer = ruputer;
	this->pix = NULL;
}

/** 共通LCD描画ルーチン デストラクタ
 */
CMLcdFunc::~CMLcdFunc(void)
{
}

/*
 */
CMLcdPix *CMLcdFunc::getCMLcdPix(void)
{
	return pix;
}


/** ピクセルの残映処理
 */
void CMLcdFunc::updatePixel(int x, int y, enum UpdateRegion reg)
{
	pix->updatePixel(x, y, reg);
}


/** LCDの点の明暗を設定する（public）
 * @param x x 座標
 * @param y y 座標
 * @param color 0:白/1:黒
 * @param logic  gv_line 系の logic は下位 2 bitに格納され、
 * gv_kput 系の logic は 2bit 左シフトして格納される
 */
void CMLcdFunc::setPoint(int x, int y, int color, int logic)
{
	int c;

	switch(logic)
	{
	case 0:	/* 上書き */
		pix->pset(x, y, color);
		break;
	case 1: /* xor */
		c = pix->pget(x, y);
		c=c ^ color;
		pix->pset(x, y, c);
		break;
	case 4: /* string or */
	case 2:	/* or */
		c = pix->pget(x, y);
		c=c | color;
		pix->pset(x, y, c);
		break;
	case 3:	/* and */
		c = pix->pget(x, y);
		c=c & color;
		pix->pset(x, y, c);
		break;

	case 8: /* 反転上書き */
		color = color ^ 1;
		pix->pset(x, y, color);
		break;

	case 0xc: /* 反転 OR */
		c = pix->pget(x, y);
		c = c | ( color ^ 1 );
		pix->pset(x, y, c);
		break;
	}
}

int CMLcdFunc::getPoint(int x, int y)
{
	return pix->pget( x, y );
}


/** MMPデータの取得
 * @param x1 x1 座標
 * @param y1 y1 座標
 * @param x2 x2 座標
 * @param y2 y2 座標
 * @param buf 取得データ（先頭に width, height が付く）
 */
void CMLcdFunc::getMMPData(int x1, int y1, int x2, int y2, unsigned char *buf)
{
	int width, height;
	int x, y;
	int c, i;

	MemoryForInside *mem;

	mem=new MemoryForInside(buf);

	width=abs(x2-x1)+1;
	height=abs(y2-y1)+1;

	mem->write16(0, width);
	mem->write16(2, height);

#ifdef DEBUG
	fprintf(stderr, "!! getMMP (%d, %d)-(%d, %d), %d, %d %p\n", x1, y1, x2, y2, width, height, buf);
	fflush(stderr);
#endif

	buf+=4;
	for(y=0; y < height; y+=8)
	{
		for(x=0; x < width; x++, buf++)
		{
			*buf=0;
			for(i=0; i < 8 && y+i < height; i++)
			{
				c = pix->pget(x1 + x, y1 + y + i);
				if(c)
					*buf|=1 << i;
			}
		}
	}

	delete mem;
}

/** MMPデータの表示
 * @param start_x x 座標
 * @param start_y y 座標
 * @param buf 表示データ（先頭に width, height が付いていること）
 * @param logic 論理演算の指定
 */
void CMLcdFunc::putMMPData(int start_x, int start_y, unsigned char *buf, int logic)
{
	int width, height;
	MemoryForInside *mem;


	mem=new MemoryForInside(buf);

	width=mem->read16_s(0);
	height=mem->read16_s(2);

#ifdef DEBUG
	fprintf(stderr, "!! putMMP (%d, %d), %d, %d %p\n", start_x, start_y, width, height, buf);
	fflush(stderr);
#endif

	putMMPDataWithoutWH(start_x, start_y, width, height, buf+4, logic);

	delete mem;
}

/** MMPデータの表示（バッファに width, height が無いこと）
 * @param start_x x 座標
 * @param start_y y 座標
 * @param width 幅
 * @param height 高さ
 * @param buf 表示データ（先頭に width, height が付いていないこと）
 * @param logic 論理演算の指定
 */
void CMLcdFunc::putMMPDataWithoutWH(int start_x, int start_y, int width, int height, unsigned char *buf, int logic)
{
	int x, y;
	int i;
	int end_x;

	end_x=start_x+width;
	for(y=0; y < height; y+=8)
	{
		x=start_x;
		for(; x < end_x; x++, buf++)
		{
			for(i=0; i < 8 && y+i < height; i++)
				setPoint(x, start_y+y+i, (*buf >> i) & 1, logic);
		}
	}
}



/** 文字を描く。ただし実際の描画は drawStringSub を呼び出して描く。
 * ただ単に、Latin の対応、と考えて良いです。
 *
 * @param x x 座標（文字の左上の座標）
 * @param y y 座標
 * @param fontwidth 文字幅
 * @param fontheight 文字高
 * @param str 文字列へのポインタ
 * @param prop_flag プロポーショナルフォントか否か（未使用）
 * @param gap 文字間のギャップ（ドット）
 * @param logic 上書き等の設定
 */
void CMLcdFunc::drawString(int x, int y, int fontwidth, int fontheight, char const *str, int prop_flag, int gap, int logic)
{
	drawStringSub(x, y, fontwidth, fontheight, str, prop_flag, gap, logic, ::option->get_useIso8859_1() );
}
