<?php 
    $processus = [
        ['name' => 'P1', 'work' => 4],
        ['name' => 'P2', 'work' => 6],
        ['name' => 'P3', 'work' => 3],
    ];

    $quantum = 4000;

    while(count($processus) > 0) {
        foreach($processus as $index => &$process) {
            if ($process['work'] > 0) {
                echo "Execution de {$process['name']} pour $quantum ms \n";
                
                $workTime = min($process['work'], 1);
                usleep($workTime * $quantum * 1000);

                $process['work'] -= $workTime;

                if ($process['work'] <= 0) {
                    echo "{$process['name']} termine\n";
                    unset($processus[$index]);
                }
            }
        }
    }
    
?>