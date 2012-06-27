/*
 * main.cpp - android jni routine
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


#include <android/log.h>

#include "keybuffer.h"
#include "androidruputer.h"
#include "unixframe.h"
#include "unixdirectory.h"

#include "option.h"
#include "ufilerini.h"
#include "state.h"
#include "log.h"
#include "state.h"

#include "jp_ruputer_AndroAurex_JniIF.h"

AndroidRuputer *rup;

JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_aurexjnimain (JNIEnv *env, jobject thiz, jobject bitmap, jint expansion)
{
	Option *opt;
	Frame *frame;
	UnixDirectory *dir;

	__android_log_write(ANDROID_LOG_INFO, "androaurex", "JNI CALLED !!! ");

	dir = new UnixDirectory( "/sdcard" );
	opt = new Option();

//	opt->set_log_level(Log::Debug);
	opt->set_log_type(Log::TYPE_CURSES);
	opt->set_expansion((int)expansion);

	frame = new UnixFrame();

	rup = new AndroidRuputer(frame,
							 dir,
							 opt,
							 NULL, // not use filer ini
							 env,
							 thiz,
							 bitmap);

	try{
		rup->load("/sdcard/rforce.exf");
	}
	catch( ExfFileInvalidException &e ) {
		__android_log_write(ANDROID_LOG_INFO, "androaurex", "exf file is invalid");
	}
	catch( ExfFileNotFoundException &e ) {
		__android_log_write(ANDROID_LOG_INFO, "androaurex", "A exf file associated with  is not found");
	}
	catch( AssociatedFileNotInVirtualDirectoryException &e ) {
		__android_log_write(ANDROID_LOG_INFO, "androaurex", "file is out of virtual directory.");
	}

//	rup->getState()->setState( RuputerState::STOP );

	__android_log_write(ANDROID_LOG_INFO, "androaurex", "call main loop");

	rup->main_loop();

	__android_log_write(ANDROID_LOG_INFO, "androaurex", "exit main loop");

}



JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_aurexjniresume(JNIEnv *env, jobject thiz)
{
	if( rup )
		rup->getState()->setState( RuputerState::RUNNING );

	__android_log_write(ANDROID_LOG_INFO, "androaurex", "resume called");
}


JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_aurexjnipause(JNIEnv *env, jobject thiz)
{
	if( rup )
		rup->getState()->setState( RuputerState::STOP );

	__android_log_write(ANDROID_LOG_INFO, "androaurex", "pause called");

}


JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_pushButtonDown(JNIEnv *env, jobject thiz, jint key)
{
	if( rup )
		rup->getKeyBuffer()->enQueueKey( (int)key | KEY_PUSH_DOWN);

}

JNIEXPORT void JNICALL Java_jp_ruputer_AndroAurex_JniIF_pushButtonUp(JNIEnv *env, jobject thiz, jint key)
{
	if( rup )
		rup->getKeyBuffer()->enQueueKey( (int)key | KEY_PUSH_UP );
}




