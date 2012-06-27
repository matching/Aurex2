/* -*-c++-*- */
// $Id: wbios.h,v 1.15 2006/02/17 19:01:38 fumi Exp $

#ifndef WBIOS_H
#define WBIOS_H

#include <list>
using namespace std;

class Register;
class Memory;
class Thread;
class Threadable;
class KeyBuffer;
class Ruputer;
class SDSound;
class TimerMgr;
class WatchDT;
class ThreadState;

class BiosIntr
{
  private:

// System Depend ...
//	SDBiosIntr *sidos;
	Ruputer *ruputer;
	Thread *thread;
	KeyBuffer *keybuf;
	TimerMgr *timermgr;
	TimerMgr *systimermgr;
	WatchDT *watchDT;
	ThreadState *threadState;

	list<Threadable *> *threadList;

	int volume;
	int tempo;

	Register *regs;

  public:
	BiosIntr(Ruputer *ruputer, Thread *thread);
	~BiosIntr(void);

	void callBiosIntr(Register *regs);

  private:
	void bi_tmset(void);
	void bi_tmdel(void);

	void bi_tmread(void);
	void bi_dtread(void);

	void bi_buzzinit(void);
	void bi_melody(void);
	void bi_buzzer(void);


	void sys_bi_tmset(void);
	void sys_bi_tmdel(void);

};

#endif
