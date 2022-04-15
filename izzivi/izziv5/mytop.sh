#!/bin/bash

comm="comm,"
mem="%mem,"
user="user,"
cpu="%cpu"


while true
do

    clear
    ps -eo pid,$comm$mem$user$cpu | (sed -u 1q; sort -rnk 5) | head -11 | nl -v 0 | sed '1 s/0//'
    read -t 1 -n 1 keypress

    if test "$keypress" = "h"; then
        echo -e "q - izhod\nh - pomoc\nc - toggle command\nm - toggle memory\np - cpu toggle\nu - toggle user"
        read -n 1 keypress
    fi

    case $keypress in
        "q")
            exit 0
        ;;
        "c")
            if test -n "$comm"; then
                comm=""
            else
                comm="comm,"
            fi
        ;;
        "m")
            if test -n "$mem"; then
                mem=""
            else
                mem="%mem,"
            fi
        ;;
        "p")
            if test -n "$cpu"; then
                cpu=""
            else
                cpu="%cpu"
            fi
        ;;
        "u")
            if test -n "$user"; then
                user=""
            else
                user="user,"
            fi
        ;;
        *)
            continue
        ;;
    esac

done


