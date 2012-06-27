#
# emudispnow.s - dispNow... emulation code (to call C program)
#
# $Id: emudisprtc.s,v 1.1 2004/04/29 08:58:51 fumi Exp $
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

	.global _emu_dispRtcWeek
	.global _emu_dispRtcDate
	.global _emu_dispRtcAmPm
	.global _emu_dispRtcTime
	.global _emu_dispRtcSec

	.section	.text

_emu_dispRtcWeek:
	jsr _native_in

## a1 ÇË²õ²ÄÇ½¡©
## d0 -> d0 (no conv)
## d1 -> d1 (no conv)
## (8, a3) -> (0, a3)
## (A, a3) -> (2, a3)
## (C, a3) -> (4, a3)

	mov (0x4, a3), a1

	add -0xa, a3
	movx d2, (0x6, a3)

		mov (0x8+4, a1), d2
		mov d2, (0, a3)

		mov (0xa+4, a1), d2
		mov d2, (2, a3)

		mov (0xc+4, a1), d2
		mov d2, (4, a3)

		jsr _sub_dispRtcWeek

	movx (0x6, a3), d2
	add 0xa, a3


	jsr _native_out
	rts


_emu_dispRtcDate:
	jsr _native_in

## a1 ÇË²õ²ÄÇ½¡©
## d0 -> d0 (no conv)
## d1 -> d1 (no conv)
## (8, a3) -> (0, a3)
## (A, a3) -> (2, a3)
## (C, a3) -> (4, a3)

	mov (0x4, a3), a1

	add -0xa, a3
	movx d2, (0x6, a3)

		mov (0x8+4, a1), d2
		mov d2, (0, a3)

		mov (0xa+4, a1), d2
		mov d2, (2, a3)

		mov (0xc+4, a1), d2
		mov d2, (4, a3)

		jsr _sub_dispRtcDate

	movx (0x6, a3), d2
	add 0xa, a3


	jsr _native_out
	rts


_emu_dispRtcAmPm:
	jsr _native_in

## a1 ÇË²õ²ÄÇ½¡©
## d0 -> d0 (no conv)
## d1 -> d1 (no conv)
## (8, a3) -> (0, a3)
## (A, a3) -> (2, a3)

	mov (0x4, a3), a1

	add -0x8, a3
	movx d2, (0x4, a3)

		mov (0x8+4, a1), d2
		mov d2, (0, a3)

		mov (0xa+4, a1), d2
		mov d2, (2, a3)

		jsr _sub_dispRtcAmPm

	movx (0x4, a3), d2
	add 0x8, a3


	jsr _native_out
	rts




_emu_dispRtcTime:
	jsr _native_in

## a1 ÇË²õ²ÄÇ½¡©
## d0 -> d0 (no conv)
## d1 -> d1 (no conv)
## (8, a3) -> (0, a3)
## (0x0A, a3) -> (2, a3)
## (0x0C, a3) -> (4, a3)
## (0x0E, a3) -> (6, a3)
## (0x10, a3) -> (8, a3)


	mov (0x4, a3), a1

	add -0xe, a3
	movx d2, (0xa, a3)

		mov (0x8+4, a1), d2
		mov d2, (0, a3)

		mov (0xa+4, a1), d2
		mov d2, (2, a3)

		mov (0xc+4, a1), d2
		mov d2, (4, a3)

		mov (0xe+4, a1), d2
		mov d2, (6, a3)

		mov (0x10+4, a1), d2
		mov d2, (8, a3)

		jsr _sub_dispRtcTime

	movx (0xa, a3), d2
	add 0xe, a3


	jsr _native_out
	rts



_emu_dispRtcSec:
	jsr _native_in

## a1 ÇË²õ²ÄÇ½¡©
## d0 -> d0 (no conv)
## d1 -> d1 (no conv)
## (8, a3) -> (0, a3)
## (A, a3) -> (2, a3)

	mov (0x4, a3), a1

	add -0x8, a3
	movx d2, (0x4, a3)

		mov (0x8+4, a1), d2
		mov d2, (0, a3)

		mov (0xa+4, a1), d2
		mov d2, (2, a3)

		jsr _sub_dispRtcSec

	movx (0x4, a3), d2
	add 0x8, a3


	jsr _native_out
	rts

