# **PROTOCOLE DÉLIVRAPTOR - Documentation Technique**

## Vue d'ensemble

Le protocole Délivraptor est un protocolee basé sur TCP permettant à un client de se connecter au serveur de simulation de livraison. Toutes les commandes sont envoyées ene brut suivies d'un saut de ligne (`\n`).

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

### STATUS - Consultation d'un colis

STATUS <bordereau>

**Paramètres** :

- `bordereau` : Numéro de bordereau (10 chiffres)

**Format de réponse** :
<bordereau>|<commande_id>|<destination>|<localisation>|<etape>|<date_etape>|<type_livraison>|<chemin_image>

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

**Réponses d'erreur** :

- `ERROR BORDEREAU_NOT_FOUND` : Bordereau non trouvé
- `ERROR DB_QUERY` : Erreur base de données

**Exemple** :
STATUS 6864770470
6864770470|333333333|10 Rue Exemple|Chez destinataire|9|2026-01-19 09:13:45|ABSENT|/var/www/images/boite_lettres.jpg

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

## Fichier d'authentification

Format du fichier `auth.txt` :
username:md5_hash

Exemple :
alizon:e10adc3949ba59abbe56e057f20f883e
admin:21232f297a57a5a743894a0e4a801fc3

## Codes d'erreur

- `ERROR NOT_AUTHENTICATED` : Commande sans authentification
- `ERROR UNKNOWN_COMMAND` : Commande inconnue
- `ERROR DB_*` : Erreurs base de données
- `ERROR MISSING_*` : Paramètres manquants
- `ERROR INVALID_*` : Format invalide

## Logs

Toutes les actions sont logguées avec :

- Date et heure
- Adresse IP et port client
- Nom d'utilisateur (si authentifié)
- Action effectuée
- Message détaillé
