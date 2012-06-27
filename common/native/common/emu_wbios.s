#
# emu_wbios.s - common module for wbios function caller
#
# $Id: emu_wbios.s,v 1.2 2005/01/10 08:40:27 fumi Exp $
#
# Copyright (C) 2005 Fumihiko MACHIDA <machida@users.sourceforge.jp>
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

	.global _emu_sys_bi_tmset
	.global _emu_sys_bi_tmdel

	.section	.text

### int *emu_sys_bi_tmset(timkind, priority, cnt, func);
### d0: timkind
### d1: priority
### a3 + 0 -> (return address)
###      4 -> cnt  (2byte)
###      6 -> func (4byte)
_emu_sys_bi_tmset:
	add -0xc, a3
	movx d2, (0x4, a3)
	mov  a1, (0x8, a3)

	mov d0, d1		# kind
	mov 0x01011c, d0 	# sys_bi_tmset
	mov (0x10, a3), d2 	# cnt
	mov (0x12, a3), a1	# addr

	mov a2, (0, a3)
	mov 0x80200, a2
	jsr (a2)

	cmp 0, d1
	beq _sys_bi_timset_finish
	mov -1, a0

_sys_bi_timset_finish:
	## a3 -= 4 by system call
	movx (0x0, a3), d2
	mov  (0x4, a3), a1
	add 0x8, a3
	rts


### int     bi_tmdel(int *timid);
### d0: timid
_emu_sys_bi_tmdel:
	add -0xc, a3
	movx d1, (0x4, a3)
	mov  a0, (0x8, a3)

	mov d0, a0		# kind
	mov 0x01021c, d0 	# sys_bi_tmdel

	mov a2, (0, a3)
	mov 0x80200, a2
	jsr (a2)

	mov d1, d0

	## a3 -= 4 by system call
	movx (0x0, a3), d1
	mov  (0x4, a3), a0
	add 0x8, a3
	rts
