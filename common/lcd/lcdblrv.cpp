/*
 * lcdblrv.cpp - Blink and Reverse common module
 *
 * $Id: lcdblrv.cpp,v 1.7 2005/01/06 14:21:33 fumi Exp $
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
#include "cmlcd.h"
#include "lcdblrv.h"

/* 本当はパック演算したいん
 */


LcdBlRv::LcdBlRv(int x1, int y1, int x2, int y2)
{
	x=x1;
	y=y1;
	width=abs(x1-x2)+1;
	height=abs(y1-y2)+1;
	state=BLINK_REVERSE_START;

	pixel_state = ON;
}

int LcdBlRv::getX(void)
{
	return x;
}
int LcdBlRv::getY(void)
{
	return y;
}
int LcdBlRv::getWidth(void)
{
	return width;
}
int LcdBlRv::getHeight(void)
{
	return height;
}

enum BlinkReversePixelValue const *LcdBlRv::getPixelSatatePointer(void)
{
	return (enum BlinkReversePixelValue const *)&pixel_state;
}

int LcdBlRv::getState(void)
{
	return state;
}
void LcdBlRv::setState(int s)
{
	state=s;
}

void LcdBlRv::invert(void)
{
	pixel_state = REVERSE;
}

void LcdBlRv::off(void)
{
	pixel_state = OFF;
}

void LcdBlRv::pixel_on(void)
{
	pixel_state = ON;
}
