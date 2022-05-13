`Proces` = program, ki je zagnan v operacijskem sistemu
- vsebuje programsko kodo
- ima alociran pomnilnik in druge vire

`Nit` = del procesa
- zaporedje ukazov, ki se izvajajo na procesorju
- vsebuje del programske kode
- Vse niti se izvajao "vzporedno"
- vse niti si delijo pomnilnik in ostale vire

`Vecnitnost`:
- najpreprostejsi programi vsebujejo le eno nit, ukazi se izvajajo eden za drugim
- v vecini programov se izvaja vec niti hkrati

### Primer vec nitnega programa - urejevalnik besedila
1. Nit, ki spremlja gibanje miske
1. Nit, ki bere s tipkovnice 
1. nit, ki izrisuje besedilo na ekran
1. nit, ki shrani dokument na disk vsakih 10 minut

> V najboljšem primeru bi bil program manj odziven, ker bi edina nit morala "skakati" z enega opravila na drugega. V resnici je problem še večji, saj je delo z vhodno/izhodnimi napravami blokirajoče. Med pisanjem na disk ali sprejemanjem podatkov preko omrežja bi program "zmrznil".

### Prednosti vecnitnosti


### Problem vec-nitnosti
- zaklepanje spremenljivk: vse niti ki se izvajajo znotraj enega procesa, si delijo pomnilnik in 
ostale vire. Pri dostopanju do skupnih virov moramo biti pazljivi.

Primer: Ce hoceta dve niti ob istem casu povecati vrednost stevca, lahko postane vrednost spremenljivke neveljavna

> Takim situacijam se lahko izognemo z t.i. zaklepanjem blokov programske kode (`locking`)

```py
# Zaklepanje bloka kode v jeziki Python
lock = threading.Lock()
with lock:
    student.pojej(new Sladoled())
    vrstaZaSladoled.remove(student)
    zalogaSladoleda--
```
