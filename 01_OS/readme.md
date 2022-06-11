# Operacijski sistem
- Kaj je operacijski sistem?
    - nabor programske oprpeme
    - nadzoruje izvajanje programov
    - povezuje uporabnika s strojno opremo
    - deluje kot vmesnik med programi in strojno opremo

## Kaj sodi pod operacijski sistem
- jedro, gonilniki naprav, lupina, sistemska orodja
- v praksi OS pogosto vsebuje tudi neko SPO
    - npr Linux .. jedro OS, GNU ... programska oprema

## Storitve OS
- upravljanje z **uporabniki**
- upravljanje z **procesi**
- upravljanje s **pomnilnikom**
- upravljanje z **datotecnimi sistemi** in **datotekami**
- upravljanje z **V/I napravami**
- med procesna komunikacija in sinhronizacija
- racunalniska komunikacija in mreenje
- zascita procesov in varnost podatkov
- statistika in racunovodstvo (o uporabi virov) ...

> Kaj so <u>cilji</u> operacijskih sistemov?
- **preporsta** uporaba (ease of use)
- **varnost** uporabe (security)
- **zanesljivost** delovanja (reliability)
- **ucinkovitost** delovanja (performance)
- **prilagodljivost** sistema (flexibility)

## Koncepti OS
### Abstrakcija
- posplositev in skrivanje podrobnosti
- primer **datoteka**: 
    - abstrakcija datoteke kot hranjene zbirke podatkov, 
    - fizicno je datoteka lahko hranjena na veliko razlicnih nacinov

### Virtualizacija
- mehanizem ki preslikava navidezno v realno 
    - **navidezni procesor**
    - **navidezni pomnilnik**

### Abstrakcija in virtualizacija
- komplementarna koncepta
    - primer **navidezni datotecni sistem (VFS)**

### Varnost
- mehanizmi zascite sistema
    - **izolacija procesov**
    - **zascita pomnilnika**
    - **zascita V/I naprav**
    - **zascita datotek**

### Socasnost
- obstoj vec procesov hkrati
    - **niti**

### Persistenca
- Dolgorocni obstoj podatkov in informacij, ucinkovitost hrambe
    - primer: **datoteka**


## Jedro OS
### Kaj je jedro
Programska koda, ki vsebuje bistveni del OS (najpomembnejse storitve)
- izvaja se v **priviligeranem nacinu** delovanja procesorja

### Procesorski nivoji zascite
- `uporabniski prostor` (`zasciteni nacin`)
    - omejena uporaba procesorja
        - nadzorovan dostop do pomnilnika, neposreden dostop do naprav ni mogoc, nekaterih ukazov ni mogoce izvesti
    - napacna uporaba povzroci **izjemo**
- `jedrni prostor` (`priviligiran nacin`)
    - neomejen dostop do pomnilnika in naprav
    - nekateri ukazi se lahko izvajajo samo v tem nacinu
- `hipervizorski nacin`
    - za samo OS ni potreben
    - potreben za vitualizacijo

### Komunikacija z jedrom
> Kako programi zahtevajo storitve jedra?
- preko vmesnika jedra = `sistemski klici`

#### Komunikacija med jedrom in strojno opremo
- `vmesnik strojne opreme`: mehanizem programskega podajanja ukazov napravam
- `goniliniki naprav`: programska koda ki zna uporavljati z neko napravo preko vmesnikov strojne opreme (ponavadi del jedra)



## Arhitektura jedra
Vrste jedrnih arhitektur:
### `Monolitno jedro`
- velik kos strojne kode tece v jedrnem nivoju v skupnem naslovnem prostoru
- posaemzni deli OS lahko med seboj zelo hitro komunicirajo preko navadnih klicev funkcij
- **SLABOSTI**: 
    - napaka v enem delu lahko povzroci sestuje celotn
    - tezja obvladljivost kode
    - sprememba izvorne kode - ponvno prevajanje jedra
- **PRIMERI**:
    - System V, DOS, Windows 9x, FreeDOS, OpenVMS
### `Monolitno modularno`
- omogoca **modularno** zasnovo jedra
    - modul ponavadi vsebuje gonilnik naprave
- dinamicnost: module je moc vloziti (load) in tudi izlociti (unload) iz jredra tekom samega izvajanja (ni potreben reset sistema)
- **PRIMERI**:
    - BSD, Linux, OS-9, AIX, ..

### `Mikro jedro`
- vsebuje samo osnovne funkcionalnosti
    - procesi, pomnilnik, medprocesna komunikacija
- ostale funkcionalnosti so izvedene preko procesov (v uporabniskem nacinu)
    - uporaba **medprocesne komunikacije**
    - princip komunikacije: odjemalec - streznik
    - medsebojni klici so zahtevnejsti
- **PREDNOSTI**: prilagodljivost porazdeljenost, varnost, enostavnejsa implementacija

### `Hibridno`
- zasnova je **mikro jedrna** (loceni servisi)
- izvedba pa **monolitna** (skupen naslovni prostor)
- **PRIMER**: Windows NT

### `Unijedro`
- `nano jedro, piko jedro`: manjse kot mikro jedro
- `exokernel`: se mansi od mikro jedra, omogoca le zascito in souporabo virov
- `unikernel`: specificno namensko jedro za izbrano aplikacijo


