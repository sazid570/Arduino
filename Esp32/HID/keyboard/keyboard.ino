#include <PDM.h>

// Microphone pins
const int micPins[3] = {32, 33, 34}; 

// Sample rate and buffer size
const int sampleRate = 44100; // Adjust for your desired frequency range
const int samplesPerMic = 64; // Adjust for latency vs. packet size

// PDM instances
PDM mics[3];

// Data buffer
byte dataBuffer[3 * samplesPerMic];

void setup() {
  Serial.begin(115200); // For debugging

  // Disable watchdog timer (optional, use with caution)
  disableCore0WDT();
  disableCore1WDT();

  // Initialize PDM microphones
  for (int i = 0; i < 3; i++) {
    mics[i].begin(micPins[i], sampleRate);
    mics[i].setGain(30); // Adjust gain as needed
  }
}

void loop() {
  // Read data from microphones
  for (int i = 0; i < 3; i++) {
    mics[i].read((int16_t*)&dataBuffer[i * samplesPerMic], samplesPerMic); 
    // Data is stored as bytewise integers (8-bit)
    for(int j=0; j< samplesPerMic; j++){
      dataBuffer[i * samplesPerMic + j] = (byte)( ((int16_t*)&dataBuffer[i * samplesPerMic])[j] >> 8); //Keep the high byte
    }
  }
 
  // Send data over serial (for testing, replace with WiFi/Bluetooth)
  Serial.write(dataBuffer, sizeof(dataBuffer));


  // Add small delay to avoid overwhelming serial communication (adjust as needed)
  delayMicroseconds(100); 
}