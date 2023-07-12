#!/bin/bash
uname_rezult="$(uname -o)"

case "$uname_rezult" in
    Msys | Cygwin | "MS/Windows")
        OS=windows
        ;;
    Linux | GNU/Linux)
        OS=linux
        ;;
    Android)
        OS=android;
        ;;
    FreeBSD)
        OS=freebsd
        ;;
    Darwin)
        OS=macos
        ;;
    *)
        echo "unknown operating system: $uname_rezult"
        exit 1
        ;;
esac

echo "$OS"
