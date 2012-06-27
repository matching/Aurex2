// -*-c++-*-
// $Id: filerinibase.h,v 1.4 2005/07/27 18:53:28 fumi Exp $

#ifndef FILERINIBASE_H
#define FILERINIBASE_H

#include <string>
#include <map>
using namespace std;

class ExtList;

class FilerIniBase
{
  protected:
	map<string, string> assoc;

  public:
	FilerIniBase(void);
	virtual ~FilerIniBase(void);

	char const *getExecutableFileNameByExt(char const *ext);
	ExtList *getExtList(void);

	virtual void load(char const *filename) = 0;

  protected:
	void add(char const *ext, char const *exf);

  private:
	void normalizeExt(char const *in, char *out);
};

#endif

