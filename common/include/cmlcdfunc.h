// -*-c++-*-
// $Id: cmlcdfunc.h,v 1.4 2006/01/21 18:04:34 fumi Exp $

#ifndef CMLCDFUNC_H
#define CMLCDFUNC_H


#include "cmlcd.h"

class Ruputer;
class CMLcdPix;

/*
 */
class CMLcdFunc
{
  protected:
	Ruputer *ruputer;

	int expansion;
	CMLcdPix *pix;

  public:
	CMLcdFunc(Ruputer *ruputer, int expansion);
	virtual ~CMLcdFunc(void);

	CMLcdPix *getCMLcdPix(void);
	void updatePixel(int x, int y, enum UpdateRegion reg);

	void setPoint(int x, int y, int color, int logic);
	int  getPoint(int x, int y);
	void getMMPData(int x1, int y1, int x2, int y2, unsigned char *buf);
	void putMMPData(int start_x, int start_y, unsigned char *buf, int logic);
	void putMMPDataWithoutWH(int start_x, int start_y, int width, int height, unsigned char *buf, int logic);


	virtual unsigned char const *getPixelArray(int x, int y) = 0;

	virtual void drawLine(int x1, int y1, int x2, int y2, unsigned char style, int logic) = 0;
	virtual void drawLine(int x1, int y1, int x2, int y2, unsigned long style, int logic) = 0;

	virtual void drawRect(int x1, int y1, int x2, int y2, unsigned char style, int logic) = 0;
	virtual void fillRect(int x1, int y1, int x2, int y2, unsigned long style, int logic) = 0;

	virtual void drawEllipse(int xc, int yc, int rx, int ry, int logic) = 0;
	virtual void drawString(int x, int y, int width, int height, char const *str, int prop_flag, int gap, int logic);

	virtual void drawStringSub(int x, int y, int width, int height, char const *str, int prop_flag, int gap, int logic, int latin) = 0;

	virtual void reverseRect(int x1, int y1, int x2, int y2) = 0;
	virtual void clearRect(int x1, int y1, int x2, int y2) = 0;

};

#endif
