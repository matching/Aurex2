/* $Id: address.h,v 1.6 2006/02/17 13:55:51 fumi Exp $ */

#ifndef ADDRESS_H
#define ADDRESS_H

/* Rupsys */
#define A_SYSTEMCTL_STR "00000100"

#define A_SYSTEMCTL 		0x100
#define A_SYSTEMCTL_SIZE 	0x240
/* EVENTMANAGE */
#define A_SYSCTL_00 		A_SYSTEMCTL+A_SYSTEMCTL_SIZE
#define A_SYSCTL_00_SIZE 	0x36

#define A_EVENTREQ 			A_SYSCTL_00+0x00
#define A_EVENT				A_SYSCTL_00+0x04

/* PARAMSYS */
#define A_SYSCTL_01 		A_SYSCTL_00+A_SYSCTL_00_SIZE
#define A_SYSCTL_01_SIZE 	0x18
/* int */
#define A_SYSCTL_02 		A_SYSCTL_01+A_SYSCTL_01_SIZE
#define A_SYSCTL_02_SIZE 	2
/* WatchDT */
#define A_SYSCTL_03 		A_SYSCTL_02+A_SYSCTL_02_SIZE
#define A_SYSCTL_03_SIZE	0xe

#define A_WATCHDT			A_SYSCTL_03


/* RUPSYS */
#define A_SYSCTL_04 		A_SYSCTL_03+A_SYSCTL_03_SIZE
#define A_SYSCTL_04_SIZE	0x100
/* int */
#define A_SYSCTL_05 		A_SYSCTL_04+A_SYSCTL_04_SIZE
#define A_SYSCTL_05_SIZE	2
/* RELATED */
#define A_SYSCTL_06 		A_SYSCTL_05+A_SYSCTL_05_SIZE
#define A_SYSCTL_06_SIZE	0x36
/* int */
#define A_SYSCTL_07 		A_SYSCTL_06+A_SYSCTL_06_SIZE
#define A_SYSCTL_07_SIZE	2
/* FILEINFO */
#define A_SYSCTL_08 		A_SYSCTL_07+A_SYSCTL_07_SIZE
#define A_SYSCTL_08_SIZE	0x16
/* FILEFINFO * */
#define A_SYSCTL_09 		A_SYSCTL_08+A_SYSCTL_08_SIZE
#define A_SYSCTL_09_SIZE	4
/* char [256] */
#define A_SYSCTL_0a			A_SYSCTL_09+A_SYSCTL_09_SIZE
#define A_SYSCTL_0a_SIZE 	0x100
/* int */
#define A_SYSCTL_0b 		A_SYSCTL_0a+A_SYSCTL_0a_SIZE
#define A_SYSCTL_0b_SIZE	2
/* int */
#define A_SYSCTL_0c 		A_SYSCTL_0b+A_SYSCTL_0b_SIZE
#define A_SYSCTL_0c_SIZE 	2
/* int */
#define A_SYSCTL_0d 		A_SYSCTL_0c+A_SYSCTL_0c_SIZE
#define A_SYSCTL_0d_SIZE	2
/* int */
#define A_SYSCTL_0e 		A_SYSCTL_0d+A_SYSCTL_0d_SIZE
#define A_SYSCTL_0e_SIZE	2
/* int MelodyPlay */
#define A_SYSCTL_0f 		A_SYSCTL_0e+A_SYSCTL_0e_SIZE
#define A_SYSCTL_0f_SIZE	0x402

/* int */
#define A_SYSCTL_10 		A_SYSCTL_0f+A_SYSCTL_0f_SIZE
#define A_SYSCTL_10_SIZE	2
/* STW */
#define A_SYSCTL_11 		A_SYSCTL_10+A_SYSCTL_10_SIZE
#define A_SYSCTL_11_SIZE	8
/* int */
#define A_SYSCTL_12 		A_SYSCTL_11+A_SYSCTL_11_SIZE
#define A_SYSCTL_12_SIZE	2
/* int */
#define A_SYSCTL_13 		A_SYSCTL_12+A_SYSCTL_12_SIZE
#define A_SYSCTL_13_SIZE	2

#define A_TIMERSTAT			A_SYSCTL_13
/* Tdata */
#define A_SYSCTL_14 		A_SYSCTL_13+A_SYSCTL_13_SIZE
#define A_SYSCTL_14_SIZE	8

#define A_TDATA				A_SYSCTL_14

/********************* 以上、システム構造体 *************************/
/********** この辺、Ruputer 依存 *********************/
/* ブートスタックアドレス */
#define A_BOOT_STACK	0x120000

/* LCD の現在位置を保存するアドレス */
#define A_LCD_POS_X_PORT 0x1001d6
#define A_LCD_POS_Y_PORT 0x1001d8

/* LCD コントロール・データポートアドレス */
#define A_LCD_CTRL_PORT 0x10
#define A_LCD_DATA_PORT A_LCD_CTRL_PORT+4

#define A_EVENTREGISTER 0xf080
#define A_NOTE_COUNT 0xf27c
#define A_KEY_PORT 0x51
#define A_SWINTENT 0x80200


#define A_EVENTTABLE 0x10d3fc

/*************************************************************/

// メインのカーネルスタックはおおきめにしておく
#define A_KERNEL_MAIN_SP 0xB000

#define A_RUPSYS_FUNCTION 0x10000


#define A_BOOT_DTA		0x1020


/******************** PSP *************************************/
#define A_PSP_ARG 0x88
#define A_PSP_SYS 0x80	/* システム構造体へのアドレスを格納 */



/***************** FONTS ************************************/

#define A_FONT_BASE_HAN 0x200000
#define A_FONT_BASE_ZEN 0x200900
#define A_FONT_END      0x233fff

#endif

