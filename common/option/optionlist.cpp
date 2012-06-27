/*
 * optionlist.cpp - keep option list as option
 *
 * $Id: optionlist.cpp,v 1.8 2006/02/16 18:19:40 fumi Exp $
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
#include <string.h>
#include <ctype.h>
#ifdef DEBUG
#include <unistd.h>
#endif

#include <assert.h>

#include <string>
#include <iterator>
#include <list>
#include <map>
using namespace std;

#include "optionlist.h"

static void removeHeadAndTailSpace(char **str)
{

	char *p;

	p=*str;
// 前から削除
	while(isspace(*p))
		p++;
	*str=p;

	if( **str == 0 )
		return;

// 後を削除
	p=strchr(*str, 0);
	p--;
	while(isspace(*p))
		*p--=0;
}


OptionList::OptionList(void)
{
}


OptionList::~OptionList(void)
{
}

/** オプションを追加する
 * @param key オプションのキーワード
 * @param value その値
 */
void OptionList::addOption(char const *key, char const *value)
{
	assert( key );
//	assert( value );


#ifdef DEBUG
	fprintf(stderr, "add option key: %s , value:%s\n", key, value);
#endif

	options.insert(pair<string, string>(key, value));
}


/** オプションを保存する
 * 変更されてなければ保存しない（未実装）
 * 実行中にオプションが変更されないことが示唆されている場合は保存しない（未実装）
 * しかし、オプションのファイルが存在しない場合は作成する（未実装）
 *
 * 現状は無条件にファイル書き出す
 * @param filename ファイル名
 */
bool OptionList::save(char const *filename)
{
	FILE *fp;

#ifdef DEBUG
	{
		char dir[1024];
		getcwd(dir, sizeof(dir));
		fprintf(stderr, "save %s / %s\n", dir, filename);
	}
#endif

	fp=fopen(filename, "w");
	if(fp == NULL)
		return false;

	map<string, string>::iterator i;
	i=options.begin();
	while(i != options.end())
	{
		fprintf(fp, "%s=%s\n", i->first.c_str(), i->second.c_str());
		i++;
	}

	fclose(fp);
	return true;
}
/** オプションをロードする
 * @param filename ロードするファイル名
 * @retval true 成功
 * @retval false 異常
 */
bool OptionList::load(char const *filename)
{
	FILE *fp;

	fp=fopen(filename, "r");
	if(fp == NULL)
		return false;

	{
		char str[512];
		char *key, *eq, *value;

		while(fgets(str, sizeof(str), fp) != NULL)
		{
			if(*str == '#')
				continue;

			eq=strchr(str, '=');
			if(eq == NULL)
			{
		// error ?
				continue;
			}

			*eq=0;
			value=eq+1;
			key=str;
			removeHeadAndTailSpace(&key);
			removeHeadAndTailSpace(&value);

			addOption(key, value);
		}
		fclose(fp);

	}
	return true;
}

/** ロード時のオプションと*現在*のオプションが同じかどうか判断する（現在未実装）
 * @retval 1 同じ
 * @retval 0 違う
 */
int OptionList::equalLoadOption(void)
{
	return 1;
}

/** オプションを得る
 * @param key キー（オプション名）
 * @retval bool その値
 */
char const *OptionList::getOptionValue(char const *key)
{
	map<string, string>::iterator i;
	string str(key);

	i = options.find(str);
// 見つからなかった
	if(i == options.end())
		return NULL;

	return i->second.c_str();
}

void OptionList::setOptionValue(char const *key, char const *value)
{
	map<string, string>::iterator i;
	string str(key);

	i = options.find(str);
// 見つからなかった→追加する？
	if(i == options.end())
		return;

	i->second.assign(value);
}
