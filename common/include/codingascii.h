/* -*-c++-*-
 * $Id: codingascii.h,v 1.2 2005/01/06 14:21:33 fumi Exp $
 */
#ifndef CODINGASCII_H
#define CODINGASCII_H

#include "codingbase.h"

class CodingAscii : public CodingBase
{
  public:
	CodingAscii(char const *str);
	virtual ~CodingAscii(void);

	virtual int searchForward(int start, char c);
	virtual int searchReverse(char c);
	virtual void replaceCharacter(char from, char to);

	void subString(int start, int end, char *out);
	void toString(char *out);
};

#endif
