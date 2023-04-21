#!/bin/sh

for con in $(nmcli -t -f SSID device wifi); do
    test -f /etc/NetworkManager/system-connections/$con* \
        && nmcli connection up $con \
        && break
done

