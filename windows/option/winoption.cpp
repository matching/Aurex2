/*
 * winoption.cpp - option for windows
 *
 * $Id: winoption.cpp,v 1.9 2006/01/24 18:24:24 fumi Exp $
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
#include <assert.h>

#include "option.h"
#include "optionitem.h"
#include "winoption.h"

/** コンストラクタ
 */
WINOption::WINOption(void)
{
// 各オプションのデフォルト値の設定
	addOptionItem(no_caption  = new OptionBool(OPT_NO_CAPTION, false)      );
	addOptionItem(alpha       = new OptionBool(OPT_ALPHA, false)           );
	addOptionItem(alpha_value = new OptionInt(OPT_ALPHA_VALUE, false)      );
	addOptionItem(transparent = new OptionBool(OPT_TRANSPARENT, false)     );
	addOptionItem(always_on_top = new OptionBool(OPT_ALWAYS_ON_TOP, false) );
	addOptionItem(window_x_pos  = new OptionInt(OPT_X_POS, 0)              );
	addOptionItem(window_y_pos  = new OptionInt(OPT_Y_POS, 0)              );

	addOptionItem(status_bar = new OptionBool(OPT_STATUS_BAR, false)       );

	addOptionItem(only_a_process = new OptionBool(OPT_ONLY_A_PROCESS, false)       );

}

/** デストラクタ
 */
WINOption::~WINOption(void)
{
	delete no_caption;
	delete alpha;
	delete alpha_value;
	delete transparent;
	delete always_on_top;
	delete window_x_pos;
	delete window_y_pos;
}





bool WINOption::get_no_caption(void)
{
	return no_caption->getBoolValue();
}
void WINOption::set_no_caption(bool v)
{
	no_caption->setBoolValue(v);
}

bool WINOption::get_alpha(void)
{
	return alpha->getBoolValue();
}
void WINOption::set_alpha(bool v)
{
	alpha->setBoolValue(v);
}

int WINOption::get_alpha_value(void)
{
	return alpha_value->getIntValue();
}
void WINOption::set_alpha_value(int v)
{
	alpha_value->setIntValue(v);
}



bool WINOption::get_transparent(void)
{
	return transparent->getBoolValue();
}
void WINOption::set_transparent(bool v)
{
	return transparent->setBoolValue(v);
}



bool WINOption::get_always_on_top(void)
{
	return always_on_top->getBoolValue();
}
void WINOption::set_always_on_top(bool v)
{
	always_on_top->setBoolValue(v);
}

int WINOption::get_window_x_pos(void)
{
	return window_x_pos->getIntValue();
}
void WINOption::set_window_x_pos(int v)
{
	window_x_pos->setIntValue(v);
}

int WINOption::get_window_y_pos(void)
{
	return window_y_pos->getIntValue();
}

void WINOption::set_window_y_pos(int v)
{
	window_y_pos->setIntValue(v);
}


bool WINOption::get_status_bar(void)
{
	return status_bar->getBoolValue();
}

void WINOption::set_status_bar(bool v)
{
	status_bar->setBoolValue(v);
}



bool WINOption::get_only_a_process(void)
{
	return only_a_process->getBoolValue();
}

void WINOption::set_only_a_process(bool v)
{
	only_a_process->setBoolValue(v);
}

