# Sistemski klici


### Rezultat funkcije
- izhodni status izvedbe funkcije
- izhodni status sistemskega klica
    - uspeh: 0
    - neuspeh: -1
        - polno izjem
            - read, write vrneta st zapisanih bajtov
            - getpid, getppid .. vrne pid procesa in ne (0 ter -1)

### Obdelava napak
- Koda zadnje napake
    - spremenljivka errno
- Izpis obvestila o napaki funkcija `perror()`

```bash
errno -l            # izpise vse kode napak
errno 42            # izpise: ENOMSG 42 No message of desired type
```