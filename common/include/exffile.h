// -*-c++-*-
// $Id: exffile.h,v 1.3 2005/01/06 14:21:33 fumi Exp $

#ifndef EXFFILE_H
#define EXFFILE_H

#include "throwable.h"
class ExfFileException : public Throwable
{
};
class ExfFileNotFoundException : public ExfFileException
{
};
class ExfFileInvalidException : public ExfFileException
{
};

class AssociatedFileException : public Throwable
{
};
class AssociatedFileNotInVirtualDirectoryException : public AssociatedFileException
{
};

#include <stdio.h>

class ExfFile
{
  private:
	FILE *fp;

	unsigned long pc, psp;
	unsigned long loadAddr, size;

  public:
	ExfFile(char const *filename) throw(ExfFileNotFoundException);
	~ExfFile(void);

	void loadToMemory(void) throw(ExfFileInvalidException);
	unsigned long getPc(void);
	unsigned long getPsp(void);

	bool getMiniIcon(unsigned char *out);
	bool getLargeIcon(unsigned char *out, int *width, int *height);

  private:
	unsigned long getLoadAddressFromFile(void) throw(ExfFileInvalidException);
	unsigned long getPCAddressFromFile(void) throw(ExfFileInvalidException);
	unsigned long getFileSize(void);

	unsigned long getUnsignedLong(long offset) throw(ExfFileInvalidException);


	bool getMMPData(int width, int height, unsigned char const *icon, unsigned char *out);
};

#endif
