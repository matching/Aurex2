/*
 * mncore.cpp - Panasonic MN10200 CPU emulation module for a fetch, decode and exection.
 *
 * $Id: mncore.cpp,v 1.27 2006/01/26 06:56:22 fumi Exp $
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
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "option.h"
#include "memorycpu.h"
#include "interruption.h"
#include "ruputerexception.h"


/**
 *
 */
int Cpu::emulate_core(void)
{
	unsigned long effectiveAddress;
	unsigned char op_code;
	int c = cycle;

	{
#ifdef CPU_DEBUG

		printRegs();
//		printMnemonic(op_code, NULL);
		printMnemonic(NULL);
//		fputs(codeString, stderr);
		fputs("-----------------------------------------------\n", stderr);
		fflush(stderr);
		op_code=mem->readAbsolute8(regs.pc++);
#else
		decrementCycleForMemoryAccess(regs.pc, 1);
		op_code=mem->readAbsolute8(regs.pc++);
#endif
		switch(op_code)
		{
/* MOV Dm,(An)  Dm ¢ªmem16(An)---------1 1 00+An<<2+Dm */
		case 0x00:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			mem->write16(regs.a[0], regs.d[0]);
			break;
		case 0x01:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			mem->write16(regs.a[0], regs.d[1]);
			break;
		case 0x02:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			mem->write16(regs.a[0], regs.d[2]);
			break;
		case 0x03:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			mem->write16(regs.a[0], regs.d[3]);
			break;
		case 0x04:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			mem->write16(regs.a[1], regs.d[0]);
			break;
		case 0x05:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			mem->write16(regs.a[1], regs.d[1]);
			break;
		case 0x06:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			mem->write16(regs.a[1], regs.d[2]);
			break;
		case 0x07:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			mem->write16(regs.a[1], regs.d[3]);
			break;
		case 0x08:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			mem->write16(regs.a[2], regs.d[0]);
			break;
		case 0x09:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			mem->write16(regs.a[2], regs.d[1]);
			break;
		case 0x0a:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			mem->write16(regs.a[2], regs.d[2]);
			break;
		case 0x0b:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			mem->write16(regs.a[2], regs.d[3]);
			break;
		case 0x0c:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			mem->write16(regs.a[3], regs.d[0]);
			break;
		case 0x0d:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			mem->write16(regs.a[3], regs.d[1]);
			break;
		case 0x0e:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			mem->write16(regs.a[3], regs.d[2]);
			break;
		case 0x0f:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			mem->write16(regs.a[3], regs.d[3]);
			break;
/* MOVB Dm,(An)  Dm ¢ªmem8(An)---------1 1 10+Dm<<2+An
 * ¤¿¤Ö¤ó±³¤Ç¤¹¡£10+An<<2+Dm ¤¬¤¿¤Ö¤óÀµ²ò
 */
#if 1
	// gcc ÅÇ¤¯ÈÇ¡£¡Ê¥É¥­¥å¥á¥ó¥È¤È¤Ï°Û¤Ê¤ë¡Ë
		case 0x10:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			mem->write8(regs.a[0], regs.d[0]);
			break;
		case 0x11:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			mem->write8(regs.a[0], regs.d[1]);
			break;
		case 0x12:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			mem->write8(regs.a[0], regs.d[2]);
			break;
		case 0x13:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			mem->write8(regs.a[0], regs.d[3]);
			break;
		case 0x14:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			mem->write8(regs.a[1], regs.d[0]);
			break;
		case 0x15:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			mem->write8(regs.a[1], regs.d[1]);
			break;
		case 0x16:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			mem->write8(regs.a[1], regs.d[2]);
			break;
		case 0x17:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			mem->write8(regs.a[1], regs.d[3]);
			break;
		case 0x18:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			mem->write8(regs.a[2], regs.d[0]);
			break;
		case 0x19:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			mem->write8(regs.a[2], regs.d[1]);
			break;
		case 0x1a:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			mem->write8(regs.a[2], regs.d[2]);
			break;
		case 0x1b:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			mem->write8(regs.a[2], regs.d[3]);
			break;
		case 0x1c:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			mem->write8(regs.a[3], regs.d[0]);
			break;
		case 0x1d:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			mem->write8(regs.a[3], regs.d[1]);
			break;
		case 0x1e:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			mem->write8(regs.a[3], regs.d[2]);
			break;
		case 0x1f:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			mem->write8(regs.a[3], regs.d[3]);
			break;
#else
	// ¥É¥­¥å¥á¥ó¥ÈÄÌ¤êÈÇ
		case 0x10:
			mem->write8(regs.a[0], regs.d[0]);
			cycle--;
			break;
		case 0x11:
			mem->write8(regs.a[1], regs.d[0]);
			cycle--;
			break;
		case 0x12:
			mem->write8(regs.a[2], regs.d[0]);
			cycle--;
			break;
		case 0x13:
			mem->write8(regs.a[3], regs.d[0]);
			cycle--;
			break;
		case 0x14:
			mem->write8(regs.a[0], regs.d[1]);
			cycle--;
			break;
		case 0x15:
			mem->write8(regs.a[1], regs.d[1]);
			cycle--;
			break;
		case 0x16:
			mem->write8(regs.a[2], regs.d[1]);
			cycle--;
			break;
		case 0x17:
			mem->write8(regs.a[3], regs.d[1]);
			cycle--;
			break;
		case 0x18:
			mem->write8(regs.a[0], regs.d[2]);
			cycle--;
			break;
		case 0x19:
			mem->write8(regs.a[1], regs.d[2]);
			cycle--;
			break;
		case 0x1a:
			mem->write8(regs.a[2], regs.d[2]);
			cycle--;
			break;
		case 0x1b:
			mem->write8(regs.a[3], regs.d[2]);
			cycle--;
			break;
		case 0x1c:
			mem->write8(regs.a[0], regs.d[3]);
			cycle--;
			break;
		case 0x1d:
			mem->write8(regs.a[1], regs.d[3]);
			cycle--;
			break;
		case 0x1e:
			mem->write8(regs.a[2], regs.d[3]);
			cycle--;
			break;
		case 0x1f:
			mem->write8(regs.a[3], regs.d[3]);
			cycle--;
			break;
#endif
/************************** MOVB Dm, (An) *********************/
/* MOV (An),Dm mem16(An)¢ªDm S --------1 1 20+An<<2+Dm */
		case 0x20:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			regs.d[0]=mem->read16_se(regs.a[0]);
			break;
		case 0x21:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			regs.d[1]=mem->read16_se(regs.a[0]);
			break;
		case 0x22:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			regs.d[2]=mem->read16_se(regs.a[0]);
			break;
		case 0x23:
			decrementCycleForMemoryAccess(regs.a[0], 2);
			regs.d[3]=mem->read16_se(regs.a[0]);
			break;
		case 0x24:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			regs.d[0]=mem->read16_se(regs.a[1]);
			break;
		case 0x25:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			regs.d[1]=mem->read16_se(regs.a[1]);
			break;
		case 0x26:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			regs.d[2]=mem->read16_se(regs.a[1]);
			break;
		case 0x27:
			decrementCycleForMemoryAccess(regs.a[1], 2);
			regs.d[3]=mem->read16_se(regs.a[1]);
			break;
		case 0x28:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			regs.d[0]=mem->read16_se(regs.a[2]);
			break;
		case 0x29:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			regs.d[1]=mem->read16_se(regs.a[2]);
			break;
		case 0x2a:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			regs.d[2]=mem->read16_se(regs.a[2]);
			break;
		case 0x2b:
			decrementCycleForMemoryAccess(regs.a[2], 2);
			regs.d[3]=mem->read16_se(regs.a[2]);
			break;
		case 0x2c:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			regs.d[0]=mem->read16_se(regs.a[3]);
			break;
		case 0x2d:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			regs.d[1]=mem->read16_se(regs.a[3]);
			break;
		case 0x2e:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			regs.d[2]=mem->read16_se(regs.a[3]);
			break;
		case 0x2f:
			decrementCycleForMemoryAccess(regs.a[3], 2);
			regs.d[3]=mem->read16_se(regs.a[3]);
			break;
/* MOVBU (An),Dm mem8(An)¢ªDm 0 --------1 1 30+An<<2+Dm */
		case 0x30:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			regs.d[0]=mem->read8_0e(regs.a[0]);
			break;
		case 0x31:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			regs.d[1]=mem->read8_0e(regs.a[0]);
			break;
		case 0x32:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			regs.d[2]=mem->read8_0e(regs.a[0]);
			break;
		case 0x33:
			decrementCycleForMemoryAccess(regs.a[0], 1);
			regs.d[3]=mem->read8_0e(regs.a[0]);
			break;
		case 0x34:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			regs.d[0]=mem->read8_0e(regs.a[1]);
			break;
		case 0x35:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			regs.d[1]=mem->read8_0e(regs.a[1]);
			break;
		case 0x36:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			regs.d[2]=mem->read8_0e(regs.a[1]);
			break;
		case 0x37:
			decrementCycleForMemoryAccess(regs.a[1], 1);
			regs.d[3]=mem->read8_0e(regs.a[1]);
			break;
		case 0x38:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			regs.d[0]=mem->read8_0e(regs.a[2]);
			break;
		case 0x39:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			regs.d[1]=mem->read8_0e(regs.a[2]);
			break;
		case 0x3a:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			regs.d[2]=mem->read8_0e(regs.a[2]);
			break;
		case 0x3b:
			decrementCycleForMemoryAccess(regs.a[2], 1);
			regs.d[3]=mem->read8_0e(regs.a[2]);
			break;
		case 0x3c:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			regs.d[0]=mem->read8_0e(regs.a[3]);
			break;
		case 0x3d:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			regs.d[1]=mem->read8_0e(regs.a[3]);
			break;
		case 0x3e:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			regs.d[2]=mem->read8_0e(regs.a[3]);
			break;
		case 0x3f:
			decrementCycleForMemoryAccess(regs.a[3], 1);
			regs.d[3]=mem->read8_0e(regs.a[3]);
			break;
/* MOV Dm,(d8,An) Dm ¢ªmem16(An+d8)---------2 1 40+An<<2+Dm:d8 */
		case 0x40:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[0]);
			break;
		case 0x41:
			effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[1]);
			break;
		case 0x42:
			effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[2]);
			break;
		case 0x43:
			effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[3]);
			break;
		case 0x44:
			effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[0]);
			break;
		case 0x45:
			effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[1]);
			break;
		case 0x46:
			effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[2]);
			break;
		case 0x47:
			effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[3]);
			break;
		case 0x48:
			effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[0]);
			break;
		case 0x49:
			effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[1]);
			break;
		case 0x4a:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[2]);
			cycle--;
			break;
		case 0x4b:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[3]);
			break;
		case 0x4c:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[0]);
			break;
		case 0x4d:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[1]);
			break;
		case 0x4e:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[2]);
			break;
		case 0x4f:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[3]);
			break;
/* MOV Am,(d8,An)Am ¢ªmem24(An+d8)---------2 2 50+An<<2+Am:d8 */
		case 0x50:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[0]);
			break;
		case 0x51:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[1]);
			break;
		case 0x52:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[2]);
			break;
		case 0x53:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[3]);
			break;
		case 0x54:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[0]);
			break;
		case 0x55:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[1]);
			break;
		case 0x56:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[2]);
			break;
		case 0x57:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[3]);
			break;
		case 0x58:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[0]);
			break;
		case 0x59:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[1]);
			break;
		case 0x5a:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[2]);
			break;
		case 0x5b:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[3]);
			break;
		case 0x5c:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[0]);
			break;
		case 0x5d:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[1]);
			break;
		case 0x5e:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[2]);
			break;
		case 0x5f:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			mem->write24(effectiveAddress, regs.a[3]);
			break;
/* MOV (d8,An),Dm mem16(An+d8)¢ªDm S --------2 1 60+An<<2+Dm:d8 */
		case 0x60:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[0]=mem->read16_se(effectiveAddress);
			break;
		case 0x61:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[1]=mem->read16_se(effectiveAddress);
			break;
		case 0x62:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[2]=mem->read16_se(effectiveAddress);
			break;
		case 0x63:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[3]=mem->read16_se(effectiveAddress);
			break;
		case 0x64:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[0]=mem->read16_se(effectiveAddress);
			break;
		case 0x65:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[1]=mem->read16_se(effectiveAddress);
			break;
		case 0x66:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[2]=mem->read16_se(effectiveAddress);
			break;
		case 0x67:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[3]=mem->read16_se(effectiveAddress);
			break;
		case 0x68:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[0]=mem->read16_se(effectiveAddress);
			break;
		case 0x69:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[1]=mem->read16_se(effectiveAddress);
			break;
		case 0x6a:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[2]=mem->read16_se(effectiveAddress);
			break;
		case 0x6b:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[3]=mem->read16_se(effectiveAddress);
			break;
		case 0x6c:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[0]=mem->read16_se(effectiveAddress);
			break;
		case 0x6d:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[1]=mem->read16_se(effectiveAddress);
			break;
		case 0x6e:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[2]=mem->read16_se(effectiveAddress);
			break;
		case 0x6f:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[3]=mem->read16_se(effectiveAddress);
			break;
/* MOV (d8,An),Am mem24(An+d8)¢ªAm ---------2 2 70+An<<2+Am:d8 */
		case 0x70:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[0]=mem->read24(effectiveAddress);
			break;
		case 0x71:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[1]=mem->read24(effectiveAddress);
			break;
		case 0x72:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[2]=mem->read24(effectiveAddress);
			break;
		case 0x73:
			effectiveAddress = regs.a[0] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[3]=mem->read24(effectiveAddress);
			break;
		case 0x74:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[0]=mem->read24(effectiveAddress);
			break;
		case 0x75:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[1]=mem->read24(effectiveAddress);
			break;
		case 0x76:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[2]=mem->read24(effectiveAddress);
			break;
		case 0x77:
			effectiveAddress = regs.a[1] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[3]=mem->read24(effectiveAddress);
			break;
		case 0x78:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[0]=mem->read24(effectiveAddress);
			break;
		case 0x79:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[1]=mem->read24(effectiveAddress);
			break;
		case 0x7a:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[2]=mem->read24(effectiveAddress);
			break;
		case 0x7b:
			effectiveAddress = regs.a[2] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[3]=mem->read24(effectiveAddress);
			break;
		case 0x7c:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[0]=mem->read24(effectiveAddress);
			break;
		case 0x7d:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[1]=mem->read24(effectiveAddress);
			break;
		case 0x7e:
			effectiveAddress = regs.a[3] + mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[2]=mem->read24(effectiveAddress);
			break;
		case 0x7f:
			effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			regs.a[3]=mem->read24(effectiveAddress);
			break;
/* MOV Dn,Dm     Dn ¢ªDm ---------1 1 80+Dn<<2+Dm
 * MOV imm8,Dn imm8 ¢ªDn S --------2 1 80+Dn<<2+Dn:imm8
 */
		case 0x80:
			regs.d[0]=mem->readImmediate8(regs.pc++);
			cycle--;
			break;
		case 0x81:
			regs.d[1]=regs.d[0];
			cycle--;
			break;
		case 0x82:
			regs.d[2]=regs.d[0];
			cycle--;
			break;
		case 0x83:
			regs.d[3]=regs.d[0];
			cycle--;
			break;
		case 0x84:
			regs.d[0]=regs.d[1];
			cycle--;
			break;
		case 0x85:
			regs.d[1]=mem->readImmediate8(regs.pc++);
			cycle--;
			break;
		case 0x86:
			regs.d[2]=regs.d[1];
			cycle--;
			break;
		case 0x87:
			regs.d[3]=regs.d[1];
			cycle--;
			break;
		case 0x88:
			regs.d[0]=regs.d[2];
			cycle--;
			break;
		case 0x89:
			regs.d[1]=regs.d[2];
			cycle--;
			break;
		case 0x8a:
			regs.d[2]=mem->readImmediate8(regs.pc++);
			cycle--;
			break;
		case 0x8b:
			regs.d[3]=regs.d[2];
			cycle--;
			break;
		case 0x8c:
			regs.d[0]=regs.d[3];
			cycle--;
			break;
		case 0x8d:
			regs.d[1]=regs.d[3];
			cycle--;
			break;
		case 0x8e:
			regs.d[2]=regs.d[3];
			cycle--;
			break;
		case 0x8f:
			regs.d[3]=mem->readImmediate8(regs.pc++);
			cycle--;
			break;
