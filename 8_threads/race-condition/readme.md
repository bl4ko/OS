A `race condition` occurs when two or more threads **can access shared data** and they try to **change it at the same time**. Because the thread scheduling algorithm can swap between threads at any time, you don't know the order in which the threads will attempt to access the shared data. Therefore, the result of the change in data is dependent on the thread scheduling algorithm, i.e. both threads are "racing" to access/change the data.

```c
void* routine() {
    // this is a critical section
    // not atomic operation
    for (int i = 0; i < 10000; i++) {
        mails++;
        // load mails
        // increment mails
        // store
    }
}
```

## Solution: Lock(Mutex)
- Only one thread at the timecan enter the **critical section**
    - we achive this by locking the code

`Mutex` = lock around a section of code.

```c
void* routine() {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}
```

