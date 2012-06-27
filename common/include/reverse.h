// -*-c++-*-
// $Id: reverse.h,v 1.5 2005/01/06 14:21:33 fumi Exp $

#ifndef REVERSE_H
#define REVERSE_H

#include "lcdblrv.h"

class Reverse : public LcdBlRv
{
  private:
	int x, y;
	int width;
	int height;

  public:
	Reverse(int x1, int y1, int x2, int y2);
	int proceed(void);
};

#endif
