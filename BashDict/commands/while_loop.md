- example with array
```bash
while read a; do 
    echo "$a"
done < mojadatoteka.txt
```
- example with string
```bash
a=$'ena\ndva\ntri\nstiri'
while read line; do
    echo "$line"
done <<< "$a"
```

