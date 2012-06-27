#
# emu_lcdbios.s - common module for lcdbios function caller
#
# $Id: emu_lcdbios.s,v 1.1 2004/03/06 18:36:25 fumi Exp $
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

	.global _pre_lcdfunc
	.global _post_lcdfunc
	.global _call_lcdfunc

	.section	.text


# d0, d1 をスタックに積む
#

_pre_lcdfunc:
	add -0x10, a3
	mov a0, (0xc, a3)

# 戻りアドレスの移動
	mov (0x10, a3), a0
	mov a0, (0x0, a3)
	mov (0x14, a3), a0
	mov a0, (0x4, a3)

# 引数をスタックに積む
	mov a3, a0
	add 0x12, a0
	mov d0, (0x2, a0)
	mov d1, (0x4, a0)
	rts

_call_lcdfunc:
	mov d0, (0, a0)
	mov 22, d0
	add -4, a3
	mov a1, (0, a3)
	mov 0x80200, a1
	jsr (a1)
	rts

_post_lcdfunc:
	mov (0x0, a3), a0
	mov a0, (0x10, a3)
	mov (0x4, a3), a0
	mov a0, (0x14, a3)

	mov (0xc, a3), a0

	add 0x10, a3
	rts


