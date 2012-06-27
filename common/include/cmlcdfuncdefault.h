// -*-c++-*-
// $Id: cmlcdfuncdefault.h,v 1.4 2006/01/21 18:04:34 fumi Exp $

#ifndef CMLCDFUNCDEFAULT_H
#define CMLCDFUNCDEFAULT_H

#include "cmlcdfunc.h"

class Ruputer;
class CMLcdPix;

/*
 */
class CMLcdFuncDefault : public CMLcdFunc
{
  private:

  public:
	CMLcdFuncDefault(Ruputer *ruputer, int expansion, bool after_image, int ai_value);
	virtual ~CMLcdFuncDefault(void);

	virtual unsigned char const *getPixelArray(int x, int y);

	virtual void drawLine(int x1, int y1, int x2, int y2, unsigned char style, int logic);
	virtual void drawLine(int x1, int y1, int x2, int y2, unsigned long style, int logic);

	virtual void drawRect(int x1, int y1, int x2, int y2, unsigned char style, int logic);
	virtual void fillRect(int x1, int y1, int x2, int y2, unsigned long style, int logic);

	virtual void drawEllipse(int xc, int yc, int rx, int ry, int logic);
	virtual void drawStringSub(int x, int y, int width, int height, char const *str, int prop_flag, int gap, int logic, int latin);

	virtual void reverseRect(int x1, int y1, int x2, int y2);
	virtual void clearRect(int x1, int y1, int x2, int y2);

  private:
	void subDrawString(unsigned char **buf, int x, int y, int w, int h, int logic);

};

#endif
