// // qrt8rc that I have is a digital sensor. When it finds a black surface it returns High(~5v) and when it finds a white surface it returns Low(0v). Also do not connect the ir pin to anywhere, leave it as it is.

// #include <Arduino.h>

// // IR sensor pins (7 connected from the 8-array QRT8RC)
// #define IR1 4
// #define IR2 18
// #define IR3 19
// #define IR4 21  // Center sensor
// #define IR5 22
// #define IR6 23
// #define IR7 13
// // I have connected 7 pins because there were no safe pins available

// #define NUM_SENSORS 7

// void readSensors(int *rawValues) {
//     // Read raw sensor values
//     rawValues[0] = digitalRead(IR1);
//     rawValues[1] = digitalRead(IR2);
//     rawValues[2] = digitalRead(IR3);
//     rawValues[3] = digitalRead(IR4);
//     rawValues[4] = digitalRead(IR5);
//     rawValues[5] = digitalRead(IR6);
//     rawValues[6] = digitalRead(IR7);
    
//     // Debug: Print raw values
//     Serial.print("Raw: ");
//     for (int i = 0; i < NUM_SENSORS; i++) {
//       Serial.print(rawValues[i]);
//       Serial.print(" ");
      
//     }
//     Serial.println();
    
//   }

// void setup() {
//     Serial.begin(115200);
//     pinMode(IR1, INPUT);
//     pinMode(IR2, INPUT);
//     pinMode(IR3, INPUT);
//     pinMode(IR4, INPUT);
//     pinMode(IR5, INPUT);
//     pinMode(IR6, INPUT);
//     pinMode(IR7, INPUT);
//     // Add any other setup code here
//     Serial.println("Setup complete. Ready to read sensors.");
// }

// void loop() {
//     int rawValues[NUM_SENSORS];
    
    
//     readSensors(rawValues);
    
//     // Add any other logic you want to implement in the loop
//     delay(10); // Adjust the delay as needed
// }