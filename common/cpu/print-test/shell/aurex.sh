#!/bin/sh
# $Id: aurex.sh,v 1.1 2004/11/14 14:03:09 fumi Exp $

mkdir out-aurex/

files=`find ins/ -type f`
for f in $files
do
  ./print-test $f 2> out-aurex/`basename $f`.txt
done