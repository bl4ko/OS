#!/bin/bash

comm="comm"
mem="%mem"
user="user"
cpu="%cpu"


while true
do
    clear
    ps -eo pid,$comm,$mem,$user,$cpu | (sed -u 1q; sort -rnk 5) | head -11 | nl -v 0 | sed '1 s/0//'
    read -t 3 -n 1 keypress

    case $keypress in
        "q")
            exit 0
        ;;
        "h")
            help
            echo -e "q - izhod iz programa (quit)\nh - pomoc\n"
        ;;
        "p")
            if test -n "$cpu"; then
                cpu=""
            else
                cpu="%cpu"
            fi
        ;;
        *)
            continue
        ;;
    esac

done


