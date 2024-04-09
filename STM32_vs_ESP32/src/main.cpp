#include <Arduino.h>
#include <SoftwareSerial.h>

#define LED_PIN PC13 // Built-in LED pin on STM32F103C8T6
SoftwareSerial espSerial(PA10, PA9); // RX, TX

void setup() {
  Serial.begin(9600); // Serial monitor
  espSerial.begin(9600); // Communication with ESP32

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure LED is initially off

  Serial.println("STM32 Ready");
}

void loop() {
  if (espSerial.available()) {
    char received = espSerial.read();
    if (received == '1') {
      Serial.println("Received '1' from ESP32, turning on LED");
      digitalWrite(LED_PIN, HIGH);
      espSerial.println("1");
    } else if (received == '0') {
      Serial.println("Received '0' from ESP32, turning off LED");
      digitalWrite(LED_PIN, LOW);
      espSerial.println("0");
    }
  }
}
