// $Id: emu_lcdbios.h,v 1.5 2006/01/21 18:04:35 fumi Exp $

#ifndef EMU_LCDBIOS_H
#define EMU_LCDBIOS_H

int emu_blink(int x1, int y1, int x2, int y2, int color);
int emu_blstop(int no);
int emu_blstart(int no);
int emu_blend(int no);

int	emu_gv_kput(int x, int y, char *str, int font, int gap, int lop);
int emu_gv_sput(int x, int y, unsigned short code, int lop);
int emu_gv_clear(int x1, int y1, int x2, int y2);
int emu_gv_reverse(int x1, int y1, int x2, int y2);
int emu_gv_put(int x, int y, char *buf, int logic);
int emu_gv_get(int x1, int y1, int x2, int y2, char *buf);
int emu_gv_place(int x, int y);
int emu_gv_line(int x1, int y1, int x2, int y2, int logic, unsigned char style);

/* emulator only */
int emu_gv_kput2( int x, int y, char const *str, int font, int gap, int lop, int lang );

#endif
