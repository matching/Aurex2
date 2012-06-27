// $Id: widgetconfig.h,v 1.1 2007/07/28 17:36:52 fumi Exp $

#ifndef CONFIG_H
#define CONFIG_H

#ifdef HAVE_SDCONFIG
# include "sdconfig.h"
#endif

/* 以下デフォルトオプションの設定 */

#ifndef OPTION_DEFAULT_FULL_WINDOW
# define OPTION_DEFAULT_FULL_WINDOW false
#endif

#ifndef OPTION_DEFAULT_FLIP
# define OPTION_DEFAULT_FLIP false
#endif

#endif // CONFIG_H
