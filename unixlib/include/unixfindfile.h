// -*-c++-*-
// $Id: unixfindfile.h,v 1.3 2006/02/22 17:40:39 fumi Exp $

#ifndef UNIXFINDFILE
#define UNIXFINDFILE

#include "sdfindfile.h"
#include <glob.h>
#include <sys/stat.h>

class UnixFindFile : public SDFindFile
{
  private:
	glob_t glob_data;
	int glob_index;

	char matchpath[256];

  public:
	UnixFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);
	~UnixFindFile(void);

	int findFirst(void);
	int findNext(void);

  private:
	int processNextElement(void);
	int pickupNextElement(char *name, struct stat *stat_data);

	int is_mokuteki_attrib(mode_t attrib);
	unsigned char to_ruputer_attrib(mode_t attrib);
	void set_file_time(time_t mtime);
};

#endif
