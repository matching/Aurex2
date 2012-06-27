#
# emu_lcdfunc.s - call lcdfunc function for MN10200 code
#
# $Id: emu_lcdfunc.s,v 1.4 2006/01/21 18:04:35 fumi Exp $
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
	.global _emu_blink
	.global _emu_blstop
	.global _emu_blstart
	.global _emu_blend

	.global _emu_gv_kput
	.global _emu_gv_clear
	.global _emu_gv_reverse
	.global _emu_gv_put
	.global _emu_gv_get
	.global _emu_gv_place
	.global _emu_gv_line
	.global _emu_gv_sput

	.global _emu_gv_kput2

	.section	.text



_emu_blink:
	jsr _pre_lcdfunc
	mov 13, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_blstop:
	jsr _pre_lcdfunc
	mov 14, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_blstart:
	jsr _pre_lcdfunc
	mov 15, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_blend:
	jsr _pre_lcdfunc
	mov 16, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts



_emu_gv_kput:
	jsr _pre_lcdfunc
	mov 61, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts
_emu_gv_sput:
	jsr _pre_lcdfunc
	mov 62, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_gv_clear:
	jsr _pre_lcdfunc
	mov 74, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_gv_reverse:
	jsr _pre_lcdfunc
	mov 73, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_gv_put:
	jsr _pre_lcdfunc
	mov 71, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_gv_get:
	jsr _pre_lcdfunc
	mov 70, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_gv_place:
	jsr _pre_lcdfunc
	mov 60, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts

_emu_gv_line:
	jsr _pre_lcdfunc
	mov 65, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts




_emu_gv_kput2:
	jsr _pre_lcdfunc
	mov 161, d0
	jsr _call_lcdfunc
	jsr _post_lcdfunc
	rts
