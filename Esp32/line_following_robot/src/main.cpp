// #include <Arduino.h>

// /*
//  * ESP32 Line Follower Robot
//  * Using TB6612FNG motor driver and QRT8RC IR sensor array (7 sensors)
//  * Features: Sensor calibration, junction detection, and PD line following
//  */

// // Motor driver pins
// #define PWMA 32  // Speed control for motor A
// #define AIN1 25  // Direction control 1 for motor A
// #define AIN2 33  // Direction control 2 for motor A
// #define PWMB 14  // Speed control for motor B
// #define BIN1 26  // Direction control 1 for motor B
// #define BIN2 27  // Direction control 2 for motor B

// // IR sensor pins (7 connected from the 8-array QRT8RC)
// #define IR1 4
// #define IR2 18
// #define IR3 19
// #define IR4 21  // Center sensor
// #define IR5 22
// #define IR6 23
// #define IR7 13

// // Number of sensors
// #define NUM_SENSORS 7

// // PWM properties
// #define PWM_FREQ 5000
// #define PWM_RESOLUTION 8
// #define PWM_CHANNEL_A 0
// #define PWM_CHANNEL_B 1

// // Motor speed values for 500 RPM N20 motors with 8V supply
// #define MAX_SPEED 255      // Maximum possible PWM value
// #define BASE_SPEED 120     // Reduced base speed for better control with fast motors
// #define TURN_SPEED 90      // Speed for turns
// #define SLOW_SPEED 70      // Speed for junctions and precise movements

// // Control variables - adjusted for 500 RPM N20 motors
// int lastError = 0;
// float Kp = 20;   // Reduced proportional gain for smoother control with faster motors
// float Kd = 25;   // Increased derivative gain to reduce oscillation

// // Sensor calibration values
// int minValues[NUM_SENSORS];
// int maxValues[NUM_SENSORS];
// int threshold[NUM_SENSORS];

// // Robot states
// enum RobotState {
//   FOLLOWING_LINE,
//   JUNCTION_DETECTED,
//   LOST_LINE,
//   STOP
// };

// RobotState currentState = FOLLOWING_LINE;
// unsigned long lostLineTimer = 0;
// const unsigned long LOST_LINE_TIMEOUT = 1000; // Time in ms before giving up when line is lost

// void calibrateSensors();
// void readSensors(int *rawValues, int *normalizedValues);
// void updateRobotState(int *normalizedValues);
// void followLine(int *normalizedValues);
// void handleJunction(int *normalizedValues);
// void searchForLine();
// void stopRobot();
// int calculatePosition(int *normalizedValues);
// void driveMotors(int leftSpeed, int rightSpeed);

// void setup() {
//   Serial.begin(115200);
  
//   // Set motor pins as outputs
//   pinMode(AIN1, OUTPUT);
//   pinMode(AIN2, OUTPUT);
//   pinMode(BIN1, OUTPUT);
//   pinMode(BIN2, OUTPUT);
  
//   // Configure PWM for motor speed control - adjusted for 500 RPM N20 motors
//   ledcSetup(PWM_CHANNEL_A, 20000, PWM_RESOLUTION);  // Higher frequency (20kHz) for smoother operation
//   ledcSetup(PWM_CHANNEL_B, 20000, PWM_RESOLUTION);  // Higher frequency reduces motor noise
//   ledcAttachPin(PWMA, PWM_CHANNEL_A);
//   ledcAttachPin(PWMB, PWM_CHANNEL_B);
  
//   // Set initial motor state to stopped
//   digitalWrite(AIN1, LOW);
//   digitalWrite(AIN2, LOW);
//   digitalWrite(BIN1, LOW);
//   digitalWrite(BIN2, LOW);
  
//   // Set IR sensor pins as inputs
//   pinMode(IR1, INPUT);
//   pinMode(IR2, INPUT);
//   pinMode(IR3, INPUT);
//   pinMode(IR4, INPUT);
//   pinMode(IR5, INPUT);
//   pinMode(IR6, INPUT);
//   pinMode(IR7, INPUT);
  
//   // Initialize calibration values
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     minValues[i] = 1023;  // Start with max possible value
//     maxValues[i] = 0;     // Start with min possible value
//     threshold[i] = 512;   // Default threshold
//   }
  
//   Serial.println("Starting sensor calibration in 2 seconds...");
//   delay(2000);
  
//   // Run calibration routine
//   calibrateSensors();
  
//   Serial.println("Line follower robot initialized and calibrated");
//   delay(1000);
// }

// void loop() {
//   // Read sensor values
//   int sensorValues[NUM_SENSORS];
//   int normalizedValues[NUM_SENSORS];
//   readSensors(sensorValues, normalizedValues);
  
//   // Determine robot state
//   updateRobotState(normalizedValues);
  
//   // Handle different states
//   switch (currentState) {
//     case FOLLOWING_LINE:
//       followLine(normalizedValues);
//       break;
      
