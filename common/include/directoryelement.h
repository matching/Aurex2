// -*-c++-*-
// $Id: directoryelement.h,v 1.1 2004/11/07 11:07:55 fumi Exp $

#ifndef DIRECTORYELEMENT
#define DIRECTORYELEMENT

class DirectoryElement
{
  public:
	char *directory;
	DirectoryElement *parent;
	DirectoryElement *child;

  public:
	DirectoryElement(void);
	DirectoryElement(char const *dir);
	DirectoryElement(char const *start, char const *end);

	virtual ~DirectoryElement(void);
	bool equal(DirectoryElement *dst);
};

#endif
