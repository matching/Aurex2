// -*-c++-*-
#ifndef SDLCDFACTORY_H
#define SDLCDFACTORY_H


class WidgetOption;
class SDLcd;

class SDLcdFactory
{
  protected:
	WidgetOption *widgetOption;

	bool changed;

  public:
	SDLcdFactory(WidgetOption *opt);
	virtual ~SDLcdFactory(void);
//	virtual SDLcd *getSDLcd(void)=0;

	bool isChanged(void);

	int getFlipFlag(void);
	int getFullWindowFlag(void);

//	virtual void OptionIsChanged(Option *options);
};



#endif
