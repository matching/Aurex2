// -*-c++-*-
// $Id: timerresourcemgr.h,v 1.1 2005/01/08 09:40:31 fumi Exp $

#ifndef TIMERRESOURCEMGR_H
#define TIMERRESOURCEMGR_H

class TimerResource;

// スタックポインタと、
// カーネルスタックを管理する

class TimerResourceMgr
{
  private:
	int used[16];

  public:
	TimerResourceMgr(void);
	~TimerResourceMgr(void);

	TimerResource *allocTimerResource(void);
	void freeTimerResource(TimerResource *res);

  private:
	unsigned long indexToBaseAddress(int index);
	int baseAddressToIndex(unsigned long addr);
	int getFreeIndex(void);

};


#endif
