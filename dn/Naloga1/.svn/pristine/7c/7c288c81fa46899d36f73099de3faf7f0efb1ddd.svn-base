#!/bin/bash

# TODO: E, K, L
preveri_uporanika () {
    for uporabnik in $@
    do
        # testiramo ce uporabnik obstaja
        if id -u $uporabnik &> /dev/null
        then
            uid=$(id -u $uporabnik)
            gid=$(id -g $uporabnik)
            test $uid = $gid && echo -n "$uporabnik: enaka " || echo -n "$uporabnik: razlicna "
        else
            echo "$uporabnik: err"
	    continue
        fi
        # testiramo ce uporabnik ima home directory
        if cat /etc/passwd | grep "^$uporabnik" | cut -d: -f6 | xargs test -d  
        then
            echo -n "obstaja "
        else
            echo -n "ne-obstaja "
        fi
        # stevilo skupin ki jim pripada uporabnik
        echo -n $(groups $uporabnik | cut -d" " -f3- | wc -w)
        echo " $(ps -u $uporabnik | sed '1d' | wc -l)"
    done
}

fibiter () {
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

fibonacci () {
    for (( i=$#; i>0; i-- ))
    do
        value=$(fibiter ${!i})
        echo "${!i}: $value"
    done
}

stej () {
    # Ignorirajmo prazne vrstice ter vrstice ki se zacnejo z #
    # Stevilo pojavitev vrednosti ki se nahajajo v drugem stolpcu
    # izpis naj bo urejen po stevilu pojavitev
    # izpisane vrstice pa naj bojo ostevilcene
    cat "$@" | sed -e '/^$/d' -e '/^#/d' | tr -d " \t" | cut -d: -f2 | sort -r | uniq -c | sort -r | cat -n
}

isUint () {
    # Delete all numbers and check if length is zero
    [ "$1" ] && [ -z "${1//[0-9]}" ]
}

jePrestopno () {
    (( $1 % 4 == 0)) && (( $1 % 100 != 0)) || (( $1 % 400 == 0)) 
}

prestopno_leto () {
    for arg in $@
    do
        isUint $arg && jePrestopno $arg && echo "Leto $arg je prestopno." || echo "Leto $arg ni prestopno."
    done
}

izpis_argumentov () {
    counter=0
    for arg in "$@" 
    do
        # arg=$(echo $arg | sed -e 's/a/ha/g' -e 's/e/he/g' -e 's/i/hi/g' -e 's/o/ho/g' -e 's/u/hu/g')
        arg=${arg//a/ha}
        arg=${arg//e/he}
        arg=${arg//i/hi}
        arg=${arg//o/ho}
        arg=${arg//u/hu}
        echo "$counter: $arg"
        (( counter += 1 ))
    done
}

gcd () {
    if (( $1 % $2 == 0))
    then 
        echo $2
    else 
        gcd $2 $(( $1 % $2))
    fi
}

tocke () {
    stStudentov=0
    stVsehTock=0
    RANDOM=42
    while read vpisna a b c tip || test -n "$vpisna"
    do
        if test $vpisna = "#"
        then
            continue
        fi
        
        sestevek=$(( $a + $b + $c ))

        if test "${vpisna:2:2}" = "14"
        then
            pristej=true
        else 
            pristej=false
        fi

        if  test -n "$tip" && (test $tip = "p" || test $tip = "P")
        then
            prepolovi=true
        else
            prepolovi=false
        fi

        if $prepolovi && $pristej || $prepolovi; then
            sestevek=$(( sestevek /= 2 ))
        elif $pristej; then 
            pristevek=$(( (( $RANDOM % 5 )) + 1 ))
            sestevek=$(( sestevek += $pristevek ))
        fi

        if test $sestevek -gt 50; then
            sestevek=50
        fi

        echo "$vpisna: $sestevek"
        
        (( stStudentov += 1 ))
        (( stVsehTock += $sestevek))
    done

    echo "St. studentov: $stStudentov"
    echo "Povprecne tocke: $(( $stVsehTock / stStudentov ))"
}

repeatChar() {
    for (( i=0; i<$1; i++)); do
        echo -n '----';
    done
}

drevo () {

    imenik="${1:-.}"
    max_globina=${2:-3}
    trenutna_globina=${3:-1}

    if (( $trenutna_globina <= $max_globina )); then
        for file in "$imenik"/*; do
            base="${file##*/}"
            
            # preverimo ce je direktorij
            if test -d "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "DIR" "$base"
                (drevo "$file" $max_globina $(( $trenutna_globina + 1 )))
            elif test -L "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "LINK" "$base"
            elif test -b "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "BLOCK" "$base"
            elif test -c "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "CHAR" "$base"
            elif test -p "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "PIPE" "$base"
            elif test -S "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "SOCK" "$base"
            elif test -f "$file"; then 
                repeatChar $trenutna_globina && printf "%-5s %s\n" "FILE" "$base"
            fi
        done
    fi
}

recSize () {
    imenik="${1:-.}"
    max_globina=${2:-3}
    trenutna_globina=${3:-1}
    velikost=0
    stBlokov=0
    prostor=0
    
    if (( $trenutna_globina <= $max_globina )); then
        for file in "$imenik"/*; do
            if test -f "$file"; then 
                read a b c <<< $(stat "$file" --printf="%s %b %B")
                (( velikost += a ))
                (( stBlokov += b ))
                (( prostor += b * c))
            else 
                echo "$(recSize "$file" $max_globina $(( trenutna_globina + 1 )))" | read a b c
                (( velikost += a ))
                (( stBlokov += b ))
                (( prostor += c))
            fi
        done
    fi
    echo "$velikost $stBlokov $prostor" 
}

napacnaUporabaSkripte () {
    echo "Napacna uporaba skripte!"
    echo "Uporaba: $0 akcija parametri"
    exit 42
}

akcija=$1
case $akcija in
    pomoc)
        echo "Uporaba: $0 akcija parametri"
        exit 0
    ;;
    hehho)
        izpis_argumentov "${@:2}"
        exit 0
    ;;
    status)
        gcd=$(gcd $2 $3)
        echo $gcd 
        exit $gcd
    ;;
    leto)
        prestopno_leto "${@:2}"
        exit 0
    ;;
    stej)
        stej "${@:2}" 
        exit 0
    ;;
    fib)
        fibonacci "${@:2}"
        exit 0
    ;;
    upori)
        preveri_uporanika "${@:2}"
        exit 0
    ;;
    tocke)
        tocke "${@:2}"
        exit 0
    ;;
    drevo)
        if (test -n "$3") && (! isUint "$3"); then
            napacnaUporabaSkripte
        fi
        printf "%-5s %s\n" "DIR" "$2"
        drevo "${@:2}"
        exit 0
    ;;
    prostor)
        recSize "${@:2}"
    ;;
    *)
        napacnaUporabaSkripte
    ;;
esac