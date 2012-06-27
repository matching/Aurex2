// -*-c++-*-
// $Id: ruputerdirectory.h,v 1.6 2004/11/06 17:16:01 fumi Exp $

#ifndef RUPUTERDIRECTORY_H
#define RUPUTERDIRECTORY_H

#include "dosdirectory.h"

class Ruputer;

class RuputerDirectory : public DosDirectory
{
  private:
	Directory *localDirectory;

  public:
	RuputerDirectory(Directory *localDirectory);
	RuputerDirectory(Directory *localDirectory, char const *path);

	int chdir(char const *path);
	int getRestPath(char const *src, char *restpath) throw(DirectoryException);;

	void getReplacePath(char *dst);
	bool isSetupDirectory(void);
};

#endif
