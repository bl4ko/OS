#### Stream editor - sed

| Descriptor | Usage |
| - | - |
| `p` | prints the line |
| `d` | deletes the line |
| `s` | substitues pattern and action |

##### Deleting lines
```bash
sed `4,10d` file    # Delete Lines [4,10 ]
sed `4,+5d` file    # deletes Lines [4,4+5]
sed `2,5!`  file    # Deletes all lines except range [2,5]
```

##### Printing lines
```bash
# Use the -n option to avoid repetiotion of line printing
sed - n`4,10p`         # Print lines [4,10]
```

##### Substitution
```bash
‚ûú  ~/Faks/2_year/OS/vaje git:(main) ‚úó cat text | sed 's/t/T/'
10 Tiny toes
This is that
5 funny 0
one Two three
Tree twice
```

- replace all occurances on each line: `sed s/<replace>/<with>/g`
```bash
‚ûú  ~/Faks/2_year/OS/vaje git:(main) ‚úó cat text | sed 's/t/T/g'
10 Tiny Toes
This is ThaT
5 funny 0
one Two Three
Tree Twice
```

