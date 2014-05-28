<?php

  // Get data
  $key = $_GET['key'];
  $command = $_GET['command'];

  // Create cURL call
  $service_url = 'http://us01.proxy.teleduino.org/api/1.0/328.php?k=' . 
  $key . '&r=' . $command;
  $curl = curl_init($service_url);
     
  // Send cURL to board
  curl_setopt($curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 ); 
  //curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($curl, CURLOPT_CONNECTTIMEOUT , 0.5);
  $curl_response = curl_exec($curl);
  curl_close($curl);

?>
