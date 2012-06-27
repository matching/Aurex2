/*
 * exffile.cpp - utility for exf file
 *
 * $Id: exffile.cpp,v 1.3 2005/01/06 14:21:33 fumi Exp $
 *
 * Copyright (C) 2004, 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

#include <assert.h>

#include "memory.h"
#include "exffile.h"

/**
 * ExfFile のコンストラクタ
 * ファイルをオープンする
 * @param filename *.exf ファイルを仮定。拡張子の検査は行なわない
 */

ExfFile::ExfFile(char const *filename) throw(ExfFileNotFoundException)
{
	fp = fopen(filename, "rb");
	if(fp == NULL)
		throw ExfFileNotFoundException();
}
/**
 * デストラクタ
 */
ExfFile::~ExfFile(void)
{
	fclose(fp);
}
/**
 */
void ExfFile::loadToMemory(void) throw(ExfFileInvalidException)
{
	this->loadAddr = getLoadAddressFromFile();
	this->pc = getPCAddressFromFile();
	this->psp = loadAddr - 0x100;
	this->size = getFileSize();

	if( this->loadAddr + this->size > MEMORY_SIZE )
		throw ExfFileInvalidException();

	char *loadPointer = (char *)Memory::getMemoryPointer( this->loadAddr );

	fseek(fp, 0x10L, SEEK_SET);


	fread(loadPointer, 1, size - 0x10, fp);

	memset(loadPointer - 0x100, 0, 0x100); // PSP 領域のクリア
}

unsigned long ExfFile::getPc(void)
{
	return this->pc;
}

unsigned long ExfFile::getPsp(void)
{
	return this->psp;
}


unsigned long ExfFile::getFileSize(void)
{
	unsigned long size;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	return size;
}
unsigned long ExfFile::getLoadAddressFromFile(void) throw(ExfFileInvalidException)
{
	return getUnsignedLong(0);
}

unsigned long ExfFile::getPCAddressFromFile(void) throw(ExfFileInvalidException)
{
	return getUnsignedLong(4);
}

unsigned long ExfFile::getUnsignedLong(long offset) throw(ExfFileInvalidException)
{
	unsigned long res;
	int c;
	int i;

	fseek(fp, offset, SEEK_SET);

	res = 0;
	for(i = 0; i < 4; i++)
	{
		c = getc(fp);
		if(c == EOF)
			throw ExfFileInvalidException();

		res |= c << (i*8);
	}

	return res;
}

/*
 * +00h 	ロードアドレス
 * +04h 	初期PC値
 * +08h 	ファイルサイズ
 * +0Ch 	予約(NULL)
 * +10h 	プログラム本体 実際にはアイコンの領域を飛び越えるジャンプ命令
 * +16h 	(24 bytes) リストアイコン ( 12x12 )
 * +2eh 	mmp フォーマットを持つアイコン表示用 （MMP のパーザによる解析）
 * +96h 	実際のプログラムエリア
 */
bool ExfFile::getMiniIcon(unsigned char *out)
{
	unsigned char *icon;

	icon = (unsigned char *)Memory::getMemoryPointer(this->loadAddr + 0x16 - 0x10);

	return getMMPData(12, 12, icon, out);
}


bool ExfFile::getLargeIcon(unsigned char *out, int *width, int *height)
{
	unsigned char *icon;
	MemoryForInside mem(this->loadAddr + 0x2e - 0x10);

	*width = mem.read16_s(0);
	*height = mem.read16_s(2);

	icon = (unsigned char *)Memory::getMemoryPointer(this->loadAddr + 0x2e - 0x10 + 4);

	return getMMPData( *width, *height, icon, out);
}

bool ExfFile::getMMPData(int width, int height, unsigned char const *icon, unsigned char *out)
{
	int x, y;
	int i;

	for(y=0; y < height; y+=8)
	{
		x=0;
		for(; x < width; x++, icon++)
		{
			for(i = 0; i < 8 && y + i < height; i++)
				*(out + ( y + i ) * width + x ) = (*icon >> i) & 1;
		}
	}

#ifdef DEBUG
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
			putc( *(out + y * width + x) ? '@' : ' ' , stderr);
		putc('\n', stderr);
	}
	fflush(stdout);
#endif



	return true;
}
