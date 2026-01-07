<?php 
// Code coté Alizon pas de connection a la BD Delivraptor
$port = 8080;
$socket = fsockopen("localhost", 8080, $errno, $errstr, 30);


if(!$socket) {
    echo "Erreur $errno : $errstr\n";
} else {
    // Demander un numero de bordereau (id colis) au service
    

    // le service Demande du numero de commande 

    
}
?>