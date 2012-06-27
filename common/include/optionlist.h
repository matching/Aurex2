// -*-c++-*-
// $Id: optionlist.h,v 1.4 2004/11/13 18:35:47 fumi Exp $

#ifndef OPTIONLIST_H
#define OPTIONLIST_H

#include <string>
#include <list>
#include <map>
using namespace std;


class OptionItem;

class OptionList
{
  private:
	map<string, string> options;

  public:
	OptionList(void);
	~OptionList(void);


	char const *getOptionValue(char const *key);
	void setOptionValue(char const *key, char const *value);
/*
	void syncToFile(void);
	void syncToValue(void);
*/

	bool load(char const *filename);
	bool save(char const *filename);

	void addOption(char const *key, char const *value);

  private:
	int equalLoadOption(void);
};


#endif
