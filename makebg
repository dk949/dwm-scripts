#!/bin/sh
if test "$#" -ne 1; then
    echo "Expected path to image"
    exit 1
fi

feh --bg-fill "$1" &&
    mv "$HOME/.fegbh" "$XDG_CACHE_HOME/fehbg" ||
    echo "Something went wrong"
