#include <Arduino.h>

#include <Keyboard.h>

void setup() {
  delay(2000);  // Short delay to ensure host detects the device
  Keyboard.begin();
}

void loop() {
  Keyboard.println("Hello, world!");
  delay(5000);  // Wait 5 seconds
}