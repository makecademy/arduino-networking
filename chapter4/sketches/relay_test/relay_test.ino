// Sketch to test the relay
 
// Relay pin
const int relay_pin = 7;

void setup() {
  Serial.begin(9600);
  pinMode(relay_pin,OUTPUT);
}

void loop() {
  
  // Activate relay
  digitalWrite(relay_pin, HIGH);
  
  // Wait for 1 second
  delay(1000);
  
   // Deactivate relay
  digitalWrite(relay_pin, LOW);
  
  // Wait for 1 second
  delay(1000);
}
