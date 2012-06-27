/*
 * cmlcd.cpp - common drawing module for Lcd
 *
 * $Id: cmlcd.cpp,v 1.28 2006/02/16 18:19:40 fumi Exp $
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "cmlcd.h"
#include "cmlcdfunc.h"
#include "cmlcdfuncaa.h"
#include "cmlcdfuncdefault.h"
#include "cmlcdpix.h"

#include "sdlcd.h"
#include "sdlcdfont.h"
#include "ruputer.h"
#include "rupaddress.h"
#include "blinkmgr.h"
#include "reversemgr.h"

#include "option.h"


/** 共通LCD描画ルーチンコンストラクタ
 */
CMLcd::CMLcd(Ruputer *ruputer)
{
	this->expansion = ::option->get_expansion();
//	latin=false;

	this->ruputer = ruputer;
	this->updateRegion = PART;

	blinkmgr = new BlinkMgr();
	reversemgr = new ReverseMgr();

	func = createCMLcdFunc(this->expansion);
}

/** 共通LCD描画ルーチン デストラクタ
 */
CMLcd::~CMLcd(void)
{
	delete blinkmgr;
	delete reversemgr;
	delete func;
}


CMLcdFunc *CMLcd::getCMLcdFunc(void)
{
	return func;
}

void CMLcd::updateAllPixel(void)
{
	updateRegion = ALL;
}

void CMLcd::updatePartPixel(void)
{
	updateRegion = PART;
}

/** ピクセルの残映処理
 */
void CMLcd::updatePixel(void)
{
//blink
	blinkmgr->proceed();
//reverse
// 呼び出す意味なし
//	reversemgr->proceed();

//	fprintf(stderr, "CMLcd::updatePixel:: %d (ALL: %d, PART: %d)\n", updateRegion, ALL, PART);


	if( updateRegion == PART )
	{
		int x, y;

		ruputer->getSystemAddress()->get_lcdPlace(&x, &y);
		func->updatePixel(x, y, PART);
	}
	else
	{
		func->updatePixel(0, 0, ALL);
	}
}




int CMLcd::getRealWidth102(void)
{
	return expansion * 102;
}
int CMLcd::getRealWidth320(void)
{
	return expansion * 320;
}

int CMLcd::getRealHeight64(void)
{
	return expansion * 64;
}
int CMLcd::getRealHeight240(void)
{
	return expansion * 240;
}

int CMLcd::getExpansion(void)
{
	return expansion;
}


void CMLcd::setExpansion(int expansion)
{
#if 0
	if( this->expansion == expansion )
		return;
#endif

	this->expansion = expansion;

	CMLcdFunc *newFunc;

	newFunc = createCMLcdFunc(expansion);

// todo Pixel情報引き継ぐようにする。
	newFunc->getCMLcdPix()->setPreviousPix( func->getCMLcdPix() );
	delete func;

	func = newFunc;
}

CMLcdFunc *CMLcd::createCMLcdFunc(int expansion)
{
	assert( ::option->get_ai_value() > 0);

	if( ::option->get_anti_alias() )
		return new CMLcdFuncAa(this->ruputer, expansion, ::option->get_after_image(), ::option->get_ai_value());
	else
		return new CMLcdFuncDefault(this->ruputer, expansion, ::option->get_after_image(), ::option->get_ai_value());
}
