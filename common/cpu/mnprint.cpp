/*
 * mnprintr.cpp - Panasonic MN10200 CPU emulation module for debug.
 *
 * $Id: mnprint.cpp,v 1.10 2004/11/14 14:13:02 fumi Exp $
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


#include <stdio.h>
#include "cpu.h"
#include "memorycpu.h"


#define reg_index_high_low(c) (((c)>>4) & 0x3)
#define reg_index_low_high(c) (((c)>>2) & 0x3)
#define reg_index_low_low(c)  ((c) & 0x3)

/*
 * 目標としては逆アセンブラとしても使用したいー
 */
int Cpu::printMnemonic(char *str)
{
	unsigned long pc;

	unsigned char op_code;
	unsigned char op_code2;
	int ll, lh;
	int ll2, lh2, hl2;
	unsigned char m8;
	unsigned short m16;
	unsigned long  m24;
	unsigned char m82;
	unsigned short m162;
	unsigned long  m242;

	int ibyte=0;

#if 0
	int i=0;
	for(i = -1; i < 4; i++)
	{
		int m=mem->readAbsolute8(regs.pc+i);
		fprintf(stderr, "%02x ", m);
	}
	fputs("\n", stderr);
#endif

	pc = regs.pc; // 以降 regs は触らない

	op_code = mem->readAbsolute8(pc++);

	m8=mem->readAbsolute8(pc);
	m16=mem->readAbsolute16(pc);
	m24=mem->readAbsolute24(pc);

	ll=reg_index_low_low(op_code);
	lh=reg_index_low_high(op_code);

	switch(op_code & 0xf0)
	{
	case 0x00:
		fprintf(stderr, "mov d%d, (a%d)\n", ll, lh);
		break;
	case 0x10:
		fprintf(stderr, "movb d%d, (a%d)\n", ll, lh);
		break;
	case 0x20:
		fprintf(stderr, "mov (a%d), d%d\n", lh, ll);
		break;
	case 0x30:
		fprintf(stderr, "movbu (a%d), d%d\n", lh, ll);
		break;
	case 0x40:
		fprintf(stderr, "mov d%d, (0x%x, a%d)\n", ll, m8, lh);
		break;
	case 0x50:
		fprintf(stderr, "mov a%d, (0x%x, a%d)\n", ll, m8, lh);
		break;
	case 0x60:
		fprintf(stderr, "mov (0x%x, a%d), d%d\n", m8, lh, ll);
		break;
	case 0x70:
		fprintf(stderr, "mov (0x%x, a%d), a%d\n", m8, lh, ll);
		break;
	case 0x80:
		if(ll != lh)
			fprintf(stderr, "mov d%d, d%d\n", lh, ll);
		else
			fprintf(stderr, "mov 0x%x, d%d\n", m8, ll);
		break;
	case 0x90:
		fprintf(stderr, "add d%d, d%d\n", lh, ll);
		break;
	case 0xa0:
		fprintf(stderr, "sub d%d, d%d\n", lh, ll);
		break;
	case 0xb0:
		switch(op_code & 0xc)
		{
		case 0x00:
			fprintf(stderr, "extx d%d\n", ll);
			break;
		case 0x4:
			fprintf(stderr, "extxu d%d\n", ll);
			break;
		case 0x8:
			fprintf(stderr, "extxb d%d\n", ll);
			break;
		case 0xc:
			fprintf(stderr, "extxbu d%d\n", ll);
			break;
		}
		break;
	case 0xc0:
		switch(op_code & 0x0c)
		{
		case 0x0:
			fprintf(stderr, "mov d%d, (0x%x)\n", ll, m16);
			break;
		case 0x4:
			fprintf(stderr, "movb d%d, (0x%x)\n", ll, m16);
			break;
		case 0x8:
			fprintf(stderr, "mov (0x%x), d%d\n", m16, ll);
			break;
		case 0xc:
			fprintf(stderr, "movbu (0x%x), d%d\n", m16, ll);
			break;
		}
		break;
	case 0xd0:
		switch(op_code & 0xc)
		{
		case 0:
			fprintf(stderr, "add 0x%x, a%d\n", m8, ll);
			break;
		case 0x4:
			fprintf(stderr, "add 0x%x, d%d\n", m8, ll);
			break;
		case 0x8:
			fprintf(stderr, "cmp 0x%x, d%d\n", m8, ll);
			break;
		case 0xc:
			fprintf(stderr, "mov 0x%x, a%d\n", m16, ll);
			break;
		}
		break;
	case 0xe0:
	{
		static char *bra[]={
			"blt", "bgt", "bge", "ble", "bcs", "bhi", "bcc", "bls", "beq", "bne", "bra"};

		switch(op_code & 0xf)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 0xa:
			fprintf(stderr, "%s 0x%x\n", bra[op_code & 0xf], m8);
			break;
		case 0xb:
			fprintf(stderr, "rti \n");
			break;
		case 0xc:
		case 0xd:
		case 0xe:
		case 0xf:
			fprintf(stderr, "cmp 0x%x, a%d\n", m16, ll);
			break;
		}
		break;
	}
	case 0xf0:
		hl2=reg_index_high_low(m8);
		lh2=reg_index_low_high(m8);
		ll2=reg_index_low_low(m8);

		switch(op_code & 0xf)
		{
		case 0x0:

			switch(m8 & 0xf0)
			{
			case 0:
				if(m8 == 0 || m8 == 4 || m8 == 8 || m8 == 0xc)
					fprintf(stderr, "jmp (a%d)\n", lh2);
				else if(m8 == 1 || m8 == 5 || m8 == 9 || m8 == 0xd)
					fprintf(stderr, "jsr (a%d)\n", lh2);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;

			case 0x40:
			case 0x50:
			case 0x60:
			case 0x70:
				fprintf(stderr, "movb (d%d, a%d), d%d\n", hl2, lh2, ll2);
				break;
			case 0x80:
			case 0x90:
			case 0xa0:
			case 0xb0:
				fprintf(stderr, "movbu (d%d, a%d), d%d\n", hl2, lh2, ll2);
				break;

			case 0xc0:
			case 0xd0:
			case 0xe0:
			case 0xf0:
				fprintf(stderr, "movb d%d, (d%d, a%d)\n", ll2, hl2, lh2);
				break;

			case 0x20:
				fprintf(stderr, "bset d%d, (a%d)\n", ll2, lh2);
				break;
			case 0x30:
				fprintf(stderr, "bclr d%d, (a%d)\n", ll2, lh2);
				break;
			default:
				fprintf(stderr, "Undefined Instraction %x\n", op_code);

			}
			break;
		case 0x1:
			switch(m8 & 0xc0)
			{
			case 0x00:
				fprintf(stderr, "mov (d%d, a%d), a%d\n", hl2, lh2, ll2);
				break;
			case 0x40:
				fprintf(stderr, "mov (d%d, a%d), d%d\n", hl2, lh2, ll2);
				break;
			case 0x80:
				fprintf(stderr, "mov a%d, (d%d, a%d)\n", ll2, hl2, lh2);
				break;
			case 0xc0:
				fprintf(stderr, "mov d%d, (d%d, a%d)\n", ll2, hl2, lh2);
				break;
			}
			break;
		case 0x2:
			switch(m8 & 0xf0)
			{
			case 0x0:
				fprintf(stderr, "add d%d, a%d\n", lh2, ll2);
				break;
			case 0x10:
				fprintf(stderr, "sub d%d, a%d\n", lh2, ll2);
				break;
			case 0x20:
				fprintf(stderr, "cmp d%d, a%d\n", lh2, ll2);
				break;
			case 0x30:
				fprintf(stderr, "mov d%d, a%d\n", lh2, ll2);
				break;
			case 0x40:
				fprintf(stderr, "add a%d, a%d\n", lh2, ll2);
				break;
			case 0x50:
				fprintf(stderr, "sub a%d, a%d\n", lh2, ll2);
				break;
			case 0x60:
				fprintf(stderr, "cmp a%d, a%d\n", lh2, ll2);
				break;
			case 0x70:
				fprintf(stderr, "mov a%d, a%d\n", lh2, ll2);
				break;
			case 0x80:
				fprintf(stderr, "addc d%d, d%d\n", lh2, ll2);
				break;
			case 0x90:
				fprintf(stderr, "subc d%d, d%d\n", lh2, ll2);
				break;
			case 0xc0:
				fprintf(stderr, "add a%d, d%d\n", lh2, ll2);
				break;
			case 0xd0:
				fprintf(stderr, "sub a%d, d%d\n", lh2, ll2);
				break;
			case 0xe0:
				fprintf(stderr, "cmp a%d, d%d\n", lh2, ll2);
				break;
			case 0xf0:
				fprintf(stderr, "mov a%d, d%d\n", lh2, ll2);
				break;
			default:
				fprintf(stderr, "Undefined Instraction %x\n", op_code);
			}
			break;
		case 0x3:
			switch(m8 & 0xf0)
			{
			case 0x00:
				fprintf(stderr, "and d%d, d%d\n", lh2, ll2);
				break;
			case 0x10:
				fprintf(stderr, "or d%d, d%d\n", lh2, ll2);
				break;
			case 0x20:
				fprintf(stderr, "xor d%d, d%d\n", lh2, ll2);
				break;
			case 0x30:
				switch(m8 & 0xc)
				{
				case 0:
					fprintf(stderr, "rol d%d\n", ll2);
					break;
				case 4:
					fprintf(stderr, "ror d%d\n", ll2);
					break;
				case 8:
					fprintf(stderr, "asr d%d\n", ll2);
					break;
				case 0xc:
					fprintf(stderr, "lsr d%d\n", ll2);
					break;
				}
				break;
			case 0x40:
				fprintf(stderr, "mul d%d, d%d\n", lh2, ll2);
				break;
			case 0x50:
				fprintf(stderr, "mulu d%d, d%d\n", lh2, ll2);
				break;
			case 0x60:
				fprintf(stderr, "divu d%d, d%d\n", lh2, ll2);
				break;
			case 0x90:
				fprintf(stderr, "cmp d%d, d%d\n", lh2, ll2);
				break;
			case 0xc0:
				if(m8 == 0xc0 || m8 == 0xc4 || m8 == 0xc8 || m8 == 0xcc)
					fprintf(stderr, "mov d%d, mdr\n", lh2);
				else if(m8 == 0xc1 || m8 == 0xc5 || m8 == 0xc9 || m8 == 0xcd)
					fprintf(stderr, "ext d%d\n", lh2);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			case 0xd0:
				if(m8 == 0xd0 || m8 == 0xd4 || m8 == 0xd8 || m8 == 0xdc)
					fprintf(stderr, "mov d%d, psw\n", lh2);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			case 0xe0:
				switch(m8 & 0xc)
				{
				case 0:
					fprintf(stderr, "mov mdr, d%d\n", ll2);
					break;
				case 4:
					fprintf(stderr, "not d%d\n", ll2);
					break;
				default:
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
					break;
				}
				break;
			case 0xf0:
				if(m8 >= 0xf0 && m8 <= 0xf3)
					fprintf(stderr, "mov psw, d%d\n", ll2);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			default:
				fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			}
			break;
		case 0x4: // 0xf4
			pc++;
			op_code2=m8;
			m82=mem->readAbsolute8(pc);
			m162=mem->readAbsolute16(pc);
			m242=mem->readAbsolute24(pc);

			lh2=reg_index_low_high(op_code2); // m
			ll2=reg_index_low_low(op_code2); // n
			switch(op_code2 & 0xf0)
			{
			case 0x00:
				fprintf(stderr, "mov d%d, (0x%lx, a%d)\n", ll2, m242, lh2);
				break;
			case 0x10:
				fprintf(stderr, "mov a%d, (0x%lx, a%d)\n", ll2, m242, lh2);
				break;
			case 0x20:
				fprintf(stderr, "movb d%d, (0x%lx, a%d)\n", ll2, m242, lh2);
				break;
			case 0x30:
				fprintf(stderr, "movx d%d, (0x%lx, a%d)\n", ll2, m242, lh2);
				break;
			case 0x40:
				switch(op_code2 & 0xc)
				{
				case 0:
					fprintf(stderr, "mov d%d, (0x%lx)\n", ll2, m242);
					break;
				case 4:
					fprintf(stderr, "movb d%d, (0x%lx)\n", ll2, m242);
					break;
				default:
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				}
				break;
			case 0x50:
				if(op_code2 >= 0x50 && op_code2 <= 0x53)
					fprintf(stderr, "mov a%d, (0x%lx)\n", ll2, m242);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			case 0x60:
				switch(op_code2 & 0xc)
				{
				case 0:
					fprintf(stderr, "add 0x%lx, d%d\n", m242, ll2);
					break;
				case 4:
					fprintf(stderr, "add 0x%lx, a%d\n", m242, ll2);
					break;
				case 8:
					fprintf(stderr, "sub 0x%lx, d%d\n", m242, ll2);
					break;
				case 0xc:
					fprintf(stderr, "sub 0x%lx, a%d\n", m242, ll2);
					break;
				}
				break;
			case 0x70:
				switch(op_code2 & 0xc)
				{
				case 0:
					fprintf(stderr, "mov 0x%lx, d%d\n", m242, ll2);
					break;
				case 4:
					fprintf(stderr, "mov 0x%lx, a%d\n", m242, ll2);
					break;
				case 8:
					fprintf(stderr, "cmp 0x%lx, d%d\n", m242, ll2);
					break;
				case 0xc:
					fprintf(stderr, "cmp 0x%lx, a%d\n", m242, ll2);
					break;
				}
				break;
			case 0x80:
				fprintf(stderr, "mov (0x%lx, a%d), d%d\n", m242, lh2, ll2);
				break;
			case 0x90:
				fprintf(stderr, "movbu (0x%lx, a%d), d%d\n", m242, lh2, ll2);
				break;
			case 0xa0:
				fprintf(stderr, "movb (0x%lx, a%d), d%d\n", m242, lh2, ll2);
				break;
			case 0xb0:
				fprintf(stderr, "movx (0x%lx, a%d), d%d\n", m242, lh2, ll2);
				break;
			case 0xc0:
				switch(op_code2 & 0xc)
				{
				case 0:
					fprintf(stderr, "mov (0x%lx), d%d\n", m242, ll2);
					break;
				case 4:
					fprintf(stderr, "movb (0x%lx), d%d\n", m242, ll2);
					break;
				case 8:
					fprintf(stderr, "movbu (0x%lx), d%d\n", m242, ll2);
					break;
				case 0xc:
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
					break;
				}
				break;
			case 0xd0:
				if(op_code2 >= 0xd0 && op_code2 <= 0xd3)
					fprintf(stderr, "mov (0x%lx), a%d\n", m242, ll2);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			case 0xe0:
				switch(op_code2 & 0xf)
				{
				case 0:
					fprintf(stderr, "jmp 0x%lx\n", m242);
					break;
				case 1:
					fprintf(stderr, "jsr 0x%lx\n", m242);
					break;
				default:
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
					break;
				}
				break;
			case 0xf0:
				fprintf(stderr, "mov (0x%lx, a%d), a%d\n", m242, lh2, ll2);
				break;
			}
			break;


		case 0x5: // 0xf5
			pc++;
			op_code2=m8;
			lh2=reg_index_low_high(m8); // m
			ll2=reg_index_low_low(m8); // n
			m82=mem->readAbsolute8(pc);
			switch(op_code2 & 0xf0)
			{
			case 0x00:
				switch(op_code2 & 0xc)
				{
				case 0:
					fprintf(stderr, "and 0x%x, d%d\n", m82, ll2);
					break;
				case 4:
					fprintf(stderr, "btst 0x%x, d%d\n", m82, ll2);
					break;
				case 8:
					fprintf(stderr, "or 0x%x, d%d\n", m82, ll2);
					break;
				case 0xc:
					fprintf(stderr, "addnf 0x%x, a%d\n", m82, ll2);
					break;
				}
				break;
			case 0x10:
				fprintf(stderr, "movb d%d, (0x%x, a%d)\n", ll2, m82, lh2);
				break;
			case 0x20:
				fprintf(stderr, "movb (0x%x, a%d), d%d\n", m82, lh2, ll2);
				break;
			case 0x30:
				fprintf(stderr, "movbu (0x%x, a%d), d%d\n", m82, lh2, ll2);
				break;
			case 0x50:
				fprintf(stderr, "movx d%d, (0x%x, a%d)\n", ll2, m82, lh2);
				break;
			case 0x70:
				fprintf(stderr, "movx (0x%x, a%d), d%d\n", m82, lh2, ll2);
				break;
			case 0xe0:
			{
				static char *brax[]={
					"bltx", "bgtx", "bgex", "blex", "bcsx", "bhix", "bccx",
					"blsx", "beqx", "bnex", "undefined ins.", "undefined ins.", "bvcx", "bvsx", "bncx", "bnsx"};

				fprintf(stderr, "%s 0x%x\n", brax[op_code2 & 0xf], m82);
				break;
			}
			case 0xf0:
			{
				switch(op_code2 & 0xf)
				{
				case 0xc:
					fprintf(stderr, "bvc 0x%x\n", m82);
					break;
				case 0xd:
					fprintf(stderr, "bvs 0x%x\n", m82);
					break;
				case 0xe:
					fprintf(stderr, "bnc 0x%x\n", m82);
					break;
				case 0xf:
					fprintf(stderr, "bns 0x%x\n", m82);
					break;
				default:
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
					break;
				}
				break;
			}
			default:
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
			}
			break;
		case 0x6:
			fprintf(stderr, "nop \n");
			break;
		case 0x7:
		{
			pc++;
			op_code2=m8;
			lh2=reg_index_low_high(op_code2); // m
			ll2=reg_index_low_low(op_code2); // n
			m162=mem->readAbsolute16(pc);

			switch(op_code2 & 0xfC)
			{
			case 0:
				fprintf(stderr, "and 0x%x, d%d\n", m162, ll2);
				break;
			case 4:
				fprintf(stderr, "btst 0x%x, d%d\n", m162, ll2);
				break;
			case 8:
				fprintf(stderr, "add 0x%x, a%d\n", m162, ll2);
				break;
			case 0xc:
				fprintf(stderr, "sub 0x%x, a%d\n", m162, ll2);
				break;
			case 0x10:
				if(op_code2 == 0x10)
					fprintf(stderr, "and 0x%x, psw\n", m162);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			case 0x14:
				if(op_code2 == 0x14)
					fprintf(stderr, "or 0x%x, psw\n", m162);
				else
					fprintf(stderr, "Undefined Instraction %x\n", op_code);
				break;
			case 0x18:
				fprintf(stderr, "add 0x%x, d%d\n", m162, ll2);
				break;
			case 0x1C:
				fprintf(stderr, "sub 0x%x, d%d\n", m162, ll2);
				break;
			case 0x20:
				fprintf(stderr, "mov a%d, (0x%x)\n", ll2, m162);
				break;
			case 0x30:
				fprintf(stderr, "mov (0x%x), a%d\n", m162, ll2);
				break;
			case 0x40:
				fprintf(stderr, "or 0x%x, d%d\n", m162, ll2);
				break;
			case 0x48:
				fprintf(stderr, "cmp 0x%x, d%d\n", m162, ll2);
				break;
			case 0x4c:
				fprintf(stderr, "xor 0x%x, d%d\n", m162, ll2);
				break;
			case 0x50:
			case 0x54:
			case 0x58:
			case 0x5c:
				fprintf(stderr, "movbu (0x%x, a%d), d%d\n", m162, lh2, ll2);
				break;
			case 0x60:
			case 0x64:
			case 0x68:
			case 0x6c:
				fprintf(stderr, "movx d%d, (0x%x, a%d)\n", ll2, m162, lh2);
				break;
			case 0x70:
			case 0x74:
			case 0x78:
			case 0x7c:
				fprintf(stderr, "movx (0x%x, a%d), d%d\n", m162, lh2, ll2);
				break;
			case 0x80:
			case 0x84:
			case 0x88:
			case 0x8c:
				fprintf(stderr, "mov d%d, (0x%x, a%d)\n", ll2, m162, lh2);
				break;
			case 0x90:
			case 0x94:
			case 0x98:
			case 0x9c:
				fprintf(stderr, "movb d%d, (0x%x, a%d)\n", ll2, m162, lh2);
				break;
			case 0xa0:
			case 0xa4:
			case 0xa8:
			case 0xac:
				fprintf(stderr, "mov a%d, (0x%x, a%d)\n", ll2, m162, lh2);
				break;
			case 0xb0:
			case 0xb4:
			case 0xb8:
			case 0xbc:
				fprintf(stderr, "mov (0x%x, a%d), a%d\n", m162, lh2, ll2);
				break;
			case 0xc0:
			case 0xc4:
			case 0xc8:
			case 0xcc:
				fprintf(stderr, "mov (0x%x, a%d), d%d\n", m162, lh2, ll2);
				break;
			case 0xd0:
			case 0xd4:
			case 0xd8:
			case 0xdc:
				fprintf(stderr, "movb (0x%x, a%d), d%d\n", m162, lh2, ll2);
				break;
			default:
				fprintf(stderr, "Undefined Instraction %x\n", op_code);
			}
			break;
		}
		case 0x8:
		case 0x9:
		case 0xa:
		case 0xb:
			fprintf(stderr, "mov 0x%x, d%d\n", m16, ll);
			break;
		case 0xc:
			fprintf(stderr, "jmp 0x%x\n", m16);
			break;
		case 0xd:
			fprintf(stderr, "jsr 0x%x\n", m16);
			break;
		case 0xe:
			fprintf(stderr, "rts \n");
			break;
		default:
			fprintf(stderr, "Undefined Instraction %x\n", op_code);
		}
		break;
	}
	return ibyte;
}
