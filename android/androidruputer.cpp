/*
 * androidruputer.cpp - ruputer emulation for android
 *
 * $Id$
 *
 * Copyright (C) 2011, 2012 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include "androidruputer.h"
#include "androidlcd.h"
#include "androidlogoutput.h"

#include "option.h"
#include "log.h"

#include "jp_ruputer_AndroAurex_JniIF.h"

AndroidRuputer::AndroidRuputer(Frame *frame, Directory *sddir, Option *opt, UFilerIni *uini, JNIEnv *env, jobject jniif, jobject bitmap) : Ruputer(frame, sddir, opt, uini)
{
	andlcd = new AndroidLcd( getCMLcd(), env, jniif, bitmap );
	setSDLcd( andlcd );

	setupLog();
}

AndroidRuputer::~AndroidRuputer(void)
{
	delete andlcd;
}


int AndroidRuputer::EventProcedure(void)
{
#if 0
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
#endif

    return 0;
}


SDFindFile *AndroidRuputer::createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute)
{
	return NULL;
//	return new UnixFindFile(dta_area, pathname, search_attribute);
}

void AndroidRuputer::setupLog(void)
{
	Log::getInstance()->setLogOutput( new AndroidLogOutput() );

	Ruputer::setupLog();
}
