<?php

require_once 'pdo.php';
session_start();

//Cette fonction lit exactement $len octets depuis un socket, même si
//les données arrivent en plusieurs morceaux.function 
function recv_all($sock, int $len){
    $data = '';
    //Si par exemple on veut 8 octets donc $len = 8
    //Alors on va regarder la taille de data et tant que c'est inférieur à 8 octets 
    //Alors on va continuer à lire pour bien avoir toutes les informations
    while (strlen($data) < $len) {
        //$len - strlen($data) = octets restants à lire
        $dataPart = fread($sock, $len - strlen($data));
        if ($dataPart === false || $dataPart === '') {
            throw new Exception("Connexion fermée prématurément");
        }
        //on ajoute chaque nouvelle partie de donnée à chaque itération
        $data .= $dataPart;
    }
    return $data;
}

function sendPrograptor(string $host, int $port, int $type, array $payload): array
{
    $version = 1;
    $json = json_encode($payload, JSON_UNESCAPED_UNICODE);
    if ($json === false) {
        throw new Exception("JSON invalide");
    }

    $packet =
        pack("n", $version) .
        pack("n", $type) .
        pack("N", strlen($json)) .
        $json;

        $sock = fsockopen($host, $port, $errno, $errstr, 5);
    if (!$sock) {
        throw new Exception("Connexion échouée");
    }

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        $idCommande = $_POST['idCommande'];
        $stmt = $pdo->prepare('SELECT idCommande, CONCAT(adresse, ", " ,codePostal, " ", ville) as destination FROM _commande INNER JOIN _adresseLivraison ON idAdresseLivr = idAdresseLivraison WHERE idCommande = :idCommande');
        $stmt->execute([
            ':idCommande' => $idCommande
        ]);
    }    

    fwrite($sock, $packet);

    $header = recv_all($sock, 8);

    [$v, $t, $len] = array_values(unpack("nversion/ntype/Nlength", $header));
    $body = fread($sock, $len);

    fclose($sock);

    return [
        "version" => $v,
        "type" => $t,
        "data" => json_decode($body, true)
    ];
}

// TEST
$response = sendPrograptor(
    "127.0.0.1",
    9000,
    0x0001,
    [
        "client" => "web",
        "msg" => "hello serveur prograptor"
    ]
);

var_dump($response);        
?>