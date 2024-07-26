#!/bin/sh

default_login_path=/etc/private-internet-access/login.conf
if  echo " $* " | grep -qE "( -h )|( --help )"; then
    echo "Usage: $(basename "$0") [LOGIN_PATH]

    If LOGIN_PATH is specified, it is used to look up login credentials,
    otherwise '$default_login_path' is used.
"
    exit 0
fi

if [ -n "$1" ]; then
    login_path=$1
else
    login_path=$default_login_path
fi

systemctl start piavpn.service \
&& piactl background enable \
&& {
   piactl login "$login_path"
   piactl connect
}
