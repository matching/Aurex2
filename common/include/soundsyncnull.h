// -*-c++-*-
// $Id: soundsyncnull.h,v 1.5 2006/02/17 19:01:38 fumi Exp $

#ifndef SOUNDSYNCNULL_H
#define SOUNDSYNCNULL_H

#include "sound.h"
#include "threadable.h"

class ThreadState;
class Ruputer;

class SoundSyncNull : public Sound, public Threadable
{
  protected:
	int notelen;
	int env;
	ThreadState *state;

  public:
	SoundSyncNull(Ruputer *ruputer, int tempo, int volume);
	~SoundSyncNull(void);
	virtual void init(int note, int len, int _env);
	virtual bool execute(int *cycle);
	ThreadState *getState(void);
};

#endif
