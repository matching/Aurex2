// -*-c++-*-
// $Id: soundasync.h,v 1.5 2006/01/28 11:22:31 fumi Exp $

#ifndef SOUNDASYNC_H
#define SOUNDASYNC_H

#include "soundsync.h"
class Ruputer;

class SoundAsync : public SoundSync
{
	unsigned char *melody;

  public:
	SoundAsync(Ruputer *ruputer, int tempo, int volume, unsigned long addr);
	bool execute(int *cycle);
};

#endif
