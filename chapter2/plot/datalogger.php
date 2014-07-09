<?php

error_reporting(E_ALL); 
ini_set("display_errors", 1);

// Check that data is present
if (isset($_GET["temp"]) && isset($_GET["hum"])) {

	// Get data
	$temperature = intval($_GET["temp"]);
	$humidity = intval($_GET["hum"]);

	// Create DB instance
	$db = new SQLite3('database.db');

	// Create new table if needed
	$db->exec('CREATE TABLE IF NOT EXISTS measurements (id INTEGER PRIMARY KEY, timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL, temperature INTEGER, humidity INTEGER);');

	// Store data in DB
	if($db->exec("INSERT INTO measurements (temperature, humidity) VALUES ('$temperature', '$humidity');")){
	  echo "Data received";	
	}
	else {
	  echo "Failed to log data";		
	}
}

?>