// -*-c++-*-
// $Id: cmlcdpixdefault_noai.h,v 1.2 2005/01/06 14:21:33 fumi Exp $

#ifndef CMLCDPIXDEFAULT_NOAI_H
#define CMLCDPIXDEFAULT_NOAI_H

#include "cmlcdpixdefault.h"

class CMLcdPixDefault_Noai : public CMLcdPixDefault
{
  public:
	CMLcdPixDefault_Noai(int expansion);
	~CMLcdPixDefault_Noai(void);

	void updatePixel(int start_x, int start_y, enum UpdateRegion updateRegion);
};

#endif
