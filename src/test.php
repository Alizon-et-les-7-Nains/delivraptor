<?php

$socket = null;
$host = 'localhost';
$port = 6000;

function connect() {
    global $socket, $host, $port;
    $socket = fsockopen($host, $port, $errno, $errstr, 30);
    if (!$socket) {
        throw new Exception("Connexion impossible: $errstr ($errno)");
    }
    stream_set_timeout($socket, 30); // Timeout 30s
}

function sendCommand($command) {
    fwrite($GLOBALS['socket'], $command . "\n");
    return readResponse();
}

function readResponse() {
    $response = '';
    while (!feof($GLOBALS['socket'])) {
        $line = fgets($GLOBALS['socket'], 1024);
        $response .= $line;
        
        if (strpos($response, "---IMAGE---\n") !== false) {
            $binaryData = '';
            while (!feof($GLOBALS['socket'])) {
                $binaryData .= fread($GLOBALS['socket'], 1024);
            }
            return [
                'text' => $response,
                'image_binary' => $binaryData
            ];
        }
        
        if (strpos($line, "\n") !== false && !str_starts_with($response, '╔')) {
            break;
        }
    }
    return trim($response);
}

function disconnect() {
    fclose($GLOBALS['socket']);
}

// Exemple d'utilisation
connect();

$authResponse = sendCommand("AUTH alizon 5f4dcc3b5aa765d61d8327deb882cf99");
echo "Auth: $authResponse\n";

$createResponse = sendCommand("CREATE 10001");
echo "Create: $createResponse\n";

$bordereau = $createResponse;
$statusResponse = sendCommand("STATUS $bordereau");
echo "Status: $statusResponse\n";

disconnect();
?>