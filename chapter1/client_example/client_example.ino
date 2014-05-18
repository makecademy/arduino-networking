// Example web client using the Ethernet shield

// Include libraries
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// Define server to connect to for the test
char server[] = "www.brainjar.com";

// Set the static IP address for your board
IPAddress ip(192,168,1,50);

// Initialize the Ethernet client
EthernetClient client;

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
  // Connect to Google servers
  if (client.connect(server, 80)) {
    if (client.connected()) {
      Serial.println("connected");
    
      // Make a HTTP request:
      client.println("GET /java/host/test.html HTTP/1.1");
      client.println("Host: www.brainjar.com");
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
  
  // Repeat every 5 seconds
  delay(5000);
}

