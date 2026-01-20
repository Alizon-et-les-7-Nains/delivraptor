# Délivraptor - Cas d'utilisation pratiques

## Table des matières

1. [Introduction](#introduction)
2. [Configuration initiale](#configuration-initiale)
3. [Tests avec Telnet](#tests-avec-telnet)
4. [Création et suivi de colis](#création-et-suivi-de-colis)
5. [Gestion de la capacité](#gestion-de-la-capacité)
6. [Simulation de progression](#simulation-de-progression)
7. [Types de livraison](#types-de-livraison)

## Introduction

Ce document présente les cas d'utilisation pratiques du système Délivraptor. Vous y trouverez des exemples concrets d'utilisation du serveur de livraison, depuis la connexion jusqu'au suivi complet d'un colis.

## Configuration initiale

### Compilation
```bash
cc main.c $(mysql_config --cflags --libs) -o delivraptor_server
```

## Exécution

```bash
./delivraptor_server -p 8080 -c 3 -a auth.txt -l delivraptor.log 
```

## Options principales

- -p <num> : port d'écoute (ex. 4242)
- -a <fichier> : fichier d'authentification (ex. ./auth.txt)
- -c <n> : capacité maximale (ex. 3)
- -l <fichier> : fichier de log (ex. ./delivraptor.log)


## Mode fork du serveur

Le serveur utilise un modèle fork :

- Processus principal : Accepte les connexions
- Processus fils : Gère chaque client indépendamment

**Conséquences pratiques :**

- Chaque connexion a sa propre session
- Authentification nécessaire par connexion
- Les processus fils se terminent avec le client

## Tests avec Telnet

### Session complète d'exemple

```bash
$ telnet localhost 8080
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
```

### 1. Obtenir de l'aide

```
HELP
```

**Réponse :**

```
Commandes disponibles:
  AUTH <username> <password_md5>     - Authentification
  CREATE <commande_id> <destination> - Créer un bordereau
  STATUS <bordereau>                 - Voir le statut d'un colis
  QUEUE_STATUS                       - Voir l'état de la file d'attente
  HELP                               - Afficher cette aide
  QUIT/EXIT                          - Se déconnecter
```

### 2. S'authentifier

```
AUTH alizon e10adc3949ba59abbe56e057f20f883e
```

**Réponses possibles :**

- `AUTH_SUCCESS` : Authentification réussie
- `ERROR AUTH_FAILED` : Échec d'authentification

### 3. Créer un premier colis

```
CREATE 1000000001 10 Rue de la Paix, 75001 Paris
```

**Réponse :**

```
1467226081
```

### 4. Consulter son statut

```
STATUS 1467226081
```

**Réponse (étape 1) :**

```
1467226081|1000000001|10 Rue de la Paix, 75001 Paris|Entrepôt Alizon|1|2026-01-19 10:30:45||
```

### 5. Se déconnecter

```
QUIT
```

**Réponse :**

```
BYE
Connection closed by foreign host.
```

## Création et suivi de colis

### Cas 1 : Création normale

**Commande :**

```
CREATE 1000000002 25 Avenue Montaigne, 75008 Paris
```

**Réponse réussie :**

```
813692617
```

### Cas 2 : Commande déjà existante

**Commande :**

```
CREATE 1000000002 "Nouvelle adresse différente"
```

**Réponse :**

```
813692617
```

_Le même bordereau est retourné, évitant les doublons_

### Cas 3 : ID de commande invalide

**Commande :**

```
CREATE 0 Adresse test
```

**Réponse :**

```
ERROR INVALID_COMMANDE_ID
```

### Cas 4 : Suivi d'un colis en transit

**Commande :**

```
STATUS 6864770470
```

**Réponse (étape 4) :**

```
6864770470|333333333|15 Rue du Commerce|Départ vers plateforme régionale|4|2026-01-19 11:45:30||
```

### Cas 5 : Bordereau inexistant

**Commande :**

```
STATUS 0000000000
```

**Réponse :**

```
ERROR BORDEREAU_NOT_FOUND
```

### Cas 6 : Livraison avec réception d'image

**Contexte :** Colis livré en absence du destinataire

1. **Suivi normal :**

```
STATUS 818949116
818949116|222222222|5 Rue de la République|Chez destinataire|9|2026-01-19 12:15:30|ABSENT|binaire_img
```

2. **Client PHP doit :**
   - Lire la ligne texte
   - Vérifier que `livraison_type = "ABSENT"`
   - Lire les données binaires suivantes
   - Détecter la fin avec `"null\n"`
   - Convertir en image affichable

3. **Code exemple PHP :**

```php
$socket = fsockopen("localhost", 8080);
fwrite($socket, "STATUS 818949116\n");

// Lire la ligne texte
$text_response = fgets($socket);
$data = explode("|", $text_response);

if ($data[6] == "ABSENT") {
  // Lire l'image binaire
  $image_data = '';
  while (!feof($socket)) {
    $chunk = fread($socket, 1024);
    if ($chunk === "null\n") break;
    $image_data .= $chunk;
  }

  // Afficher
  echo '<img src="data:image/jpeg;base64,' . base64_encode($image_data) . '">';
}
```

## Scénarios d'erreur et solutions

| Problème                    | Cause probable       | Solution                         |
| --------------------------- | -------------------- | -------------------------------- |
| `ERROR NOT_AUTHENTICATED`   | Commande avant AUTH  | S'authentifier d'abord           |
| `ERROR DB_QUERY`            | Problème MySQL       | Vérifier que MariaDB tourne      |
| `ERROR BORDEREAU_NOT_FOUND` | Bordereau inexistant | Vérifier le numéro               |
| `ERROR INVALID_MD5_FORMAT`  | Hash MD5 invalide    | Vérifier 32 caractères hexa      |
| Connexion refusée           | Serveur non démarré  | Lancer `./delivraptor_server`    |
| Timeout connexion           | Port bloqué          | Vérifier firewall, utiliser `-p` |
| Capacité pleine             | File saturée         | Attendre ou augmenter `-c`       |
