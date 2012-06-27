/*
 * mnexpr.cpp - Panasonic MN10200 CPU emulation module for execution.
 *
 * $Id: mnexpr.cpp,v 1.8 2004/05/10 14:56:05 fumi Exp $
 *
 * Copyright (C) 2003, 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
 * All rights reserved.
 *
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */

/*
 * mn10200 の算術／論理命令集
 */

#include <stdio.h>

#include "cpu.h"
#include "memorycpu.h"


#define set_zf(c) (((c) & 0xffff) == 0)
#define set_nf(c) ((c) >> 15)

#define set_zx(c) (((c) & 0xffffffUL) == 0)
#define set_nx(c) ((c) >> 23)


/* add vf 
 *
 * ~dst&~src&res || dst&src&~res
 * VF 15 or VX 23 bit
 *   ds|00 01 11 10
 *    -+-----------
 * r  0| 0  0  1  0
 *    1| 1  0  0  0
 */

#define set_add_vf(s, d, r) \
(((s) & (1 << 15)) == ((d) & (1 << 15)) && (((r) & (1 << 15)) != ((d) & (1 << 15))))
#define set_add_vx(s, d, r) \
(((s) & (1 << 23)) == ((d) & (1 << 23)) && (((r) & (1 << 23)) != ((d) & (1 << 23))))


/* sub vf 
 *
 * ~dst&src&res || dst&~src&~res
 * VF 15 or VX 23 bit
 *   ds|00 01 11 10
 *    -+-----------
 * r  0| 0  0  0  1
 *    1| 0  1  0  0
 */
#define set_sub_vf(s, d, r) \
(((s) & (1 << 15)) != ((d) & (1 << 15)) && ((r) & (1 << 15)) != ((d) & (1 << 15)))
#define set_sub_vx(s, d, r) \
(((s) & (1 << 23)) != ((d) & (1 << 23)) && ((r) & (1 << 23)) != ((d) & (1 << 23)))

