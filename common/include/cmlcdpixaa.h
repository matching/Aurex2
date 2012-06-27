// -*-c++-*-
// $Id: cmlcdpixaa.h,v 1.3 2006/02/19 13:04:39 fumi Exp $

#ifndef CMLCDPIXAA_H
#define CMLCDPIXAA_H


#include "cmlcdpixdefault.h"

class AAPixel;

class CMLcdPixAA : public CMLcdPixDefault
{
  protected:
	AAPixel *aapixel;

  public:
	CMLcdPixAA(int expansion, int ai_value = 0);
	~CMLcdPixAA(void);

	void pset(int x, int y, int c);
};

#endif
