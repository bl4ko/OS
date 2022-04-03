# Bash Basics

# 1. Osnove

### Special keyboard shortcuts
| Keybinding | Shortcut |
| - | - | 
| `Ctrl+c` | Interrupt/Kill process (SIGINT) |
| `Ctrl+z` | Suspend process (SIGSTP) |
| `Ctrl+d` | __EOF marker__, or close the current shelll |
| `Ctrl+r` | Reverse search |
| `Ctrl+l` | Clear screen |

#### Shell history
```bash
history
histroy | grep ls
```

### Terminal types
- `tty` is a regular terminal device (the console on your server for example)
- `pty` (pseudo terminal device) is a terminal __which is emulated__ by an other program (example: xterm, ssh, ...)
- `pts` is the slave part of pty

```bash
# open new terminal window
$ tty
/dev/pts/0
# open second terminal window
$ tty
/dev/pts/1
# on first terminal window write:

```

# 2. Skriptiranje

## Useful commands

#### Stream editor - sed

| Descriptor | Usage |
| - | - |
| `p` | prints the line |
| `d` | deletes the line |
| `s` | substitues pattern and action |

##### Deleting lines
```bash
sed `4,10d` file    # Delete Lines [4,10 ]
sed `4,+5d` file    # deletes Lines [4,4+5]
sed `2,5!`  file    # Deletes all lines except range [2,5]
```

##### Printing lines
```bash
# Use the -n option to avoid repetiotion of line printing
sed - n`4,10p`         # Print lines [4,10]
```

##### Substitution
```bash
➜  ~/Faks/2_year/OS/vaje git:(main) ✗ cat text | sed 's/t/T/'
10 Tiny toes
This is that
5 funny 0
one Two three
Tree twice
```

- replace all occurances on each line: `sed s/<replace>/<with>/g`
```bash
➜  ~/Faks/2_year/OS/vaje git:(main) ✗ cat text | sed 's/t/T/g'
10 Tiny Toes
This is ThaT
5 funny 0
one Two Three
Tree Twice
```

## Variables

### Osnove

```bash
var=10
${var}              # osnovni nacin
$var                # krajsi nacin

# -- Privzete vrednosti
${ime:-privzeto}    # vsebina ali privzeta vrednost
${ime:=privzeto}    # nastvitev vrednosti

# -- Kadar spremnljivka vsebuje ime spremenljivke
#{!ime}
a=123; b=a; echo $b ${!b}

# -- Delo s podnizi
${#ime}                     # dolzina niza
${ime:poz}                  # podniz od poz do konca
${ime:poz:dol}              # podniz na poz dolzine dol

# -- Odstranjevanje podnizov
a=/home/user/test.txt
file_name=${a##*/}
base=${file_name%.*}

${ime#vzorec}               # najkrajsa predpona
${ime##vzorec}              # najdaljsa predpona
${ime%vzorec}               # najkrajsa pripona
${ime%vzorec}               # najkrajsa pripona
${ime%%vzorec}              # najdaljsa predpona

# -- Zamenjava podniza
${ime/vzorec/vrednost}      # zamenja prvo pojavitev
${ime//vzorec/vrednost}     # zamenja vse pojavitev
```

### Enojne in dvojne navednice
- enojne vzameejo niz __dobesedno__
- dvojne izvedejo __substitucije__  
```bash
a=Triglav
echo '$a'                   # $a
echo "$a"                   # Triglav


a='Martin        Krpan'
echo $a                 # Martin Krpan
echo "$a" .             # Martin        Krpan
```

### Special variables
| variable | function |
| - | - |
| `$_` | zadnji argument predhodno izvedenga ukaza |
| `$0` | ime skripte |
| `$1`,`$2`,...,`$n` | zaporedni argumenti skripte |
| `$?` | izhodni status zadnjega izvedenga ukaza |
| `$$` | PID lupine |
| `$!` | PID procesa, ki je bil zadnji zagnan v ozadju |
| `$*`, `$@` | Vsi argumenti skripte skupaj |
| `$-` | opcije podane lupini, ki poganja skripto |

