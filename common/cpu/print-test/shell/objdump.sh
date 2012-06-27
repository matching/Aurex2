#!/bin/sh
# $Id: objdump.sh,v 1.1 2004/11/14 14:03:09 fumi Exp $

mkdir out-objdump/

files=`find ins/ -type f`
for f in $files
do
  /usr/local/ruputer/bin/mn10200-elf-objdump.exe -D -z -b binary -m MN10200 $f | grep '0:' | cut -f 3- > out-objdump/`basename $f`
done