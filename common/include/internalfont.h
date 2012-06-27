// -*-c++-*-
// $Id: internalfont.h,v 1.4 2004/10/31 13:41:17 fumi Exp $

#ifndef INTERNALFONT_H
#define INTERNALFONT_H

class Ruputer;

class InternalFont
{
  private:
	Ruputer *ruputer;

//	unsigned char buffer[24];
	unsigned char buffer[32];
	unsigned long fontbaseaddr;

  public:
	InternalFont(Ruputer *ruputer);
	~InternalFont(void);

	bool read8(unsigned long addr, unsigned char *c);
	bool read16(unsigned long addr, unsigned short *c);
	bool read24(unsigned long addr, unsigned long *c);

  private:

	bool isInternalFontAddress(unsigned long addr);
	int getHankakuAddress(unsigned long addr);
	int getZenkakuAddress(unsigned long addr);

	unsigned char hankaku8(unsigned long addr);
	unsigned short hankaku16(unsigned long addr);
	unsigned long hankaku24(unsigned long addr);

	unsigned char zenkaku8(unsigned long addr);
	unsigned short zenkaku16(unsigned long addr);
	unsigned long zenkaku24(unsigned long addr);

	void createInternalFont(char *str, int n);
};

#endif
