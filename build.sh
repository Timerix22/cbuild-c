#!/bin/bash
set -eo pipefail
CMP="gcc"
ARGS="-Wall -Wno-discarded-qualifiers -O2"
SRC="$(find . -name '*.c')"
LINK="-L. -lkerep"

OS=$(./detect_os.sh)
ARCH=$(./detect_arch.sh)

if [[ OS == "windows" ]]; then
    OUT_FILE="cbuild.exe"
else
    OUT_FILE="cbuild"
fi

command="$CMP $ARGS
    -DOS=$OS -DARCH=$ARCH
$SRC
    $LINK
    -o bin/$OUT_FILE"

rm -rf bin
mkdir bin
echo "$command"

tar xJf libkerep.a.tar.xz

$($command)

rm libkerep.a
