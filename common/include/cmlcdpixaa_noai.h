// -*-c++-*-
// $Id: cmlcdpixaa_noai.h,v 1.2 2005/01/06 14:21:33 fumi Exp $

#ifndef CMLCDPIXAA_NOAI_H
#define CMLCDPIXAA_NOAI_H

#include "cmlcdpixaa.h"

class CMLcdPixAA_Noai : public CMLcdPixAA
{
  public:
	CMLcdPixAA_Noai(int expansion);
	virtual ~CMLcdPixAA_Noai(void);

	void updatePixel(int start_x, int start_y, enum UpdateRegion updateRegion);
};

#endif
