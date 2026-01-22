## Configuration initiale

### Compilation

```bash
cc main.c $(mysql_config --cflags --libs) -o delivraptor_server
```

## Exécution

```bash
./delivraptor_server -p 8080 -c 3 -a auth.txt -l delivraptor.log
```
