<?php
// auto_test.php
function send_delivraptor_command($host, $port, $command, $timeout = 5) {
    $socket = @fsockopen($host, $port, $errno, $errstr, $timeout);
    
    if (!$socket) {
        echo "ERREUR: Impossible de se connecter à $host:$port\n";
        echo "Code: $errno - Message: $errstr\n";
        return false;
    }
    
    // Envoi de la commande
    fwrite($socket, $command . "\n");
    
    // Lecture de la réponse
    $response = '';
    while (!feof($socket)) {
        $response .= fgets($socket, 1024);
        if (strpos($response, "\n") !== false) {
            break;
        }
    }
    
    fclose($socket);
    return trim($response);
}

// Utilisation
$host = 'localhost';
$port = 8080;

// Test 1: Authentification
echo "Test AUTH:\n";
$auth_response = send_delivraptor_command($host, $port, "AUTH admin e10adc3949ba59abbe56e057f20f883e");
echo "Réponse: $auth_response\n\n";

// Test 2: Création
echo "Test CREATE:\n";
$create_response = send_delivraptor_command($host, $port, "CREATE 123456789");
echo "Réponse: $create_response\n\n";

// Extraire le numéro de bordereau
if (preg_match('/BORDEREAU (\d+)/', $create_response, $matches)) {
    $bordereau = $matches[1];
    
    // Test 3: Consultation
    echo "Test STATUS:\n";
    $status_response = send_delivraptor_command($host, $port, "STATUS $bordereau");
    echo "Réponse: $status_response\n\n";
}

// Test 4: HELP
echo "Test HELP:\n";
$help_response = send_delivraptor_command($host, $port, "HELP");
echo "Premières lignes:\n" . substr($help_response, 0, 200) . "...\n";
?>