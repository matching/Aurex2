/*
 * soundsyncnull.cpp - Ruputer synchronous sound emulation (null sound)
 *
 * $Id: soundsyncnull.cpp,v 1.6 2006/02/18 12:35:05 fumi Exp $
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

#include "state.h"
#include "soundsyncnull.h"

/** 同期サウンド（時間調節）のコンストラクタ
 * @param _tempo テンポ
 * @param _volume 音量
 */
SoundSyncNull::SoundSyncNull(Ruputer *ruputer, int _tempo, int _volume) : Sound(ruputer, _tempo, _volume)
{
	state = new ThreadState();
	state->setState( ThreadState::RUNNING);

	notelen=0;
	env=0;
}

SoundSyncNull::~SoundSyncNull(void)
{
	delete state;
}

/** 初期化する
 * @param note 音程（使用せず）
 * @param len 長さ
 * @param _env エンベロープ
 */
void SoundSyncNull::init(int note, int len, int _env)
{
	if(len == 0)
		len=16;
	notelen=tempo*len;
	env=_env;
}


/** 1/64 だけ実行する〜
 * @param *cycle サイクルを保持する変数へのポインタ
 * @retval true 演奏は次も続く
 * @retval false 演奏終了
 */
bool SoundSyncNull::execute(int *cycle)
{
	if(notelen <= 0)
		return false;

	notelen--;
	*cycle=-1;
	return true;
}


ThreadState *SoundSyncNull::getState(void)
{
	return state;
}
