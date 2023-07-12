#!/bin/bash
set -eo pipefail

CMP="gcc"
WARN="-std=c11 -Wall -Wno-discarded-qualifiers -Wno-unused-parameter"
ARGS="-O2 -flto -fpic -fdata-sections -ffunction-sections"
SRC="$(find src -name '*.c')"

OS=$(../detect_os.sh)
ARCH=$(../detect_arch.sh)
OUTFILE="bin/libkerep-$OS-$ARCH.a"

rm -rf obj
mkdir obj
mkdir -p bin

echo "----------------[kerep]-----------------"
compiler_call="$CMP $WARN $ARGS -c"
echo "$compiler_call"
OBJECTS=
for srcfile in $SRC
do
    echo "    $srcfile"
    object="obj/$(basename $srcfile).o"
    OBJECTS="$OBJECTS
    $object"
    command="$compiler_call $srcfile -o $object"
    if ! $($command); then
        exit 1
    fi
done

command="ar rcs $OUTFILE $OBJECTS"
echo "$command"
if $($command)
then
    echo "static lib $OUTFILE created"
    rm -r obj
else
    exit 1
fi
