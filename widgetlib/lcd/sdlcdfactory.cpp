#include "sdlcdfactory.h"
#include "option.h"
#include "optionitem.h"
#include "ruputer.h"

#include "widgetoption.h"

SDLcdFactory::SDLcdFactory(WidgetOption *opt)
{
	this->widgetOption = opt;

	changed = false;
}

SDLcdFactory::~SDLcdFactory(void)
{
}

bool SDLcdFactory::isChanged(void)
{
	return changed;
}

int SDLcdFactory::getFlipFlag(void)
{
	if( widgetOption->get_flip() )
		return 1;
	return 0;
}

int SDLcdFactory::getFullWindowFlag(void)
{
	if( widgetOption->get_full_window() )
		return 1;
	return 0;
}

