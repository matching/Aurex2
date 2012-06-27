#
# emurupmenu.s - rupmenu emulation code (to call C program)
#
# $Id: emurupmenu.s,v 1.6 2006/02/16 13:05:12 fumi Exp $
#
# Copyright (C) 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
# All rights reserved.
#
# This is free software with ABSOLUTELY NO WARRANTY.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA
#


	.global _emu_initInputTable
	.global _emu_endInputTable
	.global _emu_runInputProc
	.global _emu_entryItem
	.global _emu_entryItemMenu
	.global _emu_setInputFont
	.global _emu_setItemListVal
	.global _emu_getItemId
	.global _emu_getItemListVal
	.global _emu_setItemListToggle
	.global _emu_setItemMenuNext

	.section	.text

_emu_initInputTable:
	jsr _native_in
### PP
# panax -> GNU
# d0 -> d1
# a0 -> d0

	add -4, a3
	movx d1, (0, a3)

	mov d0, d1
	mov a0, d0

	jsr _sub_initInputTable

	movx (0, a3), d1
	add 4, a3

	jsr _native_out
	rts


_emu_endInputTable:

	jsr _native_in

	jsr _sub_endInputTable

	jsr _native_out
	rts


_emu_runInputProc:
	jsr _native_in

	jsr _sub_runInputProc

	jsr _native_out
	rts


_emu_entryItem:
	jsr _native_in
### a0 -> d0
### d1 -> d1 (no convert)
### d0 -> (a3) (16 bit)

	add -2, a3
	mov d0, (a3)
	mov a0, d0

	jsr _sub_entryItem

	add 2, a3

	jsr _native_out
	rts

_emu_entryItemMenu:
	jsr _native_in
### PP
### a0 -> d0
### d0 -> d1

	add -4, a3
	movx d1, (0, a3)

	mov d0, d1
	mov a0, d0

	jsr _sub_entryItemMenu

	movx (0, a3), d1
	add 4, a3

	jsr _native_out
	rts

_emu_setInputFont:
	jsr _native_in
### I
### d0 -> d0 (not convert)

	jsr _sub_setInputFont

	jsr _native_out
	rts


_emu_setItemListVal:
	jsr _native_in
### I2
### d0 -> d0 (not convert)
### d1 -> d1 (not convert)

	jsr _sub_setItemListVal

	jsr _native_out
	rts


_emu_getItemId:
	jsr _native_in
### P
### A0 -> D0 (24bit)

	mov a0, d0
	jsr _sub_getItemId

	jsr _native_out
	rts



_emu_getItemListVal:
	jsr _native_in
### I
### d0 -> d0 (not convert)

	jsr _sub_getItemListVal

	jsr _native_out
	rts



_emu_setItemListToggle:
	jsr _native_in
### I2
### d0 -> d0 (not convert)
### d1 -> d1 (not convert)

	jsr _sub_setItemListToggle

	jsr _native_out
	rts

_emu_setItemMenuNext:
	jsr _native_in
### I2
### d0 -> d0 (not convert)
### d1 -> d1 (not convert)

	jsr _sub_setItemMenuNext

	jsr _native_out
	rts
