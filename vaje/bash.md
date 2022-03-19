# Bash Basics

### Shell history
```bash
history
histroy | grep ls
```

### Special keyboard shortcuts
| Keybinding | Shortcut |
| - | - | 
| `Ctrl+c` | Interrupt/Kill process (SIGINT) |
| `Ctrl+z` | Suspend process (SIGSTP) |
| `Ctrl+d` | __EOF marker__, or close the current shelll |
| `Ctrl+r` | Reverse search |
| `Ctrl+l` | Clear screen |

### Useful commands

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
- `

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

####


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


### Linking
- `ln` createas a link to file 
- A __link__ is an entry in your file system which connects a __file name__ to the actual __bytes of data on the disk__. 

```bash
echo "This is a file." > file1.txt
cat file1.txt
# This is a file
```
When this file was created, the operating system wrote the bytes to a location on the disk and linked the data to a file name.

![link](./images/link.png)

`Link` command: manually create a link to file data that already exists.

```bash
link file1.txt file2.txt
cat file2.txt
# This is a file.
```
- We did not make a copy of this data. Both file names point to the same bytes of data on the disk.

![linking](./images/linking.png)

```bash
echo "It points to data on the disk." >> file1.txt
cat file2.txt
# This is a file.
# It points to data on the disk
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
# cat: file2.txt: No such file or directory.
```

#### Creating symbolic links to directories
```bash
ln -s ~/documents/ test
ls documents
ls test
```


##  Subshells
__Podlupina__ je podproces trenutne lupine. 
- Navadno (naredimo podlupino):
    - `( ukaz )`
- S substitucijo
    - `$( ukaz )`
- Spremenljivka $BASH_SUBSHELL

### Podlupina
```bash
a=123
( echo $a )
# 123

( a=456 )
echo $a
# 123
```

### Substiticija
```bash
echo ($ls) # primer ukaz izvedemo in tisto kar izpisejo upoarbimo naprej
echo Ime imenika: $(basename $(pwd))
```

### Preusmerjanje
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

### Cevovod
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

### Conditional operators
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

### Zdruzevanje ukazov
- Uporabimo statement block `{ ukazi;... }`
```bash
mkdir test && { cd test; touch dat.txt; echo Zivjo; }
test -f /etc/passwd && { echo Imamo jo.; cat /etc/passwd; } || echo Napaka
```

### Aritmetika
- Uporabljamo dvojne oklepaje
    - `(( aritmetika ))`, `$(( aritmetika ))`

```bash
(( a = 1 + 2 * 3 ))
a=$(( 1 + 2*3 ))
(( b=a**a, a++, c = a + 1, a += 2 ))
(( a <= 12 )) && echo Malo || echo Veliko
echo $a $b $c $(( 42 * 24 ))
```

### Funkcije
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

### Obnasanje funkcij in iteratorjev
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


### Sistemski klici
```bash
uname                                       # get name and information about current kernel
cd /proc                                    # mape kjer so podatki o procesih
cat cmdline                                 # BOOT_IMAGE=/boot/vmlinuz-4.4.0-38-generic         tukaj je shranjeno jedro
cat uptime                                  # podatki o sistemu
dmesg                                       # print or control the kernel ring buffer
`tis ``