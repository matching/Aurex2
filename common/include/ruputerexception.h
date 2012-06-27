// -*-c++-*-
// $Id: ruputerexception.h,v 1.4 2006/01/09 07:43:15 fumi Exp $

#ifndef RUPUTEREXCEPTION_H
#define RUPUTEREXCEPTION_H

// Ruputer 実機でも起こりうる例外を書く。

#include "throwable.h"
class Register;

class RuputerException : public Throwable
{
  private:
	Register *regs;

	static char regsString[ 256 ];

  public:
	RuputerException(void);
	RuputerException(Register *regs);
	virtual ~RuputerException(void);

	void getRegisterString(char *out);

	char const *getRegisterString(void);
};

class OddAccessException : public RuputerException
{
  private:
	unsigned long addr;
  public:
	OddAccessException(unsigned long addr);
	OddAccessException(Register *regs, unsigned long addr);

	unsigned long getEffectiveAddress(void);
};

class UndefinedInstructionException : public RuputerException
{
  public:
	UndefinedInstructionException(Register *regs);
};

class MemoryBoundaryException : public RuputerException
{
  public:
	MemoryBoundaryException(void);
};

#endif
