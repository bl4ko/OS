# Process Synchronization

`Process Synchronization` means coordinating the execution of processes such that no two processes access the same shared resources and data. It is required in a multi-process system where multiple processes run together, and more than one process tries to gain access to the same shared resource or data at the same time.

## Lock/Mutex
- mehanizem **zaklepanja**
    - dve stanji: **odkelnjeno** in **zaklenjeno**
    - nit ki zaklene kljucavnico si jo lasti
        - dvojno zaklepanje ni definirano (napaka, ignoriranje, smrtni objem)
    - le ena nit si lahko socasno lasti kljucavnico
    - nit, ki zaklene kljucavnico, jo lahko tudi odklene
        - odklepanje kljucavnice, ki jo drzi druga nit, ni definirano

- **uporaba**:
    - vzaejmno izkljucevanje na kriticnih odsekih

```c
// Example C POSIX library
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&m);
// kriticni odsek
pthrad_mutex_unlock(&m);
```

