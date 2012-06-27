// -*-c++-*-
// $Id: sdfile.h,v 1.2 2004/04/29 09:44:32 fumi Exp $
#ifndef SDFILE_H
#define SDFILE_H

class SDFile
{
  public:
	int createDirectory(char const *dir);
	int removeDirectory(char const *dir);

	unsigned short createFile(char const *filename);
	unsigned short crnewFile(char const *filename);

	unsigned short open(char const *filename);

	void close(unsigned short no);

	unsigned short read(char *buf, int len, unsigned short no);
	unsigned short write(char *buf, int len, unsigned short no);
}

#endif
