/*
 * androidlcd.cpp - android graphic
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


#include "androidlcd.h"
#include "cmlcd.h"
#include "cmlcdfunc.h"

#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <stdio.h>

AndroidLcd::AndroidLcd(CMLcd *cmlcd, JNIEnv *env, jobject jniif, jobject bitmap) : SDLcd(cmlcd)
{
	this->env = env;
	this->jniif = jniif;
	this->bitmap = bitmap;
}

AndroidLcd::~AndroidLcd()
{
}


void AndroidLcd::updateWindow(void)
{
	AndroidBitmapInfo info;
	void *pixels;
	int ret;



	ret = AndroidBitmap_getInfo( env, bitmap, &info );
	if( ret < 0 ) {
		__android_log_write(ANDROID_LOG_INFO, "androaurex", "AndroidBitmap_getInfo error");
		return;
	}

	ret = AndroidBitmap_lockPixels( env, bitmap, &pixels );
	if( ret < 0 ) {
		__android_log_write(ANDROID_LOG_INFO, "androaurex", "AndroidBitmap_lockPixels error");
		return;
	}


	int x, y;
	int height;
	int width;
	unsigned short *q;
	unsigned char const *p;

//	__android_log_print(ANDROID_LOG_INFO, "androaurex", "AndroidBitmapInfo w:%d, h:%d", info.width, info.height);


//	height = 64;
//	width = 102;
	height = cmlcd->getRealHeight64();
	width = cmlcd->getRealWidth102();

//		__android_log_write(ANDROID_LOG_INFO, "androaurex", "AndroidBitmap_getInfo error");
//	__android_log_print(ANDROID_LOG_INFO, "androaurex", "cmlcd  w:%d, h:%d", width, height );


	for(y=0; y < height; y++)
	{
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);
		q = (unsigned short *)((char *)pixels + info.stride * y);

		for(x=0; x < width; x++)
		{
			q[ x ] = (((unsigned short)p[ x ] << 8) & 0xf800)		\
				| (((unsigned short)p[ x ] << 3) & 0x07e0)			\
				| (((unsigned short)p[ x ] >> 3) & 0x001f);

		}
	}

    AndroidBitmap_unlockPixels( env, bitmap );


// call java method for ...
	jclass jniifclass;
	jmethodID mid;

	jniifclass = env->GetObjectClass( jniif );
	mid = env->GetMethodID( jniifclass, "drawBitmapToCanvas", "()V" );
	if( mid == 0 )
	{
		__android_log_write(ANDROID_LOG_INFO, "androaurex", "getmethodid error");
		return;
	}

	env->CallVoidMethod( jniif, mid );

	env->DeleteLocalRef( jniifclass );


#if 0
	int x, y;
	unsigned char str[400]="";

	int height;
	int width;
	unsigned char const *p;

#if 1
	width = COLS;
	if(width > 102)
		width = 102;

	height = LINES;
	if(height >= 65)
		height = 64;
	else
		height = LINES - 1;
#else
	width=COLS;
	if(width > 320)
		width=320;

	height=LINES;
	if(height > 240)
		height=240;
#endif

	for(y=0; y < height; y++)
	{
		p = cmlcd->getCMLcdFunc()->getPixelArray(0, y);

		for(x=0; x < width; x++)
		{
			str[x] = pixelChars[ p[x] ];
		}
		mvaddstr(y, 0, (char *)str);
	}

	refresh();
#endif

}

SDLcdFont *AndroidLcd::createSDLcdFont(int width, int height, int prop, int gap, bool latin)
{
#if 0
	BdfFontSets *bfss;
	if(latin)
	{
/*
		bfss=bdfmgr->getBdfFontSetsIso(height);
		return new BdfLcdFont(width, height, gap, bfss);
*/
	}
	else
	{
		bfss=bdfmgr->getJisBdfFont(height);
		if(bfss == NULL)
			return NULL;

		return new BdfLcdFontJis(width, height, gap, bfss);
	}
#endif
	return NULL;
}
