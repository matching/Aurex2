/*
 * soundasync.cpp - Ruputer asynchronous sound emulation
 *
 * $Id: soundasync.cpp,v 1.5 2006/01/28 11:22:32 fumi Exp $
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

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "soundsync.h"
#include "soundasync.h"
#include "sdsound.h"
#include "ruputer.h"
#include "option.h"


/** 非同期サウンドのコンストラクタ
 * @param _tempo テンポ
 * @param _volume 音量
 */
SoundAsync::SoundAsync(Ruputer *ruputer, int _tempo, int _volume, unsigned long addr) : SoundSync(ruputer, _tempo, _volume)
{
	melody=(unsigned char *)Memory::getMemoryPointer(addr);

	execute(NULL);
	execute(NULL);
}

/** 1/64 だけ実行する〜
 * @param *cycle サイクルを保持する変数へのポインタ（触らない）
 * @retval true 演奏は次も続く
 * @retval false 演奏終了
 */
bool SoundAsync::execute(int *cycle)
{
#ifdef DEBUG
	fprintf(stderr, "!! notelen %d\n", notelen);
	fflush(stderr);
#endif

	if(notelen <= 0)
	{
		if(*melody == 0xff && *(melody+1) == 0xff)
		{
			return false;
		}

		int len;

		len=*(melody+1) & 0xf;
		if(len == 0)
			len=16;
		env=*(melody+1) >> 4;
		notelen=tempo*len;
		freq=frequencytable[*melody];
		melody+=2;
	}
	notelen--;

/* サウンドの判定 */
	SDSound *sdsound;

	sdsound = ruputer->getSDSound();
	if( ::option->get_enable_sound() == false || sdsound == NULL )
	{
		return true;
	}


	switch(env)
	{
	case 0:
		memset(note, 0, Sound::UNIT_LENGTH * 2 );
		break;
	default:
		make1par64wave(note, freq);
		break;
	}
	sdsound->output(note, Sound::UNIT_LENGTH * 2 );

	return true;
}

