## Funkcije
- Definciija `function ime { ukazi }`
- Argumenti: `$1, $2, $3`
- Izhodni status namenjen da pove ali funkcija deluje pravilno
    - vracamo z <u>echo</u>
    - v bashu se vse vraca na standardni izhod
- Vrnjena vrednost v spremenljivki `$?`

```bash
function fakt1 {
    if (( $1 <= 1 )); then
        echo 1
    else 
        tmp=$(fakt2 $(($1 - 1)))
        return $(($1 * $tmp))
    fi
}
```

