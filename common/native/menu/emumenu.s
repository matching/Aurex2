#
# emumenu.s - aprMenu emulation code (to call C program)
#
# $Id: emumenu.s,v 1.6 2006/01/21 18:04:35 fumi Exp $
#
# Copyright (C) 2004, 2006 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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


	.global _emu_aprMenu
	.global _emu_dispMENUplusYa
	.global _emu_aprMenu_us
	.global _emu_dispMENUplusYa_us
	.section	.text

_emu_aprMenu:
	jsr _native_in
# panax -> GNU
# d0 -> (a3)
# d1 -> d1 (no conv)
# a0 -> d0

	add -2, a3

	mov d0, (a3)
	mov a0, d0
	jsr _sub_aprMenu

	add 2, a3

	jsr _native_out
	rts


_emu_dispMENUplusYa:
	jsr _native_in

# panax -> GNU
# a0 -> d0
	mov a0, d0
	jsr _sub_dispMENUplusYa

	jsr _native_out
	rts




_emu_aprMenu_us:
	jsr _native_in
# panax -> GNU
# d0 -> (a3)
# d1 -> d1 (no conv)
# a0 -> d0

	add -2, a3

	mov d0, (a3)
	mov a0, d0
	jsr _sub_aprMenu_us

	add 2, a3

	jsr _native_out
	rts

_emu_dispMENUplusYa_us:
	jsr _native_in

# panax -> GNU
# a0 -> d0
	mov a0, d0
	jsr _sub_dispMENUplusYa_us

	jsr _native_out
	rts
