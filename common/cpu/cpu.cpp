/*
 * cpu.cpp - Panasonic MN10200 CPU emulation module
 *
 * $Id: cpu.cpp,v 1.14 2004/11/13 18:35:47 fumi Exp $
 *
 * Copyright (C) 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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
#include <string.h>

#include "cpu.h"
#include "memorycpu.h"
#include "interruption.h"

/** コンストラクタ
 * @param _thread このインスタンスを作成したスレッド
 * @param pc プログラムカウンタ
 * @param sp スタックポインタ（pc を push 後 a3 に代入される）
 * @param interruption 割り込みクラス
 * @param mem メモリアクセスクラス（CPU 用）
 */
Cpu::Cpu(unsigned long pc, unsigned long sp, Interruption *interruption, MemoryForCpu *mem)
{
	memset(&regs, 0, sizeof(Register));

	this->mem = mem;
	this->interruption = interruption;

	regs.pc=pc;
	regs.a[3]=sp;

	regs.a[3]-=4;
	mem->write24(regs.a[3], regs.pc);

}
/** デストラクタ
 */
Cpu::~Cpu(void)
{
// interruption は Thread で作成しているがここで削除する必要あり
	delete interruption;
}


/** 今のレジスタの状態を代入する
 * @param str 代入するバッファのポインタ
 */
void Cpu::convRegsToString(char *str)
{
	regs.toString(str);
}


/** 今のレジスタの状態を標準エラー出力に出力する
 */
void Cpu::printRegs(void)
{
	char str[1024];
	convRegsToString(str);
	fputs(str, stderr);
}
/**
 * エミュレートする。
 * Register を保存できない例外
 * 現在では Memory はここで捕まえて、新しい 例外を throw する（インチキ？）。
 *
 */
int Cpu::emulate(void)
{
	int n;

	try {
		n = emulate_core();
	}
	catch(OddAccessException &e) {
		throw OddAccessException(&regs, e.getEffectiveAddress());
	}

	return n;
}
