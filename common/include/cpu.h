/* -*-mode:c++; coding:euc-jp-*- */
/* $Id: cpu.h,v 1.16 2004/11/21 15:12:27 fumi Exp $ */

#ifndef CPU_H
#define CPU_H

// Register の定義
#include "mncore.h"
#include "ruputerexception.h"

class MemoryForCpu;
class Interruption;

class Cpu
{
  private:
	MemoryForCpu *mem;

	int cycle;

	Interruption *interruption;


  public:
/* アクセサ使うよりはマシか？ */
	Register regs;

	Cpu(unsigned long pc, unsigned long a3, Interruption *interruption, MemoryForCpu *mem);
	~Cpu(void);

	int emulate(void);

	void addCycle(int _cycle)
	{
		cycle+=_cycle;
	}
	void setCycle(int _cycle)
	{
		cycle=_cycle;
	}
	int getCycle(void)
	{
		return cycle;
	}

	int printMnemonic(char *str);

	void convRegsToString(char *str);
	void printRegs(void);


  private:
	int emulate_core(void);
	void undefined_instruction(void) throw(UndefinedInstructionException);

	unsigned long sub(unsigned long dst, unsigned long src);
	unsigned long add(unsigned long dst, unsigned long src);

	unsigned long subc(unsigned long dst, unsigned long src);
	unsigned long addc(unsigned long dst, unsigned long src);

	unsigned long divu(unsigned long x, unsigned long y);
	unsigned long mulu(unsigned long x, unsigned long y);
	unsigned long mul(unsigned long x, unsigned long y);
	unsigned long rol(unsigned long x);
	unsigned long ror(unsigned long x);
	unsigned long lsr(unsigned long x);
	unsigned long asr(unsigned long x);

	unsigned long _not(unsigned long x);
	unsigned long _xor(unsigned long x, unsigned long y);
	unsigned long _or(unsigned long x, unsigned long y);
	unsigned long _and(unsigned long x, unsigned long y);

	void bclr(unsigned long d, unsigned long a);
	void bset(unsigned long d, unsigned long a);

	unsigned long addnf(unsigned long dst, unsigned long src);
	unsigned short ext(unsigned long x);

	void mn10200_cpu_timer(int c);

	void decrementCycleForMemoryAccess(unsigned long effaddr, int n);
};

#endif
