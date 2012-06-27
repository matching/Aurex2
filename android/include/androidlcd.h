//-*-c++-*-
#ifndef ANDROIDLCD_H
#define ANDROIDLCD_H

#include "sdlcd.h"
#include "jp_ruputer_AndroAurex_JniIF.h"

class AndroidLcd : public SDLcd
{
  private:
	JNIEnv *env;
	jobject jniif;
	jobject bitmap;

  public:
	AndroidLcd(CMLcd *cmlcd, JNIEnv *env, jobject jniif, jobject bitmap);
	~AndroidLcd(void);

	virtual void updateWindow(void);

	SDLcdFont *createSDLcdFont(int width, int height, int prop, int gap, bool latin);
};

#endif
