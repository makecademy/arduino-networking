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
  
  // Start sensor
  dht.begin();

  // Get IP address
  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while(true);
  }
  Serial.println("OK");
  delay(5000);
  
  Serial.println("Setup complete.\n");
}

void loop() {
  
  // Measure from DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  int temperature = (int)t;
  int humidity = (int)h;
  
  Serial.println("Temperature: " + String(temperature));
  Serial.println("Humidity: " + String(humidity));
  

  // Trigger if temperature exceeds limit
  if (temperature > 23) {
    Serial.println("\nTriggered! Calling /Library/Google/Gmail/SendEmail...");
    
    // Send email
    runSendEmail(temperature);
  }
  
  // Wait 
  delay(60000);
}

// Send email
void runSendEmail(int sensorValue) {
  TembooChoreo SendEmailChoreo(client);

  // Set Temboo account credentials
  SendEmailChoreo.setAccountName(TEMBOO_ACCOUNT);
  SendEmailChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  SendEmailChoreo.setAppKey(TEMBOO_APP_KEY);

  // Set saved inputs to use for execution
  SendEmailChoreo.setSavedInputs("arduinoTemperatureEmail");

  // Set Choreo inputs
  String MessageBodyValue = String("Alert! The current sensor value is: ") + String(sensorValue);
  SendEmailChoreo.addInput("MessageBody", MessageBodyValue);

  // Identify the Choreo to run
  SendEmailChoreo.setChoreo("/Library/Google/Gmail/SendEmail");

  // Run the Choreo
  unsigned int returnCode = SendEmailChoreo.run();

  // A return code of zero means everything worked
  if (returnCode == 0) {
    Serial.println("Done!\n");
  } else {
    // A non-zero return code means there was an error
    // Read and print the error message
    while (SendEmailChoreo.available()) {
      char c = SendEmailChoreo.read();
      Serial.print(c);
    }
    Serial.println();
  }
  
  SendEmailChoreo.close();
}