/* ADD Dn,Dm Dm+Dn ¢ªDm -oooooooo1 1 90+Dn<<2+Dm */
		case 0x90:
			regs.d[0]=add(regs.d[0], regs.d[0]);
			cycle--;
			break;
		case 0x91:
			regs.d[1]=add(regs.d[1], regs.d[0]);
			cycle--;
			break;
		case 0x92:
			regs.d[2]=add(regs.d[2], regs.d[0]);
			cycle--;
			break;
		case 0x93:
			regs.d[3]=add(regs.d[3], regs.d[0]);
			cycle--;
			break;
		case 0x94:
			regs.d[0]=add(regs.d[0], regs.d[1]);
			cycle--;
			break;
		case 0x95:
			regs.d[1]=add(regs.d[1], regs.d[1]);
			cycle--;
			break;
		case 0x96:
			regs.d[2]=add(regs.d[2], regs.d[1]);
			cycle--;
			break;
		case 0x97:
			regs.d[3]=add(regs.d[3], regs.d[1]);
			cycle--;
			break;
		case 0x98:
			regs.d[0]=add(regs.d[0], regs.d[2]);
			cycle--;
			break;
		case 0x99:
			regs.d[1]=add(regs.d[1], regs.d[2]);
			cycle--;
			break;
		case 0x9a:
			regs.d[2]=add(regs.d[2], regs.d[2]);
			cycle--;
			break;
		case 0x9b:
			regs.d[3]=add(regs.d[3], regs.d[2]);
			cycle--;
			break;
		case 0x9c:
			regs.d[0]=add(regs.d[0], regs.d[3]);
			cycle--;
			break;
		case 0x9d:
			regs.d[1]=add(regs.d[1], regs.d[3]);
			cycle--;
			break;
		case 0x9e:
			regs.d[2]=add(regs.d[2], regs.d[3]);
			cycle--;
			break;
		case 0x9f:
			regs.d[3]=add(regs.d[3], regs.d[3]);
			cycle--;
			break;
/* SUB Dn,Dm Dm- Dn ¢ªDm -oooooooo1 1 A0+Dn<<2+Dm */
		case 0xa0:
			regs.d[0]=0;
			regs.psw.row=ZF|ZX;
			cycle--;
			break;
		case 0xa1:
			regs.d[1]=sub(regs.d[1], regs.d[0]);
			cycle--;
			break;
		case 0xa2:
			regs.d[2]=sub(regs.d[2], regs.d[0]);
			cycle--;
			break;
		case 0xa3:
			regs.d[3]=sub(regs.d[3], regs.d[0]);
			cycle--;
			break;
		case 0xa4:
			regs.d[0]=sub(regs.d[0], regs.d[1]);
			cycle--;
			break;
		case 0xa5:
			regs.d[1]=0;
			regs.psw.row=ZF|ZX;
			cycle--;
			break;
		case 0xa6:
			regs.d[2]=sub(regs.d[2], regs.d[1]);
			cycle--;
			break;
		case 0xa7:
			regs.d[3]=sub(regs.d[3], regs.d[1]);
			cycle--;
			break;
		case 0xa8:
			regs.d[0]=sub(regs.d[0], regs.d[2]);
			cycle--;
			break;
		case 0xa9:
			regs.d[1]=sub(regs.d[1], regs.d[2]);
			cycle--;
			break;
		case 0xaa:
			regs.d[2]=0;
			regs.psw.row=ZF|ZX;
			cycle--;
			break;
		case 0xab:
			regs.d[3]=sub(regs.d[3], regs.d[2]);
			cycle--;
			break;
		case 0xac:
			regs.d[0]=sub(regs.d[0], regs.d[3]);
			cycle--;
			break;
		case 0xad:
			regs.d[1]=sub(regs.d[1], regs.d[3]);
			cycle--;
			break;
		case 0xae:
			regs.d[2]=sub(regs.d[2], regs.d[3]);
			cycle--;
			break;
		case 0xaf:
			regs.d[3]=0;
			regs.psw.row=ZF|ZX;
			cycle--;
			break;

/* EXTX Dn Dn.bp15=0 ¤Ê¤é¤Ð    S --------1 1 B0+Dn
 *           Dn&x'00FFFF'¢ªDn
 *         Dn.bp15=1 ¤Ê¤é¤Ð
 *           Dn| x'FF0000'¢ªDn
 */
		case 0xb0:
			regs.d[0]=mem->signExtend16(regs.d[0]);
			cycle--;
			break;
		case 0xb1:
			regs.d[1]=mem->signExtend16(regs.d[1]);
			cycle--;
			break;
		case 0xb2:
			regs.d[2]=mem->signExtend16(regs.d[2]);
			cycle--;
			break;
		case 0xb3:
			regs.d[3]=mem->signExtend16(regs.d[3]);
			cycle--;
			break;
/* EXTXU Dn Dn&x'00FFFF'¢ªDn 0 --------1 1 B4+Dn */
		case 0xb4:
			regs.d[0]&=0xffff;
			cycle--;
			break;
		case 0xb5:
			regs.d[1]&=0xffff;
			cycle--;
			break;
		case 0xb6:
			regs.d[2]&=0xffff;
			cycle--;
			break;
		case 0xb7:
			regs.d[3]&=0xffff;
			cycle--;
			break;

/* EXTXB Dn Dn.bp7=0 ¤Ê¤é¤ÐS --------1 1 B8+Dn *9 64
 *            Dn&x'0000FF'¢ªDn
 *          Dn.bp7=1 ¤Ê¤é¤Ð
 *            Dn| x'FFFF00'¢ªDn
 */
		case 0xb8:
			regs.d[0]=mem->signExtend8(regs.d[0]);
			cycle--;
			break;
		case 0xb9:
			regs.d[1]=mem->signExtend8(regs.d[1]);
			cycle--;
			break;
		case 0xba:
			regs.d[2]=mem->signExtend8(regs.d[2]);
			cycle--;
			break;
		case 0xbb:
			regs.d[3]=mem->signExtend8(regs.d[3]);
			cycle--;
			break;
/* EXTXBU Dn Dn&x'0000FF'¢ªDn 0 --------1 1 BC+Dn */
		case 0xbc:
			regs.d[0]&=0xff;
			cycle--;
			break;
		case 0xbd:
			regs.d[1]&=0xff;
			cycle--;
			break;
		case 0xbe:
			regs.d[2]&=0xff;
			cycle--;
			break;
		case 0xbf:
			regs.d[3]&=0xff;
			cycle--;
			break;
/* MOV Dn,(abs16) Dn ¢ªmem16(abs16)---------3 1 C0+Dn:abs16-l:abs16-h */
		case 0xc0:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[0]);
			break;
		case 0xc1:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[1]);
			break;
		case 0xc2:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[2]);
			break;
		case 0xc3:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 2);
			mem->write16(effectiveAddress, regs.d[3]);
			break;
/* MOVB Dn,(abs16) Dn ¢ªmem8(abs16)---------3 1 C4+Dn:abs16- l:abs16- h */
		case 0xc4:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 1);
			mem->write8(effectiveAddress, regs.d[0]);
			break;
		case 0xc5:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 1);
			mem->write8(effectiveAddress, regs.d[1]);
			break;
		case 0xc6:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 1);
			mem->write8(effectiveAddress, regs.d[2]);
			break;
		case 0xc7:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			decrementCycleForMemoryAccess(effectiveAddress, 1);

			mem->write8(effectiveAddress, regs.d[3]);
			break;
/* MOV (abs16),Dn mem16(abs16)¢ªDn S --------3 1 C8+Dn:abs16- l:abs16- h */
		case 0xc8:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[0]=mem->read16_se(effectiveAddress);
			break;
		case 0xc9:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[1]=mem->read16_se(effectiveAddress);
			break;
		case 0xca:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[2]=mem->read16_se(effectiveAddress);
			break;
		case 0xcb:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 2);
			regs.d[3]=mem->read16_se(effectiveAddress);
			break;
/* MOVBU (abs16),Dn mem8(abs16)¢ªDn 0 --------3 1 CC+Dn:abs16- l:abs16- h */
		case 0xcc:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 1);
			regs.d[0]=mem->read8_0e(effectiveAddress);
			break;
		case 0xcd:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 1);
			regs.d[1]=mem->read8_0e(effectiveAddress);
			break;
		case 0xce:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 1);
			regs.d[2]=mem->read8_0e(effectiveAddress);
			break;
		case 0xcf:
			effectiveAddress = mem->readAbsolute16(regs.pc);
			regs.pc+=2;

			decrementCycleForMemoryAccess(effectiveAddress, 1);
			regs.d[3]=mem->read8_0e(effectiveAddress);
			break;

