// -*-c++-*-
// $Id: cessjis.h,v 1.7 2005/07/27 18:53:28 fumi Exp $

#ifndef CESSJIS_H
#define CDSSJIS_H

class CesSjis
{
  public:
	CesSjis(void);
	~CesSjis(void);

	int checkChar(char const *str);
	int checkChar(char const *str, unsigned short *achar);

	int convToEuc(char const *src, char *dst);

	bool isShiftJis1(unsigned char c);
	bool isShiftJis2(unsigned char c);

};

#endif
