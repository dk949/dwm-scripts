#!/bin/sh
set -e

[ $# -ne 2 ] && { echo "Usage $(basename "$0") SOURCE TARGET"; exit 1; }

s=$1
t=$2
query=$(xrandr)
scale="$(echo "$(echo "$query" \
    | grep "$t" -A1 \
    | tail -n1 \
    | awk '{print $1}')
$(echo "$query" \
    | grep "$s" -A1 \
    | tail -n1 \
    | awk '{print $1}')" \
| awk -F'x' '
    s  { sx = $1; sy = $2 }
    !s { fx = $1; fy = $2; s = 1 }
    END { print sx / fx  "x" sy / fy }
')"

cmd="xrandr --output $t --same-as $s --auto --scale $scale"
echo "$cmd"
exec $cmd
