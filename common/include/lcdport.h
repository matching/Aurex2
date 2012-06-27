// -*-c++-*-
// $Id: lcdport.h,v 1.7 2004/10/31 13:41:17 fumi Exp $

#ifndef LCDPORT_H
#define LCDPORT_H

class CMLcd;

class LcdPort
{
  private:
	CMLcd *cmlcd;

	int x;
	int y;

	int adc;
	int rmw;
	int inv;

	int read_x;
	int rmw_prev_x;

  public:
	LcdPort(CMLcd *cmlcd);

	bool writeLcdPort8(unsigned long addr, unsigned char c);
	bool writeLcdPort16(unsigned long addr, unsigned short c);
	bool writeLcdPort24(unsigned long addr, unsigned long c);

	bool readLcdPort8(unsigned long addr, unsigned char *c);
	bool readLcdPort16(unsigned long addr, unsigned short *c);
	bool readLcdPort24(unsigned long addr, unsigned long *c);

  private:
	bool isLcdPort(unsigned long addr);
	void writeLcdDataPort(unsigned char data);
	void writeLcdCtrlPort(unsigned char data);

	unsigned char readLcdDataPort(void);

};

#endif
