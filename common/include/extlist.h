/* -*-c++-*-
 * $Id: extlist.h,v 1.2 2005/01/06 14:21:33 fumi Exp $
 */

#ifndef EXTLIST_H
#define EXTLIST_H

#include <set>
#include <string>
using namespace std;

class ExtList
{
  private:
	set<string> exts;

  public:
	ExtList(void);
	~ExtList(void);

	void add(char const *ext);
	void add(ExtList *extlist);
	void getExtListString(char *extlist);
};

#endif

