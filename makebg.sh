#!/bin/sh
if test "$#" -ne 1; then
    echo "Expected path to image"
    exit 1
fi

feh --bg-fill "$1"
if [ -f ~/.fehbg ]; then
    mv ~/.fehbg "$XDG_CACHE_HOME/.fehbg"
else
    echo "Failed to move fehbg file"
    exit 1
fi
