// #include <Servo.h>

// #define FIRE_SENSOR_DIGITAL 8  // Digital pin
// #define LED 13  // Onboard LED (for alert)


// Servo myservo;  // create Servo object to control a servo
// // twelve Servo objects can be created on most boards

// int pos = 0;    // variable to store the servo position

// void setup() {
//     Serial.begin(9600);
//     pinMode(FIRE_SENSOR_DIGITAL, INPUT);
//     pinMode(LED, OUTPUT);
//     pinMode(10, INPUT);
//   myservo.attach(9); 
//   myservo.write(pos);    
// }

// void loop() {
    
//     int digitalValue = digitalRead(FIRE_SENSOR_DIGITAL);
//     int digitaldustbin = digitalRead(10);
  

//     if (digitalValue == LOW) {  // Fire detected!
//         Serial.println("🔥 Fire Detected! 🔥");
//         digitalWrite(LED, HIGH);
//     } else {
//         digitalWrite(LED, LOW);
//     }

//     delay(500);
  
//   if(digitaldustbin == LOW){
//     Serial.println("Open dustbin");
//   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }
//     delay(1000);
//   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(15);                       // waits 15 ms for the servo to reach the position
//   }}

//   // if (digitaldustbin == LOW){
//   //   Serial.println("Detected");
//   // }
// }
