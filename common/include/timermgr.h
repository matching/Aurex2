// -*-c++-*-
// $Id: timermgr.h,v 1.2 2005/01/08 09:40:30 fumi Exp $

#ifndef TIMERMGR
#define TIMERMGR

#include "timerresourcemgr.h"

class Ruputer;
class Thread;
class Timer;
class MemoryForInside;

class TimerResource;

class TimerMgr
{
  private:
	Ruputer *ruputer;

	int n1hz;
	int n4hz;
	int n64hz;

	Timer **t1Hz;
	Timer **t4Hz;
	Timer **t64Hz;

	MemoryForInside *mem;

	static TimerResourceMgr timerResourceMgr;

  public:
	TimerMgr(Ruputer *ruputer, int n1hz, int n4hz, int n64hz);
	virtual ~TimerMgr(void);

	void closeAll(void);

	void decrement1HzTimer(void);
	void decrement4HzTimer(void);
	void decrement64HzTimer(void);

	unsigned long create1HzTimer(unsigned long addr, unsigned short count);
	unsigned long create4HzTimer(unsigned long addr, unsigned short count);
	unsigned long create64HzTimer(unsigned long addr, unsigned short count);

	void deleteTimer(unsigned long addr);

  private:
	Timer *createTimer(unsigned long addr, unsigned short count, TimerResource *tr);
	unsigned long createTimerCommon(int n, Timer **timer, unsigned long addr, unsigned short count);
	void decrementTimer(int n, Timer **timer);
	bool subDeleteTimer(int n, Timer **timer, unsigned long addr);
	void subCloseAll(int n, Timer **timer);
};


#endif
