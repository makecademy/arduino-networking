// Server with the Arduino Ethernet Shield

// Include libraries
#include "DHT.h"
#include <Ethernet.h>
#include <SPI.h>

// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// Set the static IP address for your board
IPAddress ip(192,168,1,50);

// Initialize the Ethernet client
EthernetServer server(80);

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
 // Open serial communications
  Serial.begin(9600);

  // Start the Ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  
  // Start server
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  
  // Initialize DHT sensor
  dht.begin();
}


void loop() {
  
  // Measure the humidity & temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Transform to String
  String temp = String((int) t);
  String hum = String((int) h);
  
  // Listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    
    Serial.println("New client");
    
    // An HTTP request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      
      // Read data
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        
        // Send a reply if end of line detected
        if (c == '\n' && currentLineIsBlank) {
          
          // Send a standard HTTP response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");
          client.println();
          client.print("{\"temperature\": ");
          client.print(temp);
          client.print(", \"humidity\": ");
          client.print(hum);
          client.println("}");        
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // Give the web browser time to receive the data
    delay(1);
    // Close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}

