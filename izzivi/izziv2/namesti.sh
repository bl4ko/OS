cat <<'EOF' >~/fun.sh && chmod +x ~/fun.sh
#!/bin/bash
AKCIJA=$1
N=${2-10}

function fibrek() {
    case $1 in
        0|1) 
            echo "$1"
            ;;

        *)
            echo -n "$(( $(fibrek $(($1-2)))+$(fibrek $(($1-1))) ))"
    esac
}

function fibiter() {
    case $1 in
        0|1) 
            echo "$1"
            ;;

        *)
            first=0
            second=1
            ans=0
            for (( i = 0; i < $1; i++ )); do
                ans=$(( first + second ))
                second=$first
                first=$ans
            done
            echo -n "$ans"
    esac
}

function skupine() {
    cut -d: -f1 /etc/passwd | xargs groups | sed 's/ :/:/g'
}

case $AKCIJA in

    "fibrek")
        echo $(fibrek $N)
        ;;

    "fibiter")
        echo "$(fibiter $N)"
        ;;

    "skupine")
        skupine
        ;;

    "help") 
        echo -e "./$(basename $0) [function_name] [N]\nfunction_name: ["fibrek", "fibiter", "skupine", "help"]\nN: natural number (default 10)"
        ;;

    *)
        echo "Wrong or empty function name"
        ;;
esac

exit
EOF
exit