/* ADD imm8,An An+imm8 ¢ªAn S oooooooo2 1 D0+An:imm8 */
		case 0xd0:
			regs.a[0]=add(regs.a[0], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd1:
			regs.a[1]=add(regs.a[1], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd2:
			regs.a[2]=add(regs.a[2], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd3:
			regs.a[3]=add(regs.a[3], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
/* ADD imm8,Dn Dn+imm8 ¢ªDn S oooooooo2 1 D4+Dn:imm8 */
		case 0xd4:
			regs.d[0]=add(regs.d[0], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd5:
			regs.d[1]=add(regs.d[1], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd6:
			regs.d[2]=add(regs.d[2], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd7:
			regs.d[3]=add(regs.d[3], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
/* CMP imm8,Dn Dn- imm8 S oooooooo2 1 D8+Dn:imm8 */
		case 0xd8:
			sub(regs.d[0], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xd9:
			sub(regs.d[1], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xda:
			sub(regs.d[2], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
		case 0xdb:
			sub(regs.d[3], mem->readImmediate8(regs.pc++));
			cycle--;
			break;
/* MOV imm16,An imm16 ¢ªAn 0 --------3 1 DC+An:imm16- l:imm16- h */
		case 0xdc:
			regs.a[0]=mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
		case 0xdd:
			regs.a[1]=mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
		case 0xde:
			regs.a[2]=mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
		case 0xdf:
			regs.a[3]=mem->readAbsolute16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
/* BLT label (VF^ NF)=1 ¤Ê¤é¤Ð---------2 2/1 E0:d8 *19 100 */
		case 0xe0:
			if(regs.psw.bit.vf ^ regs.psw.bit.nf)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BGT label ((VF^ NF)|ZF)=0 ¤Ê¤é¤Ð---------2 2/1 E1:d8 *22 101 */
		case 0xe1:
			if(((regs.psw.bit.vf ^ regs.psw.bit.nf) | regs.psw.bit.zf) == 0)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BGE label (VF^ NF)=0 ¤Ê¤é¤Ð---------2 2/1 E2:d8 *21 101 */
		case 0xe2:
			if((regs.psw.bit.vf ^ regs.psw.bit.nf) == 0)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BLE label ((VF^ NF)|ZF)=1 ¤Ê¤é¤Ð---------2 2/1 E3:d8 */
		case 0xe3:
			if((regs.psw.bit.vf ^ regs.psw.bit.nf) | regs.psw.bit.zf)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BCS label CF=1 ¤Ê¤é¤Ð---------2 2/1 E4:d8 */
		case 0xe4:
			if(regs.psw.bit.cf)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BHI label (CF|ZF)=0 ¤Ê¤é¤Ð---------2 2/1 E5:d8 *26 103 */
		case 0xe5:
			if((regs.psw.bit.cf | regs.psw.bit.zf) == 0)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BCC label CF=0 ¤Ê¤é¤Ð---------2 2/1 E6:d8 */
		case 0xe6:
			if(regs.psw.bit.cf == 0)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BLS label (CF|ZF)=1 ¤Ê¤é¤Ð---------2 2/1 E7:d8 */
		case 0xe7:
			if(regs.psw.bit.cf | regs.psw.bit.zf)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BEQ label ZF=1 ¤Ê¤é¤Ð---------2 2/1 E8:d8 */
		case 0xe8:
			if(regs.psw.bit.zf)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
/* BNE label ZF=0 ¤Ê¤é¤Ð---------2 2/1 E9:d8 */
		case 0xe9:
			if(regs.psw.bit.zf == 0)
			{
				cycle--;
				regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			}
			cycle--;
			regs.pc++;
			break;
		case 0xea:
/* BRA label PC+2+d8(label)¢ªPC ---------2 2 EA:d8 */
			regs.pc = regs.pc + mem->readDisplacement8(regs.pc);
			cycle-=2;
			regs.pc++;
			break;
		case 0xeb:
/* RTI mem16(A3)¢ªPSW -oooooooo1 6EB
 *     mem24(A3+2)¢ªPC
 *     A3+6 ¢ªA3
 */
			decrementCycleForMemoryAccess(regs.a[3], 2);
			decrementCycleForMemoryAccess(regs.a[3] + 2, 3);
			cycle -= 3;

			regs.psw.row = mem->readAbsolute16(regs.a[3]);
			regs.pc = mem->read24(regs.a[3]+2);
			regs.a[3]+=6;
			break;
/* CMP imm16,An An- imm16 0 oooooooo3 1 EC+An:imm16- l:imm16- h */
		case 0xec:
			sub(regs.a[0], mem->readImmediate16(regs.pc));
			cycle--;
			regs.pc+=2;
			break;
		case 0xed:
			sub(regs.a[1], mem->readImmediate16(regs.pc));
			cycle--;
			regs.pc+=2;
			break;
		case 0xee:
			sub(regs.a[2], mem->readImmediate16(regs.pc));
			cycle--;
			regs.pc+=2;
			break;
		case 0xef:
			sub(regs.a[3], mem->readImmediate16(regs.pc));
			cycle--;
			regs.pc+=2;
			break;

/************************************************************************/
		case 0xf0:	/* £²¥Ð¥¤¥ÈÌ¿Îá */
			op_code=mem->readAbsolute8(regs.pc++);

			switch(op_code)
			{
			case 0x02:
			case 0x03:
			case 0x06:
			case 0x07:
			case 0x0a:
			case 0x0b:
			case 0x0e:
			case 0x0f:
			case 0x10:
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x14:
			case 0x15:
			case 0x16:
			case 0x17:
			case 0x18:
			case 0x19:
			case 0x1a:
			case 0x1b:
			case 0x1c:
			case 0x1d:
			case 0x1e:
			case 0x1f:
				undefined_instruction();
				break;

/* JSR (An)  A3-4 ¢ªA3 ---------2 5 F0:01+An<<2
 *           PC+2 ¢ªmem24(A3)
 *           An ¢ªPC
 * JMP (an) 3 cycle
 */
			case 0x00:
				regs.pc=regs.a[0];
				cycle-=3;
				break;
			case 0x01:
				switch(interruption->execSystemCall(&regs, &regs.a[0]))
				{
				case 1:
			/* ¥¢¥É¥ì¥¹¤ÎÃÍ¤ò¸µ¤ËÌá¤¹ */
					regs.a[0]=mem->read24(regs.a[3]);
					regs.a[3]+=4;
					cycle-=5;
					break;
				case 0:
					regs.a[3]-=4;
					mem->write24(regs.a[3], regs.pc);
					regs.pc=regs.a[0];

					decrementCycleForMemoryAccess(regs.a[3], 3);
					cycle-=3;

					break;
				}
				break;
			case 0x04:
				regs.pc=regs.a[1];
				cycle-=3;
				break;
			case 0x05:
				switch(interruption->execSystemCall(&regs, &regs.a[1]))
				{
				case 1:
			/* ¥¢¥É¥ì¥¹¤ÎÃÍ¤ò¸µ¤ËÌá¤¹ */
					regs.a[1]=mem->read24(regs.a[3]);
					regs.a[3]+=4;
					cycle-=5;
					break;
				case 0:
					regs.a[3]-=4;
					mem->write24(regs.a[3], regs.pc);
					regs.pc=regs.a[1];

					decrementCycleForMemoryAccess(regs.a[3], 3);
					cycle-=3;

					break;
				}
				break;
			case 0x08:
				regs.pc=regs.a[2];
				cycle-=3;
				break;
			case 0x09:
				switch(interruption->execSystemCall(&regs, &regs.a[2]))
				{
				case 1:
			/* ¥¢¥É¥ì¥¹¤ÎÃÍ¤ò¸µ¤ËÌá¤¹ */
					regs.a[2]=mem->read24(regs.a[3]);
					regs.a[3]+=4;
					cycle-=5;
					break;
				case 0:
					regs.a[3]-=4;
					mem->write24(regs.a[3], regs.pc);
					regs.pc=regs.a[2];
					decrementCycleForMemoryAccess(regs.a[3], 3);
					cycle-=3;
					break;
				}
				break;
			case 0x0c:
				regs.pc=regs.a[3];
				cycle-=3;
				break;
			case 0x0d:
				switch(interruption->execSystemCall(&regs, &regs.a[3]))
				{
				case 1:
			/* ¥¢¥É¥ì¥¹¤ÎÃÍ¤ò¸µ¤ËÌá¤¹ */
					regs.a[3]=mem->read24(regs.a[3]);
					regs.a[3]+=4;
					cycle-=5;
					break;
				case 0:
					regs.a[3]-=4;
					mem->write24(regs.a[3], regs.pc);
					regs.pc=regs.a[3];
					decrementCycleForMemoryAccess(regs.a[3], 3);
					cycle-=3;
					break;
				}
				break;

/* BSET Dm,(An) mem8(An)&Dm ... PSW 0 ----0 0 0 o2 5 F0:20+An<<2+Dm *16 97
 *              mem8(An)|Dm ¢ªmem8(An)
 */
			case 0x20:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bset(regs.d[0], regs.a[0]);
				break;
			case 0x21:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bset(regs.d[1], regs.a[0]);
				break;
			case 0x22:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bset(regs.d[2], regs.a[0]);
				break;
			case 0x23:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bset(regs.d[3], regs.a[0]);
				break;
			case 0x24:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bset(regs.d[0], regs.a[1]);
				break;
			case 0x25:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bset(regs.d[1], regs.a[1]);
				break;
			case 0x26:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bset(regs.d[2], regs.a[1]);
				break;
			case 0x27:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bset(regs.d[3], regs.a[1]);
				break;
			case 0x28:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bset(regs.d[0], regs.a[2]);
				break;
			case 0x29:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bset(regs.d[1], regs.a[2]);
				break;
			case 0x2a:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bset(regs.d[2], regs.a[2]);
				break;
			case 0x2b:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bset(regs.d[3], regs.a[2]);
				break;
			case 0x2c:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bset(regs.d[0], regs.a[3]);
				break;
			case 0x2d:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bset(regs.d[1], regs.a[3]);
				break;
			case 0x2e:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bset(regs.d[2], regs.a[3]);
				break;
			case 0x2f:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bset(regs.d[3], regs.a[3]);
				break;

/* BCLR Dm,(An) mem8(An)&Dm ... PSW 0 ----0 0 0 o2 5 F0:30+An<<2+Dm *16 98
 *              mem8(An)&(~Dm)¢ªmem8(An)
 */
			case 0x30:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bclr(regs.d[0], regs.a[0]);
				break;
			case 0x31:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bclr(regs.d[1], regs.a[0]);
				break;
			case 0x32:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bclr(regs.d[2], regs.a[0]);
				break;
			case 0x33:
				decrementCycleForMemoryAccess(regs.a[0], 1);
				decrementCycleForMemoryAccess(regs.a[0], 1);
				cycle -= 3;
				bclr(regs.d[3], regs.a[0]);
				break;
			case 0x34:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bclr(regs.d[0], regs.a[1]);
				break;
			case 0x35:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bclr(regs.d[1], regs.a[1]);
				break;
			case 0x36:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bclr(regs.d[2], regs.a[1]);
				break;
			case 0x37:
				decrementCycleForMemoryAccess(regs.a[1], 1);
				decrementCycleForMemoryAccess(regs.a[1], 1);
				cycle -= 3;
				bclr(regs.d[3], regs.a[1]);
				break;
			case 0x38:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bclr(regs.d[0], regs.a[2]);
				break;
			case 0x39:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bclr(regs.d[1], regs.a[2]);
				break;
			case 0x3a:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bclr(regs.d[2], regs.a[2]);
				break;
			case 0x3b:
				decrementCycleForMemoryAccess(regs.a[2], 1);
				decrementCycleForMemoryAccess(regs.a[2], 1);
				cycle -= 3;
				bclr(regs.d[3], regs.a[2]);
				break;
			case 0x3c:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bclr(regs.d[0], regs.a[3]);
				break;
			case 0x3d:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bclr(regs.d[1], regs.a[3]);
				break;
			case 0x3e:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bclr(regs.d[2], regs.a[3]);
				break;
			case 0x3f:
				decrementCycleForMemoryAccess(regs.a[3], 1);
				decrementCycleForMemoryAccess(regs.a[3], 1);
				cycle -= 3;
				bclr(regs.d[3], regs.a[3]);
				break;

/* MOVB (Di,An),Dm mem8(An+Di)¢ªDm S --------2 2 F0:40+Di<<4+An<<2+Dm */

		/* ¥¢¥É¥ì¥¹·×»»¤¬ÈùÌ¯¤¸¤ã¤Ê¤¤¤«¡© */
			case 0x40:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x41:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(regs.a[0]+regs.d[0]);
				break;
			case 0x42:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(regs.a[0]+regs.d[0]);
				break;
			case 0x43:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(regs.a[0]+regs.d[0]);
				break;
			case 0x44:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x45:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x46:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x47:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x48:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x49:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x4a:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x4b:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x4c:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x4d:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x4e:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x4f:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x50:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x51:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x52:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x53:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x54:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x55:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x56:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x57:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x58:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x59:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x5a:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x5b:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x5c:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x5d:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x5e:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x5f:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x60:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x61:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x62:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x63:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x64:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x65:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x66:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x67:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x68:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x69:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x6a:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x6b:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x6c:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x6d:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x6e:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x6f:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x70:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x71:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x72:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x73:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x74:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x75:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x76:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x77:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x78:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x79:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x7a:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x7b:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x7c:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x7d:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x7e:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x7f:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;

/* MOVBU (Di,An),Dm mem8(An+Di)¢ªDm 0 --------2 2 F0:80+Di<<4+An<<2+Dm */
			case 0x80:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x81:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x82:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x83:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x84:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x85:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x86:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x87:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x88:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x89:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x8a:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x8b:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x8c:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x8d:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x8e:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x8f:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x90:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x91:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x92:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x93:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x94:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x95:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x96:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x97:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x98:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x99:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9a:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9b:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9c:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9d:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9e:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9f:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa0:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa1:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa2:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa3:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa4:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa5:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa6:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa7:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa8:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xa9:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xaa:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xab:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xac:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xad:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xae:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xaf:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb0:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb1:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb2:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb3:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb4:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb5:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb6:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb7:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb8:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xb9:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xba:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xbb:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0xbc:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xbd:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xbe:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xbf:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;

		/* MOVB Dm,(Di,An) Dm ¢ªmem8(An+Di)---------2 2 F0:C0+Di<<4+An<<2+Dm */
			case 0xc0:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xc1:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xc2:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xc3:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xc4:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xc5:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xc6:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xc7:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xc8:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xc9:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xca:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xcb:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xcc:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xcd:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xce:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xcf:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xd0:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xd1:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xd2:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xd3:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xd4:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xd5:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xd6:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xd7:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xd8:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xd9:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xda:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xdb:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xdc:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xdd:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xde:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xdf:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xe0:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xe1:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xe2:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xe3:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xe4:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xe5:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xe6:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xe7:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xe8:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xe9:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xea:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xeb:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xec:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xed:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xee:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xef:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xf0:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xf1:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xf2:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xf3:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xf4:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xf5:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xf6:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xf7:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xf8:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xf9:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xfa:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xfb:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0xfc:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0xfd:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0xfe:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0xff:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			}

			break;
/**************************************************************/
		case 0xf1:
			op_code = mem->readAbsolute8(regs.pc++);

			switch(op_code)
			{
/* MOV (Di,An),Am mem24(An+Di)¢ªAm ---------2 3 F1:00+Di<<4+An<<2+Am 35 */
			case 0x00:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x01:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x02:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x03:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x04:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x05:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x06:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x07:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x08:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x09:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x0a:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x0b:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x0c:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x0d:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x0e:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x0f:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x10:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x11:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x12:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x13:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x14:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x15:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x16:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x17:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x18:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x19:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x1a:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x1b:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x1c:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x1d:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x1e:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x1f:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x20:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x21:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x22:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x23:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x24:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x25:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x26:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x27:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x28:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x29:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x2a:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x2b:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x2c:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x2d:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x2e:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x2f:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x30:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x31:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x32:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x33:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x34:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x35:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x36:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x37:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x38:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x39:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x3a:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x3b:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0x3c:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x3d:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x3e:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x3f:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.a[3]=mem->read24(effectiveAddress);
				break;

/* MOV (Di,An),Dm mem16(An+Di)¢ªDm S --------2 2 F1:40+Di<<4+An<<2+Dm */
			case 0x40:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x41:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x42:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x43:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x44:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x45:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x46:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x47:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x48:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x49:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x4a:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x4b:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x4c:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x4d:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x4e:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x4f:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x50:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x51:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x52:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x53:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x54:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x55:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x56:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x57:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x58:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x59:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x5a:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x5b:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x5c:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x5d:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x5e:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x5f:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x60:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x61:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x62:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x63:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x64:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x65:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x66:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x67:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x68:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x69:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x6a:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x6b:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x6c:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x6d:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x6e:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x6f:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x70:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x71:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x72:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x73:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x74:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x75:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x76:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x77:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x78:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x79:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x7a:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x7b:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x7c:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x7d:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x7e:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x7f:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
/* MOV Am,(Di,An)Am ¢ªmem24(An+Di)---------2 3 F1:80+Di<<4+An<<2+Am 42 */
			case 0x80:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x81:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x82:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x83:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x84:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x85:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x86:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x87:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x88:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x89:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x8a:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x8b:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x8c:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x8d:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x8e:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x8f:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x90:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x91:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x92:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x93:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x94:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x95:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x96:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x97:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x98:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x99:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x9a:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x9b:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x9c:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x9d:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x9e:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x9f:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xa0:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xa1:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xa2:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xa3:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xa4:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xa5:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xa6:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xa7:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xa8:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xa9:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xaa:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xab:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xac:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xad:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xae:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xaf:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xb0:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xb1:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xb2:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xb3:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xb4:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xb5:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xb6:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xb7:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xb8:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xb9:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xba:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xbb:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xbc:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xbd:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xbe:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xbf:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
/* MOV Dm,(Di,An)Dm ¢ªmem16(An+Di)---------2 2 F1:C0+Di<<4+An<<2+Dm */
			case 0xc0:
			     	effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xc1:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xc2:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xc3:
				effectiveAddress = regs.a[0]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xc4:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xc5:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xc6:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xc7:
				effectiveAddress = regs.a[1]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xc8:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xc9:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xca:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xcb:
				effectiveAddress = regs.a[2]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xcc:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xcd:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xce:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xcf:
				effectiveAddress = regs.a[3]+regs.d[0];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xd0:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xd1:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xd2:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xd3:
				effectiveAddress = regs.a[0]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xd4:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xd5:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xd6:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xd7:
				effectiveAddress = regs.a[1]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xd8:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xd9:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xda:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xdb:
				effectiveAddress = regs.a[2]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xdc:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xdd:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xde:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xdf:
				effectiveAddress = regs.a[3]+regs.d[1];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xe0:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xe1:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xe2:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xe3:
				effectiveAddress = regs.a[0]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xe4:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xe5:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xe6:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xe7:
				effectiveAddress = regs.a[1]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xe8:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xe9:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xea:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xeb:
				effectiveAddress = regs.a[2]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xec:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xed:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xee:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xef:
				effectiveAddress = regs.a[3]+regs.d[2];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xf0:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xf1:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xf2:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xf3:
				effectiveAddress = regs.a[0]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xf4:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xf5:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xf6:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xf7:
				effectiveAddress = regs.a[1]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xf8:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xf9:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xfa:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xfb:
				effectiveAddress = regs.a[2]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0xfc:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0xfd:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0xfe:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0xff:
				effectiveAddress = regs.a[3]+regs.d[3];
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			}
			break;
		case 0xf2:
			op_code=mem->readAbsolute8(regs.pc++);

			cycle-=2;

			switch(op_code)
			{
			case 0xa0:
			case 0xa1:
			case 0xa2:
			case 0xa3:
			case 0xa4:
			case 0xa5:
			case 0xa6:
			case 0xa7:
			case 0xa8:
			case 0xa9:
			case 0xaa:
			case 0xab:
			case 0xac:
			case 0xad:
			case 0xae:
			case 0xaf:
			case 0xb0:
			case 0xb1:
			case 0xb2:
			case 0xb3:
			case 0xb4:
			case 0xb5:
			case 0xb6:
			case 0xb7:
			case 0xb8:
			case 0xb9:
			case 0xba:
			case 0xbb:
			case 0xbc:
			case 0xbd:
			case 0xbe:
			case 0xbf:
				undefined_instruction();
				break;
/* ADD Dm,An An+Dm ¢ªAn -oooooooo2 2 F2:00+Dm<<2+An*/
			case 0x00:
				regs.a[0]=add(regs.a[0], regs.d[0]);
				cycle-=2;
				break;
			case 0x01:
				regs.a[1]=add(regs.a[1], regs.d[0]);
				cycle-=2;
				break;
			case 0x02:
				regs.a[2]=add(regs.a[2], regs.d[0]);
				cycle-=2;
				break;
			case 0x03:
				regs.a[3]=add(regs.a[3], regs.d[0]);
				cycle-=2;
				break;
			case 0x04:
				regs.a[0]=add(regs.a[0], regs.d[1]);
				cycle-=2;
				break;
			case 0x05:
				regs.a[1]=add(regs.a[1], regs.d[1]);
				cycle-=2;
				break;
			case 0x06:
				regs.a[2]=add(regs.a[2], regs.d[1]);
				cycle-=2;
				break;
			case 0x07:
				regs.a[3]=add(regs.a[3], regs.d[1]);
				cycle-=2;
				break;
			case 0x08:
				regs.a[0]=add(regs.a[0], regs.d[2]);
				cycle-=2;
				break;
			case 0x09:
				regs.a[1]=add(regs.a[1], regs.d[2]);
				cycle-=2;
				break;
			case 0x0a:
				regs.a[2]=add(regs.a[2], regs.d[2]);
				cycle-=2;
				break;
			case 0x0b:
				regs.a[3]=add(regs.a[3], regs.d[2]);
				cycle-=2;
				break;
			case 0x0c:
				regs.a[0]=add(regs.a[0], regs.d[3]);
				cycle-=2;
				break;
			case 0x0d:
				regs.a[1]=add(regs.a[1], regs.d[3]);
				cycle-=2;
				break;
			case 0x0e:
				regs.a[2]=add(regs.a[2], regs.d[3]);
				cycle-=2;
				break;
			case 0x0f:
				regs.a[3]=add(regs.a[3], regs.d[3]);
				cycle-=2;
				break;

/* SUB Dm,An An- Dm ¢ªAn -oooooooo2 2 F2:10+Dm<<2+An */
			case 0x10:
				regs.a[0]=sub(regs.a[0], regs.d[0]);
				cycle-=2;
				break;
			case 0x11:
				regs.a[1]=sub(regs.a[1], regs.d[0]);
				cycle-=2;
				break;
			case 0x12:
				regs.a[2]=sub(regs.a[2], regs.d[0]);
				cycle-=2;
				break;
			case 0x13:
				regs.a[3]=sub(regs.a[3], regs.d[0]);
				cycle-=2;
				break;
			case 0x14:
				regs.a[0]=sub(regs.a[0], regs.d[1]);
				cycle-=2;
				break;
			case 0x15:
				regs.a[1]=sub(regs.a[1], regs.d[1]);
				cycle-=2;
				break;
			case 0x16:
				regs.a[2]=sub(regs.a[2], regs.d[1]);
				cycle-=2;
				break;
			case 0x17:
				regs.a[3]=sub(regs.a[3], regs.d[1]);
				cycle-=2;
				break;
			case 0x18:
				regs.a[0]=sub(regs.a[0], regs.d[2]);
				cycle-=2;
				break;
			case 0x19:
				regs.a[1]=sub(regs.a[1], regs.d[2]);
				cycle-=2;
				break;
			case 0x1a:
				regs.a[2]=sub(regs.a[2], regs.d[2]);
				cycle-=2;
				break;
			case 0x1b:
				regs.a[3]=sub(regs.a[3], regs.d[2]);
				cycle-=2;
				break;
			case 0x1c:
				regs.a[0]=sub(regs.a[0], regs.d[3]);
				cycle-=2;
				break;
			case 0x1d:
				regs.a[1]=sub(regs.a[1], regs.d[3]);
				cycle-=2;
				break;
			case 0x1e:
				regs.a[2]=sub(regs.a[2], regs.d[3]);
				cycle-=2;
				break;
			case 0x1f:
				regs.a[3]=sub(regs.a[3], regs.d[3]);
				cycle-=2;
				break;

/* CMP Dm,An An- Dm -oooooooo2 2 F2:20+Dm<<2+An */
			case 0x20:
				sub(regs.a[0], regs.d[0]);
				cycle-=2;
				break;
			case 0x21:
				sub(regs.a[1], regs.d[0]);
				cycle-=2;
				break;
			case 0x22:
				sub(regs.a[2], regs.d[0]);
				cycle-=2;
				break;
			case 0x23:
				sub(regs.a[3], regs.d[0]);
				cycle-=2;
				break;
			case 0x24:
				sub(regs.a[0], regs.d[1]);
				cycle-=2;
				break;
			case 0x25:
				sub(regs.a[1], regs.d[1]);
				cycle-=2;
				break;
			case 0x26:
				sub(regs.a[2], regs.d[1]);
				cycle-=2;
				break;
			case 0x27:
				sub(regs.a[3], regs.d[1]);
				cycle-=2;
				break;
			case 0x28:
				sub(regs.a[0], regs.d[2]);
				cycle-=2;
				break;
			case 0x29:
				sub(regs.a[1], regs.d[2]);
				cycle-=2;
				break;
			case 0x2a:
				sub(regs.a[2], regs.d[2]);
				cycle-=2;
				break;
			case 0x2b:
				sub(regs.a[3], regs.d[2]);
				cycle-=2;
				break;
			case 0x2c:
				sub(regs.a[0], regs.d[3]);
				cycle-=2;
				break;
			case 0x2d:
				sub(regs.a[1], regs.d[3]);
				cycle-=2;
				break;
			case 0x2e:
				sub(regs.a[2], regs.d[3]);
				cycle-=2;
				break;
			case 0x2f:
				sub(regs.a[3], regs.d[3]);
				cycle-=2;
				break;

/* MOV Dm,An Dm ¢ªAn ---------2 2 F2:30+Dm<<2+An */
			case 0x30:
				regs.a[0]=regs.d[0];
				cycle-=2;
				break;
			case 0x31:
				regs.a[1]=regs.d[0];
				cycle-=2;
				break;
			case 0x32:
				regs.a[2]=regs.d[0];
				cycle-=2;
				break;
			case 0x33:
				regs.a[3]=regs.d[0];
				cycle-=2;
				break;
			case 0x34:
				regs.a[0]=regs.d[1];
				cycle-=2;
				break;
			case 0x35:
				regs.a[1]=regs.d[1];
				cycle-=2;
				break;
			case 0x36:
				regs.a[2]=regs.d[1];
				cycle-=2;
				break;
			case 0x37:
				regs.a[3]=regs.d[1];
				cycle-=2;
				break;
			case 0x38:
				regs.a[0]=regs.d[2];
				cycle-=2;
				break;
			case 0x39:
				regs.a[1]=regs.d[2];
				cycle-=2;
				break;
			case 0x3a:
				regs.a[2]=regs.d[2];
				cycle-=2;
				break;
			case 0x3b:
				regs.a[3]=regs.d[2];
				cycle-=2;
				break;
			case 0x3c:
				regs.a[0]=regs.d[3];
				cycle-=2;
				break;
			case 0x3d:
				regs.a[1]=regs.d[3];
				cycle-=2;
				break;
			case 0x3e:
				regs.a[2]=regs.d[3];
				cycle-=2;
				break;
			case 0x3f:
				regs.a[3]=regs.d[3];
				cycle-=2;
				break;

/* ADD An,Am Am+An ¢ªAm -oooooooo2 2 F2:40+An<<2+Am */
			case 0x40:
				regs.a[0]=add(regs.a[0], regs.a[0]);
				cycle-=2;
				break;
			case 0x41:
				regs.a[1]=add(regs.a[1], regs.a[0]);
				cycle-=2;
				break;
			case 0x42:
				regs.a[2]=add(regs.a[2], regs.a[0]);
				cycle-=2;
				break;
			case 0x43:
				regs.a[3]=add(regs.a[3], regs.a[0]);
				cycle-=2;
				break;
			case 0x44:
				regs.a[0]=add(regs.a[0], regs.a[1]);
				cycle-=2;
				break;
			case 0x45:
				regs.a[1]=add(regs.a[1], regs.a[1]);
				cycle-=2;
				break;
			case 0x46:
				regs.a[2]=add(regs.a[2], regs.a[1]);
				cycle-=2;
				break;
			case 0x47:
				regs.a[3]=add(regs.a[3], regs.a[1]);
				cycle-=2;
				break;
			case 0x48:
				regs.a[0]=add(regs.a[0], regs.a[2]);
				cycle-=2;
				break;
			case 0x49:
				regs.a[1]=add(regs.a[1], regs.a[2]);
				cycle-=2;
				break;
			case 0x4a:
				regs.a[2]=add(regs.a[2], regs.a[2]);
				cycle-=2;
				break;
			case 0x4b:
				regs.a[3]=add(regs.a[3], regs.a[2]);
				cycle-=2;
				break;
			case 0x4c:
				regs.a[0]=add(regs.a[0], regs.a[3]);
				cycle-=2;
				break;
			case 0x4d:
				regs.a[1]=add(regs.a[1], regs.a[3]);
				cycle-=2;
				break;
			case 0x4e:
				regs.a[2]=add(regs.a[2], regs.a[3]);
				cycle-=2;
				break;
			case 0x4f:
				regs.a[3]=add(regs.a[3], regs.a[3]);
				cycle-=2;
				break;

/* SUB An,Am Am- An ¢ªAm -oooooooo2 2 F2:50+An<<2+Am */
			case 0x50:
				regs.a[0]=sub(regs.a[0], regs.a[0]);
				cycle-=2;
				break;
			case 0x51:
				regs.a[1]=sub(regs.a[1], regs.a[0]);
				cycle-=2;
				break;
			case 0x52:
				regs.a[2]=sub(regs.a[2], regs.a[0]);
				cycle-=2;
				break;
			case 0x53:
				regs.a[3]=sub(regs.a[3], regs.a[0]);
				cycle-=2;
				break;
			case 0x54:
				regs.a[0]=sub(regs.a[0], regs.a[1]);
				cycle-=2;
				break;
			case 0x55:
				regs.a[1]=sub(regs.a[1], regs.a[1]);
				cycle-=2;
				break;
			case 0x56:
				regs.a[2]=sub(regs.a[2], regs.a[1]);
				cycle-=2;
				break;
			case 0x57:
				regs.a[3]=sub(regs.a[3], regs.a[1]);
				cycle-=2;
				break;
			case 0x58:
				regs.a[0]=sub(regs.a[0], regs.a[2]);
				cycle-=2;
				break;
			case 0x59:
				regs.a[1]=sub(regs.a[1], regs.a[2]);
				cycle-=2;
				break;
			case 0x5a:
				regs.a[2]=sub(regs.a[2], regs.a[2]);
				cycle-=2;
				break;
			case 0x5b:
				regs.a[3]=sub(regs.a[3], regs.a[2]);
				cycle-=2;
				break;
			case 0x5c:
				regs.a[0]=sub(regs.a[0], regs.a[3]);
				cycle-=2;
				break;
			case 0x5d:
				regs.a[1]=sub(regs.a[1], regs.a[3]);
				cycle-=2;
				break;
			case 0x5e:
				regs.a[2]=sub(regs.a[2], regs.a[3]);
				cycle-=2;
				break;
			case 0x5f:
				regs.a[3]=sub(regs.a[3], regs.a[3]);
				cycle-=2;
				break;

/* CMP An,Am Am- An -oooooooo2 2 F2:60+An<<2+Am */
			case 0x60:
				sub(regs.a[0], regs.a[0]);
				cycle-=2;
				break;
			case 0x61:
				sub(regs.a[1], regs.a[0]);
				cycle-=2;
				break;
			case 0x62:
				sub(regs.a[2], regs.a[0]);
				cycle-=2;
				break;
			case 0x63:
				sub(regs.a[3], regs.a[0]);
				cycle-=2;
				break;
			case 0x64:
				sub(regs.a[0], regs.a[1]);
				cycle-=2;
				break;
			case 0x65:
				sub(regs.a[1], regs.a[1]);
				cycle-=2;
				break;
			case 0x66:
				sub(regs.a[2], regs.a[1]);
				cycle-=2;
				break;
			case 0x67:
				sub(regs.a[3], regs.a[1]);
				cycle-=2;
				break;
			case 0x68:
				sub(regs.a[0], regs.a[2]);
				cycle-=2;
				break;
			case 0x69:
				sub(regs.a[1], regs.a[2]);
				cycle-=2;
				break;
			case 0x6a:
				sub(regs.a[2], regs.a[2]);
				cycle-=2;
				break;
			case 0x6b:
				sub(regs.a[3], regs.a[2]);
				cycle-=2;
				break;
			case 0x6c:
				sub(regs.a[0], regs.a[3]);
				cycle-=2;
				break;
			case 0x6d:
				sub(regs.a[1], regs.a[3]);
				cycle-=2;
				break;
			case 0x6e:
				sub(regs.a[2], regs.a[3]);
				cycle-=2;
				break;
			case 0x6f:
				sub(regs.a[3], regs.a[3]);
				cycle-=2;
				break;

/* MOV An,Am An ¢ªAm ---------2 2 F2:70+An<<2+Am */
			case 0x70:
				regs.a[0]=regs.a[0];
				cycle-=2;
				break;
			case 0x71:
				regs.a[1]=regs.a[0];
				cycle-=2;
				break;
			case 0x72:
				regs.a[2]=regs.a[0];
				cycle-=2;
				break;
			case 0x73:
				regs.a[3]=regs.a[0];
				cycle-=2;
				break;
			case 0x74:
				regs.a[0]=regs.a[1];
				cycle-=2;
				break;
			case 0x75:
				regs.a[1]=regs.a[1];
				cycle-=2;
				break;
			case 0x76:
				regs.a[2]=regs.a[1];
				cycle-=2;
				break;
			case 0x77:
				regs.a[3]=regs.a[1];
				cycle-=2;
				break;
			case 0x78:
				regs.a[0]=regs.a[2];
				cycle-=2;
				break;
			case 0x79:
				regs.a[1]=regs.a[2];
				cycle-=2;
				break;
			case 0x7a:
				regs.a[2]=regs.a[2];
				cycle-=2;
				break;
			case 0x7b:
				regs.a[3]=regs.a[2];
				cycle-=2;
				break;
			case 0x7c:
				regs.a[0]=regs.a[3];
				cycle-=2;
				break;
			case 0x7d:
				regs.a[1]=regs.a[3];
				cycle-=2;
				break;
			case 0x7e:
				regs.a[2]=regs.a[3];
				cycle-=2;
				break;
			case 0x7f:
				regs.a[3]=regs.a[3];
				cycle-=2;
				break;

/* ADDC Dn,Dm Dm+Dn+CF ¢ªDm -oooooooo2 2 F2:80+Dn<<2+Dm */
			case 0x80:
				regs.d[0]=addc(regs.d[0], regs.d[0]);
				cycle-=2;
				break;
			case 0x81:
				regs.d[1]=addc(regs.d[1], regs.d[0]);
				cycle-=2;
				break;
			case 0x82:
				regs.d[2]=addc(regs.d[2], regs.d[0]);
				cycle-=2;
				break;
			case 0x83:
				regs.d[3]=addc(regs.d[3], regs.d[0]);
				cycle-=2;
				break;
			case 0x84:
				regs.d[0]=addc(regs.d[0], regs.d[1]);
				cycle-=2;
				break;
			case 0x85:
				regs.d[1]=addc(regs.d[1], regs.d[1]);
				cycle-=2;
				break;
			case 0x86:
				regs.d[2]=addc(regs.d[2], regs.d[1]);
				cycle-=2;
				break;
			case 0x87:
				regs.d[3]=addc(regs.d[3], regs.d[1]);
				cycle-=2;
				break;
			case 0x88:
				regs.d[0]=addc(regs.d[0], regs.d[2]);
				cycle-=2;
				break;
			case 0x89:
				regs.d[1]=addc(regs.d[1], regs.d[2]);
				cycle-=2;
				break;
			case 0x8a:
				regs.d[2]=addc(regs.d[2], regs.d[2]);
				cycle-=2;
				break;
			case 0x8b:
				regs.d[3]=addc(regs.d[3], regs.d[2]);
				cycle-=2;
				break;
			case 0x8c:
				regs.d[0]=addc(regs.d[0], regs.d[3]);
				cycle-=2;
				break;
			case 0x8d:
				regs.d[1]=addc(regs.d[1], regs.d[3]);
				cycle-=2;
				break;
			case 0x8e:
				regs.d[2]=addc(regs.d[2], regs.d[3]);
				cycle-=2;
				break;
			case 0x8f:
				regs.d[3]=addc(regs.d[3], regs.d[3]);
				cycle-=2;
				break;

/* SUBC Dn,Dm Dm- Dn- CF ¢ªDm -oooooooo2 2 F2:90+Dn<<2+Dm */
			case 0x90:
				regs.d[0]=subc(regs.d[0], regs.d[0]);
				cycle-=2;
				break;
			case 0x91:
				regs.d[1]=subc(regs.d[1], regs.d[0]);
				cycle-=2;
				break;
			case 0x92:
				regs.d[2]=subc(regs.d[2], regs.d[0]);
				cycle-=2;
				break;
			case 0x93:
				regs.d[3]=subc(regs.d[3], regs.d[0]);
				cycle-=2;
				break;
			case 0x94:
				regs.d[0]=subc(regs.d[0], regs.d[1]);
				cycle-=2;
				break;
			case 0x95:
				regs.d[1]=subc(regs.d[1], regs.d[1]);
				cycle-=2;
				break;
			case 0x96:
				regs.d[2]=subc(regs.d[2], regs.d[1]);
				cycle-=2;
				break;
			case 0x97:
				regs.d[3]=subc(regs.d[3], regs.d[1]);
				cycle-=2;
				break;
			case 0x98:
				regs.d[0]=subc(regs.d[0], regs.d[2]);
				cycle-=2;
				break;
			case 0x99:
				regs.d[1]=subc(regs.d[1], regs.d[2]);
				cycle-=2;
				break;
			case 0x9a:
				regs.d[2]=subc(regs.d[2], regs.d[2]);
				cycle-=2;
				break;
			case 0x9b:
				regs.d[3]=subc(regs.d[3], regs.d[2]);
				cycle-=2;
				break;
			case 0x9c:
				regs.d[0]=subc(regs.d[0], regs.d[3]);
				cycle-=2;
				break;
			case 0x9d:
				regs.d[1]=subc(regs.d[1], regs.d[3]);
				cycle-=2;
				break;
			case 0x9e:
				regs.d[2]=subc(regs.d[2], regs.d[3]);
				cycle-=2;
				break;
			case 0x9f:
				regs.d[3]=subc(regs.d[3], regs.d[3]);
				cycle-=2;
				break;

/* ADD An,Dm Dm+An ¢ªDm -oooooooo2 2 F2:C0+An<<2+Dm */
			case 0xc0:
				regs.d[0]=add(regs.d[0], regs.a[0]);
				cycle-=2;
				break;
			case 0xc1:
				regs.d[1]=add(regs.d[1], regs.a[0]);
				cycle-=2;
				break;
			case 0xc2:
				regs.d[2]=add(regs.d[2], regs.a[0]);
				cycle-=2;
				break;
			case 0xc3:
				regs.d[3]=add(regs.d[3], regs.a[0]);
				cycle-=2;
				break;
			case 0xc4:
				regs.d[0]=add(regs.d[0], regs.a[1]);
				cycle-=2;
				break;
			case 0xc5:
				regs.d[1]=add(regs.d[1], regs.a[1]);
				cycle-=2;
				break;
			case 0xc6:
				regs.d[2]=add(regs.d[2], regs.a[1]);
				cycle-=2;
				break;
			case 0xc7:
				regs.d[3]=add(regs.d[3], regs.a[1]);
				cycle-=2;
				break;
			case 0xc8:
				regs.d[0]=add(regs.d[0], regs.a[2]);
				cycle-=2;
				break;
			case 0xc9:
				regs.d[1]=add(regs.d[1], regs.a[2]);
				cycle-=2;
				break;
			case 0xca:
				regs.d[2]=add(regs.d[2], regs.a[2]);
				cycle-=2;
				break;
			case 0xcb:
				regs.d[3]=add(regs.d[3], regs.a[2]);
				cycle-=2;
				break;
			case 0xcc:
				regs.d[0]=add(regs.d[0], regs.a[3]);
				cycle-=2;
				break;
			case 0xcd:
				regs.d[1]=add(regs.d[1], regs.a[3]);
				cycle-=2;
				break;
			case 0xce:
				regs.d[2]=add(regs.d[2], regs.a[3]);
				cycle-=2;
				break;
			case 0xcf:
				regs.d[3]=add(regs.d[3], regs.a[3]);
				cycle-=2;
				break;

/* SUB An,Dm Dm- An ¢ªDm -oooooooo2 2 F2:D0+An<<2+Dm */
			case 0xd0:
				regs.d[0]=sub(regs.d[0], regs.a[0]);
				cycle-=2;
				break;
			case 0xd1:
				regs.d[1]=sub(regs.d[1], regs.a[0]);
				cycle-=2;
				break;
			case 0xd2:
				regs.d[2]=sub(regs.d[2], regs.a[0]);
				cycle-=2;
				break;
			case 0xd3:
				regs.d[3]=sub(regs.d[3], regs.a[0]);
				cycle-=2;
				break;
			case 0xd4:
				regs.d[0]=sub(regs.d[0], regs.a[1]);
				cycle-=2;
				break;
			case 0xd5:
				regs.d[1]=sub(regs.d[1], regs.a[1]);
				cycle-=2;
				break;
			case 0xd6:
				regs.d[2]=sub(regs.d[2], regs.a[1]);
				cycle-=2;
				break;
			case 0xd7:
				regs.d[3]=sub(regs.d[3], regs.a[1]);
				cycle-=2;
				break;
			case 0xd8:
				regs.d[0]=sub(regs.d[0], regs.a[2]);
				cycle-=2;
				break;
			case 0xd9:
				regs.d[1]=sub(regs.d[1], regs.a[2]);
				cycle-=2;
				break;
			case 0xda:
				regs.d[2]=sub(regs.d[2], regs.a[2]);
				cycle-=2;
				break;
			case 0xdb:
				regs.d[3]=sub(regs.d[3], regs.a[2]);
				cycle-=2;
				break;
			case 0xdc:
				regs.d[0]=sub(regs.d[0], regs.a[3]);
				cycle-=2;
				break;
			case 0xdd:
				regs.d[1]=sub(regs.d[1], regs.a[3]);
				cycle-=2;
				break;
			case 0xde:
				regs.d[2]=sub(regs.d[2], regs.a[3]);
				cycle-=2;
				break;
			case 0xdf:
				regs.d[3]=sub(regs.d[3], regs.a[3]);
				cycle-=2;
				break;

/* CMP An,Dm Dm- An -oooooooo2 2 F2:E0+An<<2+Dm */
			case 0xe0:
				sub(regs.d[0], regs.a[0]);
				cycle-=2;
				break;
			case 0xe1:
				sub(regs.d[1], regs.a[0]);
				cycle-=2;
				break;
			case 0xe2:
				sub(regs.d[2], regs.a[0]);
				cycle-=2;
				break;
			case 0xe3:
				sub(regs.d[3], regs.a[0]);
				cycle-=2;
				break;
			case 0xe4:
				sub(regs.d[0], regs.a[1]);
				cycle-=2;
				break;
			case 0xe5:
				sub(regs.d[1], regs.a[1]);
				cycle-=2;
				break;
			case 0xe6:
				sub(regs.d[2], regs.a[1]);
				cycle-=2;
				break;
			case 0xe7:
				sub(regs.d[3], regs.a[1]);
				cycle-=2;
				break;
			case 0xe8:
				sub(regs.d[0], regs.a[2]);
				cycle-=2;
				break;
			case 0xe9:
				sub(regs.d[1], regs.a[2]);
				cycle-=2;
				break;
			case 0xea:
				sub(regs.d[2], regs.a[2]);
				cycle-=2;
				break;
			case 0xeb:
				sub(regs.d[3], regs.a[2]);
				cycle-=2;
				break;
			case 0xec:
				sub(regs.d[0], regs.a[3]);
				cycle-=2;
				break;
			case 0xed:
				sub(regs.d[1], regs.a[3]);
				cycle-=2;
				break;
			case 0xee:
				sub(regs.d[2], regs.a[3]);
				cycle-=2;
				break;
			case 0xef:
				sub(regs.d[3], regs.a[3]);
				cycle-=2;
				break;

/* MOV An,Dm An ¢ªDm ---------2 2 F2:F0+An<<2+Dm */
			case 0xf0:
				regs.d[0]=regs.a[0];
				cycle-=2;
				break;
			case 0xf1:
				regs.d[1]=regs.a[0];
				cycle-=2;
				break;
			case 0xf2:
				regs.d[2]=regs.a[0];
				cycle-=2;
				break;
			case 0xf3:
				regs.d[3]=regs.a[0];
				cycle-=2;
				break;
			case 0xf4:
				regs.d[0]=regs.a[1];
				cycle-=2;
				break;
			case 0xf5:
				regs.d[1]=regs.a[1];
				cycle-=2;
				break;
			case 0xf6:
				regs.d[2]=regs.a[1];
				cycle-=2;
				break;
			case 0xf7:
				regs.d[3]=regs.a[1];
				cycle-=2;
				break;
			case 0xf8:
				regs.d[0]=regs.a[2];
				cycle-=2;
				break;
			case 0xf9:
				regs.d[1]=regs.a[2];
				cycle-=2;
				break;
			case 0xfa:
				regs.d[2]=regs.a[2];
				cycle-=2;
				break;
			case 0xfb:
				regs.d[3]=regs.a[2];
				cycle-=2;
				break;
			case 0xfc:
				regs.d[0]=regs.a[3];
				cycle-=2;
				break;
			case 0xfd:
				regs.d[1]=regs.a[3];
				cycle-=2;
				break;
			case 0xfe:
				regs.d[2]=regs.a[3];
				cycle-=2;
				break;
			case 0xff:
				regs.d[3]=regs.a[3];
				cycle-=2;
				break;
			}

			break;
		case 0xf3:
			op_code=mem->readAbsolute8(regs.pc++);
			switch(op_code)
			{
			case 0x70:
			case 0x71:
			case 0x72:
			case 0x73:
			case 0x74:
			case 0x75:
			case 0x76:
			case 0x77:
			case 0x78:
			case 0x79:
			case 0x7a:
			case 0x7b:
			case 0x7c:
			case 0x7d:
			case 0x7e:
			case 0x7f:
			case 0x80:
			case 0x81:
			case 0x82:
			case 0x83:
			case 0x84:
			case 0x85:
			case 0x86:
			case 0x87:
			case 0x88:
			case 0x89:
			case 0x8a:
			case 0x8b:
			case 0x8c:
			case 0x8d:
			case 0x8e:
			case 0x8f:
			case 0xa0:
			case 0xa1:
			case 0xa2:
			case 0xa3:
			case 0xa4:
			case 0xa5:
			case 0xa6:
			case 0xa7:
			case 0xa8:
			case 0xa9:
			case 0xaa:
			case 0xab:
			case 0xac:
			case 0xad:
			case 0xae:
			case 0xaf:
			case 0xb0:
			case 0xb1:
			case 0xb2:
			case 0xb3:
			case 0xb4:
			case 0xb5:
			case 0xb6:
			case 0xb7:
			case 0xb8:
			case 0xb9:
			case 0xba:
			case 0xbb:
			case 0xbc:
			case 0xbd:
			case 0xbe:
			case 0xbf:
			case 0xc2:
			case 0xc3:
			case 0xc6:
			case 0xc7:
			case 0xca:
			case 0xcb:
			case 0xce:
			case 0xcf:

			case 0xd1:
			case 0xd2:
			case 0xd3:
			case 0xd5:
			case 0xd6:
			case 0xd7:
			case 0xd9:
			case 0xda:
			case 0xdb:
			case 0xdd:
			case 0xde:
			case 0xdf:

			case 0xe8:
			case 0xe9:
			case 0xea:
			case 0xeb:
			case 0xec:
			case 0xed:
			case 0xee:
			case 0xef:
			case 0xf4:
			case 0xf5:
			case 0xf6:
			case 0xf7:
			case 0xf8:
			case 0xf9:
			case 0xfa:
			case 0xfb:
			case 0xfc:
			case 0xfd:
			case 0xfe:
			case 0xff:
				undefined_instruction();
				break;

/* AND Dn,Dm Dm&(x'FF0000'|Dn)¢ªDm -----0 0 oo2 2 F3:00+Dn<<2+Dm */
			case 0x00:
				regs.d[0]=_and(regs.d[0], 0xff0000UL | regs.d[0]);
				cycle-=2;
				break;
			case 0x01:
				regs.d[1]=_and(regs.d[1], 0xff0000UL | regs.d[0]);
				cycle-=2;
				break;
			case 0x02:
				regs.d[2]=_and(regs.d[2], 0xff0000UL | regs.d[0]);
				cycle-=2;
				break;
			case 0x03:
				regs.d[3]=_and(regs.d[3], 0xff0000UL | regs.d[0]);
				cycle-=2;
				break;
			case 0x04:
				regs.d[0]=_and(regs.d[0], 0xff0000UL | regs.d[1]);
				cycle-=2;
				break;
			case 0x05:
				regs.d[1]=_and(regs.d[1], 0xff0000UL | regs.d[1]);
				cycle-=2;
				break;
			case 0x06:
				regs.d[2]=_and(regs.d[2], 0xff0000UL | regs.d[1]);
				cycle-=2;
				break;
			case 0x07:
				regs.d[3]=_and(regs.d[3], 0xff0000UL | regs.d[1]);
				cycle-=2;
				break;
			case 0x08:
				regs.d[0]=_and(regs.d[0], 0xff0000UL | regs.d[2]);
				cycle-=2;
				break;
			case 0x09:
				regs.d[1]=_and(regs.d[1], 0xff0000UL | regs.d[2]);
				cycle-=2;
				break;
			case 0x0a:
				regs.d[2]=_and(regs.d[2], 0xff0000UL | regs.d[2]);
				cycle-=2;
				break;
			case 0x0b:
				regs.d[3]=_and(regs.d[3], 0xff0000UL | regs.d[2]);
				cycle-=2;
				break;
			case 0x0c:
				regs.d[0]=_and(regs.d[0], 0xff0000UL | regs.d[3]);
				cycle-=2;
				break;
			case 0x0d:
				regs.d[1]=_and(regs.d[1], 0xff0000UL | regs.d[3]);
				cycle-=2;
				break;
			case 0x0e:
				regs.d[2]=_and(regs.d[2], 0xff0000UL | regs.d[3]);
				cycle-=2;
				break;
			case 0x0f:
				regs.d[3]=_and(regs.d[3], 0xff0000UL | regs.d[3]);
				cycle-=2;
				break;
/* OR Dn,Dm Dm|(Dn&x'00FFFF')¢ªDm -----0 0 oo2 2 F3:10+Dn<<2+Dm */
			case 0x10:
				regs.d[0]=_or(regs.d[0], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x11:
				regs.d[1]=_or(regs.d[1], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x12:
				regs.d[2]=_or(regs.d[2], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x13:
				regs.d[3]=_or(regs.d[3], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x14:
				regs.d[0]=_or(regs.d[0], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x15:
				regs.d[1]=_or(regs.d[1], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x16:
				regs.d[2]=_or(regs.d[2], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x17:
				regs.d[3]=_or(regs.d[3], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x18:
				regs.d[0]=_or(regs.d[0], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x19:
				regs.d[1]=_or(regs.d[1], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x1a:
				regs.d[2]=_or(regs.d[2], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x1b:
				regs.d[3]=_or(regs.d[3], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x1c:
				regs.d[0]=_or(regs.d[0], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
			case 0x1d:
				regs.d[1]=_or(regs.d[1], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
			case 0x1e:
				regs.d[2]=_or(regs.d[2], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
			case 0x1f:
				regs.d[3]=_or(regs.d[3], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
/* XOR Dn,Dm Dm^ (x'00FFFF'&Dn)¢ªDm -----0 0 oo2 2 F3:20+Dn<<2+Dm */
			case 0x20:
				regs.d[0]=_xor(regs.d[0], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x21:
				regs.d[1]=_xor(regs.d[1], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x22:
				regs.d[2]=_xor(regs.d[2], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x23:
				regs.d[3]=_xor(regs.d[3], regs.d[0] & 0xffffUL);
				cycle-=2;
				break;
			case 0x24:
				regs.d[0]=_xor(regs.d[0], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x25:
				regs.d[1]=_xor(regs.d[1], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x26:
				regs.d[2]=_xor(regs.d[2], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x27:
				regs.d[3]=_xor(regs.d[3], regs.d[1] & 0xffffUL);
				cycle-=2;
				break;
			case 0x28:
				regs.d[0]=_xor(regs.d[0], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x29:
				regs.d[1]=_xor(regs.d[1], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x2a:
				regs.d[2]=_xor(regs.d[2], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x2b:
				regs.d[3]=_xor(regs.d[3], regs.d[2] & 0xffffUL);
				cycle-=2;
				break;
			case 0x2c:
				regs.d[0]=_xor(regs.d[0], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
			case 0x2d:
				regs.d[1]=_xor(regs.d[1], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
			case 0x2e:
				regs.d[2]=_xor(regs.d[2], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
			case 0x2f:
				regs.d[3]=_xor(regs.d[3], regs.d[3] & 0xffffUL);
				cycle-=2;
				break;
/* ROL Dn Dn.bp15 ¢ªtemp -----0 ooo2 2 F3:30+Dn
 *        Dn.bp ¢ªDn.bp+1(bp14 ¡Á0)
 *        CF ¢ªDn.lsb
 *        temp ¢ªCF
 */
			case 0x30:
				regs.d[0]=rol(regs.d[0]);
				cycle-=2;
				break;
			case 0x31:
				regs.d[1]=rol(regs.d[1]);
				cycle-=2;
				break;
			case 0x32:
				regs.d[2]=rol(regs.d[2]);
				cycle-=2;
				break;
			case 0x33:
				regs.d[3]=rol(regs.d[3]);
				cycle-=2;
				break;
/* ROR Dn Dn.lsb ¢ªtemp -----0 ooo2 2 F3:34+Dn *15 94
 *        Dn.bp ¢ªDn.bp- 1(bp15 ¡Á1)
 *        CF ¢ªDn.bp15
 *        temp ¢ªCF
 */
			case 0x34:
				regs.d[0]=ror(regs.d[0]);
				cycle-=2;
				break;
			case 0x35:
				regs.d[1]=ror(regs.d[1]);
				cycle-=2;
				break;
			case 0x36:
				regs.d[2]=ror(regs.d[2]);
				cycle-=2;
				break;
			case 0x37:
				regs.d[3]=ror(regs.d[3]);
				cycle-=2;
				break;
/* ASR Dn Dn.lsb ¢ªCF -----0 ooo2 2 F3:38+Dn *15 92
 *        Dn.bp ¢ªDn.bp- 1(bp15 ¡Á1)
 *        Dn.bp15 ¢ªDn.bp15
 *        Notes:15*16 ¥Ó¥Ã¥È±é»»¡£
 */
			case 0x38:
				regs.d[0]=asr(regs.d[0]);
				cycle-=2;
				break;
			case 0x39:
				regs.d[1]=asr(regs.d[1]);
				cycle-=2;
				break;
			case 0x3a:
				regs.d[2]=asr(regs.d[2]);
				cycle-=2;
				break;
			case 0x3b:
				regs.d[3]=asr(regs.d[3]);
				cycle-=2;
				break;

/* LSR Dn Dn.lsb ¢ªCF -----0 o0 o2 2 F3:3C+Dn *15 93
 *        Dn.bp ¢ªDn.bp- 1(bp15 ¡Á1)
 *        0 ¢ªDn.bp15
 */			case 0x3c:
				regs.d[0]=lsr(regs.d[0]);
				cycle-=2;
				break;
			case 0x3d:
				regs.d[1]=lsr(regs.d[1]);
				cycle-=2;
				break;
			case 0x3e:
				regs.d[2]=lsr(regs.d[2]);
				cycle-=2;
				break;
			case 0x3f:
				regs.d[3]=lsr(regs.d[3]);
				cycle-=2;
				break;
/* MUL Dn,Dm Dm ¡öDn ¢ªDm -????0 ?oo2 12 F3:40+Dn<<2+Dm
 *          (Dm ¡öDn)>>16 ¢ªMDR
 */
			case 0x40:
				regs.d[0]=mul(regs.d[0], regs.d[0]);
				cycle-=12;
				break;
			case 0x41:
				regs.d[1]=mul(regs.d[1], regs.d[0]);
				cycle-=12;
				break;
			case 0x42:
				regs.d[2]=mul(regs.d[2], regs.d[0]);
				cycle-=12;
				break;
			case 0x43:
				regs.d[3]=mul(regs.d[3], regs.d[0]);
				cycle-=12;
				break;
			case 0x44:
				regs.d[0]=mul(regs.d[0], regs.d[1]);
				cycle-=12;
				break;
			case 0x45:
				regs.d[1]=mul(regs.d[1], regs.d[1]);
				cycle-=12;
				break;
			case 0x46:
				regs.d[2]=mul(regs.d[2], regs.d[1]);
				cycle-=12;
				break;
			case 0x47:
				regs.d[3]=mul(regs.d[3], regs.d[1]);
				cycle-=12;
				break;
			case 0x48:
				regs.d[0]=mul(regs.d[0], regs.d[2]);
				cycle-=12;
				break;
			case 0x49:
				regs.d[1]=mul(regs.d[1], regs.d[2]);
				cycle-=12;
				break;
			case 0x4a:
				regs.d[2]=mul(regs.d[2], regs.d[2]);
				cycle-=12;
				break;
			case 0x4b:
				regs.d[3]=mul(regs.d[3], regs.d[2]);
				cycle-=12;
				break;
			case 0x4c:
				regs.d[0]=mul(regs.d[0], regs.d[3]);
				cycle-=12;
				break;
			case 0x4d:
				regs.d[1]=mul(regs.d[1], regs.d[3]);
				cycle-=12;
				break;
			case 0x4e:
				regs.d[2]=mul(regs.d[2], regs.d[3]);
				cycle-=12;
				break;
			case 0x4f:
				regs.d[3]=mul(regs.d[3], regs.d[3]);
				cycle-=12;
				break;
/* MULU Dn,Dm Dm ¡öDn ¢ªDm -????0 ?oo2 12 F3:50+Dn<<2+Dm 
 *            (Dm ¡öDn)>>16 ¢ªMDR
 */
			case 0x50:
				regs.d[0]=mulu(regs.d[0], regs.d[0]);
				cycle-=12;
				break;
			case 0x51:
				regs.d[1]=mulu(regs.d[1], regs.d[0]);
				cycle-=12;
				break;
			case 0x52:
				regs.d[2]=mulu(regs.d[2], regs.d[0]);
				cycle-=12;
				break;
			case 0x53:
				regs.d[3]=mulu(regs.d[3], regs.d[0]);
				cycle-=12;
				break;
			case 0x54:
				regs.d[0]=mulu(regs.d[0], regs.d[1]);
				cycle-=12;
				break;
			case 0x55:
				regs.d[1]=mulu(regs.d[1], regs.d[1]);
				cycle-=12;
				break;
			case 0x56:
				regs.d[2]=mulu(regs.d[2], regs.d[1]);
				cycle-=12;
				break;
			case 0x57:
				regs.d[3]=mulu(regs.d[3], regs.d[1]);
				cycle-=12;
				break;
			case 0x58:
				regs.d[0]=mulu(regs.d[0], regs.d[2]);
				cycle-=12;
				break;
			case 0x59:
				regs.d[1]=mulu(regs.d[1], regs.d[2]);
				cycle-=12;
				break;
			case 0x5a:
				regs.d[2]=mulu(regs.d[2], regs.d[2]);
				cycle-=12;
				break;
			case 0x5b:
				regs.d[3]=mulu(regs.d[3], regs.d[2]);
				cycle-=12;
				break;
			case 0x5c:
				regs.d[0]=mulu(regs.d[0], regs.d[3]);
				cycle-=12;
				break;
			case 0x5d:
				regs.d[1]=mulu(regs.d[1], regs.d[3]);
				cycle-=12;
				break;
			case 0x5e:
				regs.d[2]=mulu(regs.d[2], regs.d[3]);
				cycle-=12;
				break;
			case 0x5f:
				regs.d[3]=mulu(regs.d[3], regs.d[3]);
				cycle-=12;
				break;
/* DIVU Dn,Dm (MDR<<16+Dm)/Dn ¢ªDm -??0/?o/?0/1 ?o/?o/?2 13 F3:60+Dn<<2+Dm *14 80
 *            ...MDR
 */
			case 0x60:
				regs.d[0]=divu(regs.d[0], regs.d[0]);
				cycle-=13;
				break;
			case 0x61:
				regs.d[1]=divu(regs.d[1], regs.d[0]);
				cycle-=13;
				break;
			case 0x62:
				regs.d[2]=divu(regs.d[2], regs.d[0]);
				cycle-=13;
				break;
			case 0x63:
				regs.d[3]=divu(regs.d[3], regs.d[0]);
				cycle-=13;
				break;
			case 0x64:
				regs.d[0]=divu(regs.d[0], regs.d[1]);
				cycle-=13;
				break;
			case 0x65:
				regs.d[1]=divu(regs.d[1], regs.d[1]);
				cycle-=13;
				break;
			case 0x66:
				regs.d[2]=divu(regs.d[2], regs.d[1]);
				cycle-=13;
				break;
			case 0x67:
				regs.d[3]=divu(regs.d[3], regs.d[1]);
				cycle-=13;
				break;
			case 0x68:
				regs.d[0]=divu(regs.d[0], regs.d[2]);
				cycle-=13;
				break;
			case 0x69:
				regs.d[1]=divu(regs.d[1], regs.d[2]);
				cycle-=13;
				break;
			case 0x6a:
				regs.d[2]=divu(regs.d[2], regs.d[2]);
				cycle-=13;
				break;
			case 0x6b:
				regs.d[3]=divu(regs.d[3], regs.d[2]);
				cycle-=13;
				break;
			case 0x6c:
				regs.d[0]=divu(regs.d[0], regs.d[3]);
				cycle-=13;
				break;
			case 0x6d:
				regs.d[1]=divu(regs.d[1], regs.d[3]);
				cycle-=13;
				break;
			case 0x6e:
				regs.d[2]=divu(regs.d[2], regs.d[3]);
				cycle-=13;
				break;
			case 0x6f:
				regs.d[3]=divu(regs.d[3], regs.d[3]);
				cycle-=13;
				break;
/* 0x70 ~ 0x8f ¤Ê¤· */
/* CMP Dn,Dm Dm- Dn -oooooooo2 2 F3:90+Dn<<2+Dm*/
			case 0x90:
				sub(regs.d[0], regs.d[0]);
				cycle-=2;
				break;
			case 0x91:
				sub(regs.d[1], regs.d[0]);
				cycle-=2;
				break;
			case 0x92:
				sub(regs.d[2], regs.d[0]);
				cycle-=2;
				break;
			case 0x93:
				sub(regs.d[3], regs.d[0]);
				cycle-=2;
				break;
			case 0x94:
				sub(regs.d[0], regs.d[1]);
				cycle-=2;
				break;
			case 0x95:
				sub(regs.d[1], regs.d[1]);
				cycle-=2;
				break;
			case 0x96:
				sub(regs.d[2], regs.d[1]);
				cycle-=2;
				break;
			case 0x97:
				sub(regs.d[3], regs.d[1]);
				cycle-=2;
				break;
			case 0x98:
				sub(regs.d[0], regs.d[2]);
				cycle-=2;
				break;
			case 0x99:
				sub(regs.d[1], regs.d[2]);
				cycle-=2;
				break;
			case 0x9a:
				sub(regs.d[2], regs.d[2]);
				cycle-=2;
				break;
			case 0x9b:
				sub(regs.d[3], regs.d[2]);
				cycle-=2;
				break;
			case 0x9c:
				sub(regs.d[0], regs.d[3]);
				cycle-=2;
				break;
			case 0x9d:
				sub(regs.d[1], regs.d[3]);
				cycle-=2;
				break;
			case 0x9e:
				sub(regs.d[2], regs.d[3]);
				cycle-=2;
				break;
			case 0x9f:
				sub(regs.d[3], regs.d[3]);
				cycle-=2;
				break;
/* 0xa0 ~ 0xbf ¤Ê¤· */
/* MOV Dn,MDR Dn ¢ªMDR ---------2 2 F3:C0+Dn<<2 */
			case 0xc0:
				regs.mdr=regs.d[0];
				cycle-=2;
				break;
			case 0xc4:
				regs.mdr=regs.d[1];
				cycle-=2;
				break;
			case 0xc8:
				regs.mdr=regs.d[2];
				cycle-=2;
				break;
			case 0xcc:
				regs.mdr=regs.d[3];
				cycle-=2;
				break;
/* EXT Dn Dn.bp15=0 ¤Ê¤é¤ÐS --------2 3 F3:C1+Dn<<2 *6 61
 * x'0000'¢ªMDR
 * Dn.bp15=1 ¤Ê¤é¤Ð
 * x'FFFF'¢ªMDR
 */
			case 0xc1:
				regs.mdr=ext(regs.d[0]);
				cycle-=3;
				break;
			case 0xc5:
				regs.mdr=ext(regs.d[1]);
				cycle-=3;
				break;
			case 0xc9:
				regs.mdr=ext(regs.d[2]);
				cycle-=3;
				break;
			case 0xcd:
				regs.mdr=ext(regs.d[3]);
				cycle-=3;
				break;
/* MOV Dn,PSW Dn ¢ªPSW -oooooooo2 3 F3:D0+Dn<<2 */
			case 0xd0:
				regs.psw.row=regs.d[0];
				cycle-=3;
				break;
			case 0xd4:
				regs.psw.row=regs.d[1];
				cycle-=3;
				break;
			case 0xd8:
				regs.psw.row=regs.d[2];
				cycle-=3;
				break;
			case 0xdc:
				regs.psw.row=regs.d[3];
				cycle-=3;
				break;
/* MOV MDR,Dn MDR ¢ªDn 0 --------2 2 F3:E0+Dn */
			case 0xe0:
				regs.d[0]=regs.mdr;
				cycle-=2;
				break;
			case 0xe1:
				regs.d[1]=regs.mdr;
				cycle-=2;
				break;
			case 0xe2:
				regs.d[2]=regs.mdr;
				cycle-=2;
				break;
			case 0xe3:
				regs.d[3]=regs.mdr;
				cycle-=2;
				break;
/* NOT Dn Dn^ x'00FFFF'¢ªDn -----0 0 oo2 2 F3:E4+Dn */
			case 0xe4:
				regs.d[0]=_not(regs.d[0]);
				cycle-=2;
				break;
			case 0xe5:
				regs.d[1]=_not(regs.d[1]);
				cycle-=2;
				break;
			case 0xe6:
				regs.d[2]=_not(regs.d[2]);
				cycle-=2;
				break;
			case 0xe7:
				regs.d[3]=_not(regs.d[3]);
				cycle-=2;
				break;
/* MOV PSW,Dn PSW ¢ªDn 0 --------2 2 F3:F0+Dn */
			case 0xf0:
				regs.d[0]=regs.psw.row;
				cycle-=2;
				break;
			case 0xf1:
				regs.d[1]=regs.psw.row;
				cycle-=2;
				break;
			case 0xf2:
				regs.d[2]=regs.psw.row;
				cycle-=2;
				break;
			case 0xf3:
				regs.d[3]=regs.psw.row;
				cycle-=2;
				break;
			}
			break;
		case 0xf4:
			op_code=mem->readAbsolute8(regs.pc++);

			switch(op_code)
			{
			case 0x48:
			case 0x49:
			case 0x4a:
			case 0x4b:
			case 0x4c:
			case 0x4d:
			case 0x4e:
			case 0x4f:
			case 0x54:
			case 0x55:
			case 0x56:
			case 0x57:
			case 0x58:
			case 0x59:
			case 0x5a:
			case 0x5b:
			case 0x5c:
			case 0x5d:
			case 0x5e:
			case 0x5f:
			case 0xcc:
			case 0xcd:
			case 0xce:
			case 0xcf:
			case 0xd4:
			case 0xd5:
			case 0xd6:
			case 0xd7:
			case 0xd8:
			case 0xd9:
			case 0xda:
			case 0xdb:
			case 0xdc:
			case 0xdd:
			case 0xde:
			case 0xdf:
			case 0xe2:
			case 0xe3:
			case 0xe4:
			case 0xe5:
			case 0xe6:
			case 0xe7:
			case 0xe8:
			case 0xe9:
			case 0xea:
			case 0xeb:
			case 0xec:
			case 0xed:
			case 0xee:
			case 0xef:
				undefined_instruction();
				break;


/* MOV Dm,(d24,An) Dm ¢ªmem16(An+d24)---------5 3 F4:00+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0x00:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x01:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x02:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x03:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0x04:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x05:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x06:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x07:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0x08:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x09:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x0a:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x0b:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0x0c:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x0d:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x0e:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x0f:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
/* MOV Am,(d24,An)Am ¢ªmem24(An+d24)---------5 4 F4:10+An<<2+Am:d24- l:d24- m:d24- h */
			case 0x10:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x11:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x12:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x13:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x14:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x15:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x16:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x17:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x18:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x19:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x1a:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x1b:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0x1c:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x1d:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x1e:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x1f:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
/* MOVB Dm,(d24,An)Dm ¢ªmem8(An+d24)---------5 3 F4:20+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0x20:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x21:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x22:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x23:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x24:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x25:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x26:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x27:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x28:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x29:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x2a:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x2b:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x2c:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x2d:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x2e:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x2f:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
/* MOVX Dm,(d24,An)Dm ¢ªmem24(An+d24)---------5 4 F4:30+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0x30:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x31:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x32:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x33:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x34:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x35:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x36:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x37:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x38:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x39:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x3a:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x3b:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x3c:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x3d:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x3e:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x3f:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
/* MOV Dn,(abs24)Dn ¢ªmem16(abs24)---------5 3 F4:40+Dn:abs24- l:abs24- m:abs24- h */
			case 0x40:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x41:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x42:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x43:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
/* MOVB Dn,(abs24)Dn ¢ªmem8(abs24)---------5 3 F4:44+Dn:abs24- l:abs24- m:abs24- h */
			case 0x44:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x45:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x46:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x47:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
		/***** 0x48 ¡Á 0x4f ¤Þ¤Ç¤ÏÇ§¤á¤Ê¤¤ ******/
/* MOV An,(abs24)An ¢ªmem24(abs24)---------5 4 F4:50+An:abs24- l:abs24- m:abs24- h */
			case 0x50:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x51:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x52:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x53:
				effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
		/****** 0x54 ¡Á 0x5f ¤ÏÇ§¤á¤Ê¤¤ *****/
/* ADD imm24,Dn Dn+imm24 ¢ªDn -oooooooo5 3 F4:60+Dn:imm24- l:imm24- m:imm24- h 69 */
			case 0x60:
				regs.d[0]=add(regs.d[0], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x61:
				regs.d[1]=add(regs.d[1], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x62:
				regs.d[2]=add(regs.d[2], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x63:
				regs.d[3]=add(regs.d[3], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
/* ADD imm24,An An+imm24 ¢ªAn -oooooooo5 3 F4:64+An:imm24- l:imm24- m:imm24- h */
			case 0x64:
				regs.a[0]=add(regs.a[0], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x65:
				regs.a[1]=add(regs.a[1], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x66:
				regs.a[2]=add(regs.a[2], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x67:
				regs.a[3]=add(regs.a[3], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
/* SUB imm24,Dn Dn- imm24 ¢ªDn -oooooooo5 3 F4:68+Dn:imm24- l:imm24- m:imm24- h */
			case 0x68:
				regs.d[0]=sub(regs.d[0], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x69:
				regs.d[1]=sub(regs.d[1], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x6a:
				regs.d[2]=sub(regs.d[2], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x6b:
				regs.d[3]=sub(regs.d[3], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
/* SUB imm24,An An- imm24 ¢ªAn -oooooooo5 3 F4:6C+An:imm24- l:imm24- m:imm24- h */
			case 0x6c:
				regs.a[0]=sub(regs.a[0], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x6d:
				regs.a[1]=sub(regs.a[1], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x6e:
				regs.a[2]=sub(regs.a[2], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x6f:
				regs.a[3]=sub(regs.a[3], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
/* MOV imm24,Dn imm24 ¢ªDn ---------5 3 F4:70+Dn:imm24- l:imm24- m:imm24- h*/
			case 0x70:
				regs.d[0]=mem->readImmediate24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x71:
				regs.d[1]=mem->readImmediate24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x72:
				regs.d[2]=mem->readImmediate24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x73:
				regs.d[3]=mem->readImmediate24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
/* MOV imm24,An imm24 ¢ªAn ---------5 3 F4:74+An:imm24- l:imm24- m:imm24- h 46 */
			case 0x74:
				regs.a[0]=mem->readAbsolute24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x75:
				regs.a[1]=mem->readAbsolute24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x76:
				regs.a[2]=mem->readAbsolute24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x77:
				regs.a[3]=mem->readAbsolute24(regs.pc);
				regs.pc+=3;
				cycle-=3;
				break;
/* CMP imm24,Dn Dn- imm24 -oooooooo5 3 F4:78+Dn:imm24- l:imm24- m:imm24- h */
			case 0x78:
				sub(regs.d[0], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x79:
				sub(regs.d[1], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x7a:
				sub(regs.d[2], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x7b:
				sub(regs.d[3], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
/* CMP imm24,An An- imm24 -oooooooo5 3 F4:7C+An:imm24- l:imm24- m:imm24- h */
			case 0x7c:
				sub(regs.a[0], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x7d:
				sub(regs.a[1], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x7e:
				sub(regs.a[2], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
			case 0x7f:
				sub(regs.a[3], mem->readImmediate24(regs.pc));
				regs.pc+=3;
				cycle-=3;
				break;
/* MOV (d24,An),Dm mem16(An+d24)¢ªDm S --------5 3 F4:80+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0x80:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x81:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x82:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x83:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x84:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x85:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x86:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x87:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x88:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x89:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x8a:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x8b:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0x8c:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0x8d:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0x8e:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0x8f:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
/* MOVBU (d24,An),Dm mem8(An+d24)¢ªDm 0 --------5 3 F4:90+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0x90:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x91:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x92:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x93:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x94:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x95:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x96:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x97:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x98:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x99:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9a:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9b:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9c:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9d:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9e:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x9f:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
/* MOVB (d24,An),Dm mem8(An+d24)¢ªDm S --------5 3 F4:A0+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0xa0:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xa1:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xa2:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xa3:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0xa4:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xa5:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xa6:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xa7:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0xa8:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xa9:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xaa:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xab:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0xac:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xad:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xae:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xaf:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				regs.pc+=3;
				cycle-=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
/* MOVX (d24,An),Dm mem24(An+d24)¢ªDm ---------5 4 F4:B0+An<<2+Dm:d24- l:d24- m:d24- h */
			case 0xb0:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0xb1:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0xb2:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0xb3:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0xb4:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0xb5:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0xb6:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0xb7:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0xb8:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0xb9:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0xba:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0xbb:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0xbc:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0xbd:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0xbe:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0xbf:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
/* MOV (abs24),Dn mem16(abs24)¢ªDn S --------5 3 F4:C0+Dn:abs24- l:abs24- m:abs24- h 33 */
			case 0xc0:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0xc1:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0xc2:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0xc3:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
/* MOVB (abs24),Dn mem8(abs24)¢ªDn S --------5 3 F4:C4+Dn:abs24- l:abs24- m:abs24- h */
			case 0xc4:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xc5:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xc6:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xc7:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
/* MOVBU (abs24),Dn mem8(abs24)¢ªDn 0 --------5 3 F4:C8+Dn:abs24- l:abs24- m:abs24- h */
			case 0xc8:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0xc9:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0xca:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0xcb:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle-=2;
				regs.pc+=3;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
	/********** 0xcc ~ 0xcf ¤Ê¤· ****/
/* MOV (abs24),An mem24(abs24)¢ªAn ---------5 4 F4:D0+An:abs24- l:abs24- m:abs24- h 36 */
			case 0xd0:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xd1:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xd2:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xd3:
			        effectiveAddress = mem->readAbsolute24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
	/* 0xd4 ~ 0xdf ¤Ê¤· */
/* JMP label24 PC+5+d24(label24)¢ªPC ---------5 4 F4:E0:d24- l:d24- m:d24- h */
			case 0xe0:
				regs.pc=regs.pc+mem->readDisplacement24(regs.pc);
				regs.pc+=3;
				cycle-=4;
				break;
/* JSR label24 A3- 4 ¢ªA3 ---------5 5F4:E1:d24- l:d24- m:d24- h
 *             PC+5 ¢ªmem24(A3)
 *          PC+5+d24(label24)¢ªPC
 */
			case 0xe1:
				regs.a[3]-=4;
				mem->write24(regs.a[3], regs.pc+3);
				regs.pc=regs.pc+mem->readDisplacement24(regs.pc);
				cycle-=5;
				regs.pc+=3;
				break;
		/* 0xe2 ~ 0xef ¤Ê¤· */
/* MOV (d24,An),Am mem24(An+d24)¢ªAm ---------5 4 F4:F0+An<<2+Am:d24- l:d24- m:d24- h */
			case 0xf0:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xf1:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xf2:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xf3:
				effectiveAddress = regs.a[0]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0xf4:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xf5:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xf6:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xf7:
				effectiveAddress = regs.a[1]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0xf8:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xf9:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xfa:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xfb:
				effectiveAddress = regs.a[2]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0xfc:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xfd:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xfe:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xff:
				effectiveAddress = regs.a[3]+mem->readDisplacement24(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle-=2;
				regs.pc+=3;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			}	/* end 1st byte 0xf4 */
			break;
		case 0xf5:
			op_code=mem->readAbsolute8(regs.pc++);
			switch(op_code)
			{
			case 0x40:
			case 0x41:
			case 0x42:
			case 0x43:
			case 0x44:
			case 0x45:
			case 0x46:
			case 0x47:
			case 0x48:
			case 0x49:
			case 0x4a:
			case 0x4b:
			case 0x4c:
			case 0x4d:
			case 0x4e:
			case 0x4f:
			case 0x60:
			case 0x61:
			case 0x62:
			case 0x63:
			case 0x64:
			case 0x65:
			case 0x66:
			case 0x67:
			case 0x68:
			case 0x69:
			case 0x6a:
			case 0x6b:
			case 0x6c:
			case 0x6d:
			case 0x6e:
			case 0x6f:
			case 0x80:
			case 0x81:
			case 0x82:
			case 0x83:
			case 0x84:
			case 0x85:
			case 0x86:
			case 0x87:
			case 0x88:
			case 0x89:
			case 0x8a:
			case 0x8b:
			case 0x8c:
			case 0x8d:
			case 0x8e:
			case 0x8f:
			case 0x90:
			case 0x91:
			case 0x92:
			case 0x93:
			case 0x94:
			case 0x95:
			case 0x96:
			case 0x97:
			case 0x98:
			case 0x99:
			case 0x9a:
			case 0x9b:
			case 0x9c:
			case 0x9d:
			case 0x9e:
			case 0x9f:
			case 0xa0:
			case 0xa1:
			case 0xa2:
			case 0xa3:
			case 0xa4:
			case 0xa5:
			case 0xa6:
			case 0xa7:
			case 0xa8:
			case 0xa9:
			case 0xaa:
			case 0xab:
			case 0xac:
			case 0xad:
			case 0xae:
			case 0xaf:
			case 0xb0:
			case 0xb1:
			case 0xb2:
			case 0xb3:
			case 0xb4:
			case 0xb5:
			case 0xb6:
			case 0xb7:
			case 0xb8:
			case 0xb9:
			case 0xba:
			case 0xbb:
			case 0xbc:
			case 0xbd:
			case 0xbe:
			case 0xbf:
			case 0xc0:
			case 0xc1:
			case 0xc2:
			case 0xc3:
			case 0xc4:
			case 0xc5:
			case 0xc6:
			case 0xc7:
			case 0xc8:
			case 0xc9:
			case 0xca:
			case 0xcb:
			case 0xcc:
			case 0xcd:
			case 0xce:
			case 0xcf:
			case 0xd0:
			case 0xd1:
			case 0xd2:
			case 0xd3:
			case 0xd4:
			case 0xd5:
			case 0xd6:
			case 0xd7:
			case 0xd8:
			case 0xd9:
			case 0xda:
			case 0xdb:
			case 0xdc:
			case 0xdd:
			case 0xde:
			case 0xdf:
			case 0xea:
			case 0xeb:
			case 0xf0:
			case 0xf1:
			case 0xf2:
			case 0xf3:
			case 0xf4:
			case 0xf5:
			case 0xf6:
			case 0xf7:
			case 0xf8:
			case 0xf9:
			case 0xfa:
			case 0xfb:
				undefined_instruction();
				break;


/* AND imm8,Dn Dn&(x'FF0000'|imm8)¢ªDn 0 ----0 0 oo3 2 F5:00+Dn:imm8 */
			case 0x00:
				regs.d[0]=_and(regs.d[0], mem->readAbsolute8(regs.pc++) | 0xff0000);
				cycle-=2;
				break;
			case 0x01:
				regs.d[1]=_and(regs.d[1], mem->readAbsolute8(regs.pc++) | 0xff0000);
				cycle-=2;
				break;
			case 0x02:
				regs.d[2]=_and(regs.d[2], mem->readAbsolute8(regs.pc++) | 0xff0000);
				cycle-=2;
				break;
			case 0x03:
				regs.d[3]=_and(regs.d[3], mem->readAbsolute8(regs.pc++) | 0xff0000);
				cycle-=2;
				break;
/* BTST imm8,Dn Dn&imm8 ... PSW 0 ----0 0 0 o3 2 F5:04+Dn:imm8 */
			case 0x04:
				_and(regs.d[0], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
			case 0x05:
				_and(regs.d[1], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
			case 0x06:
				_and(regs.d[2], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
			case 0x07:
				_and(regs.d[3], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
/* OR imm8,Dn Dn imm8 ¢ªDn 0 ----0 0 oo3 2 F5:08+Dn:imm8 */
			case 0x08:
				regs.d[0]=_or(regs.d[0], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
			case 0x09:
				regs.d[1]=_or(regs.d[1], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
			case 0x0a:
				regs.d[2]=_or(regs.d[2], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
			case 0x0b:
				regs.d[3]=_or(regs.d[3], mem->readAbsolute8(regs.pc++));
				cycle-=2;
				break;
/* ADDNF imm8,An An+imm8 ¢ªAn S --------3 2 F5:0C+An:imm8 */
			case 0x0c:
				regs.a[0]=addnf(regs.a[0], mem->readImmediate8(regs.pc++));
				cycle-=2;
				break;
			case 0x0d:
				regs.a[1]=addnf(regs.a[1], mem->readImmediate8(regs.pc++));
				cycle-=2;
				break;
			case 0x0e:
				regs.a[2]=addnf(regs.a[2], mem->readImmediate8(regs.pc++));
				cycle-=2;
				break;
			case 0x0f:
				regs.a[3]=addnf(regs.a[3], mem->readImmediate8(regs.pc++));
				cycle-=2;
				break;
/* MOVB Dm,(d8,An)Dm ¢ªmem8(An+d8)---------3 2 F5:10+An<<2+Dm:d8 */
			case 0x10:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x11:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x12:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x13:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x14:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x15:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x16:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x17:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x18:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x19:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x1a:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x1b:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x1c:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x1d:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x1e:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x1f:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				mem->write8(effectiveAddress, regs.d[3]);
				break;

/* MOVB (d8,An),Dm mem8(An+d8)¢ªDm S --------3 2 F5:20+An<<2+Dm:d8 */
			case 0x20:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x21:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x22:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x23:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x24:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x25:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x26:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x27:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x28:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x29:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x2a:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x2b:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0x2c:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0x2d:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0x2e:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0x2f:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
/* MOVBU (d8,An),Dm mem8(An+d8)¢ªDm 0 --------3 2 F5:30+An<<2+Dm:d8 57 */
			case 0x30:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x31:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x32:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x33:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x34:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x35:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x36:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x37:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x38:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x39:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x3a:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x3b:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x3c:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x3d:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x3e:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x3f:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
/* MOVX Dm,(d8,An)Dm ¢ªmem24(An+d8)---------3 3 F5:50+An<<2+Dm:d8 */
			case 0x50:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x51:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x52:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x53:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x54:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x55:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x56:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x57:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x58:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x59:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x5a:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x5b:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x5c:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x5d:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x5e:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x5f:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				mem->write24(effectiveAddress, regs.d[3]);
				break;

/* MOVX (d8,An),Dm mem24(An+d8)¢ªDm ---------3 3 F5:70+An<<2+Dm:d8 */
			case 0x70:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x71:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x72:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x73:
				effectiveAddress = regs.a[0]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0x74:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x75:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x76:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x77:
				effectiveAddress = regs.a[1]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0x78:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x79:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x7a:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x7b:
				effectiveAddress = regs.a[2]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0x7c:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x7d:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x7e:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x7f:
				effectiveAddress = regs.a[3]+mem->readDisplacement8(regs.pc++);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
/*
 * PC+3+d8(label)¢ªPC
 *or.
 * PC+3 ¢ªPC
 */
			case 0xe0:
/* BLTX label (VX^ NX)=1 ¤Ê¤é¤Ð---------3 3/2 F5:E0:d8 */
				if(regs.psw.bit.vx ^ regs.psw.bit.nx)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;
				regs.pc++;
				break;
			case 0xe1:
/* BGTX label ((VX^ NX)|ZX)=0 ¤Ê¤é¤Ð---------3 3/2 F5:E1:d8 */
				if(((regs.psw.bit.vx ^ regs.psw.bit.nx) | regs.psw.bit.zx) == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;
				regs.pc++;
				break;
			case 0xe2:
/* BGEX label (VX^ NX)=0 ¤Ê¤é¤Ð---------3 3/2 F5:E2:d8 */
				if((regs.psw.bit.vx ^ regs.psw.bit.nx) == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;
				regs.pc++;
				break;
			case 0xe3:
/* BLEX label ((VX^ NX)|ZX)=1 ¤Ê¤é¤Ð---------3 3/2 F5:E3:d8 */
				if((regs.psw.bit.vx ^ regs.psw.bit.nx) | regs.psw.bit.zx)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;
				regs.pc++;
				break;
			case 0xe4:
/* BCSX label CX=1 ¤Ê¤é¤Ð---------3 3/2 F5:E4:d8 */
				if(regs.psw.bit.cx)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;
				regs.pc++;
				break;
			case 0xe5:
/* BHIX label (CX | ZX)=0 ¤Ê¤é¤Ð---------3 3/2 F5:E5:d8 */
				if((regs.psw.bit.cx | regs.psw.bit.zx) == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;
				regs.pc++;
				break;
			case 0xe6:
/* BCCX label CX=0 ¤Ê¤é¤Ð---------3 3/2 F5:E6:d8 */
				if(regs.psw.bit.cx == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xe7:
/* BLSX label (CX | ZX)=1 ¤Ê¤é¤Ð---------3 3/2 F5:E7:d8 */
				if(regs.psw.bit.cx == 1 || regs.psw.bit.zx == 1)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;

			case 0xe8:
/* BEQX label ZX=1 ¤Ê¤é¤Ð---------3 3/2 F5:E8:d8 *31 107 */
				if(regs.psw.bit.zx)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xe9:
/* BNEX label ZX=0 ¤Ê¤é¤Ð---------3 3/2 F5:E9:d8 */
				if(regs.psw.bit.zx == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xec:
/* BVCX label VX=0 ¤Ê¤é¤Ð---------3 3/2 F5:EC:d8 */
				if(regs.psw.bit.vx == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xed:
/* BVSX label VX=1 ¤Ê¤é¤Ð---------3 3/2 F5:ED:d8 */
				if(regs.psw.bit.vx)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xee:
/* BNCX label NX=0 ¤Ê¤é¤Ð---------3 3/2 F5:EE:d8 */
				if(regs.psw.bit.nx == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xef:
/* BNSX label NX=1 ¤Ê¤é¤Ð---------3 3/2 F5:EF:d8 */
				if(regs.psw.bit.nx == 1)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xfc:
/* BVC label VF=0 ¤Ê¤é¤Ð---------3 3/2 F5:FC:d8 */
				if(regs.psw.bit.vf == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xfd:
/* BVS label VF=1 ¤Ê¤é¤Ð---------3 3/2 F5:FD:d8 */
				if(regs.psw.bit.vf)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xfe:
/* BNC label NF=0 ¤Ê¤é¤Ð---------3 3/2 F5:FE:d8 */
				if(regs.psw.bit.nf == 0)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;
			case 0xff:
/* BNS label NF=1 ¤Ê¤é¤Ð---------3 3/2 F5:FF:d8 */
				if(regs.psw.bit.nf)
				{
					regs.pc+=mem->readDisplacement8(regs.pc);
					cycle-=3;
				}
				else
					cycle-=2;

				regs.pc++;
				break;

			}
			break;

/* NOP PC+1 ¢ªPC ---------1 1 F6 */
		case 0xf6:
			cycle--;
			break;

		case 0xf7:
			op_code=mem->readAbsolute8(regs.pc++);
			switch(op_code)
			{
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x15:
			case 0x16:
			case 0x17:
			case 0x24:
			case 0x25:
			case 0x26:
			case 0x27:
			case 0x28:
			case 0x29:
			case 0x2a:
			case 0x2b:
			case 0x2c:
			case 0x2d:
			case 0x2e:
			case 0x2f:
			case 0x34:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x38:
			case 0x39:
			case 0x3a:
			case 0x3b:
			case 0x3c:
			case 0x3d:
			case 0x3e:
			case 0x3f:
			case 0x44:
			case 0x45:
			case 0x46:
			case 0x47:
			case 0xe0:
			case 0xe1:
			case 0xe2:
			case 0xe3:
			case 0xe4:
			case 0xe5:
			case 0xe6:
			case 0xe7:
			case 0xe8:
			case 0xe9:
			case 0xea:
			case 0xeb:
			case 0xec:
			case 0xed:
			case 0xee:
			case 0xef:
			case 0xf0:
			case 0xf1:
			case 0xf2:
			case 0xf3:
			case 0xf4:
			case 0xf5:
			case 0xf6:
			case 0xf7:
			case 0xf8:
			case 0xf9:
			case 0xfa:
			case 0xfb:
			case 0xfc:
			case 0xfd:
			case 0xfe:
			case 0xff:
				undefined_instruction();
				break;


/* AND imm16,Dn Dn&(x'FF0000'imm16)¢ªDn -----0 0 oo4 2 F7:00+Dn:imm16- l:imm16- h */
			case 0x00:
				regs.d[0]=_and(regs.d[0], 0xff0000 | mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x01:
				regs.d[1]=_and(regs.d[1], 0xff0000 | mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x02:
				regs.d[2]=_and(regs.d[2], 0xff0000 | mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x03:
				regs.d[3]=_and(regs.d[3], 0xff0000 | mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
/* BTST imm16,Dn Dn&imm16 ... PSW 0 ----0 0 oo4 2 F7:04+Dn:imm16- l:imm16- h */
			case 0x04:
				_and(regs.d[0], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x05:
				_and(regs.d[1], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x06:
				_and(regs.d[2], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x07:
				_and(regs.d[3], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
/* ADD imm16,An An+imm16 ¢ªAn S oooooooo4 2 F7:08+An:imm16- l:imm16- h */
			case 0x08:
				regs.a[0]=add(regs.a[0], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x09:
				regs.a[1]=add(regs.a[1], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x0a:
				regs.a[2]=add(regs.a[2], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x0b:
				regs.a[3]=add(regs.a[3], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
/* SUB imm16,An An- imm16 ¢ªAn S oooooooo4 2 F7:0C+An:imm16- l:imm16- h */
			case 0x0c:
				regs.a[0]=sub(regs.a[0], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x0d:
				regs.a[1]=sub(regs.a[1], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x0e:
				regs.a[2]=sub(regs.a[2], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x0f:
				regs.a[3]=sub(regs.a[3], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;

/* AND imm16,PSW PSW&imm16 ¢ªPSW -oooooooo4 3 F7:10:imm16- l:imm16- h */
			case 0x10:
				regs.psw.row&=mem->readAbsolute16(regs.pc);
				regs.pc+=2;
				cycle-=3;
				break;
/*OR imm16,PSW PSW | imm16 ¢ªPSW -oooooooo4 3 F7:14:imm16- l:imm16- h */
			case 0x14:
				regs.psw.row|=mem->readAbsolute16(regs.pc);
				regs.pc+=2;
				cycle-=3;
				break;

/* ADD imm16,Dn Dn+imm16 ¢ªDn S oooooooo4 2 F7:18+Dn:imm16- l:imm16- h */
			case 0x18:
				regs.d[0]=add(regs.d[0], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x19:
				regs.d[1]=add(regs.d[1], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x1a:
				regs.d[2]=add(regs.d[2], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x1b:
				regs.d[3]=add(regs.d[3], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
/* SUB imm16,Dn Dn- imm16 ¢ªDn S oooooooo4 2 F7:1C+Dn:imm16- l:imm16- h */
			case 0x1c:
				regs.d[0]=sub(regs.d[0], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x1d:
				regs.d[1]=sub(regs.d[1], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x1e:
				regs.d[2]=sub(regs.d[2], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x1f:
				regs.d[3]=sub(regs.d[3], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;

/* MOV An,(abs16)An ¢ªmem24(abs16)---------4 3 F7:20+An:abs16- l:abs16- h */
			case 0x20:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0x21:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0x22:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0x23:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
/* MOV (abs16),An mem24(abs16)¢ªAn ---------4 3 F7:30+An:abs16- l:abs16- h */
			case 0x30:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0x31:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0x32:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0x33:
			        effectiveAddress = mem->readAbsolute16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
/* OR imm16,Dn Dn|imm16 ¢ªDn -----0 0 oo4 2 F7:40+Dn:imm16- l:imm16- h */
			case 0x40:
				regs.d[0]=_or(regs.d[0], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x41:
				regs.d[1]=_or(regs.d[1], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x42:
				regs.d[2]=_or(regs.d[2], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x43:
				regs.d[3]=_or(regs.d[3], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
/* CMP imm16,Dn Dn- imm16 S oooooooo4 2 F7:48+Dn:imm16- l:imm16- h */
			case 0x48:
				sub(regs.d[0], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x49:
				sub(regs.d[1], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x4a:
				sub(regs.d[2], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x4b:
				sub(regs.d[3], mem->readImmediate16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;

/* XOR imm16,Dn Dn^ imm16 ¢ªDn -----0 0 oo4 2 F7:4C+Dn:imm16- l:imm16- h */
			case 0x4c:
				regs.d[0]=_xor(regs.d[0], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x4d:
				regs.d[1]=_xor(regs.d[1], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x4e:
				regs.d[2]=_xor(regs.d[2], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
			case 0x4f:
				regs.d[3]=_xor(regs.d[3], mem->readAbsolute16(regs.pc));
				regs.pc+=2;
				cycle-=2;
				break;
/* MOVBU (d16,An),Dm mem8(An+d16)¢ªDm 0 --------4 2 F7:50+An<<2+Dm:d16- l:d16- h */
			case 0x50:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x51:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x52:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x53:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x54:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x55:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x56:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x57:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x58:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x59:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x5a:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x5b:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
			case 0x5c:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_0e(effectiveAddress);
				break;
			case 0x5d:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_0e(effectiveAddress);
				break;
			case 0x5e:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_0e(effectiveAddress);
				break;
			case 0x5f:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_0e(effectiveAddress);
				break;
/* MOVX Dm,(d16,An)Dm ¢ªmem24(An+d16)---------4 3 F7:60+An<<2+Dm:d16- l:d16- h */
			case 0x60:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x61:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x62:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x63:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x64:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x65:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x66:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x67:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x68:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x69:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x6a:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x6b:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
			case 0x6c:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[0]);
				break;
			case 0x6d:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[1]);
				break;
			case 0x6e:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[2]);
				break;
			case 0x6f:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.d[3]);
				break;
/* MOVX (d16,An),Dm mem24(An+d16)¢ªDm ---------4 3 F7:70+An<<2+Dm:d16- l:d16- h */
			case 0x70:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x71:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x72:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x73:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0x74:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x75:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x76:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x77:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0x78:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x79:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x7a:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x7b:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
			case 0x7c:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read24(effectiveAddress);
				break;
			case 0x7d:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read24(effectiveAddress);
				break;
			case 0x7e:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read24(effectiveAddress);
				break;
			case 0x7f:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read24(effectiveAddress);
				break;
/* MOV Dm,(d16,An)Dm ¢ªmem16(An+d16)---------4 2 F7:80+An<<2+Dm:d16- l:d16- h */
			case 0x80:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x81:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x82:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x83:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0x84:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x85:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x86:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x87:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0x88:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x89:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x8a:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x8b:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
			case 0x8c:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[0]);
				break;
			case 0x8d:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[1]);
				break;
			case 0x8e:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[2]);
				break;
			case 0x8f:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				mem->write16(effectiveAddress, regs.d[3]);
				break;
/* MOVB Dm,(d16,An)Dm ¢ªmem8(An+d16)---------4 2 F7:90+An<<2+Dm:d16- l:d16- h */
			case 0x90:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x91:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x92:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x93:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x94:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x95:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x96:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x97:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x98:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x99:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x9a:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x9b:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[3]);
				break;
			case 0x9c:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[0]);
				break;
			case 0x9d:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[1]);
				break;
			case 0x9e:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[2]);
				break;
			case 0x9f:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				mem->write8(effectiveAddress, regs.d[3]);
				break;

/* MOV Am,(d16,An)Am ¢ªmem24(An+d16)---------4 3 F7:A0+An<<2+Am:d16- l:d16- h */
			case 0xa0:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xa1:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xa2:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xa3:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xa4:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xa5:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xa6:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xa7:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xa8:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xa9:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xaa:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xab:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
			case 0xac:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[0]);
				break;
			case 0xad:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[1]);
				break;
			case 0xae:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[2]);
				break;
			case 0xaf:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				mem->write24(effectiveAddress, regs.a[3]);
				break;
/* MOV (d16,An),Am mem24(An+d16)¢ªAm ---------4 3 F7:B0+An<<2+Am:d16- l:d16- h */
			case 0xb0:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xb1:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xb2:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xb3:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0xb4:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xb5:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xb6:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xb7:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0xb8:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xb9:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xba:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xbb:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
			case 0xbc:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[0]=mem->read24(effectiveAddress);
				break;
			case 0xbd:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[1]=mem->read24(effectiveAddress);
				break;
			case 0xbe:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[2]=mem->read24(effectiveAddress);
				break;
			case 0xbf:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 3);
				cycle--;
				regs.pc+=2;
				regs.a[3]=mem->read24(effectiveAddress);
				break;
/* MOV (d16,An),Dm mem16(An+d16)¢ªDm S --------4 2 F7:C0+An<<2+Dm:d16- l:d16- h */
			case 0xc0:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0xc1:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0xc2:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0xc3:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0xc4:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0xc5:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0xc6:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0xc7:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0xc8:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0xc9:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0xca:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0xcb:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
			case 0xcc:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read16_se(effectiveAddress);
				break;
			case 0xcd:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read16_se(effectiveAddress);
				break;
			case 0xce:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read16_se(effectiveAddress);
				break;
			case 0xcf:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 2);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read16_se(effectiveAddress);
				break;
/* MOVB (d16,An),Dm mem8(An+d16)¢ªDm S --------4 2 F7:D0+An<<2+Dm:d16- l:d16- h */
			case 0xd0:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xd1:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xd2:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xd3:
				effectiveAddress = regs.a[0]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0xd4:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xd5:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xd6:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xd7:
				effectiveAddress = regs.a[1]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0xd8:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xd9:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xda:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xdb:
				effectiveAddress = regs.a[2]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			case 0xdc:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[0]=mem->read8_se(effectiveAddress);
				break;
			case 0xdd:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[1]=mem->read8_se(effectiveAddress);
				break;
			case 0xde:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[2]=mem->read8_se(effectiveAddress);
				break;
			case 0xdf:
				effectiveAddress = regs.a[3]+mem->readDisplacement16(regs.pc);
				decrementCycleForMemoryAccess(effectiveAddress, 1);
				cycle--;
				regs.pc+=2;
				regs.d[3]=mem->read8_se(effectiveAddress);
				break;
			}
			break;
/* MOV imm16,Dn imm16 ¢ªDn S --------3 1 F8+Dn:imm16- l:imm16- h */
		case 0xf8:
			regs.d[0]=mem->readImmediate16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
		case 0xf9:
			regs.d[1]=mem->readImmediate16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
		case 0xfa:
			regs.d[2]=mem->readImmediate16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
		case 0xfb:
			regs.d[3]=mem->readImmediate16(regs.pc);
			regs.pc+=2;
			cycle--;
			break;
/* JMP label16 PC+3+d16(label16)¢ªPC ---------3 2 FC:d16- l:d16- h */
		case 0xfc:
			regs.pc+=mem->readDisplacement16(regs.pc);
			regs.pc+=2;
			cycle-=2;
			break;
/* JSR label16 A3- 4 ¢ªA3 ---------3 4 FD:d16- l:d16- h
 *             PC+3 ¢ªmem24(A3)
 *             PC+3+d16(label16)¢ªPC
 */
		case 0xfd:
			regs.a[3]-=4;

			effectiveAddress = regs.pc+2;
			decrementCycleForMemoryAccess(effectiveAddress, 3);
			cycle -= 2;

			mem->write24(regs.a[3], effectiveAddress);

			regs.pc+=mem->readDisplacement16(regs.pc)+2;
			break;
/* RTS mem24(A3)¢ªPC ---------1 5 FE
 *       A3+4 ¢ªA3
 */
		case 0xfe:
			decrementCycleForMemoryAccess(regs.a[3], 3);
			regs.pc=mem->read24(regs.a[3]);
			regs.a[3]+=4;
			cycle-=3;
			break;
		case 0xff:
			undefined_instruction();
			break;
		}
	}

	if( option->get_enable_cpu_timer() )
		mn10200_cpu_timer(c-cycle);
/*
	{
		int i;

		for(i=0; i < 4; i++)
			if((regs.a[i] >> 24) || (regs.d[i] >> 24))
				printf("24 bit error !!");
	}
*/


	return cycle;
}

/* Ì¤ÄêµÁÌ¿Îá¤Î¼Â¹Ô¤ËÂÐ¤·¤Æ¤Î½èÍý */
void Cpu::undefined_instruction(void) throw(UndefinedInstructionException)
{
	cycle = -1;

	throw UndefinedInstructionException(&regs);
}

/* tm0, tm1 ¤Î¤ß */
void Cpu::mn10200_cpu_timer(int c)
{

	struct tm_mode{
		unsigned char s:2;			/* src */
		unsigned char _reserve:4;
		unsigned char ld:1;
		unsigned char en:1;
	};

/*
 * TMnIO Ã¼»Ò¤ÎÈ¯¿¶¤Ï¡¢ºÇÄã°Ê²¼¤Î¤è¤¦¤Ë¤Ê¤Ã¤Æ¤¤¤ë¡£
 * 20MHz È¯¿¶»þ tcyc=100ns 10MHz È¯¿¶»þ tcyc=200ns
 * 20MHz 1 ¥µ¥¤¥¯¥ë 50 ns ¤Ä¤Þ¤ê¡¢ÇÜ¤ÇÈ¯¿¶¤¹¤ë¡£
 *
 * ¤À¤«¤é¡¢TMnIO ¤òÆþÎÏ¤Ë¤·¤Æ¤¤¤ë¥¿¥¤¥Þ¡¼0/1 ¤Ë¤Ä¤¤¤Æ¤Ï¡¢
 * ¡Ö»È¤Ã¤¿¥µ¥¤¥¯¥ë¤ÎÈ¾Ê¬¤ò°ú¤¤¤Æ¤ä¤ë¡×¤È¤¤¤¦¤³¤È¡£
 *
 *
 */
/* ¥¢¥É¥ì¥¹¤Ï¡¢MN10200 ¤Î»ÅÍÍ */
	static unsigned char * const tm_count	= (unsigned char *)Memory::getMemoryPointer(0xfe00);
	static unsigned char * const tm_base	= (unsigned char *)Memory::getMemoryPointer(0xfe10);
	static struct tm_mode * const tm_mode	= (struct tm_mode *)Memory::getMemoryPointer(0xfe20);

	static unsigned char * const ps_count	= (unsigned char *)Memory::getMemoryPointer(0xfe0a);
	static unsigned char * const ps_base	= (unsigned char *)Memory::getMemoryPointer(0xfe1a);
	static struct tm_mode * const ps_mode	= (struct tm_mode *)Memory::getMemoryPointer(0xfe2a);

	int ps_under_flow[2];
	int tm0_under_flow=0;
	int i;
	int tmp;

	for(i=0; i < 2; i++)
	{
		if(ps_mode[i].en == 0)
			continue;

/* ²¿²ó¥¢¥ó¥À¡¼¥Õ¥í¡¼¤·¤¿¡© */
		ps_under_flow[i]=c/ps_base[i];
		tmp=c%ps_base[i];

		if(ps_count[i] <= tmp)
		{
			ps_under_flow[i]++;
			ps_count[i]=ps_base[i]-(tmp-ps_count[i]);
		}
		else
			ps_count[i]-=tmp;

/*printf("ps %d: c:%d, b:%d, m:%x ... under flow %d\n", i, ps_count[i], ps_base[i], *(unsigned char *)&ps_mode[i], ps_under_flow[i]);
*/		}

	if(tm_mode[0].en == 1)
	{
		switch(tm_mode[0].s)

		{
		case 0:
			{
				int tmp2;
				static int odd;

				tmp=c/2;

				if((c & 1) == 1)
				{
					if(odd == 1)
					{
						tmp++;
						odd=0;
					}
					else
						odd=1;
				}

				tm0_under_flow=tmp/tm_base[0];
				tmp2=tmp%tm_base[0];
				if(tm_count[0] < tmp2)
				{
					tm0_under_flow++;
					tm_count[0]=tm_base[0]-(tmp2-tm_count[0]);
				}
				else
					tm_count[0]-=tmp2;
/*printf("tm0  c:%u, b:%u, m:%x ... under flow %d\n", tm_count[0], tm_base[0], *(unsigned char *)&tm_mode[0], tm0_under_flow);
*/				}
			break;
		case 1:
		case 2:
		case 3:
			break;
		}
	}
	if(tm_mode[1].en == 1)
	{
		switch(tm_mode[1].s)
		{
		case 0:
			break;
		case 1:
			if(tm0_under_flow != 0)
			{
				tmp=tm0_under_flow%tm_base[1];
				if(tm_count[1] < tmp)
				{
/* under flow */
/* ³ä¤ê¹þ¤ßÆþ¤ì¤ë¡£
 * ³ä¤ê¹þ¤ß¤Î¥¢¥É¥ì¥¹¤¬¤ï¤«¤é¤Ê¤¤¤Î¤Ç¡¢´ðËÜÅª¤ËÌµÍý¡£
 */
//printf("interrupt\n");
					tm_count[1]=tm_base[1]-(tmp-tm_count[1]);
				}
				else
					tm_count[1]-=tmp;
			}
/*printf("tm1  c:%u, b:%u, m:%x ... \n", tm_count[1], tm_base[1], *(unsigned char *)&tm_mode[1]);
*/				break;
		case 2:
		case 3:
			break;
		}
	}
	if(tm_mode[2].en == 1)
	{
	}
	if(tm_mode[3].en == 1)
	{
	}

}

/**
 * Register ¤òÊ¸»úÎó²½¤¹¤ë
 * @param str ½ÐÎÏÊ¸»úÎó
 */
void Register::toString(char *str)
{
	sprintf(str,
"\
d0:0x%06lx, d1:0x%06lx, d2:0x%06lx, d3:0x%06lx\n\
a0:0x%06lx, a1:0x%06lx, a2:0x%06lx, a3:0x%06lx\n\
pc:0x%06lx \n\
mdr:0x%04x, psw:0x%04x\n\
vx:%d cx:%d nx:%d zx:%d vf:%d cf:%d nf:%d zf:%d\n",
			d[0], d[1], d[2], d[3],
			a[0], a[1], a[2], a[3],
			pc,
			mdr, psw.row,
			psw.bit.vx, psw.bit.cx, psw.bit.nx, psw.bit.zx,
			psw.bit.vf, psw.bit.cf, psw.bit.nf, psw.bit.zf);
}


/**
 * ¥µ¥¤¥¯¥ë¿ô¤ò°ú¤¯
 * @param effaddr ¼Â¸ú¥¢¥É¥ì¥¹
 * @param n byte¿ô
 */
void Cpu::decrementCycleForMemoryAccess(unsigned long effaddr, int n)
{
	if( effaddr < 0x100000 )
	{
		switch(n)
		{
		case 1:
		case 2:
			cycle -= 1;
			break;
		case 3:
		default:
			cycle -= 2;
		}
	}
	else
	{
		switch(n)
		{
		case 1:
			cycle -= 2;
			break;
		case 2:
			cycle -= 4;
			break;
		case 3:
		default:
			cycle -= 6;
			break;
		}
	}
}

