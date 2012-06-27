/*
 * curruputer.cpp - curses library ruputer emulation
 *
 * $Id: curruputer.cpp,v 1.13 2006/02/19 13:04:40 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

//#include "optionmgr.h"
#include "keybuffer.h"

#include "state.h"
#include <stdio.h>
#include <stdlib.h>

#include "curruputer.h"
#include "curlcd.h"
#include "curlogoutput.h"
#include "option.h"
#include "log.h"

#include "unixfindfile.h"

extern Option *option;

CURRuputer::CURRuputer(Frame *frame, Directory *sddir, Option *opt, UFilerIni *uini) : Ruputer(frame, sddir, opt, uini)
{
	curlcd=new CURLcd( getCMLcd() );
	setSDLcd(curlcd);

	setupLog();
}

CURRuputer::~CURRuputer(void)
{
	delete curlcd;
}



int CURRuputer::EventProcedure(void)
{
	static int prev_ch;
	int ch;

/* イベントを取り出す */
	ch=getch();
	if(prev_ch == ch)
		return 0;
	else
		prev_ch=ch;

	if(ch == 'q')
	{
		requestDestroy();
		return 0;
	}
	switch(ch)
	{
	// ゲームの入力
	case KEY_LEFT:
		keybuf->enQueueKey(Bkey_lf | KEY_PUSH_DOWN);
		break;
	case KEY_DOWN:
		keybuf->enQueueKey(Bkey_dw | KEY_PUSH_DOWN);
		break;
	case KEY_UP:
		keybuf->enQueueKey(Bkey_up | KEY_PUSH_DOWN);
		break;
	case KEY_RIGHT:
		keybuf->enQueueKey(Bkey_rg | KEY_PUSH_DOWN);
		break;
	case 'z':
		keybuf->enQueueKey(Bkey_A | KEY_PUSH_DOWN);
		break;
	case 'w':
		keybuf->enQueueKey(Bkey_B | KEY_PUSH_DOWN);
		break;
	case 's':
		keybuf->enQueueKey(Bkey_C | KEY_PUSH_DOWN);
		break;
	case 'x':
		keybuf->enQueueKey(Bkey_D | KEY_PUSH_DOWN);
		break;
#if 0
	default: //空キーで「離れた」と認識させる？
		keybuf->enQueueKey(KEY_PUSH_DOWN);
		break;
#endif
	}

    return 0;
}

SDFindFile *CURRuputer::createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute)
{
	return new UnixFindFile(dta_area, pathname, search_attribute);
}

void CURRuputer::setupLog(void)
{
	if( ::option->get_log_type() == Log::TYPE_CURSES )
	{
		Log::getInstance()->setLogOutput( new CURLogOutput( curlcd->getBottomLine() ) );
	}

	Ruputer::setupLog();
}

