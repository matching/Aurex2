// -*-c++-*-
// $Id: cmlcdpixdefault.h,v 1.4 2006/02/16 13:05:12 fumi Exp $

#ifndef CMLCDPIXDEFAULT_H
#define CMLCDPIXDEFAULT_H

#include "cmlcd.h"

class CMLcdPixDefault : public CMLcdPix
{
  protected:
	unsigned char **pixelData;
	int ai_value;

	unsigned char aiTransientTable[256][256];

  public:
	CMLcdPixDefault(int expansion, int ai_value = 0);
	virtual ~CMLcdPixDefault(void);

	void pset(int x, int y, int c);

	void psetReal(int x, int y, int c);
	int  pgetReal(int x, int y);
	virtual void updatePixel(int start_x, int start_y, enum UpdateRegion updateRegion);
};

#endif
