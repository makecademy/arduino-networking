<?php 

error_reporting(E_ALL); 
ini_set("display_errors", 1); 

$db = new SQLite3('database.db');

$results = $db->query('SELECT id, timestamp, temperature, humidity FROM measurements');

while($row = $results->fetchArray())
{
    $dataset[] = array(strtotime($row['timestamp']) * 1000,$row['temperature']);
}

echo json_encode($dataset);

?>