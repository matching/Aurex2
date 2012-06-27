/* -*-c++-*-
 * $Id: codingshiftjis.h,v 1.2 2005/01/06 14:21:33 fumi Exp $
 */
#ifndef CODINGSHIFTJIS_H
#define CODINGSHIFTJIS_H

#include "codingascii.h"

class CesSjis;

class CodingShiftJis : public CodingAscii
{
  private:
	CesSjis *cessjis;

  public:
	CodingShiftJis(char const *str);
	virtual ~CodingShiftJis(void);

	int searchForward(int start, char c);
	int searchReverse(char c);
	void replaceCharacter(char from, char to);
};

#endif
