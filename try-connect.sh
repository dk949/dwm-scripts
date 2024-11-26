#!/bin/sh

for con in $(nmcli -t -f SSID device wifi); do
    nmcli connection up "$con" && break
done

