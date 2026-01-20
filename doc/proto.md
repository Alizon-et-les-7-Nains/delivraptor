# **PROTOCOLE DÉLIVRAPTOR - Documentation Technique**

## Vue d'ensemble

Le protocole Délivraptor est un protocole basé sur TCP permettant à un client de se connecter au serveur de simulation de livraison. Toutes les commandes sont envoyées un texte brut suivies d'un saut de ligne (`\n`).

## Connexion

- **Port par défaut** : 8080
- **Protocole** : TCP
- **Encodage** : UTF-8

## Authentification requise

Toutes les commandes (sauf `AUTH` et `HELP`) nécessitent une authentification préalable.

## Grammaire des commandes

### AUTH - Authentification

AUTH <username> <password_md5>

**Paramètres** :

- `username` : Identifiant utilisateur
- `password_md5` : Mot de passe hashé en MD5 (32 caractères hexadécimaux)

**Réponses possibles** :

- `AUTH_SUCCESS` : Authentification réussie
- `ERROR AUTH_FAILED` : Identifiants incorrects
- `ERROR INVALID_MD5_FORMAT` : Format MD5 invalide
- `ERROR MISSING_CREDENTIALS` : Paramètres manquants
- `ERROR ALREADY_AUTHENTICATED` : Déjà authentifié

**Exemple** :
AUTH alizon e10adc3949ba59abbe56e057f20f883e

### CREATE - Création d'un bordereau

CREATE <commande_id> <destination>

**Paramètres** :

- `commande_id` : Numéro de commande Alizon (entier positif)
- `destination` : Adresse de livraison (chaîne, peut contenir des espaces)

**Réponses possibles** :

- `<bordereau>` (10 chiffres) : Bordereau créé avec succès
- `<bordereau_existant>` : Bordereau déjà existant pour cette commande
- `<commande_id>` : Commande mise en file d'attente (capacité pleine)
- `ERROR MISSING_PARAMETERS` : Paramètres manquants
- `ERROR INVALID_COMMANDE_ID` : ID de commande invalide
- `ERROR DB_QUERY` : Erreur base de données

**Exemple** :
CREATE 1234567890 "10 Rue de la Paix, 75001 Paris"

**Exemples CREATE (gestion capacité) :**

Cas capacité pleine :

```
CREATE 1000000004 "30 Rue de Rivoli, 75004 Paris"
1029384756                    ← Bordereau toujours retourné, pas commande_id
```

Cas capacité disponible :

```
CREATE 1000000005 "5 Avenue des Champs-Élysées"
1928374650                    ← Bordereau créé et ajouté à la file
```

### STATUS - Consultation d'un colis

STATUS <bordereau>

**Paramètres** :

- `bordereau` : Numéro de bordereau (10 chiffres)

**Format de réponse** :
<bordereau>|<commande_id>|<destination>|<localisation>|<etape>|<date_etape>|<type_livraison>|<binaire_img>

**Étapes possibles** :

1. Entrepôt Alizon
2. En transit vers plateforme transporteur
3. Arrivé chez transporteur
4. Départ vers plateforme régionale
5. Arrivé plateforme régionale
6. Départ vers centre local
7. Arrivé centre local
8. Départ pour livraison finale
9. Chez destinataire

**Types de livraison (étape 9 uniquement)** :

- `MAINS_PROPRES` : Livré en mains propres
- `ABSENT` : Livré en l'absence (avec image)
- `REFUSE` : Colis refusé (avec raison)

**Transmission d'images pour livraisons ABSENT :**

Pour les livraisons de type `ABSENT` à l'étape 9, le serveur envoie :

1. Les données texte au format défini ci-dessus (`<bordereau>|...|<binaire_img>`), suivies d'un `\n`
2. L'image binaire (si disponible) envoyée immédiatement après (flux binaire)
3. La chaîne `null` suivie d'un retour à la ligne (`null\n`) pour marquer la fin

**Séquence complète :**

```
[données texte]\n
[données binaires de l'image]
null\n
```

**Cas sans image :**

```
[données texte]\n
null\n
```

