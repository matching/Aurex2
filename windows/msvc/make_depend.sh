#!/bin/sh

cat ../../common/makefile.dep | \
sed -e '/CXX/d;s+$(TARGET_DIR)+$(COM_OUT_DIR)+g;s+ \([a-z]\)+ $(COMMON_DIR)/\1+g;s/\.o/\.obj/g' | \
while read line
  do
  FILENAME=`echo $line | sed -e 's/.*: \([^ ]*\).*/\1/'`

  while :
  do
    echo $line
    if echo $line | grep -E "\\\\$" > /dev/null; then
	read line
	continue
    fi
    break
  done
  echo "	lf2crlf $FILENAME \$(TEMP_FILE)"
  echo "	\$(CXX) \$(CXXFLAGS) -I`dirname $FILENAME` -Fo\$@ -c \$(TEMP_FILE)"
done |\
nkf -c > Makefile.msvc.dep
