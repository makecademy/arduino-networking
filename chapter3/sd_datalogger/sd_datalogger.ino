// Datalogger with the Arduino Ethernet Shield

// Include libraries
#include "DHT.h"
#include <SD.h>
#include <Time.h> 
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>

// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// Chip select pin
const int chipSelect = 4;

// NTP Server
IPAddress timeServer(132, 163, 4, 101);
const int timeZone = 1;

// Create UDP server
EthernetUDP Udp;
unsigned int localPort = 8888;

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
 // Open serial communications
  Serial.begin(9600);
  
  // Start Ethernet
  if (Ethernet.begin(mac) == 0) {
    // no point in carrying on, so do nothing forevermore:
    while (1) {
      Serial.println("Failed to configure Ethernet using DHCP");
      delay(10000);
    }
  }
  
  // Init SD card
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // Initialize DHT sensor
  dht.begin();
  
  // Print server info
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(Ethernet.localIP());
  Udp.begin(localPort);
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
}


void loop() {
  
  // Measure the humidity & temperature
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Transform to String
  String temp = String((int) t);
  String hum = String((int) h);
  
  // Format time
  String log_time = String(day()) + "/" + 
  String(month()) + "/" + String(year()) + " " + 
  String(hour()) + ":" + String(minute()) + ":" + 
  String(second());
  
  // Make a string for assembling the data to log
  String dataString = log_time + "," + temp + "," + hum;
  
  // Open file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  // Write data to file
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println(dataString);
  }  
  else {
    Serial.println("error opening datalog.txt");
  }
  
  // Repeat every 10 seconds
  delay(10000);
}

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

