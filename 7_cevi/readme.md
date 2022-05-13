Inmemory file, from which we can write or read.

## Sistemski klic dup2
```txt
orig --> [|||]                                   orig ---> [|||]
                                                           ^ 
                        dup2(orig, new)                   /  
                                                         /
new  --> [|||]                                      new /
```