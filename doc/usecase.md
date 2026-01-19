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

### 1. Démarrer le serveur
```bash
# Avec paramètres par défaut
./delivraptor_server

# Avec configuration personnalisée
./delivraptor_server -p 9000 -c 3 -a auth.txt -l server.log
```

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
CREATE 1000000001 "10 Rue de la Paix, 75001 Paris"
```
**Réponse :**
```
1467226081
```
*(Numéro de bordereau à 10 chiffres)*

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
CREATE 1000000002 "25 Avenue Montaigne, 75008 Paris"
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
*Le même bordereau est retourné, évitant les doublons*

### Cas 3 : ID de commande invalide
**Commande :**
```
CREATE 0 "Adresse test"
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

## Gestion de la capacité

### Situation 1 : Capacité disponible
**Serveur démarré avec :** `-c 3` (capacité de 3 colis)

**Commande :**
```
QUEUE_STATUS
```
**Réponse :**
```
QUEUE_STATUS En attente: 0, Capacité actuelle: 1/3, Places libres: 2
```

### Situation 2 : Capacité pleine
Après création de 3 colis :
```
QUEUE_STATUS
```
**Réponse :**
```
QUEUE_STATUS En attente: 0, Capacité actuelle: 3/3, Places libres: 0
```

### Situation 3 : Nouveau colis avec capacité pleine
**Commande :**
```
CREATE 1000000004 "30 Rue de Rivoli, 75004 Paris"
```
**Réponse :**
```
1000000004
```
*Le numéro de commande est retourné (pas un bordereau), indiquant la mise en file d'attente*

**Vérification :**
```
QUEUE_STATUS
```
**Réponse :**
```
QUEUE_STATUS En attente: 1, Capacité actuelle: 3/3, Places libres: 0
```

### Situation 4 : Libération de capacité
Quand un colis passe de l'étape 4 à 5, une place se libère automatiquement et la file d'attente est traitée.

## Simulation de progression

### 1. Lancement manuel du simulateur
```bash
php simulateur.php
```
**Sortie :**
```
[Nettoyage] Colis aux étapes >= 5 retirés de la file de prise en charge
Bordereau 1467226081 : Étape 1 → 2 (En transit vers plateforme transporteur)
Bordereau 813692617 : Étape 2 → 3 (Arrivé chez transporteur)
=== Simulation terminée ===
2 colis avancés d'une étape.
```

### 2. Configuration automatique (cron)
Ajouter dans la crontab :
```bash
# Éditer la crontab
crontab -e

# Ajouter cette ligne pour exécuter toutes les minutes
* * * * * /usr/bin/php /chemin/complet/simulateur.php >> /var/log/delivraptor_sim.log 2>&1
```

### 3. Suivi de la progression
**Étape par étape :**

| Étape | Localisation | Commande STATUS |
|-------|-------------|-----------------|
| 1 | Entrepôt Alizon | `STATUS 1467226081` → `...|Entrepôt Alizon|1|...` |
| 2 | En transit vers plateforme transporteur | `...|En transit vers plateforme transporteur|2|...` |
| 3 | Arrivé chez transporteur | `...|Arrivé chez transporteur|3|...` |
| 4 | Départ vers plateforme régionale | `...|Départ vers plateforme régionale|4|...` |
| 5 | Arrivé plateforme régionale | `...|Arrivé plateforme régionale|5|...` |
| 6 | Départ vers centre local | `...|Départ vers centre local|6|...` |
| 7 | Arrivé centre local | `...|Arrivé centre local|7|...` |
| 8 | Départ pour livraison finale | `...|Départ pour livraison finale|8|...` |
| 9 | Chez destinataire | `...|Chez destinataire|9|...` |

## Types de livraison (étape 9)

### Type 1 : Livraison en mains propres
**Commande :**
```
STATUS 5158575785
```
**Réponse :**
```
5158575785|555555555|42 Rue Exemple|Chez destinataire|9|2026-01-19 14:25:10|MAINS_PROPRES|
```

### Type 2 : Livraison en absence
**Commande :**
```
STATUS 818949116
```
**Réponse :**
```
818949116|222222222|5 Rue de la République|Chez destinataire|9|2026-01-19 12:15:30|ABSENT|/var/www/images/boite_aux_lettres.jpg
```
*Le chemin de l'image est fourni pour preuve de livraison*

### Type 3 : Colis refusé
**Commande :**
```
STATUS 3643866199
```
**Réponse :**
```
3643866199|111111111|20 Avenue Foch|Chez destinataire|9|2026-01-19 12:30:15|REFUSE|Destinataire absent
```
*La raison du refus est spécifiée*