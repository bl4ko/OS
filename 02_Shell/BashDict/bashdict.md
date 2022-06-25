
| `Command` | Meaning |
| - |  - |
| [history](./commands/history.md) | command history |
| [tty](./commands/tty.md) | return user's terminal name |
| [`uname`](./commands/uname.md) | print operating system name | 
| [`dmesg`](./commands/dmesg.md) | print or control the kernel ring buffer |
| [`uptime`](./commands/uptime.md) | show how long system has been running |


| `Stream editing` | Meaning |
| - | - |
| [sed](./commands/sed.md) | **stream editor** | 


| `Variables` | Meaning |
| - | - |
|  | |


### Compound commands

| <h5 style="text-align: left;"><u>Grouping</u></h5> | Meaning |
| - | - |
| [`{ ...; }`]() | command grouping |
| [`( ... )`]() | command grouping in a subshell |
| <h5><u>Conditionals</u></h5> | |
| [`[[ ... ]]`](./commands/conditional-expression.md) | conditional expression | 
| [`if ...; then ...; fi `](./commands/conditional-branching.md) | conditional branching | 
| [`case ... esac`](./commands/pattern-based-branching.md) | pattern-based branching | 
| <h5><u>Loops</u></h5> |  |
| [`for word in ...; do ...; done`](./commands/classic_for_loop.md) | classic for-loop | 
| [`for (x=1; x<=10; x++); do ...; done`](./commands/C_style_for_loop.md) | C-style for-loop | 
| [`while ...; do ...; done`](./commands/while_loop.md) | while loop  | 
| [`until ...; do ...; done`](./commands/until_loop.md) | until loop | 
| <h5><u>Misc</u></h5> |  |
| [`(( ... ))`](./commands/arithmetic_evaluation.md) | arithmetic evaluation | 
| [`select word in ...; do ... ; done ))`](./commands/user_selections.md) | user selections | 
| [`function function_name { commands; }`](./commands/functions.md) | user selections | 



### Expansions and substitutions
| <h5 style="text-align: left;"><u>Syntax</u></h5> | Meaning |
| - | - |
| [`{A,B,C} {A..C}`](https://wiki.bash-hackers.org/syntax/expansion/brace) | brace expansion | 
| [`command $(command)`](./commands/command_substitiution.md) | command substitution |

### Builtin Commands
| <h5 style="text-align: left;"><u>Grouping</u></h5> | Meaning |
| - | - |
| [`{ ...; }`]() | command grouping |
| [`( ... )`]() | command grouping in a subshell |
| <h5><u>Conditionals</u></h5> | |
| [`[[ ... ]]`](./commands/conditional-expression.md) | conditional expression | 
| [`if ...; then ...; fi `](./commands/conditional-branching.md) | conditional branching | 
| [`case ... esac`](./commands/pattern-based-branching.md) | pattern-based branching | 
| <h5><u>Loops</u></h5> |  |
| [`for word in ...; do ...; done`](./commands/classic_for_loop.md) | classic for-loop | 
| [`for (x=1; x<=10; x++); do ...; done`](./commands/C_style_for_loop.md) | C-style for-loop | 
| [`while ...; do ...; done`](./commands/while_loop.md) | while loop  | 
| [`until ...; do ...; done`](./commands/until_loop.md) | until loop | 
| <h5><u>Misc</u></h5> |  |
| [`(( ... ))`](./commands/arithmetic_evaluation.md) | arithmetic evaluation | 
| [`select word in ...; do ... ; done ))`](./commands/user_selections.md) | user selections | 
| [`function function_name { commands; }`](./commands/functions.md) | user selections | 


### Variables
| <h5 style="text-align: left;"><u>Variable substiutions</u></h5> | Meaning |
| - | - |
| [`${x:-default}`](./commands/default_values.md) | vsebina ali privzeta vrednost | 
| [`${x:=default}`](./commands/set_default_values.md) | nastavitev vrednosti |

### Arrays
| <h5 style="text-align: left;"><u>Destruction</u></h5> | Meaning |
| - | - |
| [`unset -v ARRAY`](./commands/arrays/destroy_complete_array.md) | Destroys a complete array | 
| [`unset -v ARRAY[N]`](./commands/arrays/destroy_element_at_index.md) | Destroy element at index N | 

