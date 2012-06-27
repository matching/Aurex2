/*
 * optionitem.cpp -
 *
 * $Id: optionitem.cpp,v 1.15 2004/11/12 19:06:11 fumi Exp $
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

#include "optionitem.h"

/** オプションの項目ひとつに対応するオブジェクト
 * コンストラクタ
 */
OptionItem::OptionItem(char const *key)
{
	assert( key );

	int len;

	len = strlen(key);
	this->key = new char[len+1];
	memcpy(this->key, key, len+1);
}
OptionItem::~OptionItem(void)
{
	delete[] this->key;
}

/**
 * key 値を得る
 */
char const *OptionItem::getKeyString(void)
{
	return (char const *)key;
}
