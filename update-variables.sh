#!/bin/sh
# Checking for updates

if [ $# -ne 1 ]; then
    echo "Usage: $(basename "$0") TIME"
    exit 1
fi

updateFunc() {
    PACVARS_DIR="$XDG_CACHE_HOME/pacvars"
    [ -d "$PACVARS_DIR" ] || mkdir "$PACVARS_DIR"

    UPDATES=$(checkupdates)
    WATCHLIST="gcc clang "

    if [ ${#UPDATES} -eq 0 ]; then
        NUM_UPDATES=0
    else
        NUM_UPDATES=$(echo "$UPDATES" | wc -l)

    fi

    echo "$NUM_UPDATES" > "$PACVARS_DIR/NUM_UPDATES"

    # Go through available updates and remove anything not on the watch list
    IMPORTANT_UPDATES=$(echo "$UPDATES" | sed -r "/$WATCHLIST/!d" | wc -l)
    echo "$IMPORTANT_UPDATES" > "$PACVARS_DIR/IMPORTANT_UPDATES"

    # Remove anything that is not a kernel (or firmware) update
    KERNEL_UPDATES=$(echo "$UPDATES" | sed -r "/linux/!d" | wc -l)
    echo "$KERNEL_UPDATES" > "$PACVARS_DIR/KERNEL_UPDATES"


    NUMPACKAGES="$(pacman -Q | wc -l)"
    echo "$NUMPACKAGES" > "$PACVARS_DIR/NUMPACKAGES"



    pacman -Qen | awk '{print $1}' >> "$HOME"/install/packages_native.txt
    sort < "$HOME"/install/packages_native.txt | uniq > /tmp/tmp_pac
    mv /tmp/tmp_pac "$HOME"/install/packages_native.txt
    cd "$HOME"/install && git add packages_native.txt
    cd || exit 1

    pacman -Qm | awk '{print $1}' > "$HOME"/install/packages_foreign.txt
    sort < "$HOME"/install/packages_foreign.txt | uniq > /tmp/tmp_pac
    mv /tmp/tmp_pac "$HOME"/install/packages_foreign.txt
    cd "$HOME"/install && git add packages_foreign.txt
    cd || exit 1
}

while true; do
    updateFunc
    sleep "$1"
done &
