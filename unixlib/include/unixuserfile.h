// -*-c++-*-
// $Id: unixuserfile.h,v 1.2 2006/02/09 17:58:39 fumi Exp $

#ifndef UNIXUSERFILE
#define UNIXUSERFILE


class Option;
class UFilerIni;

class UnixUserFile
{
  private:
	Option *opt;
	UFilerIni *uini;

	char *homeDirectory;

  public:
	UnixUserFile(void);
	UnixUserFile(Option *opt);
	~UnixUserFile(void);

	Option *getOption(void);
	UFilerIni *getUFilerIni(void);

	bool load(void);
};

#endif