### Exporting variables
Exported varaibles such as `$HOME` and `$PATH` are available to (inherited by) other programs run by the shell that exports them (and the programs 
run by those other programs, and so on) as __enviromental variables__. Regular (non-exported) variables are not avaliable to other programs.
- we find them by command `env`
- `$PATH`: tukaj bash isce ukaze (kje se nahaja)

```bash
$ env | grep '^variable='
$                                 # No environment variable called variable
$ variable=Hello                  # Create local (non-exported) variable with value
$ env | grep '^variable='
$                                 # Still no environment variable called variable
$ export variable                 # Mark variable for export to child processes
$ env | grep '^variable='
variable=Hello
$
$ export other_variable=Goodbye   # create and initialize exported variable
$ env | grep '^other_variable='
other_variable=Goodbye
$
```

Note that non-exported varaibles will be available to subshells run via `( ... )` and similiar notations because those subshells are direct clones
of the main shell.

```bash
$ othervar=present
$ (echo $othervar; echo $variable; variable=elephant; echo $variable)
present
Hello
elephant
$ echo $variable
Hello
$
```
The subshell can change its own copy of any variable, exported or not, and may affect the values seen by the processes it runs
,but the subshell's changes cannot affect the variable in the parent shell, of course.



## Zanke

- `continue`
- `break`

```bash
# reverse order args
for (( i=$#;i>0;i-- ));do
        echo "${!i}"
done 
```

- read line by line

## Branje vhoda
- `$IFS` (internal field separator), nastavimo po cem iterira zanka
    - na koncu obvezzno nastavimo na privzeto: `\n`

```bash
while read vpisna a b c tip
do
    echo "$vpisna $a $b $c $tip"
done
```

##  Random
```bash
RANDOM=42               # nastavimo seme
a=$(( (( $RANDOM % 5 )) + 1))       # pristejemo stevilo od (1, 5)
```

## Substition
```bash
echo ($ls) # primer ukaz izvedemo in tisto kar izpisejo upoarbimo naprej
echo Ime imenika: $(basename $(pwd))
```

## Redirection
- __standardni vhod__ (stdin: deskriptor 0)
- __standardni izhod__ (stdout: deskriptor 1)
- __standardni izhod za napake__ (stderr: deskriptor 2)


```bash
ukaz < datoteka         # preusmerjanje standardnega vhoda
ukaz > datoteka         # preusmerjanje standardnega izhoda
ukaz >> datoteka        # dodajanje

# splosno preusmerjanje
ukaz deskriptor>datoteka

# damo vedt da je deskriptor
cat spisek 1>2 #narobe
cat spisek 1>&2

# preusmeritev stderr (vrstni red preusmeritev je vazn)
ukaz 1>datoteka 2>&1
ukaz &>datoteka         # krajsi nacin

# preusmeritev naprav v "crno luknjo"
mkdir test 2>/dev/null

#  Podajanje programa na standardni vhod koncamo z podanimo locilom ('EOF')
cat <<'EOF' >skripta.sh
➜  python <<EOF
heredoc> print(1+2)
heredoc> EOF
3
```

## Pipeline
Vec ukazov
- `ukaz1 | ukaz2 | ukaz3 | ... | ukazN`

Izrezemo spisek vseh uporabniskih lupin
```bash
cat /etc/passwd | cut -d: -f7 | sort | uniq

# Koliko uporabnikov uporablja posamezno lupino po velikosti (prve tri)
cat /etc/passwd | cut -d: -f7 | sort | uniq -c | sort -nr | head -3

# Razvejitev cevovoda z ukazom tee
echo "JUHUHUHU" | tee izpis.text &>/dev/null

# xargs podajanje argumentov v cevovodu
echo 'one two three' | xargs mkdir
ls
# one two tree
```

## Conditional operators
```bash
# izhodni status
# - 0 true
# - 1 false

# ukaz1 && ukaz2 (nacin in)
false && echo "Se tole izpise"      # ne

# ukaz1 || ukaz2 (nacin ali)
true || echo "Se tole izpise?"      # ne

cd ~ && mkdir test && cd test && touch juhuhu.txt || echo "Tezave na vidiku"
```

