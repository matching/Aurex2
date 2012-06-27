// -*-c++-*-
// $Id: winlcdfactory.h,v 1.7 2005/01/14 04:09:40 fumi Exp $
#ifndef WINLCDFACTORY_H
#define WINLCDFACTORY_H

#include "sdlcdfactory.h"
#include <windows.h>

class WINLcd;
class WidgetOption;
class WINLcdFactory : public SDLcdFactory
{
  public:
	WINLcdFactory(WidgetOption *option);
	~WINLcdFactory(void);
	WINLcd *getWINLcd(RupAddress *systemAddress, HWND hWnd, CMLcd *cmlcd);

	int getWidth(void);
	int getHeight(void);

  private:
	int getType(void);

};

#endif