unsigned long Cpu::_and(unsigned long x, unsigned long y)
{
	x=x & y;

	regs.psw.row&=0xfff0;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::_or(unsigned long x, unsigned long y)
{
	x=x | y;

	regs.psw.row&=0xfff0;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::_xor(unsigned long x, unsigned long y)
{
	x=x ^ y;

	regs.psw.row&=0xfff0;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::_not(unsigned long x)
{

	x=x ^ 0xffff;

	regs.psw.row&=0xfff0;

	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::asr(unsigned long x)
{
	unsigned long high_word;	/* 15 ~ 23 ビットを保存しておく */

	high_word=x & 0xff8000UL;

	x&=0xffff;

	regs.psw.row&=0xfff0;

	regs.psw.bit.cf=x & 1;
	x>>=1;
	x|=high_word;

	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::lsr(unsigned long x)
{
	unsigned long high_word;	/* 16 ~ 23 ビットを保存しておく */

	high_word=x & 0xff0000UL;

	x&=0xffff;

	regs.psw.row&=0xfff0;

	regs.psw.bit.cf=x & 1;
	x>>=1;
	x|=high_word;

	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::ror(unsigned long x)
{
	unsigned long high_word;	/* 16 ~ 23 ビットを保存しておく */
	int tmp;

	high_word=x & 0xff0000UL;

	x&=0xffff;


	tmp=x & 1;
	x>>=1;
	x|=regs.psw.bit.cf << 15;
	x|=high_word;

	regs.psw.row&=0xfff0;
	regs.psw.bit.cf=tmp;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}
unsigned long Cpu::rol(unsigned long x)
{
	unsigned long high_word;	/* 16 ~ 23 ビットを保存しておく */
	int tmp;

	high_word=x & 0xff0000UL;


	tmp=x >> 15;

	x&=0x7fff;

	x<<=1;
	x|=regs.psw.bit.cf;
	x|=high_word;

	regs.psw.row&=0xfff0;
	regs.psw.bit.cf=tmp;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	return x;
}

unsigned long Cpu::mul(unsigned long x, unsigned long y)
{
	int sign=0;

	x&=0xffff;
	y&=0xffff;

	if(x >> 15)
	{
		sign=1;
		x=(x ^ 0xffff)+1;
		x&=0xffff;
	}
	if(y >> 15)
	{
		sign^=1;
		y=(y ^ 0xffff)+1;
		y&=0xffff;
	}

	x=x*y;
	if(sign)
		x=~x+1;

	regs.psw.row&=0xfff4;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	regs.mdr=x>>16;

	return mem->to24bit(x);
}

unsigned long Cpu::mulu(unsigned long x, unsigned long y)
{

	x&=0xffff;
	y&=0xffff;

	x=x*y;

	regs.psw.row&=0xfff4;
	regs.psw.bit.nf=set_nf(x);
	regs.psw.bit.zf=set_zf(x);

	if(x == 0)
		regs.psw.bit.zf=1;
	if(x >> 31)
		regs.psw.bit.nf=1;

	regs.mdr=x>>16;

	return mem->to24bit(x);
}

unsigned long Cpu::divu(unsigned long x, unsigned long y)
{
	y&=0xffff;

	if(y == 0)
	{
/* divide by zero */
		regs.psw.bit.vf=1;
		return 0;
	}

	x&=0xffff;
	x|=(unsigned long)regs.mdr << 16;

	regs.mdr=x%y;
	x=x/y;

	if(x >> 16) /* overflow */
		regs.psw.bit.vf=1;
	else
	{
		regs.psw.row&=0xffc4;

		regs.psw.bit.nf=set_nf(x);
		if(x == 0)
			regs.psw.bit.zf=1;
	}

	return x & 0xffff;
}

unsigned long Cpu::sub(unsigned long dst, unsigned long src)
{
	unsigned long res;

	res=dst-src;

	regs.psw.bit.zf=set_zf(res);
	regs.psw.bit.nf=set_nf(res);
	regs.psw.bit.zx=set_zx(res);
	regs.psw.bit.nx=set_nx(res);
	regs.psw.bit.cf=((src & 0xffff) > (dst & 0xffff));
	regs.psw.bit.cx=(src > dst);
	regs.psw.bit.vf=set_sub_vf(src, dst, res);
	regs.psw.bit.vx=set_sub_vx(src, dst, res);

	return mem->to24bit(res);
}
unsigned long Cpu::subc(unsigned long dst, unsigned long src)
{
	unsigned long res;

	src+=regs.psw.bit.cf;
	res=dst-src;

	regs.psw.bit.nf=set_nf(res);
	regs.psw.bit.zf=set_zf(res);
	regs.psw.bit.nx=set_nx(res);
	regs.psw.bit.zx=set_zx(res);
	regs.psw.bit.cf=((src & 0xffff) > (dst & 0xffff));
	regs.psw.bit.cx=(src > dst);
	regs.psw.bit.vf=set_sub_vf(src, dst, res);
	regs.psw.bit.vx=set_sub_vx(src, dst, res);

	return mem->to24bit(res);
}
unsigned long Cpu::add(unsigned long dst, unsigned long src)
{
	unsigned long res;

	res=dst+src;

	regs.psw.bit.zf=set_zf(res);
	regs.psw.bit.nf=set_nf(res);
	regs.psw.bit.zx=set_zx(res);
	regs.psw.bit.nx=set_nx(res);
	regs.psw.bit.cf=((~dst & 0xffff) < (src & 0xffff));
	regs.psw.bit.cx=(~dst < src);
	regs.psw.bit.vf=set_add_vf(src, dst, res);
	regs.psw.bit.vx=set_add_vx(src, dst, res);

	return mem->to24bit(res);
}
unsigned long Cpu::addc(unsigned long dst, unsigned long src)
{
	unsigned long res;

	src+=regs.psw.bit.cf;
	res=dst+src;

	regs.psw.bit.nf=set_nf(res);
	regs.psw.bit.zf=set_zf(res);
	regs.psw.bit.nx=set_nx(res);
	regs.psw.bit.zx=set_zx(res);
	regs.psw.bit.cf=((~dst & 0xffff) < (src & 0xffff));
	regs.psw.bit.cx=(~dst < src);
	regs.psw.bit.vf=set_add_vf(src, dst, res);
	regs.psw.bit.vx=set_add_vx(src, dst, res);

	return mem->to24bit(res);
}


void Cpu::bset(unsigned long d, unsigned long a)
{
	unsigned long tmp;

	tmp=mem->read8_0e(a);
	regs.psw.row&=0xfff0;
	if((tmp & d) == 0)
		regs.psw.bit.zf=1;

	mem->write8(a, tmp | d);

}

void Cpu::bclr(unsigned long d, unsigned long a)
{
	unsigned long tmp;

	tmp=mem->read8_0e(a);

	regs.psw.row&=0xfff0;
	if((tmp & d) == 0)
		regs.psw.bit.zf=1;

	mem->write8(a, tmp & ~d);
}




unsigned long Cpu::addnf(unsigned long dst, unsigned long src)
{
	return mem->to24bit(dst+src);
}

/* EXT Dn Dn.bp15=0 ならばS --------2 3 F3:C1+Dn<<2 *6 61
 * x'0000'→MDR
 * Dn.bp15=1 ならば
 * x'FFFF'→MDR
 */
unsigned short Cpu::ext(unsigned long x)
{
	if(x & (1 << 15))
		return 0xffff;

	return 0;
}