//     case JUNCTION_DETECTED:
//       handleJunction(normalizedValues);
//       break;
      
//     case LOST_LINE:
//       searchForLine();
//       break;
      
//     case STOP:
//       stopRobot();
//       break;
//   }
  
//   // Small delay for stability
//   delay(10);
// }

// void calibrateSensors() {
//   Serial.println("Move the robot over the line for calibration");
  
//   // Blink LED to indicate calibration mode (if you have an LED connected)
//   for (int i = 0; i < 5; i++) {
//     // You can add LED blinking code here if you have an LED connected
//     delay(200);
//   }
  
//   // Calibration for 3 seconds
//   unsigned long startTime = millis();
//   while (millis() - startTime < 3000) {
//     int sensorValue;
//     // Read the sensors multiple times
//     for (int i = 0; i < NUM_SENSORS; i++) {
//       // Select the sensor pin based on index
//       int sensorPin;
//       switch (i) {
//         case 0: sensorPin = IR1; break;
//         case 1: sensorPin = IR2; break;
//         case 2: sensorPin = IR3; break;
//         case 3: sensorPin = IR4; break;
//         case 4: sensorPin = IR5; break;
//         case 5: sensorPin = IR6; break;
//         case 6: sensorPin = IR7; break;
//       }
      
//       // For digital sensors, we'll take multiple readings to improve reliability
//       int sum = 0;
//       for (int j = 0; j < 10; j++) {
//         sum += digitalRead(sensorPin);
//         delayMicroseconds(200);
//       }
//       sensorValue = sum > 5 ? 1 : 0;  // If more than half readings are HIGH, consider it HIGH
      
//       // Update min and max values
//       if (sensorValue < minValues[i]) minValues[i] = sensorValue;
//       if (sensorValue > maxValues[i]) maxValues[i] = sensorValue;
//     }
//     delay(10);
//   }
  
//   // Calculate thresholds
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     // For digital sensors, this might not be as useful, but keeping for flexibility
//     threshold[i] = (minValues[i] + maxValues[i]) / 2;
//   }
  
//   Serial.println("Calibration complete");
//   // Print calibration values
//   Serial.println("Sensor thresholds:");
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     Serial.print(i);
//     Serial.print(": ");
//     Serial.print(threshold[i]);
//     Serial.print(" (min: ");
//     Serial.print(minValues[i]);
//     Serial.print(", max: ");
//     Serial.print(maxValues[i]);
//     Serial.println(")");
//   }
// }

// void readSensors(int *rawValues, int *normalizedValues) {
//   // Read raw sensor values
//   rawValues[0] = digitalRead(IR1);
//   rawValues[1] = digitalRead(IR2);
//   rawValues[2] = digitalRead(IR3);
//   rawValues[3] = digitalRead(IR4);
//   rawValues[4] = digitalRead(IR5);
//   rawValues[5] = digitalRead(IR6);
//   rawValues[6] = digitalRead(IR7);
  
//   // Debug: Print raw values
//   Serial.print("Raw: ");
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     Serial.print(rawValues[i]);
//     Serial.print(" ");
    
//     // Normalize values based on calibration
//     // For digital sensors: if raw value is closer to maxValue, then line is detected
//     // Note: Adjust this logic based on your sensor (0=line or 1=line)
//     normalizedValues[i] = (rawValues[i] == 0) ? 1 : 0;  // Assuming 0 means line detected
//   }
//   Serial.println();
// }

