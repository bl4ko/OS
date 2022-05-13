# Signali
`Signal` je programska prekinitev procesa (kratko sporocilo (stevilka signala))

- **izvor** (posiljatelj):
    - jedro OS ali poljuben procees
    - jedro uoprablja signale za sporocanje izjemnih dogodkov

- **ponor** (prejemnik):
    - poljuben proces
    - ciljni proces izovra neposredno nepozna

## Rokovanje signalov
**Rokovanje signalov**:
- vsak signal ima svoj rokovalnik
- odziv na signal
- kratka rutina, ki se izvede ob prejetju signala

**Nacini rokovanja**:
- privzeti rokovalnik: `SIG_DFL` (default)
- ignoriranje signala: `SIG_IGN` (ignore)

**Mozni privzeti odzivi na signale**:
- koncanje procesa
- koncanje procesa in zapis core datoteke (core dump)
- zaustavitev ali nadaljevanje procesa
- privzeto rokovanje opravi OS

## Vrste signalov
- je podana s __stevilko__ oz __oznako__ signala

![trda-povezava](./images/vrstasignala.png)

#### Ukinjanje procesa
Privzeti odziv je koncnje procesa
![trda-povezava](./images/ukinjanje-procesa.png)

#### Programske napake
Privzeti odziv je koncanje procesa in zapis core datoteke
![trda-povezava](./images/programske-napake.png)

#### Nadzor procesov/poslov

![trda-povezava](./images/nadzorprocesov-poslov.png)

#### Razno
Privzeti odziv je koncanje procesa
| Oznaka | Opomba |
| - | - | 
| `SIGUSR1` | prvi uporabniski signal |
| `SIGUSR2` | drugi uporabniski signal |


## Bash
Ukaz `kill`

```bash
kill -l                         # list signalov
kill -l 5                       # pove ime signala
kill -l TRAP                    # cifra signala
kill -SIGUSR1 123 456 789       # koncamo procese 
kill -0 $$; echo $?             # ali procees obstaja
kill -CONT -1                   #
```

## Lovljenje signalov
- **Otrok** deduje rokovalnike starsa 
- **Nov program** ima privzete rokovalnike
    - sistemski klic `exec()` izgubi rokovalnike

Uporabniski rokovalnik
- sistemski klic `signal(signum, sigfun)`
- ob prejetu signala `signum` se poklice funkcija `sigfun`
- podpis rokovalne funkcije

```c
void rokovalnik(int signum) {
    // telo funkcije
}
```
- Rokovalnik koncanih otrok
```c
void sigchld_handler (int signum) {
    int pid, status, serrno;
    serrno = errno;
    while (1) {
        pid = waitpid(WAIT_ANY, &status, WNOHANG);
    if (pid < 0) {
        perror ("waitpid");
        break;
    }
    if (pid == 0) break;
        notice_termination (pid, status);
    }
    errno = serrno;
}

...
signal(SIGCHLD, sigchld_handler);
... 
```

