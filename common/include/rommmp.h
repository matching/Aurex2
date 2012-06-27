// -*-c++-*-
// $Id: rommmp.h,v 1.2 2004/04/29 09:44:32 fumi Exp $

#ifndef ROMMMP_H
#define ROMMMP_H

class RomMmp
{
  private:
	static unsigned char romdata[];

	unsigned long startaddress;
	unsigned long endaddress;

  public:
	RomMmp(void);
	~RomMmp(void);

	bool read8(unsigned long addr, unsigned char *c);
	bool read16(unsigned long addr, unsigned short *c);
	bool read24(unsigned long addr, unsigned long *c);

	unsigned char *getMemoryPointer(unsigned long addr);

  private:
	bool isRomMmpArea(unsigned long addr);
	unsigned long getOffset(unsigned long addr);
};

#endif
