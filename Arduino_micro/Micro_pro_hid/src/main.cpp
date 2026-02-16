#include <Arduino.h>
#include <FingerprintUSBHost.h>
#include <Keyboard.h>

// Variable to hold the detected OS
String osGuess = "Unknown";

void setup() {
  // 1. Initialize Serial for debugging (Optional, good for testing)
  Serial.begin(9600);

  // 2. fingerprinting happens AUTOMATICALLY during the USB handshake.
  // We just need to ask the library what it found.
  
  // Give the host a moment to settle the USB connection
  delay(2000); 

  // 3. Get the guess
  FingerprintUSBHost.guessHostOS(osGuess);

  // 4. Debug output (Open Serial Monitor to see this)
  Serial.print("Detected OS: ");
  Serial.println(osGuess);

  // 5. Initialize Keyboard
  Keyboard.begin();
  delay(100);

  // 6. EXECUTE PAYLOAD BASED ON OS
  if (osGuess == "Windows") {
    // Windows Payload: Open Notepad and type a message
    Serial.println("Running Windows Payload...");
    Keyboard.press(KEY_LEFT_GUI); // Win Key
    Keyboard.press('r');
    delay(100);
    Keyboard.releaseAll();
    delay(500);
    Keyboard.print("notepad");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.print("Hello Windows User! I see you.");
    
  } else if (osGuess == "MacOS") {
    // Mac Payload: Open Spotlight
    Serial.println("Running Mac Payload...");
    Keyboard.press(KEY_LEFT_GUI); // Command Key
    Keyboard.press(' ');
    delay(100);
    Keyboard.releaseAll();
    delay(500);
    Keyboard.print("TextEdit");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    
  } else if (osGuess == "Linux") {
    // Linux Payload
    Serial.println("Running Linux Payload...");
    // Linux is tricky because shortcuts vary (Gnome vs KDE), 
    // but usually Ctrl+Alt+T opens terminal
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('t');
    delay(100);
    Keyboard.releaseAll();
  } else {
    // Fallback if detection fails
    Serial.println("OS Unknown. Staying silent.");
  }
  
  Keyboard.end();
}

void loop() {
  // Put nothing here. We only want the payload to run ONCE when plugged in.
}