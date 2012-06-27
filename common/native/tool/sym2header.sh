#!/bin/sh

grep "\.text.*emu" | \
while read line
do
  address=`echo $line | cut -d' ' -f 1`
  name=`echo $line | cut -d' ' -f 5 | sed -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
  echo "#define A$name 0x$address"

done

