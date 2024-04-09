#include <Arduino.h>
#define LED_PIN 2
#define BAUD_RATE 9600

bool ledState = LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
}

void loop() {
  while (Serial.available() > 0) {
    char receivedChar = Serial.read();
    
    if (receivedChar == '1') {
      ledState = HIGH;
      digitalWrite(LED_PIN, ledState);
    }
    
    else if (receivedChar == '0') {
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
    }
  }
  
  Serial.print("LED State: ");
  Serial.println(ledState);
  
  delay(1000);
}
