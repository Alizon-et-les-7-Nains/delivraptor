<?php

function sendTimLeBg(string $host, int $port, int $type, array $payload): array
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

    fwrite($sock, $packet);

 
    $header = fread($sock, 8);
    if (strlen($header) < 8) {
        throw new Exception("Réponse incomplète");
    }

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
$response = sendTimLeBg(
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