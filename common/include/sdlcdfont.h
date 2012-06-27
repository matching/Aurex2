// -*-c++-*-
// $Id: sdlcdfont.h,v 1.3 2004/04/29 09:44:32 fumi Exp $

#ifndef SDLCDFONT_H
#define SDLCDFONT_H

class SDLcdFont
{
  public:
	SDLcdFont(void);
	virtual ~SDLcdFont(void);

	virtual unsigned char **drawString(char const *str, int *resultwidth)=0;
	virtual void freeBuffer(unsigned char **buf)=0;
};

#endif
