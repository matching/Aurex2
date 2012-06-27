// -*-c++-*-
/* $Id: bdflcdfont.h,v 1.2 2004/05/07 08:25:03 fumi Exp $ */

#ifndef BDFLCDFONT_H
#define BDFLCDFONT_H

#include "sdlcdfont.h"

class BdfFontSets;

/** SDLcdFont ¤Î BDF ÈÇ
 */
class BdfLcdFont : public SDLcdFont
{
  protected:
	BdfFontSets *fontsets;
	int gap;
	int width;
	int height;

  public:
	BdfLcdFont(int width, int height, int gap, BdfFontSets *fontsets);
	~BdfLcdFont(void);

	virtual unsigned char **drawString(char const *str, int *resultwidth);
	virtual void freeBuffer(unsigned char **buf);
};

#endif
