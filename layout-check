#!/bin/sh

if [ -z "$1" ]; then
    echo "$0 takes 1 numeric parameter"
    exit 1
fi

if [ $# -gt 1 ]; then
    echo "$0 takes 1 numeric parameter"
    exit 1
fi

case "$1" in
    *[!0-9]*)
        echo "$0 takes 1 numeric parameter"
        exit 1
        ;;
    *)
        :
        ;;
esac

if command -v figlet 1,2> /dev/null ; then
    CMD="figlet"
else
    CMD="echo"
fi

if [ -n "$TERM" ]; then
    T="$TERM"
elif [ -n "$TERMINAL" ]; then
    T="$TERMINAL"
elif command -v  xterm 1,2> /dev/null; then
    T="xterm"
elif command -v  urxvt 1,2> /dev/null; then
    T="urxvt"
else
    echo "Could not detect Terminal, try exporting TERM or TERMINAL env variables"
fi


for i in $(seq "$1"); do
    "$T" &
done

sleep 0.5

for i in /dev/pts*; do
    case "$i" in
        *[0-9]*)
            out "$i" clear
            out "$i" "$CMD" "$i"
            ;;
        *)
            :
            ;;
    esac
done
