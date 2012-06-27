/*
 * frame.cpp - 1/64 frame management class
 *
 * $Id: frame.cpp,v 1.27 2006/02/16 18:19:40 fumi Exp $
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

#include "frame.h"
#include "log.h"
#include "option.h"

#include <stdio.h>
#include <stdlib.h>



//#define LIGHT_FRAME



/*
 * 64 Hz のための ms 数
 *
 * 40 つ 16。24 つ 15 。平均するには
 * 10 6
 * --+-+--+
 * --+-+--+
 *
 * って感じにする。
 */
int Frame::defaultFrameTime[64]={
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
	16, 16, 15, 16, 15, 16, 16, 15,
};

Frame::Frame(void)
{
	no=0;
	updateMilliSec=1000;
	lasttick=0;
	delay=0;
//	lasttick=getMilliSec();

	frame=0;
	skipframe=0;

	start_end_diff = 0;

	renzoku_skipframe = 0;
}

Frame::~Frame(void)
{
}

void Frame::updateFrameTime(void)
{
	int i;

	for(i = 0; i < 64; i++)
		frameTime[i] = defaultFrameTime[i] - option->get_timer_speed();
}

void Frame::startFrame(void)
{
	updateFrameTime();

	sync();
}

void Frame::endFrame(void)
{
//	long diff=getMilliSec()-firsttime;

	double fps = 0.0;
	double fps_graphic = 0.0;
	if( start_end_diff != 0 )
	{
		fps = (double)frame/start_end_diff*1000;
		fps_graphic = (double)(frame-skipframe)/start_end_diff*1000;
	}

/*
	fprintf(stderr, "\n%ld ms, %d frame, %d skip, %f fps, graphic %f fps\n",
			start_end_diff,
			frame,
			skipframe,
			fps,
			fps_graphic);
*/

	Log::getInstance()->output(Log::Info,
							   "\n%ld ms, %d frame, % f fps, graphic %f fps (%d frame skip)\n",
							   start_end_diff,
							   frame,
							   fps,
							   fps_graphic,
							   skipframe);
}

/**
 * 1フレーム、時間チェック
 * @retval 1 前フレームより、1/64 sec 経った（もしくはそう判断した）ので、実行許可
 * @retval 0 1/64 経っていない。（ただし、Sleep 入れているので、次実行すると1になる考算が高い）
 */
int Frame::checkTime(void)
{

#ifdef LIGHT_FRAME
	unsigned long now;
	int d;

	now=getMilliSec();
	d=now-lasttick;
	if(d > frameTime[no])
	{
		frame++;
		no++;
		if(no >= 64)
			no=0;
		lasttick=now;
		return 1;
	}

	d=frameTime[no]-d;
	waitMilliSec(d);
	return 0;
#else

	unsigned long now;
	long diff;
	long rest;

#define SLEEP_THRESHOLD 10

	now=getMilliSec();
	diff=delay+now-lasttick;
	rest=abs(diff-frameTime[no]);

	if(diff >= frameTime[no] || rest <= SLEEP_THRESHOLD)
	{
// 実行許す
		delay=diff-frameTime[no];
		frame++;

		no++;
		if(no >= 64)
			no=0;

		start_end_diff += now - lasttick;

		lasttick=now;
		return 1;
	}

	waitMilliSec(rest);
	return 0;
#endif //LIGHT_FRAME

}

int Frame::isFrameUpdateSkip(void)
{
#ifdef LIGHT_FRAME
	return 0;
#else

	beforeupdate=getMilliSec();
//	if(delay > updateMilliSec+15) // windows だと 0 になるので、15 足しとく？
	if(delay > updateMilliSec + frameTime[no])
	{
		renzoku_skipframe++;
		if( renzoku_skipframe > 4 )  //max_renzoku_skipframe
		{
			renzoku_skipframe = 0;
			Log::getInstance()->output(Log::Warning, "over skipmaxframe. force to update frame. frame update time = %d\n", updateMilliSec);
			return 0;
		}
		else
		{
			skipframe++;
			return 1; // skip
		}
	}

	renzoku_skipframe = 0;
	return 0; // exec
#endif

}

void Frame::endFrameUpdate(void)
{
	updateMilliSec=getMilliSec()-beforeupdate;
}

void Frame::wait(void)
{
	waitMilliSec(20);
}


void Frame::sync(void)
{
	lasttick=getMilliSec();
	delay=0;
}

int Frame::getGraphicFrameRateTimes1000(void)
{
	int ret = 0;
	if( start_end_diff != 0 )
		ret = (int)((double)( frame - skipframe ) / start_end_diff * 1000 * 1000);

	return ret;
}

int Frame::getFrameRateTimes1000(void)
{
	int ret = 0;
	if( start_end_diff != 0 )
		ret = (int)((double)( frame ) / start_end_diff * 1000 * 1000);

	return ret;
}
