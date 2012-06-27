// -*-c++-*-
// $Id: threadable.h,v 1.4 2006/02/17 19:01:38 fumi Exp $

#ifndef THREADABLE
#define THREADABLE

class ThreadState;

class Threadable
{
  public:
	Threadable(void)
	{
	}
	virtual ~Threadable(void)
	{
	}
	virtual bool execute(int *cycle)=0;
	virtual ThreadState *getState(void) = 0;
};


#endif
