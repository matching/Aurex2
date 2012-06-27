#!/bin/sh

echo '#include "romgaiji.h"'
echo 'GaijiData RomGaiji::gaijidata[]={'

cat

echo '};'
