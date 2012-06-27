/*
 * option.cpp - Aurex default option class
 *
 * $Id: option.cpp,v 1.21 2006/03/06 12:58:17 fumi Exp $
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
#include <assert.h>

#include <string>
#include <list>
#include <iterator>
#include <map>
using namespace std;

#include "config.h"
#include "option.h"
#include "optionlist.h"
#include "optionitem.h"

#include "log.h"

Option *option;

/** コンストラクタ
 */
Option::Option(void)
{
// 各オプションのデフォルト値の設定
	addOptionItem(warn_odd_access  = new OptionBool(OPT_WARN_ODD_ACCESS, OPTION_DEFAULT_WARN_ODD_ACCESS));
	addOptionItem(enable_sound     = new OptionBool(OPT_SOUND, OPTION_DEFAULT_SOUND)          );
	addOptionItem(enable_cpu_timer = new OptionBool(OPT_CPU_TIMER, OPTION_DEFAULT_CPU_TIMER)      );
//	addOptionItem(enable_log       = new OptionBool(OPT_LOG, false)            );
	addOptionItem(cpu_speed        = new OptionInt(OPT_CPU_SPEED, OPTION_DEFAULT_CPU_SPEED)         );
	addOptionItem(timer_speed      = new OptionInt(OPT_TIMER_SPEED, OPTION_DEFAULT_TIMER_SPEED)         );
	addOptionItem(root_directory   = new OptionString(OPT_ROOT_DIRECTORY, "")  );

	addOptionItem(enable_after_image = new OptionBool(OPT_AFTER_IMAGE, OPTION_DEFAULT_AFTER_IMAGE)   );
	addOptionItem(enable_anti_alias  = new OptionBool(OPT_ANTI_ALIAS, OPTION_DEFAULT_ANTI_ALIAS)    );
	addOptionItem(expansion          = new OptionInt(OPT_EXPANSION, OPTION_DEFAULT_EXPANTION) );

	addOptionItem(ai_value           = new OptionInt(OPT_AI_VALUE, OPTION_DEFAULT_AI_VALUE)           );

	addOptionItem(useIso8859_1       = new OptionBool(OPT_USE_ISO8859_1, OPTION_DEFAULT_USE_ISO8859_1) );

	addOptionItem(log_level          = new OptionInt(OPT_LOG_LEVEL, Log::Info) );
	addOptionItem(log_type           = new OptionInt(OPT_LOG_TYPE, Log::TYPE_NULL) );
	addOptionItem(log_file           = new OptionString(OPT_LOG_FILE, "" )) ;

}


/** デストラクタ
 */
Option::~Option(void)
{
	delete warn_odd_access;
	delete enable_sound;
	delete enable_cpu_timer;
//	delete enable_log;
	delete cpu_speed;
	delete timer_speed;
	delete root_directory;

	delete enable_after_image;
	delete enable_anti_alias;
	delete expansion;
	delete ai_value;
	delete useIso8859_1;

	delete log_level;
	delete log_type;
	delete log_file;
}

/**
 * オプションのロード
 * @param filename ファイル名
 * @retval true 成功
 * @retval false 失敗
 */
bool Option::load(char const *filename)
{
	assert( filename );

	optionlist.load(filename);

	syncFromOptionList();

	return true;
}

void Option::addOptionItem(OptionItem *item)
{
	assert( item );

	optionitems.push_front(item);
}

void Option::syncFromOptionList(void)
{
	list<OptionItem *>::iterator i;

	i = optionitems.begin();

	for(; i != optionitems.end(); i++)
	{
		char const *key;
		char const *value;

		key = (*i)->getKeyString();
		value = optionlist.getOptionValue(key);
		if( value == NULL)
		{
// 見つからなかった → 今のオプションをリストに追加しておく
			char value2[256];

			(*i)->getValueString(value2);
			optionlist.addOption(key, value2);

			continue;
		}

// 見つかった
		if(! (*i)->setValueString(value) )
		{
			fprintf(stderr, "Warning, parse error option=%s, value=%s\n", key, value);
			continue;
		}
	}
}

/**
 *
 */
void Option::syncToOptionList(void)
{
	list<OptionItem *>::iterator i;

	i = optionitems.begin();
	for(; i != optionitems.end(); i++)
	{
		char const *key;
		char value[256];

		key = (*i)->getKeyString();
		(*i)->getValueString(value);
#if 0
		fprintf(stderr, "Option::syncToOptionList (key . value) = (%s . %s)\n", key, value);
		fflush(stderr);
#endif
		optionlist.setOptionValue(key, value);
	}
}


/**
 * オプションのセーブ
 * @param filename ファイル名
 * @retval true 成功
 * @retval false 失敗
 */
bool Option::save(char const *filename)
{
	assert( filename );

	optionlist.save(filename);

	return true;
}


/* 以下アクセサ */
bool Option::get_warn_odd_access(void)
{
	return warn_odd_access->getBoolValue();
}
void Option::set_warn_odd_access(bool v)
{
	warn_odd_access->setBoolValue(v);
}


bool Option::get_enable_sound(void)
{
	return enable_sound->getBoolValue();
}
void Option::set_enable_sound(bool v)
{
	enable_sound->setBoolValue(v);
}

bool Option::get_enable_cpu_timer(void)
{
	return enable_cpu_timer->getBoolValue();
}
void Option::set_enable_cpu_timer(bool v)
{
	enable_cpu_timer->setBoolValue(v);
}


int Option::get_cpu_speed(void)
{
	return cpu_speed->getIntValue();
}
void Option::set_cpu_speed(int v)
{
	cpu_speed->setIntValue(v);
}

int Option::get_timer_speed(void)
{
	return timer_speed->getIntValue();
}
void Option::set_timer_speed(int v)
{
	timer_speed->setIntValue(v);
}

char const *Option::get_root_directory(void)
{
	return root_directory->getStringValue();
}

void Option::set_root_directory(char const *v)
{
	root_directory->setStringValue(v);
}


bool Option::get_anti_alias(void)
{
	return enable_anti_alias->getBoolValue();
}
void Option::set_anti_alias(bool v)
{
	enable_anti_alias->setBoolValue(v);
}

bool Option::get_after_image(void)
{
	return enable_after_image->getBoolValue();
}
void Option::set_after_image(bool v)
{
	enable_after_image->setBoolValue(v);
}


int Option::get_expansion(void)
{
	return expansion->getIntValue();
}

void Option::set_expansion(int v)
{
	expansion->setIntValue(v);
}

int Option::get_ai_value(void)
{
	return ai_value->getIntValue();
}

void Option::set_ai_value(int v)
{
	ai_value->setIntValue(v);
}

bool Option::get_useIso8859_1(void)
{
	return useIso8859_1->getBoolValue();
}
void Option::set_useIso8859_1(bool v)
{
	useIso8859_1->setBoolValue(v);
}


int Option::get_log_level(void)
{
	return log_level->getIntValue();
}

void Option::set_log_level(int v)
{
	log_level->setIntValue(v);
}


int Option::get_log_type(void)
{
	return log_type->getIntValue();
}

void Option::set_log_type(int v)
{
	log_type->setIntValue(v);
}




char const *Option::get_log_file(void)
{
	return log_file->getStringValue();
}

void Option::set_log_file(char const *v)
{
	log_file->setStringValue(v);
}

