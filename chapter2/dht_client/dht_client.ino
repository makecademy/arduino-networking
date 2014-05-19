// Example web client using the Ethernet shield

// Include libraries
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11

// Set the static IP address for your board
IPAddress ip(192,168,1,50);

// IP address of your computer
IPAddress server(192,168,1,100);

// Initialize the Ethernet client
EthernetClient client;

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  // Open serial communications
  Serial.begin(115200);

  // Start the Ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  
  // Display IP
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
  
  // Give the Ethernet shield a second to initialize
  delay(1000);
  Serial.println("Connecting...");
  
}

void loop()
{
  
  // Measure the humidity & temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Transform to String
  String temp = String((int) t);
  String hum = String((int) h);

  // Print on Serial monitor
  Serial.println("Temperature: " + temp);
  Serial.println("Humidity: " + hum);
  
  // Connect to server
  if (client.connect(server, 80)) {
    if (client.connected()) {
      Serial.println("connected");
    
      // Make a HTTP request:
      client.println("GET /datalogger/datalogger.php?temp=" + temp + "&hum=" + hum + " HTTP/1.1");
      client.println("Host: 192.168.1.100");
      client.println("Connection: close");
      client.println();
    } 
    else {
      // If you didn't get a connection to the server
      Serial.println("connection failed");
    }
  
    // Read the answer
    while (client.connected()) {
      while (client.available()) {
      char c = client.read();
      Serial.print(c);
      }
    }

    // If the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
    }
    
  }
  
  // Repeat every second
  delay(1000);
}

