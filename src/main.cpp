#include <Arduino.h>
#include <RCSwitch.h>

#define TRANSMITTER_PIN 27
#define UP_TRI_MESSAGE "1FFFFFFF0010"
#define DOWN_TRI_MESSAGE "1FFFFFFF0020"

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #27
  mySwitch.enableTransmit(TRANSMITTER_PIN);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1);

  // Optional set pulse length.
  mySwitch.setPulseLength(500);
}

void sendTriState(char* Code) {
  mySwitch.sendTriState(Code);
  Serial.print("Sent ");
  Serial.println(Code);
}

void loop() {
  sendTriState(UP_TRI_MESSAGE);
  delay(1000);  
  sendTriState(DOWN_TRI_MESSAGE);
  delay(1000);
}