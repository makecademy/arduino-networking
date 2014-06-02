// Upload weather data to Xively

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

// Initialize the Ethernet client
EthernetClient client;

// Set the static IP address for your board in case DHCP fails
IPAddress ip(192,168,1,50);

// IP address of Xively
IPAddress server(216,52,233,120);

// Xively parameters
#define WEBSITE  "api.xively.com"
#define API_key  "yourAPIKey"
#define feedID  "yourFeedID"

void setup(void)
{
  
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

void loop(void)
{
  // Measure the humidity & temperature
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
        
  // Prepare JSON for Xively & get length
  int length = 0;
  String data = "";
  data = data + "\n" + "{\"version\":\"1.0.0\",\"datastreams\" : [ {\"id\" : \"Temperature\",\"current_value\" : \"" + String((int)temperature) + "\"}," + "{\"id\" : \"Humidity\",\"current_value\" : \"" + String((int)humidity) + "\"}]}";
  Serial.println(data);
  length = data.length();
  
  // Send request
  if (client.connect(server, 80)) {
    if (client.connected()) {
      Serial.println("connected");
    client.println("PUT /v2/feeds/" + String(feedID) + ".json HTTP/1.1");
    client.println("Host: api.xively.com");
    client.println("X-ApiKey: " + String(API_key));
    client.println("Content-Length: " + String(length));
    client.print("Connection: close");
    client.println();
    client.print(data);
    client.println();
  } else {
    Serial.println(F("Connection failed"));    
    return;
  }
  
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
  
  // Wait 10 seconds until next update
  delay(10000);
  
}
