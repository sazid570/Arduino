#include <Arduino.h>

#include <Keyboard.h>

void openCmd() {
  // Press Windows key + R to open Run dialog
  Keyboard.press(KEY_LEFT_GUI);  // Windows key
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  
  // Wait for Run dialog to open
  delay(500);
  
  // Type "cmd"
  Keyboard.print("cmd");
  
  // Press Enter to execute
  delay(200);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);

  delay(500);

  Keyboard.print("start https://tinyurl.com/rn8vb7dm");
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
}

void setup() {

  Keyboard.begin();
  delay(1000);
  openCmd();
}

void loop() {
  // Your main code here
}

