// -*-c++-*-
// $Id: winfindfile.h,v 1.2 2006/02/22 17:40:39 fumi Exp $

#ifndef WINFINDFILE
#define WINFINDFILE

#include <windows.h>
#include "sdfindfile.h"

class WINFindFile : public SDFindFile
{
  private:
	HANDLE hFind;

	static DWORD const win_attrib_tbl[];

  public:
	WINFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);
	~WINFindFile(void);

	int findFirst(void);
	int findNext(void);
  private:
	int is_mokuteki_attrib(DWORD attrib, int rup_attrib);
	unsigned char to_ruputer_attrib(DWORD attrib);
	void set_file_time(WIN32_FIND_DATA const *find_data);
};

#endif
