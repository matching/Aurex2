// -*-c++-*-
/* $Id: bdflcdfontjis.h,v 1.2 2004/05/07 08:25:03 fumi Exp $ */

#ifndef BDFLCDFONTJIS_H
#define BDFLCDFONTJIS_H

#include "bdflcdfont.h"
class CesSjis;

/** SDLcdFont ¤Î BDF ÈÇ JIS
 */
class BdfLcdFontJis : public BdfLcdFont
{
  private:
	CesSjis *sjis;

  public:
	BdfLcdFontJis(int width, int height, int gap, BdfFontSets *fontsets);
	~BdfLcdFontJis(void);

	unsigned char **drawString(char const *str, int *resultwidth);
	void freeBuffer(unsigned char **buf);
};

#endif
