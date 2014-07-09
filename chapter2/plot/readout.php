<?php 

// Show errors
error_reporting(E_ALL); 
ini_set("display_errors", 1); 

// Open database
$db = new SQLite3('database.db');

// Set default timezone
date_default_timezone_set('America/Los_Angeles');

// Get data
$results = $db->query('SELECT id, timestamp, temperature, humidity FROM measurements');

// Parse data
while($row = $results->fetchArray())
{
    $dataset[] = array(strtotime($row['timestamp']) * 1000,$row['temperature']);
}

// Return data
echo json_encode($dataset);

?>