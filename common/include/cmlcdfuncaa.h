// -*-c++-*-
// $Id: cmlcdfuncaa.h,v 1.4 2006/01/21 18:04:34 fumi Exp $

#ifndef CMLCDFUNCAA_H
#define CMLCDFUNCAA_H

#include "cmlcdfuncdefault.h"

class Ruputer;
class CMLcdPix;

/*
 */
class CMLcdFuncAa : public CMLcdFuncDefault
{
  private:

  public:
	CMLcdFuncAa(Ruputer *ruputer, int expansion, bool after_image, int ai_value);
	virtual ~CMLcdFuncAa(void);

	unsigned char const *getPixelArray(int x, int y);

	void drawLine(int x1, int y1, int x2, int y2, unsigned char style, int logic);
	void drawLine(int x1, int y1, int x2, int y2, unsigned long style, int logic);

	void drawRect(int x1, int y1, int x2, int y2, unsigned char style, int logic);
	void fillRect(int x1, int y1, int x2, int y2, unsigned long style, int logic);

	void drawEllipse(int xc, int yc, int rx, int ry, int logic);
	void drawStringSub(int x, int y, int width, int height, char const *str, int prop_flag, int gap, int logic, int latin);

	void reverseRect(int x1, int y1, int x2, int y2);
	void clearRect(int x1, int y1, int x2, int y2);

  private:
	void setPointReal(int x, int y, int color, int logic);
	void subDrawStringReal(unsigned char **buf, int x, int y, int w, int h, int logic);

};

#endif
