<?php

error_reporting(E_ALL); 
ini_set("display_errors", 1);

// Arduino board
$url = 'http://192.168.1.103';

// Get cURL resource
$curl = curl_init();

// Set some options - we are passing in a useragent too here
curl_setopt_array($curl, array(
    CURLOPT_RETURNTRANSFER => 1,
    CURLOPT_URL => $url,
));

// Send the request & save response to $resp
$resp = curl_exec($curl);

// Close request to clear up some resources
curl_close($curl);

// Get data
$json = json_decode($resp, true);
$temperature = intval($json["temperature"]);
$humidity = intval($json["humidity"]);

// Create DB instance
$db = new SQLite3('database.db');

// Create new table if needed
$db->exec('CREATE TABLE IF NOT EXISTS measurements (id INTEGER PRIMARY KEY, timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL, temperature INTEGER, humidity INTEGER);');

// Store data in DB
$db->exec("INSERT INTO measurements (temperature, humidity) VALUES ('$temperature', '$humidity');");
    	
// Answer
echo "Data received";

?>