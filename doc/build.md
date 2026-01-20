# delivraptor

## Compilation

```bash
gcc -Wall -Wextra main.c -o delivraptor -lmysqlclient
```

## Exécution

```bash
./delivraptor --port 4242 --auth ./auth.txt --capacity 3 --log ./delivraptor.log
```

## Options principales

- --port <num> : port d'écoute (ex. 4242)
- --auth <fichier> : fichier d'authentification (ex. ./auth.txt)
- --capacity <n> : capacité maximale (ex. 3)
- --log <fichier> : fichier de log (ex. ./delivraptor.log)
