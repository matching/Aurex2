/* -*-c++-*- */
// $Id: rupsys.h,v 1.12 2006/02/17 19:01:38 fumi Exp $

#ifndef RUPSYS_H
#define RUPSYS_H


class Thread;
class ThreadState;
class RupAddress;
class Register;
class KeyBuffer;
class RuputerState;

class SysIntr
{
  private:
	static int initial_flag;

	unsigned long kernelsp;
	ThreadState *threadState;
	RuputerState *ruputerState;
	KeyBuffer *keybuffer;
	RupAddress *systemAddress;

  public:
	SysIntr(Ruputer *ruputer, Thread *thread, unsigned long kernelsp);

  public:
	void initRupSysTable(void);

	void callSysIntr(Register *regs, unsigned long addr);
	void callNativeCode(Register *regs, unsigned long addr);
};

#endif

