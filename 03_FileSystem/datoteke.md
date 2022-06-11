# Datotecni sistem
## Fizicna organizacija podatkov
## Vrste datotecnih sistemov
- `diskovni`: ext2, ext3, ext4, btrfs (Linux), hfs(Mac), ntfs(Windows)
- `mrezni`: nfs (Network), 
- `posebni`: proc `/proc/`, sysfs `/sys/`, umsdos,userfs, udev `/dev`

## Gonilniki
- `gonilnik za pomnilno napravo`
    - napravo predstavi kot zaporedje blokov
    - velikost bloka je fiksna, npr. 4KiB
- `gonilnik za datotecni sistem`
    - organizira bloke med seboj in jim doda pomen
    - blok je najmanjsa alokacijska enota
    - vsebina datoteke je zapisana v blokih

## Naslavjljanje datotek
- `pot` do datoteke (seznam imen imenikov od ustreznega izvornega imenika do dane datoteke)
- `absolutna pot` pot ki se pricne v korenskem imeniku
- `relativna pot` pot ki se pricne v delovnem imeniku

## Abstrakcija datoteke
| **Oznaka** | **Opis** | **Primer** |
| - | - | - |
| `-` | navadna datoteka (regular file) | /etc/passwd |
| `d` | imenik (directory) | /bin |
| `s` | simbolicna povezava (symbolic link) | /usr/src/linux |
| `b` | blocno orientirana naprava (block special device) | /dev/sda |
| `c` | Znakovno orientirana naprava (character special device) | /dev/tty |
| `p` | poimenovana cev (named pipe) | |
| `s` | Vticnica (socket) | |

### Navadna datoteka
- vsebina:
    - poljubna interna strukutra oz. format datoteke
    - **Binarne** in **tekstovne** datoteke
    - OS omogoca notranje operacije (nad vsebino) odpiranje, zapiranje, branje pisanje
- **vrsta datoteke**
    - razlicne vrste glede na vsebino
    - pogosto del imena (npr. koncnia) podajo vrsto

### Imenik
- Datoteka posebnega tipa
    - njena vsebina je seznam imeniskih vnosov (vsak imeniski vnos ustreza neki datoteki)
- `imeniski vnos / directory entry`: ime datoteke, kazalci na ostale podatke (vsebina, meta-podatki), znotraj imenika ni mogoce imeti imeniskih vnosov z enakim imenom

### Simbolicna (mehka) povezava
- datoteke posebnega tipa 
- vsebina -> naslov (pot do) ciljne datoteke
- ln -s original mehka
- `mehka --> original --> podatki`

### Trda povezava
- dodaten imeniski vnos za isto datoteko
- ln original trda
```txt
original ---> podatki
            /
trda -------
```

### Blocna in znakovna naprava
- branje in pisanje taksne datoteke se neposredno nanasa na napravo
- blocne naprave:
    - dostop po blokih (npr. 4096B)
- znakovne naprave
    - dostop po znakih oz. bajtih

### Cev (pipe) in vticnica (socket)
- mehanizem za medprocesno komunikacijo
- naslavljanje preko imen datotek


# Nadzor dostopa in zascita datotek
**Nadzorni seznam dostopa** `Access control list`
