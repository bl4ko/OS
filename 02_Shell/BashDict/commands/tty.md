### Terminal types
- `tty` is a regular terminal device (the console on your server for example)
- `pty` (pseudo terminal device) is a terminal __which is emulated__ by an other program (example: xterm, ssh, ...)
- `pts` is the slave part of pty

```bash
# open new terminal window
$ tty
/dev/pts/0
# open second terminal window
$ tty
/dev/pts/1
# on first terminal window write:
```

