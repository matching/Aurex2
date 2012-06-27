/*
 * lcdblrvmgr.cpp - Blink and Reverse region manager common module
 *
 * $Id: lcdblrvmgr.cpp,v 1.13 2006/02/16 13:05:12 fumi Exp $
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


#include "cmlcd.h"
#include "lcdblrv.h"
#include "lcdblrvmgr.h"

#include <map>
#include <iterator>
using namespace std;

enum BlinkReversePixelValue const *LcdBlRvMgr::pixel_status[240][320];

/** コンストラクタ
 * @param _cmlcd cmlcdのインスタンス
 */
LcdBlRvMgr::LcdBlRvMgr(void)
{
	memset(pixel_status, 0, sizeof(enum BlinkReversePixelValue *) * 240 * 320);
}
LcdBlRvMgr::~LcdBlRvMgr(void)
{
}

/** リージョンのインスタンスを追加する
 * @param dat リージョンのインスタンス
 * @retval 0xffff 失敗
 * @retval 上記以外 成功。戻り値はリージョン番号
 */
int LcdBlRvMgr::add(LcdBlRv *dat)
{
	int no;

	no = findFreeNo();
	if(no == 0xffff)
		return no;

	updatePixelStatus(dat, dat->getPixelSatatePointer());

	regions.insert(pair<int, LcdBlRv *>(no, dat));

	return no;
}

/**
 *
 * @retval 0xffff 空きが見つからない
 * @retval !0xffff リージョン番号
 */
int LcdBlRvMgr::findFreeNo(void)
{
	unsigned short usedBit=0;
	int no;

	map<int, LcdBlRv *>::iterator i;

	i=regions.begin();
	for(; i != regions.end(); i++)
		usedBit|=1 << (i->first);

	if(usedBit == 0xffff)
		return 0xffff;

	for(no=0; no <= 15; no++)
	{
		if((usedBit & (1 << no)) == 0)
			return no;
	}

// not reached
	return 0xffff;
}

/** リージョンの削除
 * @param no リージョン番号
 * @retval -1 削除失敗
 * @retval  0 削除成功
 */
int LcdBlRvMgr::del(int no)
{
	map<int, LcdBlRv *>::iterator i;

	i=regions.find(no);
	if(i == regions.end())
		return -1;

	updatePixelStatus(i->second, NULL);

	delete i->second;
	regions.erase(i);

	return 0;
}

/** リージョンの状態変更
 * @param no リージョン番号
 * @param state 変更する状態
 * @retval -1 失敗
 * @retval  0 成功
 */
int LcdBlRvMgr::setState(int no, int state)
{
	map<int, LcdBlRv *>::iterator i;

	i=regions.find(no);
	if(i == regions.end())
		return -1;

	i->second->setState(state);
	return 0;
}

/** リージョンを開始する
 * @param no リージョン番号
 * @retval -1 失敗
 * @retval  0 成功
 */
int LcdBlRvMgr::start(int no)
{
	return setState(no, BLINK_REVERSE_START);
}

/** リージョンを停止状態にする
 * @param no リージョン番号
 * @retval -1 失敗
 * @retval  0 成功
 */
int LcdBlRvMgr::stop(int no)
{
	return setState(no, BLINK_REVERSE_STOP);
}


/** 登録されているリージョンを実行する
 */
void LcdBlRvMgr::proceed(void)
{
	map<int, LcdBlRv *>::iterator i;

	i=regions.begin();
	for(; i != regions.end(); i++)
	{
		if(i->second->getState() == BLINK_REVERSE_START)
			i->second->proceed();
		else
			i->second->pixel_on();
	}
}

/** 全てのリージョンを閉じる
 */
void LcdBlRvMgr::closeAll(void)
{
	map<int, LcdBlRv *>::iterator i;

	i=regions.begin();
	while(i != regions.end())
	{
		updatePixelStatus(i->second, NULL);

		delete i->second;
		i++;
	}

	regions.clear();
}





#if 0
enum BlinkReversePixelValue LcdBlRvMgr::getPixelStatus(int x, int y)
{
	enum BlinkReversePixelValue const *p;

	p = pixel_status[y][x];

	if( p == NULL )
		return ON;
	return *p;
}
#endif


void LcdBlRvMgr::updatePixelStatus(LcdBlRv *dat, enum BlinkReversePixelValue const *p)
{
	int x, y;
	int w, h;

	x = dat->getX();
	y = dat->getY();
	w = dat->getWidth();
	h = dat->getHeight();

	int i, j;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
			pixel_status[ y + j ][ x + i ] = p;

	}
}

