/* $Id: emu_common.h,v 1.2 2006/01/21 18:04:35 fumi Exp $ */

#ifndef EMU_COMMON_H
#define EMU_COMMON_H

extern char buffer_for_gv_get[];


enum LANG_FLAG
{
	USE_JAPANESE,
	USE_ISO8859_1,
	USE_DEFAULT, /* オプションに従う */
};

void gv_kput2_init(int font, enum LANG_FLAG lang);
int gv_kput2( int x, int y, char const *str, int gap, int lop );

#endif
