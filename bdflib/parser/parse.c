/*
 * parse.c - To parse bdf font. (to call function bison generated)
 *
 * $Id: parse.c,v 1.4 2004/05/07 08:25:03 fumi Exp $
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

#include "bdffontset.h"


static int convert(unsigned char *buf, unsigned char *out, int width, int height);
static unsigned char convert_2byte(unsigned char a, unsigned char b);


void yyparse(void);

static BdfFontSet *global_bdffontset;
static int global_height;


extern FILE *yyin;

/** BDFファイルパーズ関数
 * @param bdffontset コールバックに使用する、該当のクラス
 * @param filename BDFファイル名
 * @param height BDFフォントの高さ(pixel)（BDFフォントの高さと一致していない場合は、切捨て／0足し）
 * @retval -1 失敗
 * @retval 0 成功
 */
int c_parse(BdfFontSet *bdffontset, char const *filename, int height)
{
	global_bdffontset=bdffontset;
	global_height=height;

	yyin=fopen(filename, "rb");
	if(yyin == NULL)
		return -1;

	global_bdffontset=bdffontset;

	yyparse();

	fclose(yyin);

	return 0;
}
/** パーザから呼ばれる関数
 * @param code 文字コード(JIS)
 * @param width 幅
 * @param buf bdf の文字のビット
 * @retval なし
 */
void kakuno(unsigned short code, int width, char *buf)
{
	unsigned char out[256]="";
	int len;

	len=convert((unsigned char *)buf, out, width, global_height);
	global_bdffontset->add(code, width, out, len);
}

/** bdf の文字のビットの変換を行なう
 * @param buf 入力（デリミタは改行(0x0a)）
 * @param out 出力
 * @param width 幅
 * @param height 高さ
 * @retval int outの使用した長さ
 */
static int convert(unsigned char *buf, unsigned char *out, int width, int height)
{
	int i;
	int byte_per_line=(width+7)/8;

	for(i=0; i < height && *buf;)
	{
		if(*buf == 0x0a)
		{
			buf++;
			i++;
		}
		else if(*(buf+1) == 0x0a)
		{
			*out=convert_2byte(*buf, '0');
			out++;
			buf+=2;
			i++;
		}
		else
		{
			*out=convert_2byte(*buf, *(buf+1));
			out++;
			buf+=2;
		}
	}

	return byte_per_line*height;
}

/** 16進数2文字を unsined char に変換する
 * @param a 上位 4bit（文字）
 * @param b 下位 4bit（文字）
 * @retval 変換した8bit
 */
static unsigned char convert_2byte(unsigned char a, unsigned char b)
{
	char temp[4];
	int i, val;

	temp[0]=a;
	temp[1]=b;
	temp[2]=0;
	return (unsigned char)strtoul(temp, NULL, 16);
}

/*
int main(void)
{
	yyparse();
}

*/

