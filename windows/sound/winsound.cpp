/*
 * winsound.cpp - Windows sound output code
 *
 * $Id: winsound.cpp,v 1.4 2006/02/16 13:05:13 fumi Exp $
 *
 * Copyright (C) 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <stdlib.h>
#include <math.h>
#define WINVER 0x0500
#include <windows.h>
#include <mmsystem.h>

#include "winsound.h"


WINSound::WINSound(void)
{
	memset(buffer, 0, sizeof(buffer));
	bufferno=0;

	wasOpened = false;
}

WINSound::~WINSound(void)
{
	if( wasOpened )
		subClose();
}

int WINSound::subOutput(unsigned char const *buf, int len)
{
	WAVEHDR *wav;

	wav=w_header+bufferno++;
	if(bufferno >= NBUFFER)
		bufferno=0;

//	memcpy(wav->lpData, buf, 354*2);
//	wav->dwBufferLength=len;
	memcpy(wav->lpData, buf, len);
	wav->dwBufferLength=len;

	MMRESULT res;

	res=waveOutWrite(hWaveout, wav, sizeof(WAVEHDR));
	if(res != MMSYSERR_NOERROR)
		;

	wasOpened = true;

	return 0;
}


int WINSound::subClose(void)
{
	int i;

	for(i=0; i < NBUFFER; i++)
		waveOutUnprepareHeader(hWaveout, w_header+i, sizeof(WAVEHDR));

	waveOutClose(hWaveout);
	hWaveout=NULL;

	return 0;
}

/** Windows のサウンドをオープンする
 * @param freq 出力周波数
 * @param bit 量子化ビット
 * @param channel 1:モノラル 2:ステレオ
 * @retval -1 失敗
 * @retval 0 成功
 */

int WINSound::subOpen(void)
{
	unsigned long freq = 22050;
	int bit = 16;
	int channel = 1;

	int i;
	MMRESULT res;
	WAVEFORMATEX format={0};

	format.wFormatTag=WAVE_FORMAT_PCM;
	format.nChannels=channel;
	format.nSamplesPerSec=freq;
	format.nAvgBytesPerSec=freq*(bit>>3)*channel;	/* channel*freq*bit/8 */
	format.nBlockAlign=(bit>>3)*channel;
	format.wBitsPerSample=bit;

//	res=waveOutOpen(&hWaveout, WAVE_MAPPER, &format, (DWORD)waveOutProc, 0, WAVE_ALLOWSYNC | CALLBACK_FUNCTION);
	res=waveOutOpen(&hWaveout, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL);
	if(res != MMSYSERR_NOERROR)
	{
		hWaveout=NULL;
		return -1;
	}

	for(i=0; i < NBUFFER; i++)
	{
		memset(w_header+i, 0, sizeof(WAVEHDR));
		w_header[i].lpData=(char *)buffer[i];
		w_header[i].dwBufferLength = Sound::UNIT_LENGTH * 2;
		waveOutPrepareHeader(hWaveout, w_header+i, sizeof(WAVEHDR));
	}

	return 0;
}

