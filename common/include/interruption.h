/* -*-c++-*- */
/* $Id: interruption.h,v 1.9 2004/10/31 13:41:17 fumi Exp $ */

#ifndef INTERRUPTION_H
#define INTERRUPTION_H

class DosIntr;
class LcdIntr;
class BiosIntr;
class SysIntr;
class Register;

class Interruption
{
  private:
	DosIntr *dosIntr;
	LcdIntr *lcdIntr;
	BiosIntr *biosIntr;
	SysIntr *sysIntr;

  public:
	Interruption(DosIntr *dosIntr, LcdIntr *lcdIntr, BiosIntr *biosIntr, SysIntr *sysIntr);
	~Interruption(void);

	int execSystemCall(Register *regs, unsigned long *addr);
};


#endif