// void updateRobotState(int *normalizedValues) {
//   int lineCount = 0;
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     if (normalizedValues[i] == 1) {
//       lineCount++;
//     }
//   }
  
//   // Check for junction (multiple sensors detect line)
//   if (lineCount >= 5) {
//     currentState = JUNCTION_DETECTED;
//     Serial.println("Junction detected!");
//     return;
//   }
  
//   // Check for lost line
//   if (lineCount == 0) {
//     if (currentState != LOST_LINE) {
//       lostLineTimer = millis();
//       currentState = LOST_LINE;
//       Serial.println("Line lost!");
//     } else if (millis() - lostLineTimer > LOST_LINE_TIMEOUT) {
//       currentState = STOP;
//       Serial.println("Stopping due to lost line timeout");
//     }
//     return;
//   }
  
//   // Default to line following
//   currentState = FOLLOWING_LINE;
// }

// void followLine(int *normalizedValues) {
//   // Calculate position using weighted average
//   int position = calculatePosition(normalizedValues);
  
//   // Calculate PD controller output
//   int error = position - 3000;  // 3000 is the center position (weighted average)
//   int motorSpeed = Kp * error + Kd * (error - lastError);
//   lastError = error;
  
//   // Calculate motor speeds
//   int leftMotorSpeed = BASE_SPEED - motorSpeed;
//   int rightMotorSpeed = BASE_SPEED + motorSpeed;
  
//   // Constrain speeds to valid range
//   leftMotorSpeed = constrain(leftMotorSpeed, -MAX_SPEED, MAX_SPEED);
//   rightMotorSpeed = constrain(rightMotorSpeed, -MAX_SPEED, MAX_SPEED);
  
//   // Drive motors
//   driveMotors(leftMotorSpeed, rightMotorSpeed);
// }

// void handleJunction(int *normalizedValues) {
//   // Enhanced junction handling for faster motors
//   Serial.println("Handling junction");
  
//   // First slow down significantly to prevent overshooting
//   driveMotors(SLOW_SPEED, SLOW_SPEED);
//   delay(150);
  
//   // Short stop to stabilize
//   driveMotors(0, 0);
//   delay(50);
  
//   // Continue slowly through the junction
//   driveMotors(SLOW_SPEED, SLOW_SPEED);
//   delay(250);
  
//   // Return to line following
//   currentState = FOLLOWING_LINE;
// }

// void searchForLine() {
//   // Try to find the line again by rotating
//   Serial.println("Searching for line");
  
//   // Check which direction to rotate based on the last error
//   if (lastError < 0) {
//     // Line was to the left, so rotate left
//     driveMotors(-TURN_SPEED, TURN_SPEED);
//   } else {
//     // Line was to the right, so rotate right
//     driveMotors(TURN_SPEED, -TURN_SPEED);
//   }
// }

// void stopRobot() {
//   // Stop both motors
//   driveMotors(0, 0);
// }

// // Calculate weighted position of the line
// // Returns a value between 0 and 6000
// int calculatePosition(int *normalizedValues) {
//   int sum = 0;
//   int weightedSum = 0;
//   boolean lineDetected = false;
  
//   for (int i = 0; i < NUM_SENSORS; i++) {
//     if (normalizedValues[i] == 1) {  // Line detected
//       sum += 1;
//       weightedSum += i * 1000;
//       lineDetected = true;
//     }
//   }
  
//   if (lineDetected) {
//     return weightedSum / sum;
//   } else {
//     // Line not found, return a value based on the last known error
//     if (lastError < 0) {
//       return 0;  // Far left
//     } else {
//       return 6000;  // Far right
//     }
//   }
// }

// // Drive motors with the specified speeds - optimized for N20 500 RPM motors
// void driveMotors(int leftSpeed, int rightSpeed) {
//   // Apply exponential curve to motor speeds for better control at lower speeds
//   // This helps with the high RPM motors by providing finer control
//   leftSpeed = map(leftSpeed, 0, MAX_SPEED, 0, MAX_SPEED);
//   rightSpeed = map(rightSpeed, 0, MAX_SPEED, 0, MAX_SPEED);
  
//   // Apply deadzone compensation - N20 motors often have a minimum speed before they start moving
//   if (leftSpeed > 0 && leftSpeed < 45) leftSpeed = 45;
//   if (rightSpeed > 0 && rightSpeed < 45) rightSpeed = 45;
//   if (leftSpeed < 0 && leftSpeed > -45) leftSpeed = -45;
//   if (rightSpeed < 0 && rightSpeed > -45) rightSpeed = -45;
  
//   // Left motor direction
//   if (leftSpeed >= 0) {
//     digitalWrite(AIN1, HIGH);
//     digitalWrite(AIN2, LOW);
//   } else {
//     digitalWrite(AIN1, LOW);
//     digitalWrite(AIN2, HIGH);
//     leftSpeed = -leftSpeed;
//   }
  
//   // Right motor direction
//   if (rightSpeed >= 0) {
//     digitalWrite(BIN1, HIGH);
//     digitalWrite(BIN2, LOW);
//   } else {
//     digitalWrite(BIN1, LOW);
//     digitalWrite(BIN2, HIGH);
//     rightSpeed = -rightSpeed;
//   }
  
//   // Set motor speeds with soft start for N20 motors
//   // Ramping up too quickly can cause wheel slip
//   static int lastLeftSpeed = 0;
//   static int lastRightSpeed = 0;
  
//   // Limit acceleration to prevent wheel slip
//   const int MAX_ACCELERATION = 20;
//   if (leftSpeed > lastLeftSpeed + MAX_ACCELERATION) leftSpeed = lastLeftSpeed + MAX_ACCELERATION;
//   if (rightSpeed > lastRightSpeed + MAX_ACCELERATION) rightSpeed = lastRightSpeed + MAX_ACCELERATION;
  
//   // Set motor speeds
//   ledcWrite(PWM_CHANNEL_A, leftSpeed);
//   ledcWrite(PWM_CHANNEL_B, rightSpeed);
  
//   // Update last speeds
//   lastLeftSpeed = leftSpeed;
//   lastRightSpeed = rightSpeed;
  
//   // Debug: Print motor speeds
//   Serial.print("Motors - L: ");
//   Serial.print(leftSpeed);
//   Serial.print(" R: ");
//   Serial.println(rightSpeed);
// }