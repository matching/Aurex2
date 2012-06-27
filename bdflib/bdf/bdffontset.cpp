/*
 * bdffontset.cpp - handle a BDF file
 *
 * $Id: bdffontset.cpp,v 1.3 2004/05/04 06:34:11 fumi Exp $
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

#include "bdffontset.h"
#include "bdffont.h"

#include "parse.h"

/** BDFを使用したフォントのラスタライズクラスコンストラクタ
 */
BdfFontSet::BdfFontSet(void)
{
}
/** ディストラクタ
 */
BdfFontSet::~BdfFontSet(void)
{
}

/** BDFファイルをパーズする
 * @param filename パーズするBDFファイル名
 * @retval -1 失敗
 * @retval  0 成功
 */
int BdfFontSet::parse(char const *filename, int height)
{
	int retval;

	retval=::c_parse((BdfFontSet *)this, filename, height);
	if(retval != 0)
		return -1;

	return 0;
}

/** 追加
 */
void BdfFontSet::add(unsigned short code, int width, unsigned char const *data, int data_len)
{
	BdfFont *bf;

	bf=new BdfFont();
	bf->width=width;
	bf->fontdata=new unsigned char[data_len];
	memcpy(bf->fontdata, data, data_len);
	bf->data_len=data_len;


#ifdef BDF_DEBUG
	{
		fprintf(stderr, "ADD code : %04x, %c%c data : ", code, (code >> 8) | 0x80, (code & 0xff) | 0x80);
		int i;
		for(i=0; i < data_len; i++)
			fprintf(stderr, "%02x, ", data[i]);
		fprintf(stderr, "\n");
	}
#endif



	this->data.insert(pair<unsigned short, BdfFont *>(code, bf));
}
/** 取得
 */
BdfFont *BdfFontSet::getBdfFont(unsigned short code)
{
	map<unsigned short, BdfFont *>::iterator i;

	i=data.find(code);
	if(i == data.end())
		return NULL;

	return i->second;
}

#if 0
int main(void)
{
	BdfFontSet *bfs;

	bfs=new BdfFontSet();
	bfs->parse("parser/foo.bdf", 12);


	BdfFont *bf;

	bf=bfs->getBdfFont(1);

	{
		int i;

		printf("width %d, len %d, data\n", bf->width, bf->data_len, bf->fontdata);
		for(i=0; i < bf->data_len; i++)
			printf("%02x, ", bf->fontdata[i]);
		putchar('\n');
	}



	return 0;



}

#endif