## Zdruzevanje ukazov
- Uporabimo statement block `{ ukazi;... }`
```bash
mkdir test && { cd test; touch dat.txt; echo Zivjo; }
test -f /etc/passwd && { echo Imamo jo.; cat /etc/passwd; } || echo Napaka
```

## Aritmetika
- Uporabljamo dvojne oklepaje
    - `(( aritmetika ))`, `$(( aritmetika ))`

```bash
(( a = 1 + 2 * 3 ))
a=$(( 1 + 2*3 ))
(( b=a**a, a++, c = a + 1, a += 2 ))
(( a <= 12 )) && echo Malo || echo Veliko
echo $a $b $c $(( 42 * 24 ))
```

## Funkcije
- Definciija `function ime { ukazi }`
- Argumenti: `$1, $2, $3`
- Izhodni status namenjen da pove ali funkcija deluje pravilno
    - vracamo z <u>echo</u>
    - v bashu se vse vraca na standardni izhod
- Vrnjena vrednost v spremenljivki `$?`

```bash
function fakt1 {
    if (( $1 <= 1 )); then
        echo 1
    else 
        tmp=$(fakt2 $(($1 - 1)))
        return $(($1 * $tmp))
    fi
}
```

## Obnasanje funkcij in iteratorjev
- kot zunanji ukaz ki dobi na vnos __mojadatoteka.txt__
```bash
while read a; do 
    echo "$a"
done <mojadatoteka.txt
```

- array
```bash
studenti= ( Joze Mirko Miha Janez )
for stud in "$(studenti[@]}"; do 
    echo "$student"
Done
```

- niz
```bash
a=$'ena\ndva\ntri\nstiri'
while read line; do
    echo "$line"
done <<< "$a"
```

# 3. Sistemski klici

```bash
uname                                       # get name and information about current kernel
cd /proc                                    # mape kjer so podatki o procesih
cat cmdline                                 # BOOT_IMAGE=/boot/vmlinuz-4.4.0-38-generic  (tukaj je shranjeno jedro)
cat uptime                                  # podatki o sistemu
dmesg                                       # print or control the kernel ring buffer
```

# 4. Uporabniki in grupe

## 4.1 Uporabniki

### Podatki o uporabniki
- podatki o uporabnikih v `/etc/passwd`
    - Lahko pisemo na roke
        - `/usr/bin/nologin` uporabnikom prepovemo da se prijavijo v konzolo

```bash
<username>:<hasPasswd>:<UserID>:<GroupID>:<UserIDInfo>:<HomeDir>:<Shell>
oracle:x:1021:1020:Oracle user:/data/network/oracle:/bin/bash
Username:x-indicates-password-stored-in-/etc/shadow:UID:GID:User ID info:Home-directory:Command/shell
```

- gesla shranjena v __hash__ obliki v `/etc/shadow`

### Modeliranje z uporabniki
- ustvarjanje uporabnika: `useradd`
- odstranjevanje uporabnika: `userdel`
- spreminjanje uporabnika: `usermod`
- Kriptiranje gesla: `md5pass`, `sha1pass`, ...

Primer modeliranja z `usermod`:
### Change password
```bash
sudo passwd <username>          # change password of user
```

### Delete user
```bash
sudo deluser userName
sudo rm -r /home/userName
```

### Change username
```bash
sudo usermod -l newUsername oldUsername
```
### Change home-folder:
```k
sudo usermod -d /home/newHomeDir -m newUsername
```
### Bash script for creating new user
```bash
#!/usr/bin/bash
if (( $# != 2 ))
then
    echo -e "Premalo vhodnih arguemntvo <newUsername> <password>\n"
    exit 1
fi

username=$1
password=$2

useradd -m -d "/home/$username" -s "/usr/bin/zsh" -g sudo -c "Poweruser" $username
usermod --password $(echo $password | openssl passwd -1 -stdin) $username

exit 0
```


## 4.2 Groups

### Podatki o grupah

- `/etc/group`: podatki o grupah
- `/etc/gshadow`: group passwords

