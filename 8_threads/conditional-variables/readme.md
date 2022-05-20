`Pogojna spremenljivka` omogoca **mehanizem obvescanja**
- omogoci da **cakajoce niti uspavamo** 
- deluojca nit, ko konca s signalom obvesti cakajoce niti o izpolnjenosti pogoja

> Uporaba: <u>optimizacija uporabe procesorja</u>

## C functions
```c
pthread_cond_broadcast(&condVar);   # send signal to all waiting thread
pthread_cond_signal(&condVar);      # send signal to some waiting threads
```

## Problem proizvajalci-porabniki

```c
// Resitev z zaklepanjem in pogojno spremenljivko
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
queue_t q = ...;
```

## Problem bralci-pisalci
```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t r = PTHREAD_COND_INITIALIZER;
pthread_cond_t w = PTHREAD_COND_INITIALIZER;
volatile int counter = 0;  // steje stevilo bralcev (koliko trenutno bere)
// counter = -1 pisalec pise


/* -- BRALCI -------------------------------------- */
// Vstop v kriticni del branja
pthread_mutex_lock(&m)
    while counter == -1 do
        wait(m, r) // grem v vrsto
    counter++
pthread_mutex_unlock(&m)

// branje podatkov

pthread_mutex_lock(&m)
    counter--
    if counter == 0 then
        signal(w) // posljemo signal vrsti pisalcev
pthread_mutex_unlock(&m)


/* -- PISALCI ------------------------------------ */
pthread_mutex_lock(&m)
	while counter != 0 do
		wait(m, w) // grem v vrsto
	counter = -1
pthread_mutex_unlock(&m)

// pisanje podatkov

pthread_mutex_lock(&m)
	counter = 0
	broadcast(r)
	signal(w)
pthread_mutex_unlock(&m)
```
