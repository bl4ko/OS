#  Mutual exclusion

## Kljucavnica

Hocemo dobit atomicnost na strojnem nivoju (**zaporedje ukazov se izvede kot celota** (nedeljivo))
```
atomic fun lock_enter(lock) is
    while lock == 1 do nthing 
    lock = 1
```

vstop v KO: **onemogocimo prekinitve**
