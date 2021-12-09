#!/bin/sh
ps -eo pcpu,args | tail -n+2 | sort -rn | head -n 1 | awk '{print "PROC: " $2}' | sed 's|/.*/||'
