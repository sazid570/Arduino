// #include <Arduino.h>
// #include <Wire.h>
// #include <SPI.h> // Include SPI library if needed

// const int trigPin = 33;
// const int echoPin = 32;
// long duration;
// int distance;

// void setup() {
//   // ...existing code...
//   pinMode(trigPin, OUTPUT);
//   pinMode(echoPin, INPUT);
//   Serial.begin(115200);
//   // Initialize SPI if needed
//   SPI.begin();
//   // ...existing code...
// }

// void loop() {
//   // Clear the trigPin
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);

//   // Set the trigPin on HIGH state for 10 microseconds
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   // Read the echoPin, returns the sound wave travel time in microseconds
//   duration = pulseIn(echoPin, HIGH);

//   // Calculate the distance
//   distance = duration * 0.034 / 2;

//   // Print the distance on the Serial Monitor
//   Serial.print("Distance: ");
//   Serial.print(distance);
//   Serial.println(" cm");

//   // ...existing code...
//   delay(1000); // Wait for a second before the next loop
// }
