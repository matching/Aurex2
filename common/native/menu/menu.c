/*
 * menu.c - aprMenu emulation code (MN10200 code)
 *
 * $Id: menu.c,v 1.12 2006/01/21 18:04:35 fumi Exp $
 *
 * Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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



#include <string.h>

#include "../common/emu_common.h"

#include "../common/emu_lcdbios.h"

#include "../common/emu_font.h"

#include "../common/emu_rupsys.h"


typedef struct{
	int (*func)(void);
	char *str;
	int moveR;
	int disable;
}MENU;


int sub_aprMenu(char const *title, MENU const *pmenu, int num);

/*
 * <注意>
 * 『以下のプログラムは、mn10200 向けにコンパイルする』
 *   よって、int のサイズは 2 byte
 */

static void upscroll(MENU const *pmenu, int top);
static void downscroll(MENU const *pmenu, int top);
static void show_yajirusi(int num, int top);
static void show_title(char const *title);
static void item_reverse(int show_line);

static void show_menu_item(MENU const *pmenu, int num, int top);
static void show_one_menu_item(MENU const *pmenu, int n, int show_line);
static void menu_init(char const *title, MENU const *pmenu, int num, int top);
static int sub_sub_aprMenu(char const *title, MENU const *pmenu, int num);

void sub_dispMENUplusYa(char const *title)
{
	gv_kput2_init(0, USE_DEFAULT);

	show_title(title);
}

int sub_aprMenu(char const *title, MENU const *pmenu, int num)
{
	gv_kput2_init(0, USE_DEFAULT);

	return sub_sub_aprMenu(title, pmenu, num);
}

int sub_aprMenu_us(char const *title, MENU const *pmenu, int num)
{
	gv_kput2_init(5, USE_ISO8859_1);

	return sub_sub_aprMenu(title, pmenu, num);
}

void sub_dispMENUplusYa_us(char const *title)
{
	gv_kput2_init(5, USE_ISO8859_1);

	show_title(title);
}


static int sub_sub_aprMenu(char const *title, MENU const *pmenu, int num)
{
	int top;
	int y;
	int key;
	int rst=0;

	top=0;	/* 一番上に表示しているものの行 */
	y=0;	/* カレントの画面の上からの行 */

	menu_init(title, pmenu, num, top);
	item_reverse(0);

	for(;;)
	{
		key=emu_Keyin(0xff, -1);
		rst=emu_EventCall(key);
		if(rst == -1)
			return -2; // 終了要求

		item_reverse(y);

		if(pmenu[y+top].disable == 0
		   && ((key & Bkey_B)
			   || ((key & Bkey_rg) && pmenu[y+top].moveR == 1)))
		{
			int ret;

			if(pmenu[y+top].disable == 0)
			{
				ret=pmenu[y+top].func();
				switch(ret)
				{
				case -1: // setBackMenu なので終了
					return 0;
				case -2: // setExitMenu なので終了
					return -2;
				default:
					return ret;
				}
				menu_init(title, pmenu, num, top);
			}

		}
		else if(key & Bkey_rg)
		{
			if(pmenu[y+top].disable == 0 && pmenu[y+top].moveR == 1)
			{
				int ret;
				ret=pmenu[y+top].func();


			}
		}
		else if(key & Bkey_D)
		{
			return -4;
		}

		if(key & Bkey_up)
		{
			if(y == 0)
			{
				if(top > 0)
				{
					upscroll(pmenu, top);
					top--;
					show_yajirusi(num, top);
				}
			}
			else
				y--;
		}
		else if(key & Bkey_dw)
		{
			if(y == 3)
			{
				if(top+4 < num)
				{
					downscroll(pmenu, top);
					top++;
					show_yajirusi(num, top);
				}
			}
			else
				if(y < num-1)
					y++;
		}

		item_reverse(y);
	}
}




static void show_title(char const *title)
{
	int x;
	int width;

// 網模様
	emu_gv_put(0, 0, (char *)mmpGARA, 0);

	width=strlen(title)*6;
	x=(103-width)/2;

	emu_gv_clear(x-6, 0, x+width+6, 12);
	gv_kput2(x, 0, (char *)title, 0, 0);
}

/*
 * top : top menu index
 */
static void show_menu_item(MENU const *pmenu, int num, int top)
{
	int i, n;

	n=num-top;
	if(n > 4)
		n=4;

	for(i=0; i < n; i++)
		show_one_menu_item(pmenu, top+i, i);
}

/* show_line 0 to 3
 */
static void show_one_menu_item(MENU const *pmenu, int n, int show_line)
{
	int y;

	y=(show_line+1)*13;

	gv_kput2(0, y, pmenu[n].str, 0, 0);

	if(pmenu[n].disable == 1)
	{
		int i;
		for(i=0; i < 12; i++)
		{
			if(i & 1)
				emu_gv_line(0, y+i, 101, y+i, 3, 0xaa);
			else
				emu_gv_line(0, y+i, 101, y+i, 3, 0x55);
		}
	}

	if(pmenu[n].moveR == 1)
	{
		emu_gv_put(98, y+2,	(char *)mmpRYAs, 0);
	}

}

static void show_yajirusi(int num, int top)
{
	if(num <= 4)
		return;

/* 下矢印表示 */
	if(top+4 < num)
		emu_gv_put(92, 3, (char *)mmpSITAYA, 0);
	else
		emu_gv_clear(92, 3, 101, 9);

/* 上矢印表示 */
	if(top > 0)
		emu_gv_put(83, 3, (char *)mmpUEYA, 0);
	else
		emu_gv_clear(83, 3, 92, 9);
}


static void item_reverse(int show_line)
{
	int y;

	y=(show_line+1)*13;
//	emu_gv_reverse(0, y, 101, y+13);
	emu_gv_reverse(0, y-1, 101, y+12);
}

static void menu_init(char const *title, MENU const *pmenu, int num, int top)
{
	emu_gv_place(0, 0);
	emu_gv_clear(0, 0, 101, 63);
	show_title(title);

	show_menu_item(pmenu, num, top);
	show_yajirusi(num, top);
}



/*
 * (35+7)/8*102 + 4
 * 42/8 = 5
 * =514
 * =600
 */
//static char menu_buffer[600] __attribute__ ((section (".data")));

static void upscroll(MENU const *pmenu, int top)
{
	emu_gv_get(0, 13, 101, 51, buffer_for_gv_get);
	show_one_menu_item(pmenu, top-1, 0);
	emu_gv_put(0, 26, buffer_for_gv_get, 0);
}


static void downscroll(MENU const *pmenu, int top)
{
	emu_gv_get(0, 26, 101, 63, buffer_for_gv_get);
	show_one_menu_item(pmenu, top+4, 3);
	emu_gv_put(0, 13, buffer_for_gv_get, 0);
}