```bash
<username>:<encryptedPassword>:<lastPasswordChange>:<Minimum>:<Maximum>:<Warn>:<Inactive>:<Expire>
temporary:$y$j9T$XlG89ZnuW.vjWkdWvjpDy.$7Sm7ntZkV0EP10BF4X7sMR39oeHlyMkLGODa9VEOTt5:19079:0:99999:7:::
robin:$y$j9T$6GJC8c0ua.3GCUzlPhBE/0$Gjwbe/bJ9T06U1kZ5YiAFqNcEXlgd7fYi3DIil1AW10:19079:0:99999:7:::
```


2. <u>dodajanje v **grupo**</u>
```bash
sudo usermod -aG sudo user
```

- identifikacija stevilka skupine - `gid`
- spisek skuping: `groups`
- informacije o uporabniku `id`
- zamenjava skupine: `newgrp`, `sg`
```bash
sudo groupadd student                   # ustavrimo skupino student: cat /etc/group
sudo gpasswd -g student                 # nastavimo geslo za grupo student
sudo gpasswd -a robin student           # dodamo uporabnika robin v grupo student
sudo gpasswd -d robin student           # odstranimo robin iz grupe student 
```
- Nastavitev gesla: `gpasswd`
- Ustvarjanje skupine: `groupadd`
- Odstranjevanje skupine: `groupdel`
- Spreminjanje skupine: `groupmod`

### Change hostname
```bash
sudo vim /etc/hostname
sudo vim /etc/hosts             # Replace any occurance of the existing computer name with your new one
sudo reboot
```

# 5. Datoteke

## File permissions
- Operacije oz. dovoljenja nad datotekami (drugacen pomen pri imenikih!!!)
    - `r` read
    - `w` write
    - `x` execute
    - `-` prazno

| __Znak__ | __Datoteka__ | __Imenik__ |
| - | - | - |
| `r` | read | izpis imenika |
| `w`| pisanje | spreminjanje |
| `x` | izvajanje | vstop v imenik |

- Sklopi uporabnikov
    - `u` - user
    - `g` - group
    - `o` - ostali
    - `a` - vsi

- Spreminjanje dovoljenj
    - `+` - dodajanje
    - `-` - odvzemanje
    - `=` - nastavljanje
- `chmod [augo][+-=]`

```bash
chmod u+x test.txt
chmod g+rwx test.txt
chmod o-rwx test.txt
```

- spreminjanje z okteti

```bash
chmod 775 test.txt
```

-  Change owner of a file and group

```bash
chown matevz test.txt
chgrp student test.txt
```

## Zascita datotek

### Omejeno brisanje
- Restricted deleition
    - nastavimo za imenik, velja za vsebovane datoteke
        - datotteko v imeniku lahko odstrani le njen lastnik
- oznaka `t`
    - `chmod +t datoteka`

### Setuid/setgid bit
- Omgoca zagon izvsljive datoteke z dovoljenji lastnika
- Oznaka s (sklop lastnik in skupina)
    - `chmod +s` datoteka

## Patterns
- Osnovni vzorci:
    - `*` - poljuben niz
    - `?` - poljuben znak
    - `[znaki]` poljuben znak iz danega nabora znakov

```bash
ls *.zip
echo b*.txt
echo b?.txst
ls ?2.txt f*
echo ???
ls ?
echo ?
echo c[[:digit:]].txt
echo [ac][13].*
echo [a-c][13].t?t
echo [a-c][13].t?t
echo [a-c]?[02].*xt
```
- Napredni vzorci
    - `?`(vzorci) - 0 ali 1 ponovitev
    - `*`(vzorci) - 0 ali vec ponovitev


## Linking
- `ln` createas a link to file 
- A __link__ is an entry in your file system which connects a __file name__ to the actual __bytes of data on the disk__. 

```bash
echo "This is a file." > file1.txt
cat file1.txt
```
When this file was created, the operating system wrote the bytes to a location on the disk and linked the data to a file name.

![link](./images/link.png)

`Link` command: manually create a link to file data that already exists.

