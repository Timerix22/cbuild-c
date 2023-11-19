#!/bin/bash
set -eo pipefail

CONFIG_DIR="/etc/cbuild"
CMP="gcc"
WARN="-Wall -Wextra -Wno-discarded-qualifiers -Wno-unused-parameter"

ARGS_DEBUG="-O0 -g"
ARGS_RELEASE="-O2 -flto=auto -fdata-sections -ffunction-sections -Wl,--gc-sections"
if [ "$1" -eq "debug" ]; then
    ARGS=$ARGS_DEBUG
else 
    ARGS=$ARGS_RELEASE
fi

SRC="$(find src -name '*.c')"

OS="$(./detect_os.sh)"
ARCH="$(./detect_arch.sh)"
LINK="-Lkerep/bin -lkerep-$OS-$ARCH"

if [ "$OS" = "windows" ]; then
    OUT_FILE="cbuild.exe"
else
    OUT_FILE="cbuild"
fi

command="$CMP $ARGS
    $WARN
    -DOS=\"$OS\" -DARCH=\"$ARCH\" -DCONFIG_DIR=\"$CONFIG_DIR\"
$SRC
    $LINK
    -o bin/$OUT_FILE"

if [ ! -f "kerep/bin/libkerep-$OS-$ARCH.a" ]; then
    echo "libkerep-$OS-$ARCH.a not found"
    cd kerep
    ./build.sh
    cd ..
fi

rm -rf bin
mkdir -p bin
echo "----------------[cbuild]----------------"
echo "$command"

$($command)
