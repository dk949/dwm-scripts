#!/bin/sh
if test "$#" -ne 1; then
    echo "Expected path to image"
    exit 1
fi

CACHE_FILE_NAME=fehbg

feh --bg-fill "$1" && mv "$HOME/.$CACHE_FILE_NAME" "$XDG_CACHE_HOME/$CACHE_FILE_NAME" || echo "Something went wrong"
