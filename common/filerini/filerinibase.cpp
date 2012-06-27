/*
 * filerinibase.cpp - filer.ini base class
 *
 * $Id: filerinibase.cpp,v 1.5 2005/01/08 15:51:53 fumi Exp $
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
#include "filerinibase.h"
#include "extlist.h"

/** コンストラクタ
 */
FilerIniBase::FilerIniBase(void)
{
}

/** デストラクタ
 */
FilerIniBase::~FilerIniBase(void)
{
}

/** <拡張子, 対応するexf名> を追加する。
 * @param ext 拡張子（3文字以下）
 * @param exf Ruputer パスの実行exfのフルパス
 */
void FilerIniBase::add(char const *ext, char const *exf)
{
#ifdef DEBUG
	fprintf(stderr, "add < (%s), (%s) >\n", ext, exf);
	fflush(stderr);
#endif

	char normalize_ext[256];
	normalizeExt(ext, normalize_ext);

	assoc.insert(pair<string, string>(normalize_ext, exf));
}


/** 拡張子から、実行すべきexfのフルパス（Ruputer）を得る
 * @param ext 拡張子
 * @retval !=NULL 実行するexfのフルパス
 * @retval NULL 拡張子が登録されていない
 */
char const *FilerIniBase::getExecutableFileNameByExt(char const *ext)
{
	char normalize_ext[256];
	normalizeExt(ext, normalize_ext);

	map<string, string>::iterator i;
	i = assoc.find( normalize_ext );
	if(i == assoc.end())
		return NULL;

// これって大丈夫なのかしら。
	return i->second.c_str();
}

/**
 * 小文字に正規化を行なう
 *
 */
void FilerIniBase::normalizeExt(char const *in, char *out)
{
	while( *in )
	{
		if( *in >= 'A' && *in <= 'Z' )
			*out = *in - 'A' + 'a';
		else
			*out = *in;

		in++;
		out++;
	}
	*out = 0;
}



ExtList *FilerIniBase::getExtList(void)
{
	ExtList *extlist = new ExtList();

	map<string, string>::iterator i;
	i = assoc.begin();
	while( i != assoc.end() )
	{
// string で追加の方がいいかも？
		extlist->add( i->first.c_str() );

		i++;
	}

	return extlist;
}
