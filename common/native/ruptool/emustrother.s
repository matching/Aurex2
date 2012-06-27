#
# emu_strother.s - strxxx emulation code (to call C program)
#
# $Id: emustrother.s,v 1.1 2006/02/10 17:25:26 fumi Exp $
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

	.global _emu_StrnCmpI
	.global _emu_strlength
	.section	.text


_emu_StrnCmpI:
	jsr _native_in
## PPI
## a0 -> d0 (24bit)
## d1 -> d1 (no conv)
## d0 -> (0. a3) (16 bit)

	add -2, a3

		mov d0, (0, a3)
		mov a0, d0

		jsr _sub_StrnCmpI

	add 2, a3

	jsr _native_out
	rts


_emu_strlength:
	jsr _native_in
## P
## a0 -> d0 (24bit)

	mov a0, d0

	jsr _sub_strlength

	jsr _native_out
	rts
