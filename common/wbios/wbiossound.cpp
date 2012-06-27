/* -*-coding:euc-jp-unix-*-
 *
 * wbiossound.cpp - Ruputer wbios emulation (for sound)
 *
 * $Id: wbiossound.cpp,v 1.5 2006/01/28 11:22:32 fumi Exp $
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

#include "mncore.h"
#include "wbios.h"
#include "ruputer.h"
#include "thread.h"
#include "sound.h"
#include "soundsync.h"
#include "soundsyncnull.h"
#include "soundasync.h"

#include "option.h"


void BiosIntr::bi_buzzinit(void)
{
	volume=regs->d[1];
	tempo=regs->d[2];
}



void BiosIntr::bi_melody(void)
{
	SoundAsync *sa;

	sa = new SoundAsync(ruputer, tempo, volume, regs->a[0]);
	threadList->push_front(sa);
}

/** bi_buzzer のエミュレート
 *
 *
 */
void BiosIntr::bi_buzzer(void)
{
	SoundSyncNull *ss;

	ss=new SoundSync(ruputer, tempo, volume);

	ss->init(regs->d[1], regs->d[2] & 0xf, (regs->d[2] >> 4) & 0xf);
	threadList->push_front(ss);
	thread->stop();
	regs->d[1]=0;
}
