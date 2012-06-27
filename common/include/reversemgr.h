// -*-c++-*-
// $Id: reversemgr.h,v 1.5 2005/01/06 14:21:33 fumi Exp $

#ifndef REVERSEMGR_H
#define REVERSEMGR_H


#include "lcdblrvmgr.h"

class CMLcd;

class ReverseMgr : public LcdBlRvMgr
{
  public:
	ReverseMgr(void) : LcdBlRvMgr()
	{
	}

	int add(int x1, int y1, int x2, int y2);

};

#endif
