#!/bin/bash
uname_rezult="$(uname -m)"

case "$uname_rezult" in
    arm | arm32 | armhf | aarch32)
        ARCH=arm32
        ;;
    arm64 | aarch64 | aarch64_be | armv8b | armv8l)
        ARCH=arm64
        ;;
    x86 | i386 | i486 | i686)
        ARCH=x86
        ;;
    x64 | x86_64 | amd64)
        ARCH=x64
        ;;
    *)
        echo "unknown CPU architecture: $uname_rezult"
        exit 1
        ;;
esac

echo "\"$ARCH\""
