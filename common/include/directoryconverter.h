// -*- c++ -*-
// $Id: directoryconverter.h,v 1.3 2004/11/06 17:16:01 fumi Exp $

#ifndef DIRECTORYCONVERTER_H
#define DIRECTORYCONVERTER_H

class Directory;

#include "throwable.h"

class DirectoryConverterException : public Throwable
{
};

class DirectoryConverter
{
  private:
	Directory *from;
	Directory *to;

  public:
	DirectoryConverter(Directory *_from, Directory *_to) throw(DirectoryConverterException);
	int convert(char const *src, char *dst) throw(DirectoryConverterException);

  private:
	void replacePathDelimiter(char *str);

};




#endif
