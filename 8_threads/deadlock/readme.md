Deadlock when locking mutex **twice** on the same thread.
```c
void* routine(void* args) {
    pthread_mutex_lock(&mutexFuel);
    pthread_mutex_lock(&mutexFuel);
    fuel += 50;
    printf("Incremented fuel to: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

```
> Be careful in which order you are locking threads
```c
if (rand() % 2 == 0) {
    pthread_mutex_lock(&mutexFuel); // first thread locks it   
    pthread_mutex_lock(&mutexWater); // wait for this lock to be released
} else {
    pthread_mutex_lock(&mutexWater); // second thread locks it
    pthread_mutex_lock(&mutexFuel); // wait for this lock to be released
}
```

```txt 
            Fuel Mutex | Water Mutex |
Thread1 |   X          |   wait      |
Thread2 |   wait       |    x        |
```
