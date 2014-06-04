// Include libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>

// MAC address
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// IP address if DHCP fails
IPAddress ip(192,168,1,150);

// Create aREST instance
aREST rest = aREST();

// Initialize the Ethernet server library
EthernetServer server(80);

void setup() {
  
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  // Give name and ID to device
  rest.set_id("008");
  rest.set_name("mighty_dragon");

  // Start the Ethernet connection
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  
  // Start server
  server.begin();
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  
  // listen for incoming clients
  EthernetClient client = server.available();
  rest.handle(client);
  
}
