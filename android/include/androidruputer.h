//-*-c++-*-
#ifndef ANDROIDRUPUTER_H
#define ANDROIDRUPUTER_H

#include "ruputer.h"
#include "jp_ruputer_AndroAurex_JniIF.h"

class AndroidLcd;
class Directory;

class AndroidRuputer : public Ruputer{

  private:
	AndroidLcd *andlcd;

  public:
	AndroidRuputer(Frame *frame, Directory *sddi, Option *opt, UFilerIni *uini, JNIEnv *env, jobject jniif, jobject bitmap);
	~AndroidRuputer(void);

	SDFindFile *createSDFindFile(struct dta_t *dta_area, char const *pathname, unsigned char search_attribute);
	void setupLog(void);
  private:

	int EventProcedure(void);

};


#endif