```bash
link file1.txt file2.txt
cat file2.txt
---> This is a file.
```
- We did not make a copy of this data. Both file names point to the same bytes of data on the disk.

![linking](./images/linking.png)

```bash
echo "It points to data on the disk." >> file1.txt
cat file2.txt
```
> If we delete one of the files, we're deleting only one of the links to the data but not the data.

#### Difference between ln and link
- `ln` byt default, creates a hard link like link does
```bash
ln file1.txt file2.txt
link file1.txt file2.txt
```
However we can also use `ln` to create sumbolic links with the `-s` option
```bash
ln -s file1.txt file2.txt
```
Creates a <u>symbolic link</u> (soft link): instead linking to the data of a file, they link to another link.
![trda-povezava](./images/creating-symbolic-links.png)

```bash
echo "This is a file." > file1.txt
ln -s file1.txt file2.txt
rm file1.txt
cat file2.txt
# 123

( a=456 )
echo $a
# 123
```


# 6. Procesi
- Program ki se dejansko izvaja
    - ima `PID` in `PPID` (parent ID)
- drevo procesov `pstree`
    - `pstree -p 1` (od kerega PID naprej, v tem primeru systemd)

- okolje procesa -> okoljske spremenljivke
```c
int main(int argc, char* argv[], char* environ[]) {
    for (int i = 0; i < argc; i++) {

    }

}
```

- Jedrne niti (kernel threads): __procesi ki tecejo v jedrnem nacinu__
    - ustvari jih lahko samo jedro
    - v ozadju izvajajo operacije jedar
- Procesi shranjeni v `/proc/<PID>`
    - `exe`
    - `environ`
    - `cwd` in `root`
    - `fd` (file descriptors)

## Izpis procesov
- hiearhija: `pstree`
- procesi: `ps -aux`
- graficni izris: `htop`

## Info o procesih
- PID lupine: `$$`
- PPID lupine: `$PPID`
- PID procesa: `$BASHPID`

## Zagon ukazov v lupini
- Program:
    - se zazene
    - ima nadzor nad konzolo
    - vrne izhodni status lupini
- Lupina:
    - ga caka da se konca
    - prevzame izhodni status

Zagon v ozadju z `&`
```bash
┌──(temporary㉿hood)-[~]
└─$ sleep 30 &
[1] 15333

┌──(temporary㉿hood)-[~]
└─$ sleep 40 &
[2] 15871

┌──(temporary㉿hood)-[~]
└─$ jobs
[1]-  Running                 sleep 30 &
[2]+  Running                 sleep 40 &
```
- cakanje vseh otrok na dokoncanje
```bash
┌──(temporary㉿hood)-[~]
└─$ sleep 20 &
[1] 37055

┌──(temporary㉿hood)-[~]
└─$ sleep 40 &
[2] 37056

┌──(temporary㉿hood)-[~]
└─$ wait
```
- premik v ospredje in ozadje
```bash
┌──(temporary㉿hood)-[~]
└─$ sleep 30 &
[1] 37058

┌──(temporary㉿hood)-[~]
└─$ fg
sleep 30

┌──(temporary㉿hood)-[~]
└─$ firefox
^Z
[1]+  Stopped                 firefox

┌──(temporary㉿hood)-[~]
└─$ bg
[1]+ firefox &

┌──(temporary㉿hood)-[~]
└─$ fg
firefox
^CExiting due to channel error.
Exiting due to channel error.
Exiting due to channel error.
Exiting due to channel error.
```
- __screen__ naredi virtualno konzolo kjer lahko mi zazenemo stvari
    - <ctrl>+<a>+<d> (detachamo iz skrina)

## Jobs
__A job in Linux is a command or a task that is up and running but hasn't yet finished.__

| **Command** | **Meaning** |
| - | - | 
| `jobs` | __List__ all the __jobs__ that are running or suspended. |
| `fg` | Bring the job to the __foreground__ |
| `bg` | Send the job to the __background__ |
| `stop` or `Ctrl+z` | __Suspend__ the job |
| `kill` or `Ctrl+c` | __Terminate__ the job |

