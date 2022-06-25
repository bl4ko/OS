# Concurrency

1. `Socasnost/Concurrency`: obcutek hkratnega izvajanja (menjavanje stanja)
    - socasnost/brez vzporednosti (1 procesor menjava procesov)
    - socasnost z vzporednostjo (vec-procesorski sistem, prekrivanje ukazov)


1. `Vzporednost/Parallelism`: **dejansko hkratno izvajanje (vec procesorjev)**
    - **vzporednost brez socasnosti** (vektorski ukazi, vsak procesor del ukaza)

1. `Porazdeljenost`: izvajanje vec procesov v vec vozliscih omrezja

> Problem: deljenje globalnih virov, [deadlock](https://en.wikipedia.org/wiki/Deadlockzavno odkrivanje prog napak
