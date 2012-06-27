// -*-c++-*-
// $Id: soundsync.h,v 1.5 2006/01/28 11:22:31 fumi Exp $

#ifndef SOUNDSYNC_H
#define SOUNDSYNC_H

#include "soundsyncnull.h"
#include "threadable.h"

class Ruputer;
class SoundSync : public SoundSyncNull
{
  protected:
	unsigned char note[345*2];

	int freq;
	double omega;

	static int frequencytable[];

  public:
	SoundSync(Ruputer *ruputer, int tempo, int volume);
	void init(int note, int len, int _env);

	virtual bool execute(int *cycle);

	void make1par64wave(unsigned char *buf, int freq);

};

#endif
