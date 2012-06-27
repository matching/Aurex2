// -*-c++-*-
// $Id: port.h,v 1.11 2006/01/25 09:43:14 fumi Exp $

#ifndef PORT_H
#define PORT_H

class CMLcd;
class LcdPort;
class Ruputer;


class Port {

  private:
	CMLcd *cmlcd;
	LcdPort *lcdport;
	Ruputer *ruputer;

  public:
	Port(CMLcd *cmlcd, Ruputer *ruputer);
	~Port(void);

	bool read8(unsigned long addr, unsigned char *c);
	bool read16(unsigned long addr, unsigned short *c);
	bool read24(unsigned long addr, unsigned long *c);

	bool write8(unsigned long addr, unsigned char c);
	bool write16(unsigned long addr, unsigned short c);
	bool write24(unsigned long addr, unsigned long c);

};


#endif
