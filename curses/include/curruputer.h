//-*-c++-*-
#ifndef CURRUPUTER_H
#define CURRUPUTER_H

#include "ruputer.h"

class CURLcd;
class Directory;

class CURRuputer : public Ruputer{

  private:
	CURLcd *curlcd;

  public:
	CURRuputer(Frame *frame, Directory *sddi, Option *opt, UFilerIni *uini);
	~CURRuputer(void);

	SDFindFile *createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);
	void setupLog(void);

  private:

	int EventProcedure(void);
};


#endif

