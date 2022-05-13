`Kljucavnica(lock,mutex)` mehanizem **zaklepanja**.
- dve stanji: **odklenjeno** in **zaklenjeno**
- nit, ki zaklene kljucavnico si jo last
    - le ena nit si lahko socasno lasti kljucavnico
    - le nit ki zaklene kljucavnico jo lahko tudi odklene

## Uporaba
- **operacija zakleni** (lock acquire)
    - 
- **operacija odkleni** (unlock, release)
    - nit ki lasti odklene zaklenjeno kljucavnico

```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&m);
// kritični odsek
pthread_mutex_unlock(&m);
```

## Problem proizvajlci porabniki
```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
queue_t q = ...;

// Proizvajalec P
data = produce_data()
pthread_mutex_lock(&m)
q.enqueue(data)
pthread_mutex_unlock(&m)


// porabnik Q
while true do
if not queue.empty then
pthread_mutex_lock(&m)
q.dequeue(data)
pthread_mutex_unlock(&m)
consume_data(data)
else nothing
done
```

