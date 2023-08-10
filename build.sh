#!/bin/bash
set -eo pipefail

CONFIG_DIR="/etc/cbuild"
CMP="gcc"
WARN="-Wall -Wextra -Wno-discarded-qualifiers -Wno-unused-parameter"
ARGS="-O2 -flto -fdata-sections -ffunction-sections -Wl,--gc-sections"
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
mkdir bin
echo "----------------[cbuild]----------------"
echo "$command"

$($command)
