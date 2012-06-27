/*
 * bdfmgr.cpp - BDF management class
 *
 * $Id: bdfmgr.cpp,v 1.4 2004/05/07 08:25:03 fumi Exp $
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

#include "bdfmgr.h"
#include "bdffontset.h"
#include "bdffontsets.h"
#include "bdffontsetsjis.h"

#include <map>
using namespace std;

/** コンストラクタ
 */
BdfMgr::BdfMgr(void)
{
}
/** デストラクタ
 */
BdfMgr::~BdfMgr(void)
{
}

/** BdfFontSet クラスを作成する
 * @param filename ファイル名
 * @param height 高さ
 * @retval NULL 作成失敗
 * @retval != NULL BdfFontSet * のインスタンス
 */
BdfFontSet *BdfMgr::createBdfFontSet(char const *filename, int height)
{
	BdfFontSet *bfs;
	int ret;

	bfs=new BdfFontSet();
	ret=bfs->parse(filename, height);
	if(ret != 0)
	{
		delete bfs;
		return NULL;
	}
	return bfs;
}

/** iso8859-1 の bdf フォントを追加する
 * @param fontname ファイル名
 * @param height 高さ
 * @retval なし
 */
void BdfMgr::add_iso88591_font(char const *fontname, int height)
{
	BdfFontSet *bfs;

	bfs=createBdfFontSet(fontname, height);
	if(bfs == NULL)
		return;
	iso88591_fonts.insert(pair<int, BdfFontSet *>(height, bfs));
}

/** jisx0201 の bdf フォントを追加する
 * @param fontname ファイル名
 * @param height 高さ
 * @retval なし
 */
void BdfMgr::add_jisx0201_font(char const *fontname, int height)
{
	BdfFontSet *bfs;

	bfs=createBdfFontSet(fontname, height);
	if(bfs == NULL)
		return;
	jisx0201_fonts.insert(pair<int, BdfFontSet *>(height, bfs));
}

void BdfMgr::add_jisx0208_font(char const *fontname, int height)
{
	BdfFontSet *bfs;

	bfs=createBdfFontSet(fontname, height);
	if(bfs == NULL)
		return;
	jisx0208_fonts.insert(pair<int, BdfFontSet *>(height, bfs));
}

BdfFontSets *BdfMgr::getJisBdfFont(int height)
{
	map<int, BdfFontSet *>::iterator i;
	BdfFontSet *jisx0201=NULL;
	BdfFontSet *jisx0208=NULL;

	i=jisx0201_fonts.find(height);
	if(i != jisx0201_fonts.end())
		jisx0201=i->second;

	i=jisx0208_fonts.find(height);
	if(i != jisx0208_fonts.end())
		jisx0208=i->second;

	if(jisx0201 == NULL && jisx0208 == NULL)
		return NULL;

#if DEBUG
	fprintf(stderr, "-- BdfMgr::getJisBdfFont --\n jisx0201: %p, jisx0208 %p\n---\n", jisx0201, jisx0208);
	fflush(stderr);
#endif


	return new BdfFontSetsJis(jisx0201, jisx0208);
}

BdfFontSets *BdfMgr::getIsoBdfFont(int height)
{
#if 0
	map<int, BdfFontSet *>::iterator i;

	i=iso88591_fonts.find(height);
	if(i == iso88591_fonts.end())
		return NULL;

	return new BdfFontSetsIso(i->second);
#endif
}

