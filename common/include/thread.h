// -*-c++-*-
// $Id: thread.h,v 1.20 2008/07/26 06:43:08 fumi Exp $

#ifndef THREAD_H
#define THREAD_H

#include "threadable.h"

class Cpu;
class ThreadState;
class Ruputer;
class Timer;
class RuputerDirectory;
class KeyBuffer;
class RuputerState;

class Thread : public Threadable
{
  public:
  private:
	Cpu *cpu;
	KeyBuffer *keybuffer;

	unsigned long initial_pc;
	unsigned long initial_a3;
	unsigned long initial_kernelsp;

	Timer *trigger;

	bool mainThread;

  protected:
	ThreadState *state;

  public:
	Thread(unsigned long pc, unsigned long a3, Ruputer *ruputer, unsigned long kernelsp, Timer *trigger=0, ThreadState *state = 0);
	virtual ~Thread(void);

	int isStartFunctionOver(void);
	bool execute(int *cycle);

	ThreadState *getState(void);

	void stop();

	virtual bool isMainThread(void);

	unsigned long getStackPointer(void);
	unsigned long getExitCode(void);
};

class MainThread : public Thread
{
  public:
	MainThread(unsigned long pc, unsigned long a3, Ruputer *ruputer, unsigned long kernelsp, ThreadState *state);
	~MainThread(void);

	bool isMainThread(void);
};

class ChildThread : public Thread
{
  public:
	ChildThread(unsigned long pc, unsigned long a3, Ruputer *ruputer, unsigned long kernelsp);
	~ChildThread(void);

	bool isMainThread(void);
};



#endif
