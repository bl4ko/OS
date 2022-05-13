#!/bin/bash

# Variables that store colors: https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux
RED='\033[0;31m'    # Red color
GREEN='\033[1;32m'
CYAN='\033[1;36m'   # Cyan color
NC='\033[0m'        # No Color

# help function
function help() {
    echo -e "${GREEN}Press any of the following keys:${NC} "
    echo -e "   [${RED}x${NC}] - exit ${0}"
    echo -e "   [${RED}w${NC}] - send ${CYAN}SIGHUP${NC}     Terminal closure"
    echo -e "   [${RED}i${NC}] - send ${CYAN}SIGINT${NC}     Process termination: CTRL+C"
    echo -e "   [${RED}q${NC}] - send ${CYAN}SIGQUIT${NC}    Process termination: CTRL+/"
    echo -e "   [${RED}t${NC}] - send ${CYAN}SIGTERM${NC}    Software termination signal (default kill)"
    echo -e "   [${RED}k${NC}] - send ${CYAN}SIGKILL${NC}    Unconditionall process termination"
    echo -e "   [${RED}c${NC}] - send ${CYAN}SIGCONT${NC}    Continoue process execution"
    echo -e "   [${RED}s${NC}] - send ${CYAN}SIGSTOP${NC}    Unconditionall halt process"
    echo -e "   [${RED}z${NC}] - send ${CYAN}SIGSTP${NC}     Halt process CTRL+Z "
    echo -e "   [${RED}1${NC}] - send ${CYAN}SIGUSR1${NC}    1. user signal"
    echo -e "   [${RED}2${NC}] - send ${CYAN}SIGUSR2${NC}    2. user signal"
    echo -e "   [${RED}y${NC}] - send ${CYAN}SIGCHILD${NC}   Child termination"
}

# Main program
ppid=$1


while true
do
    read -n 1 -s input
    # -n defines the required character count to stop reading
    # -s hides the user's input

    case $input in
        "x") echo -e "\nChild has exited" && exit 0 ;;
        "h") help ;; 
        "w") kill -1  $ppid ;;          # SIGHUP
        "i") kill -2  $ppid ;;          # SIGINT 
        "q") kill -3  $ppid ;;          # SIGQUIT
        "t") kill -15 $ppid ;;          # SIGTERM 
        "k") kill -9  -$ppid ;;         # SIGKILL https://stackoverflow.com/questions/8533377/why-child-process-still-alive-after-parent-process-was-killed-in-linux
        "c") kill -19 $ppid ;;          # SIGCONT
        "s") kill -17 $ppid ;;          # SIGSTOP
        "z") kill -18 $ppid ;;          # SIGTSTP
        "1") kill -30 $ppid ;;          # SIGUSR1
        "2") kill -31 $ppid ;;          # SIGUSR2
        "y") kill -20 $ppid ;;          # SIGCHLD
    esac
done

