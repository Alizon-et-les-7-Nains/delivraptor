# **DOCUMENTATION CAS D'UTILISATION **

## 1. Installation et Démarrage Rapide

### 1.1 Démarrer le serveur

```bash
# Compilation
gcc main.c $(mysql_config --cflags --libs) -o delivraptor_server

# Démarrage
./delivraptor_server -p 8080 -c 3 -a auth.txt -l delivraptor.log
```

### 1.2 Vérifier que le serveur fonctionne

```bash
# Test de connexion simple
telnet localhost 8080
# Tapez HELP puis QUIT
```

## 2. Scénarios d'Utilisation

### 2.1 Scénario 1 : Première connexion

**Objectif** : Se connecter et créer un premier bordereau

```bash
# Étape 1: Authentification
$ telnet localhost 8080
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
AUTH alizon e10adc3949ba59abbe56e057f20f883e
AUTH_SUCCESS

# Étape 2: Création d'un bordereau
CREATE 123456789
BORDEREAU 3847562019 # Numero de bordereau donné à titre d'example 

# Étape 3: Vérification
STATUS 3847562019
ETAPE 1|Entrepôt Alizon|2026-01-09 15:30:00||

# Étape 4: Déconnexion
QUIT
BYE
Connection closed by foreign host.
```

### 2.2 Scénario 2 : Gestion de la capacité

**Objectif** : Voir ce qui se passe quand la file est pleine

```bash
# Avec capacité = 3, créer 4 colis
CREATE 123456
BORDEREAU 1111111111

CREATE 098765
BORDEREAU 2222222222

CREATE 385173
BORDEREAU 3333333333

CREATE 9735241  # Doit échouer !
ERROR CAPACITE
```

### 2.3 Scénario 3 : Simulation complète

**Objectif** : Suivre un colis de la création à la livraison

```bash
# 1. Création
CREATE SIMU_001
BORDEREAU 9998887776

# 2. Consultation initiale
STATUS 9998887776
ETAPE 1|Entrepôt Alizon|2026-01-09 15:35:00||

# 3. Lancer le simulateur (dans un autre terminal)
php simulateur.php

# 4. Re-vérifier le statut
STATUS 9998887776
ETAPE 2|En transit vers plateforme transporteur|2026-01-09 15:36:00||

# 5. Lancer plusieurs fois le simulateur
# (dans l'autre terminal, exécuter plusieurs fois)
php simulateur.php
php simulateur.php
php simulateur.php

# 6. Vérifier la progression
STATUS 9998887776
# Après plusieurs simulations, vous verrez :
# Étape 3, 4, 5... jusqu'à 9
```

## 4. Intégration avec Alizon (exemples PHP)

### 4.1 Dans la page de paiement

```php
// Lorsqu'un client valide son paiement
try {
    $client = new DelivraptorClient('localhost', 8080);
    $client->connect();
    $client->authenticate('alizon', 'alizon2024');

    $bordereau = $client->createBordereau($_SESSION['commande_id']);

    // Sauvegarder dans la base Alizon
    $db->query("UPDATE commandes SET bordereau = '$bordereau' WHERE id = '{$_SESSION['commande_id']}'");

    echo "Votre numéro de suivi : $bordereau";

} catch (Exception $e) {
    echo "Erreur : " . $e->getMessage();
    // Gérer l'erreur (retour au panier, message utilisateur...)
}
```

### 4.2 Dans la page de suivi

```php
// Quand un client consulte sa commande
$bordereau = $_GET['tracking'];

try {
    $client = new DelivraptorClient('localhost', 8080);
    $client->connect();
    $client->authenticate('alizon', 'alizon2024');

    $status = $client->getStatus($bordereau);

    echo "<h3>Suivi de votre colis</h3>";
    echo "<p><strong>Numéro :</strong> $bordereau</p>";
    echo "<p><strong>Statut :</strong> " . htmlspecialchars($status['etape']) . "</p>";
    echo "<p><strong>Localisation :</strong> " . htmlspecialchars($status['localisation']) . "</p>";

    if ($status['image']) {
        echo "<h4>Photo de livraison :</h4>";
        echo "<img src='" . htmlspecialchars($status['image']) . "' style='max-width: 500px;'>";
    }

} catch (Exception $e) {
    echo "<p class='error'>Impossible de récupérer le suivi pour le moment.</p>";
}
```

## 5. Dépannage Pratique

### 5.1 "Connexion refusée"

```bash
# Vérifier que le serveur tourne
ps aux | grep delivraptor_server

# Vérifier le port
sudo netstat -tulpn | grep :8080

# Relancer le serveur
pkill delivraptor_server
./delivraptor_server -p 8080 -c 3 -a auth.txt -l delivraptor.log
```

### 5.2 "ERROR CAPACITE"

```bash
# Voir combien de colis sont dans la file
mysql -u pperche -pgrognasseEtCompagnie delivraptor -e "SELECT COUNT(*) FROM _delivraptor_file_prise_en_charge;"

# Libérer manuellement un colis (si besoin)
mysql -u pperche -pgrognasseEtCompagnie delivraptor -e "DELETE FROM _delivraptor_file_prise_en_charge"
```

### 5.4 Problèmes MySQL

```bash
# Vérifier la connexion
mysql -u pperche -pgrognasseEtCompagnie delivraptor -e "SELECT 1;"

# Réinitialiser la base (attention !)
mysql -u pperche -pgrognasseEtCompagnie delivraptor < schema.sql
```
