/* -*-c++-*-
 * $Id: codingbase.h,v 1.3 2005/01/13 08:40:42 fumi Exp $
 */
#ifndef CODINGBASE_H
#define CODINGBASE_H

class CodingBase
{
  protected:
	unsigned char *buf;
	int len;

  public:
	CodingBase(void);
	virtual ~CodingBase(void);

	virtual int searchForward(int start, char c) = 0;
	virtual int searchReverse(char c) = 0;
	virtual void subString(int start, int end, char *out) = 0;

	virtual void replaceCharacter(char from, char to) = 0;
	virtual void toString(char *out) = 0;
};

#endif