**Gestion côté client :**
Le client doit lire la ligne texte jusqu'au premier `\n`, analyser le champ `type_livraison` ;
si `ABSENT`, il doit ensuite lire les octets suivants jusqu'à rencontrer la séquence `null\n`.

**Exemple** :
STATUS 6864770470
6864770470|333333333|10 Rue Exemple|Chez destinataire|9|2026-01-19 09:13:45|ABSENT|null

### Détails des étapes et localisations :

| Étape | Localisation                            | Description                         |
| ----- | --------------------------------------- | ----------------------------------- |
| 1     | Entrepôt Alizon                         | Création du bordereau               |
| 2     | En transit vers plateforme transporteur | Prise en charge par le transporteur |
| 3     | Arrivé chez transporteur                | Sur la plateforme principale        |
| 4     | Départ vers plateforme régionale        | Envoi vers la région de destination |
| 5     | Arrivé plateforme régionale             | Plateforme d'aiguillage régionale   |
| 6     | Départ vers centre local                | Redistribution locale               |
| 7     | Arrivé centre local                     | Centre de livraison final           |
| 8     | Départ pour livraison finale            | Chez le livreur                     |
| 9     | Chez destinataire                       | Livraison effectuée                 |

**Notes :**

- Les étapes 1-4 : Colis dans la file de prise en charge
- Les étapes 5-8 : Colis hors file mais en transit
- Étape 9 : Livraison finale, colis archivé

### QUEUE_STATUS - État de la file d'attente

QUEUE_STATUS

**Format de réponse** :
QUEUE_STATUS En attente: <nb_attente>, Capacité actuelle: <actuel>/<max>, Places libres: <places_libres>

**Exemple** :
QUEUE_STATUS En attente: 2, Capacité actuelle: 3/3, Places libres: 0

### HELP - Aide

HELP

Affiche la liste des commandes disponibles.

### QUIT/EXIT - Déconnexion

QUIT

ou
EXIT

## Gestion de la capacité

Le serveur a une capacité limitée de prise en charge. Quand la capacité est atteinte :

1. Les nouvelles commandes `CREATE` sont mises en file d'attente
2. Quand un colis passe de l'étape 4 à 5, une place se libère
3. La file d'attente est traitée automatiquement

## Architecture système à deux niveaux

### File de prise en charge (`_delivraptor_file_prise_en_charge`)

- Contient uniquement les colis aux **étapes 1-4**
- Capacité limitée par l'option `-c`
- Libération automatique au passage étape 4→5

### File d'attente (`_delivraptor_queue`)

- Stocke les commandes quand capacité pleine
- Traitement automatique FIFO quand place se libère
- Chaque ligne contient : noCommande, destination, username, bordereau

### Comportement CREATE avec capacité pleine :

1. Génération du bordereau unique
2. Insertion dans `_delivraptor_colis` (table principale)
3. Insertion dans `_delivraptor_queue` (file d'attente)
4. Retour du numéro de bordereau (pas de commande_id)

## Fichier d'authentification

Format du fichier `auth.txt` :
username:md5_hash

Exemple :
alizon:e10adc3949ba59abbe56e057f20f883e

## Codes d'erreur

- `ERROR NOT_AUTHENTICATED` : Commande sans authentification
- `ERROR UNKNOWN_COMMAND` : Commande inconnue
- `ERROR DB_*` : Erreurs base de données
- `ERROR MISSING_*` : Paramètres manquants
- `ERROR INVALID_*` : Format invalide
- `ERROR BORDEREAU_GENERATION_FAILED` : Impossible de générer un bordereau unique

**Codes d'erreur supplémentaires :**

- `ERROR DB_RESULT` : Erreur lors de la récupération des résultats BDD
- `ERROR DB_INSERT` : Erreur d'insertion dans la base
- `ERROR DB_QUEUE_INSERT` : Erreur d'insertion dans la file d'attente
- `ERROR BORDEREAU_GENERATION_FAILED` : Impossible de générer un bordereau unique
- `ERROR DB_QUERY_CAPACITY` : Erreur de requête de capacité

## Logs

Toutes les actions sont logguées avec :

- Date et heure
- Adresse IP et port client
- Nom d'utilisateur (si authentifié)
- Action effectuée
- Message détaillé
