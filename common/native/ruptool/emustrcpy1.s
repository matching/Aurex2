#
# emu_strcpy1.s - strcpy*** emulation code (to call C program)
#
# $Id: emustrcpy1.s,v 1.2 2006/02/10 17:25:26 fumi Exp $
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

	.global _emu_strcpy1
	.global _emu_strcpy2
	.global _emu_strcpy3
	.global _emu_StrCpy
	.global _emu_strcpyadd0
	.global _emu_wstrcpy

	.section	.text

_emu_strcpy1:
	jsr _native_in
## PP
## a0 -> d0 (24bit)
## d0 -> d1 (24bit)
	add -4, a3
	movx d1, (0, a3)

		mov d0, d1
		mov a0, d0

		jsr _sub_strcpy1

	movx (0, a3), d1
	add 4, a3

	jsr _native_out
	rts


_emu_strcpy2:
	jsr _native_in
## PPP
## a0 -> d0 (24bit)
## d0 -> d1 (24bit)
## D1 -> (A3) (24bit)

	add -4, a3

		movx d1, (0, a3)
		mov d0, d1
		mov a0, d0

		jsr _sub_strcpy2

	add 4, a3

	jsr _native_out
	rts

_emu_strcpy3:
	jsr _native_in
## PPPP
## a0 -> d0 (24bit)
## d0 -> d1 (24bit)
## D1 -> (A3) (24bit)
## a3+0x10 ( +4 戻りアドレス分) -> (4, A3) (24bit) 


	## PPPP 呼び出しレジスタなので壊しても構わない
	mov (0x4, a3), a1

	add -8, a3

		movx d1, (0, a3) # arg 3
		mov d0, d1	 # arg 2

		movx (0x10 + 4, a1), d0
		movx d0, (4, a3) # arg 4
		mov a0, d0	 # arg 1

		jsr _sub_strcpy3

	add 8, a3

	jsr _native_out
	rts



_emu_StrCpy:
	jsr _native_in
## PP
## a0 -> d0 (24bit)
## d0 -> d1 (24bit)

	add -4, a3
	movx d1, (0, a3)

		mov d0, d1
		mov a0, d0

		jsr _sub_StrCpy

	movx (0, a3), d1
	add 4, a3

	jsr _native_out
	rts



_emu_strcpyadd0:
	jsr _native_in
## PPI
## a0 -> d0 (24bit)
## d1 -> d1 (no conv)
## d0 -> (0. a3) (16 bit)

	add -2, a3

		mov d0, (0, a3)
		mov a0, d0

		jsr _sub_strcpyadd0

	add 2, a3

	jsr _native_out
	rts




_emu_wstrcpy:
	jsr _native_in
## PPPP
## a0 -> d0 (24bit)
## d0 -> d1 (24bit)
## D1 -> (A3) (24bit)
## a3+0x10 ( +4 戻りアドレス分) -> (4, A3) (24bit) 

	## PPPP 呼び出しレジスタなので壊しても構わない
	mov (0x4, a3), a1

	add -8, a3

		movx d1, (0, a3) # arg 3
		mov d0, d1	 # arg 2

		movx (0x10 + 4, a1), d0
		movx d0, (4, a3) # arg 4
		mov a0, d0	 # arg 1

		jsr _sub_wstrcpy

	add 8, a3

	jsr _native_out
	rts

