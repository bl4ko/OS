# PIPE
Inmemory file, from which we can write or read.

## Sistemski klic dup2
```txt
orig --> [file1]                orig ---> [file1]                [file1]
                                             ↑                      ↑
                 dup2(orig, new)             |      close(orig)     |
                                             |                      |
new  --> [file2]                            new                    new
```
