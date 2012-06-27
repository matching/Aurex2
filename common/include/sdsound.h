// -*-c++-*-
// $Id: sdsound.h,v 1.3 2006/01/28 11:22:31 fumi Exp $

#ifndef SDSOUND_H
#define SDSOUND_H

class SDSound
{
  private:
	enum {
		INITIAL,
		OPEN,} state;

  public:
	SDSound(void);
	virtual ~SDSound(void);

	int open(void);
	int output(unsigned char const *buf, int len);

  private:
	virtual int subOpen(void) = 0;
	virtual int subOutput(unsigned char const *buf, int len) = 0;
//	virtual int subClose(void) = 0;
};

#endif
