# Izzivi - Osnove Lupine

## Seznanite se z delovnim okoljem

### Windows

#### Informacije 
1. Hardware
    - splosne informacije o HARDWARE
    - informacije o procesorju
    ```bash
    testko@bruh:~$ whatis lscpu
    lscpu (1)            - display information about the CPU architecture
    testko@bruh:~$ lscpu
    Architecture:                    x86_64
    CPU op-mode(s):                  32-bit, 64-bit
    Byte Order:                      Little Endian
    Address sizes:                   39 bits physical, 48 bits virtual
    CPU(s):                          2
    On-line CPU(s) list:             0,1
    Thread(s) per core:              1
    Core(s) per socket:              2
    Socket(s):                       1
    NUMA node(s):                    1
    Vendor ID:                       GenuineIntel
    CPU family:                      6
    Model:                           94
    Model name:                      Intel(R) Core(TM) i7-6700K CPU @ 4.00GHz
    Stepping:                        3
    CPU MHz:                         4007.996
    BogoMIPS:                        8015.99
    Hypervisor vendor:               KVM
    Virtualization type:             full
    L1d cache:                       64 KiB
    L1i cache:                       64 KiB
    L2 cache:                        512 KiB
    L3 cache:                        16 MiB
    ```

1. Operacijski sistem
    ```bash
    testko@bruh:~$ uname -a
    Linux bruh 5.11.0-27-generic #29~20.04.1-Ubuntu SMP Wed Aug 11 15:58:17 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
    ```

1. Programska oprema
    


### Linux - Ubuntu

## Uporaba ukazov
1. Za vse nastete ukaze opravite poizvedbo tipa po ukazu
    ```bash
    ➜  $ type logout
    logout is a shell builtin
    ➜  $ type tree
    tree is /usr/bin/tree
    ➜  $ type cd
    cd is a shell builtin
    ➜  $ type pwd
    pwd is a shell builtin
    ➜  $ type date
    date is /usr/bin/date
    ➜  $ type echo
    echo is a shell builtin
    ➜  $ type printf
    printf is a shell builtin
    ➜  $ type alias
    alias is a shell builtin
    ➜  $ type gcc
    gcc is /usr/bin/gcc
    ➜  $ type while
    while is a reserved word
    ➜  $ type if
    if is a reserved word
    ```

1. Za vse nastete ukaze ugotivte, kje se nahajajo. Kateri ukaz je najprimernejsi za to poizvedbo?
    ```bash
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 whatis whereis
    whereis (1)          - locate the binary, source, and manual page files for a command
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 whatis which
    which (1)            - locate a command
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 which printf
    printf: shell built-in command
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 whereis printf
    printf: /usr/bin/printf /usr/include/printf.h /usr/share/man/man3/printf.3.gz /usr/share/man/man1/printf.1.gz
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 whereis ls
    ls: /usr/bin/ls /usr/share/man/man1/ls.1.gz
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 which ls
    ls: aliased to ls --color=tty
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 which echo
    echo: shell built-in command
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 whereis echo
    echo: /usr/bin/echo /usr/share/man/man1/echo.1.gz
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 which tree
    /usr/bin/tree
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 whereis tree
    tree: /usr/bin/tree /usr/share/man/man1/tree.1.gz
    ```

## Prekinjanje ukazov
- Zazenite ukaz sleep 1000;
    - pocakajte da se konca
        ```bash
        ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 sleep 1000
        ^C
        ```
        ```bash
        ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 sleep 1000
        ^Z
        [1]  + 62159 suspended  sleep 1000
        ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 fg
        [1]  + 62159 continued  sleep 1000
        ^C
        ```

## Uporaba pomoci
```bash
➜  ~/Faks/2_letnik/OS/izzivi/izziv1 ping -h

Usage
  ping [options] <destination>

Options:
  <destination>      dns name or ip address
  -a                 use audible ping
  -A                 use adaptive ping
  -B                 sticky source address
  -c <count>         stop after <count> replies
  -D                 print timestamps
  -d                 use SO_DEBUG socket option
  -f                 flood ping
  -h                 print help and exit
  -I <interface>     either interface name or address
  -i <interval>      seconds between sending each packet
```


## Datoteke in Strukutra imenikov

### Izziv 1
1. Ustvarite direktorijsko strukturo
    ```bash
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 mkdir t1
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 mkdir t1/t2
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 mkdir t1/t3
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 mkdir t1/t3/t5
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 mkdir t1/t3/t6
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 mkdir t1/t4
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 tree t1
    t1
    ├── t2
    ├── t3
    │   ├── t5
    │   └── t6
    └── t4
    ```
1. Uporabite ukaz tree tako, da vam izpise samo prvi nivo drevesa
    ```bash
    ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 tree ./t1 -L 1
    ./t1
    ├── t2
    ├── t3
    └── t4
    ```

1. Napisite ukaz ki izpise naslednje sklope podatkov
    - ime predmeta
    - vpisna stevilka, ime in priimek
    - studijski program oz. smer
        ```bash
        ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 echo -e "Operacijski sistemi\n63200019\tGašper Oblak\nRačunalništvo in informatika"
        Operacijski sistemi
        63200019        Gašper Oblak
        Računalništvo in informatika
        ```
        ```bash
        ➜  ~/Faks/2_letnik/OS/izzivi/izziv1 printf "%s\n%d\t%s\n%s\n" "Operacijski sistemi" 63200019 "Gašper Oblak" "Računalništvo in informatika"
        Operacijski sistemi
        63200019        Gašper Oblak
        Računalništvo in informatika
        ```
