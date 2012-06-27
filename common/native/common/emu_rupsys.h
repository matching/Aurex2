// $Id: emu_rupsys.h,v 1.2 2004/04/29 09:44:33 fumi Exp $

#ifndef EMU_RUPSYS_H
#define EMU_RUPSYS_H

int emu_Keyin(int a, int b);
int emu_EventCall(int a);

#define Bkey_dw 0x0080
#define Bkey_rg 0x0040
#define Bkey_up 0x0020
#define Bkey_lf 0x0010
#define Bkey_B	0x0008
#define Bkey_C	0x0004
#define Bkey_D	0x0002
#define Bkey_A	0x0001

#endif
