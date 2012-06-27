// $Id: emu_wbios.h,v 1.1 2005/01/08 09:40:31 fumi Exp $

#ifndef EMU_WBIOS_H
#define EMU_WBIOS_H

int *emu_sys_bi_tmset(int timkind, int priority, int cnt, void (*func)());
int  emu_sys_bi_tmdel(int *timerid);

#endif
