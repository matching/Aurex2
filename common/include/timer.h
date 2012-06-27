// -*-c++-*-
// $Id: timer.h,v 1.13 2005/01/08 09:40:30 fumi Exp $

#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>

#include <list>
using namespace std;

class Timer;
class MemoryForInside;
class Ruputer;
class Threadable;
class TimerResource;

class Timer
{
  public:
	Ruputer *ruputer;
	list<Threadable *> *threadList;

	short count;
	unsigned long func_addr;
	unsigned long stack_pointer;
	unsigned long kernelsp;

	TimerResource *tr;

	MemoryForInside *timeraddr;

	int executingFlag;

  public:
	Timer(Ruputer *ruputer, unsigned long _func_addr, int _count, TimerResource *tr);
	~Timer(void);

	void decrement(void);
	TimerResource *getTimerResource(void);

  private:
	int createThread(void);
};



#endif
