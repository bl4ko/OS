## Aritmetika
- Uporabljamo dvojne oklepaje
    - `(( aritmetika ))`, `$(( aritmetika ))`

```bash
(( a = 1 + 2 * 3 ))
a=$(( 1 + 2*3 ))
(( b=a**a, a++, c = a + 1, a += 2 ))
(( a <= 12 )) && echo Malo || echo Veliko
echo $a $b $c $(( 42 * 24 ))
```

