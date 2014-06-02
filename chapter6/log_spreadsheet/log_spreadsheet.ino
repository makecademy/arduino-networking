// Libraries
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>
#include "TembooAccount.h"
#include "DHT.h"

// DHT sensor
#define DHTPIN 7 
#define DHTTYPE DHT11

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

// Define MAC address
byte ethernetMACAddress[] = ETHERNET_SHIELD_MAC;

// Client instance
EthernetClient client;

void setup() {

  // Start Serial
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected.
  delay(4000);
  while(!Serial);

  // Get IP address
  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(5000);
 
  // Start DHT sensor
  dht.begin();

}

void loop() {
    
  // Measure from DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  int temperature = (int)t;
  int humidity = (int)h;
    
  Serial.println("Temperature: " + String(temperature));
  Serial.println("Humidity: " + String(humidity));
  
  // Send data to Temboo
  runAppendRow(temperature, humidity);
  
  // Repeat
  delay(60000);
}

// Send data to Temboo
void runAppendRow(int temperature, int humidity) {
  TembooChoreo AppendRowChoreo(client);

  // Set Temboo account credentials
  AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
  AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set saved inputs to use for execution
  AppendRowChoreo.setSavedInputs("arduinoTemperatureSpreadsheet");

  // Set Choreo inputs
  String RowDataValue = String(millis()) + "," + String(temperature) + "," + String(humidity);
  AppendRowChoreo.addInput("RowData", RowDataValue);

  // Identify the Choreo to run
  AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");

  // Run the Choreo
  unsigned int returnCode = AppendRowChoreo.run();

  // A return code of zero means everything worked
  if (returnCode == 0) {
    Serial.println("Done!\n");
  } else {
    // A non-zero return code means there was an error
    // Read and print the error message
    while (AppendRowChoreo.available()) {
      char c = AppendRowChoreo.read();
      Serial.print(c);
    }
    Serial.println();
  }
  
  AppendRowChoreo.close();
}
