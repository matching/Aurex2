#
# emusoftkey.s - softkey emulation code (to call C program)
#
# $Id: emusoftkey.s,v 1.2 2006/01/09 18:35:20 fumi Exp $
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

	.global _emu_softkey
	.section	.text

## 呼び出し直後
## a3 ->		0x00 kernel stack
## a2 => (インデント中) 0x04 retrun address
##			0x08 using panax
## 			0x0c p0 ( arg 1 ) a0
##			0x10 p1 ( arg 2 ) d1
##			0x14 i  ( arg 3 ) d0
##			0x16 p2 ( arg 4 ) none

###     panax -> gnu
### 1    a0      d0
### 2    d1      d1
### 3    d0      (0, a3)
### 4  (0e, a3)  (2, a3)
_emu_softkey:
	jsr _native_in

	add -4, a3
	mov a2, (0, a3)

	mov (8, a3), a2 	# previous a3

		add -0x6, a3
		mov d0, (0, a3)     # 3

		mov a0, d0         # 1
		mov (0x0e + 4, a2), a0 # 4 は devlog.txt 参照
		mov a0, (2, a3)    # 4

		jsr _sub_softkey

		add 0x6, a3

	mov (0, a3), a2
	add 4, a3

	jsr _native_out
	rts
