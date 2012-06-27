/*
 * psdos_execute.cpp - psdos function emulation for child process
 *
 * $Id: psdos_execute.cpp,v 1.1 2008/07/26 06:43:08 fumi Exp $
 *
 * Copyright (C) 2008 Fumihiko MACHIDA <machida@users.sourceforge.jp>
 * All rights reserved.
 *
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "x86reg.h"
#include "psdos.h"
#include "fdmgr.h"
#include "memory.h"

#include "directoryconverter.h"
#include "ruputerdirectory.h"
#include "ruputer.h"
#include "thread.h"


void DosIntr::dos_execute( X86Reg *x86reg )
{
	unsigned long filename_addr;
	unsigned long psp_addr;
	char const *filename;
	char const *psp;
	char const *psp_args_addr;
	char full_path[MAX_PATH];

	filename_addr = convToLinearAddress( x86reg->read_DS(), x86reg->read_DX() );
	filename = (char const *)Memory::getMemoryPointer( filename_addr );

	psp_addr = convToLinearAddress( x86reg->read_ES(), x86reg->read_BX() );
	psp = (char const *)Memory::getMemoryPointer(  psp_addr );

	psp_args_addr = (char const *)Memory::getMemoryPointer( *( psp + 4 ) );

	try{
		DirectoryConverter c(rupdir, localdir);
		c.convert(filename, full_path);
	}
	catch( DirectoryConverterException &e ) {
#ifdef DEBUG
		fprintf(stderr, "!! DosIntr::dos_execute catch DirectoryConverterException\n");
		fflush(stderr);
#endif
		x86reg->write_AX(3); //?
		x86reg->write_CFLAG(1);
		return;
	}

	ruputer->loadChild( full_path, (char *)psp_args_addr );

	x86reg->write_AX(0);
	x86reg->write_CFLAG(0);

}

void DosIntr::dos_child_ret( X86Reg *x86reg )
{
	x86reg->write_AL( 0 ); // return code

	x86reg->write_AH( ruputer->getChildExitCode() );
}

