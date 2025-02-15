#include "BluetoothSerial.h"
#include "Arduino.h"
#include "DHT11.h"
#include <Adafruit_Sensor.h>
#include<Wire.h>
#include <U8g2lib.h>
#include <SPI.h> 
#include <ESP32Servo.h>

const int trigPin = 33;
const int echoPin = 32;
long duration;
int distance;

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
int servoPin = 4;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
int servoPin = 4;
#else
int servoPin = 4;
#endif

DHT11 dht11(5);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


// Create a BluetoothSerial object
BluetoothSerial ESP_BT;
String value;

const int relayPin = 18; // Pin connected to the relay
const int relayPin2 = 27; 
const int relayPin3 = 14; 
const int relayPin4 = 13; 

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Start Bluetooth with a device name
  ESP_BT.begin("ESP32_Relay_Control"); 
  Serial.println("Bluetooth Started. Waiting for instructions...");

  dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a font
  u8g2.drawStr(0, 10, "Welcome to smart home");
  u8g2.sendBuffer(); // Send to display

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); 

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  SPI.begin();

  // Initialize relay pin as output
  pinMode(relayPin, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  digitalWrite(relayPin, HIGH); // Turn relay OFF initially
  digitalWrite(relayPin2, HIGH); 
  digitalWrite(relayPin3, HIGH); 
  digitalWrite(relayPin4, HIGH); 
}

void loop() {

  int temperature = 0;
  int humidity = 0;
  int result = dht11.readTemperatureHumidity(temperature, humidity);
  value = String(temperature) + " " + String(humidity);
  ESP_BT.println(value);

  if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        
        
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a font
        u8g2.drawStr(3, 10, "Temperature: ");
        u8g2.setCursor(5, 20);
        u8g2.print(temperature);
        u8g2.drawStr(3, 30, "Humidity: ");
        u8g2.setCursor(5, 40);
        u8g2.print(humidity);
        u8g2.sendBuffer(); // Send to display
        delay(200);
    } else {
        Serial.println(DHT11::getErrorString(result));
    }

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distance = duration * 0.034 / 2;

  // Print the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 5) {
     u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a font
        u8g2.drawStr(3, 10, "Welcome Home!");
        u8g2.sendBuffer(); // Send to display


        for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            myservo.write(pos);    // tell servo to go to position in variable 'pos'
            delay(15);             // waits 15ms for the servo to reach the position
        }

        delay(3000); // the gate should be open for 2 seconds
        
        for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);    // tell servo to go to position in variable 'pos'
            delay(15);             // waits 15ms for the servo to reach the position
        }
  }

  // ...existing code...
  delay(200); // Wait for a second before the next loop
  
  
  // Check if there's data from Bluetooth
  if (ESP_BT.available()) {
    char receivedChar = ESP_BT.read(); // Read the incoming data
    // Serial.print("Received: ");
    // Serial.println(receivedChar);

    if (receivedChar == '1') {
      // Turn the relay ON
      digitalWrite(relayPin, LOW); // Active LOW relay
      // Serial.println("Relay ON");
      // ESP_BT.println("Relay ON"); // Send feedback via Bluetooth
    } else if (receivedChar == '0') {
      // Turn the relay OFF
      digitalWrite(relayPin, HIGH); // Deactivate the relay
      // Serial.println("Relay OFF");
      // ESP_BT.println("Relay OFF"); // Send feedback via Bluetooth
    }else if (receivedChar == '2') {
      // Turn the relay OFF
      digitalWrite(relayPin2, LOW); // Deactivate the relay
      // Serial.println("Relay2 ON");
      // ESP_BT.println("Relay2 ON"); // Send feedback via Bluetooth
    }else if (receivedChar == '3') {
      // Turn the relay OFF
      digitalWrite(relayPin2, HIGH); // Deactivate the relay
      // Serial.println("Relay2 OFF");
      // ESP_BT.println("Relay2 OFF"); // Send feedback via Bluetooth
    }
    else if (receivedChar == '4') {
      // Turn the relay OFF
      digitalWrite(relayPin3, LOW); // Deactivate the relay
      // Serial.println("Relay3 ON");
      // ESP_BT.println("Relay3 ON"); // Send feedback via Bluetooth
    }else if (receivedChar == '5') {
      // Turn the relay OFF
      digitalWrite(relayPin3, HIGH); // Deactivate the relay
      // Serial.println("Relay3 OFF");
      // ESP_BT.println("Relay3 OFF"); // Send feedback via Bluetooth
    }
    else if (receivedChar == '8') {
      // Turn the relay OFF
      digitalWrite(relayPin4, LOW); 
      // Serial.println("Relay4 ON");
      // ESP_BT.println("Relay4 ON"); 
    }
    else if (receivedChar == '7') {
      // Turn the relay OFF
      digitalWrite(relayPin4, HIGH); // Deactivate the relay
      // Serial.println("Relay4 OFF");
      // ESP_BT.println("Relay4 OFF"); // Send feedback via Bluetooth
    }
    
     else {
      // Invalid input
      // Serial.println("Invalid command.");
      // ESP_BT.println("Invalid command.");
    }
  }

  delay(200); // Small delay to improve responsiveness
}