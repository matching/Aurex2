// -*-c++-*-
// $Id: winsound.h,v 1.5 2006/02/16 13:05:13 fumi Exp $
#ifndef WINSOUND_H
#define WINSOUND_H

#include "sdsound.h"
#include "sound.h"

#include <windows.h>
#include <mmsystem.h>

#define NBUFFER 32

class WINSound : public SDSound
{
	unsigned char buffer[NBUFFER][Sound::UNIT_LENGTH * 2];
	int bufferno;

	WAVEHDR w_header[NBUFFER];

	HWAVEOUT hWaveout;

	bool wasOpened;

  public:
	WINSound(void);
	~WINSound(void);

  private:
	int subOutput(unsigned char const *buf, int len);
	int subOpen(void);
	int subClose(void);

};

#endif
