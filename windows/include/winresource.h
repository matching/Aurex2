// -*-mode:c++; coding:euc-jp-unix-*-
// $Id: winresource.h,v 1.5 2006/01/28 14:32:23 fumi Exp $

#ifndef WINRESOURCE_H
#define WINRESOURCE_H

/* メニュー */
#define IDM_SETTING 	0x2000
#define IDM_VERSION 	0x2001
#define IDM_NEW_LOAD	0x2002
#define IDM_ADD_LOAD	0x2003
#define IDM_RELOAD      0x2004
#define IDM_END         0x2005
#define IDM_ALWAYS_TOP  0x2006

#define IDM_KEY_ENTER   0x2010
#define IDM_KEY_EL      0x2011
#define IDM_KEY_MENU    0x2012
#define IDM_KEY_FILER   0x2013
#define IDM_KEY_UP      0x2014
#define IDM_KEY_RIGHT   0x2015
#define IDM_KEY_DOWN    0x2016
#define IDM_KEY_LEFT    0x2017

#define ID_STATUS 0x5000


/* プロパティページ */
/* page misc */
#define IDC_WARN_ODD 			0x3000
#define IDC_ENABLE_STATUS_BAR   0x3004
#define IDC_ENABLE_SOUND 		0x3001
#define IDC_ENABLE_CPU_TIMER 	0x3002
#define IDC_NO_CAPTION          0x3007
#define IDC_ISO8859_1           0x3008

#define IDC_ONLY_A_PROCESS      0x3009
/*
#define IDC_ENABLE_ROM_FONT		0x3003
#define IDC_DISABLE_MENU        0x3005
#define IDC_SHELL_ICON          0x3006
#define IDC_LOG                 0x3008
*/

/* page lcd */
#define IDC_LCD_102X64 			0x3300
//#define IDC_LCD_RECTANGLE 		0x3301
#define IDT_LCD_EXPANSION       0x3303
#define IDET_LCD_EXPANSION      0x3304
#define IDC_AFTER_IMAGE			0x3305
#define IDC_ANTI_ALIASES		0x3306
#define IDC_FLIP				0x3307

#define IDC_TRANSPARENT			0x3308
#define IDC_ALPHA				0x3309
#define IDT_ALPHA_VALUE			0x330A
#define IDET_ALPHA_VALUE		0x330B

#define IDT_AI_VALUE            0x330C
#define IDET_AI_VALUE           0x330D


/* page speed */
#define IDT_CPU_SPEED			0x3100
#define IDET_CPU_SPEED	        0x3101
#define IDT_TIMER_SPEED         0x3102
#define IDET_TIMER_SPEED        0x3103
#define IDPB_RETURN_TO_DEFAULT_CPU   0x3104
#define IDPB_RETURN_TO_DEFAULT_TIMER 0x3105
#define IDPB_SET_TO_INNAR_SRAM       0x3106

/* page font */
#define IDT_FIXED_FONT   0x3200
#define IDET_FIXED_FONT  0x3201
#define IDT_PROP_FONT    0x3202
#define IDET_PROP_FONT   0x3203
#define IDPB_FIXED_FONT_SELECT 0x3204
#define IDPB_PROP_FONT_SELECT  0x3205


/* page dir */
#define IDT_ROOT_DIRECTORY 0x3300
#define IDET_ROOT_DIRECTORY 0x3301
#define IDPB_ROOT_DIRECTORY 0x3302

/* page log */
#define IDC_RADIO_LOG_NONE 0x3400
#define IDC_RADIO_LOG_STDERR 0x3401
#define IDC_RADIO_LOG_FILE 0x3402
#define IDC_RADIO_LOG_EVENT 0x3403

#define IDET_LOG_FILE_NAME   0x3404
#define IDPB_LOG_FILE_SELECT 0x3405

#define IDT_LOG_LEVEL        0x3410
#define IDET_LOG_LEVEL       0x3411


#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC (-1)

#endif
