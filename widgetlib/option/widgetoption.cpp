/*
 * winoption.cpp - option for windows
 *
 * $Id: widgetoption.cpp,v 1.5 2006/03/06 12:58:17 fumi Exp $
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
#include <assert.h>

#include "widgetconfig.h"
#include "option.h"
#include "optionitem.h"

#include "widgetoption.h"

/** コンストラクタ
 */
WidgetOption::WidgetOption(void)
{
// 各オプションのデフォルト値の設定
	addOptionItem(full_window = new OptionBool(OPT_FULL_WINDOW, OPTION_DEFAULT_FULL_WINDOW)      );
	addOptionItem(flip        = new OptionBool(OPT_FLIP, OPTION_DEFAULT_FLIP)             );
}

/** デストラクタ
 */
WidgetOption::~WidgetOption(void)
{
	delete full_window;
	delete flip;
}

bool WidgetOption::get_full_window(void)
{
	return full_window->getBoolValue();
}

void WidgetOption::set_full_window(bool v)
{
	full_window->setBoolValue(v);
}

bool WidgetOption::get_flip(void)
{
	return flip->getBoolValue();
}

void WidgetOption::set_flip(bool v)
{
	flip->setBoolValue(v);
}

