#
# common.s - common module for MN10200 code
#
# $Id: common.s,v 1.4 2006/01/09 18:35:20 fumi Exp $
#
# Copyright (C) 2004 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

	.global _dummy_entry
	.global _native_in
	.global _native_out

	.section	.text

_dummy_entry:
	rts

_native_in:
	add -0x10, a3

	mov a0, (0, a3)
	mov a1, (4, a3)
	mov a2, (8, a3)
	movx d0, (0xc, a3)

	mov a3, a0
	mov (0x10, a0), a1	# a1 return address
	mov (0x14, a0), a2	# a2 kernel stack pointer

	movx (0, a2), d0
	cmp 0, d0
	bne nesting_plus_one

	##  ネスティングレベル = 0 なので、a3 にカーネルスタックを読み込ませる。
	mov a2, a3
	add -4, a3

nesting_plus_one:
	add 1, d0
	movx d0, (0, a2)

	add -0xc, a3
	mov a1, (0, a3)		# return address
	mov a2, (4, a3)		# kernel stack address

	mov a0, a2
	add 0x18, a2		# 0x10 + 0x4   0x4 はもとの retuan address 分(native_in のリターンアドレス分)
	mov a2, (8, a3)		# previous a3

	mov (4, a0), a1
	mov (8, a0), a2
	movx (0xc, a0), d0
	mov (0, a0), a0

	rts


_native_out:
	add -0x10, a3

	mov a0, (0, a3)
	mov a1, (4, a3)
	mov a2, (8, a3)
	movx d0, (0xc, a3)

	mov a3, a0
	mov (0x10, a0), a1	# a1 retuan address
	mov (0x14, a0), a2	# a2 kernel base stack address
	mov (0x18, a0), a3	# a3 previous stack pointer

	movx (0, a2), d0
	add -1, d0
	movx d0, (0, a2)

	add -4, a3
	mov a1, (a3)		# retual address

	mov (4, a0), a1
	mov (8, a0), a2
	movx (0xc, a0), d0
	mov (0, a0), a0

	rts
