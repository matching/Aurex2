// -*-c++-*-
// $Id: blinkmgr.h,v 1.5 2005/01/06 14:21:33 fumi Exp $

#ifndef BLINKMGR_H
#define BLINKMGR_H

#include "lcdblrvmgr.h"

class CMLcd;

class BlinkMgr : public LcdBlRvMgr
{
  public:
	BlinkMgr(void) : LcdBlRvMgr()
	{
	}

	int add(int x1, int y1, int x2, int y2, int color);
};

#endif
