#!/bin/sh

sed -e '1s/\(.*\)/#define A_ROMMMP_START \1/;2i\
#include "rommmp.h"\
unsigned char RomMmp::romdata[]={'

echo '};'
