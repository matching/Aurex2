// -*-c++-*-
// $Id: timerresource.h,v 1.1 2005/01/08 09:40:30 fumi Exp $

#ifndef TIMERRESOURCE_H
#define TIMERRESOURCE_H

class TimerResource
{
	unsigned long baseAddr;

  public:
	TimerResource(unsigned long baseAddr);
	~TimerResource(void);

	unsigned long getBaseAddress(void);
	unsigned long getTimerAddress(void);
	unsigned long getStackPointer(void);
	unsigned long getKernelStackPointer(void);
};


#endif
