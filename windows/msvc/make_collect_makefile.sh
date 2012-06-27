#!/bin/sh

# run 
# $ ./make_collect_makefile.sh  > Makefile.collect

unset LANG

OUT=lib.msvc

TMP_FILE=/tmp/$$.tmp

(cd ../../ && find ./common ./widgetlib ./windows -iregex ".*\.\(cpp\|h\)" > $TMP_FILE )

echo "COLLECT_SRCS = \\"
cat $TMP_FILE | sed -e "s+.*/\\(.*\\)+$OUT/\\1	\\\\+"
echo
echo 'collect:: lf2crlf.exe $(COLLECT_SRCS)'
echo 'lf2crlf.exe:'
echo '	cl msvc/lf2crlf.c'

cat $TMP_FILE | \
while read line
do
  NAME=`basename $line`
  echo "$OUT/$NAME: .$line"
  echo "	lf2crlf \$? \$@"
done

rm $TMP_FILE
