/*
 * soundsync.cpp - Ruputer synchronous sound emulation
 *
 * $Id: soundsync.cpp,v 1.5 2006/01/28 11:22:32 fumi Exp $
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
#include "sdsound.h"
#include "ruputer.h"
#include "option.h"

#include <math.h>

/* 実際の freq の 100 倍の値としている */
int SoundSync::frequencytable[]={
	5827,
	6174,
	6541,
	6930,
	7342,
	7778,
	8241,

	8731,
	9250,
	9800,
	10383,
	11000,
	11654,
	12347,
	13081,
	13859,
	14683,

	15556,
	16481,
	17461,
	18500,
	19600,
	20765,
	22000,
	23308,
	24694,
	26163,

	27718,
	29366,
	31113,
	32963,
	34923,
	36999,
	39200,
	41530,
	44000,
	46616,

	49388,
	52325,
	55437,
	58733,
	62225,
	65926,
	69846,
	73999,
	78399,
	83061,

	88000,
	93233,
	98777,
	104650,
	110873,
	117466,
	124451,
	131851,
	139691,
	147998,

	156798,
	166122,
	176000,
	186466,
	197553,
	209300,
	221746,
	234932,
	248902,
	263702,

	279383,
	295996,
	313596,
	332244,
	352000,
	372931,
	395107,
	418601,
	443492,
	469864,

	497803,
	527404,
	558765,
	591991,
	627193,
	664488,
	704000,
	745862,
	790213,
	837202,
};


/** 同期サウンドのコンストラクタ
 * @param _tempo テンポ
 * @param _volume 音量
 */
SoundSync::SoundSync(Ruputer *ruputer, int _tempo, int _volume) : SoundSyncNull(ruputer, _tempo, _volume)
{
	omega=0.0;
	freq=0;
}

void SoundSync::init(int note, int len, int _env)
{
	SoundSyncNull::init(note, len, _env);

	freq=frequencytable[note];
}


/** 1/64 だけ実行する〜
 * @param *cycle サイクルを保持する変数へのポインタ
 * @retval true 演奏は次も続く
 * @retval false 演奏終了
 */
bool SoundSync::execute(int *cycle)
{
#ifdef DEBUG
	fprintf(stderr, "!! notelen %d %d %d\n", notelen, freq, env);
	fflush(stderr);
#endif

	if(notelen <= 0)
		return false;

	notelen--;
	*cycle=-1;

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


void SoundSync::make1par64wave(unsigned char *buf, int freq)
{
	short *p;
	double dw;
	double amp;

/* 振幅 volumeは2〜9 なので、9の時は20000にするようにする。
 * 本当は log にしないといけないけど、面倒なので却下。
 * Ruputer もどうなっているかは知らんし。
 */
	amp=20000*volume/9;
	p=(short *)buf;

#define PI 3.1415926535897932384626433832795

/* 角周波数 */
	dw=2*PI/ Sound::FREQUENCY * freq/100;

	int len = Sound::UNIT_LENGTH;

	while(len--)
	{
		*p=(short)(sin(omega)*amp);
		p++;
		omega+=dw;

		if(omega > PI)
			omega-=2*PI;
	}
}



