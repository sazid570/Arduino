// #include <DHT11.h>
// #include<arduino.h>
// #include<Adafruit_Sensor.h>

// // - For ESP32: Connect the sensor to pin GPIO2 or P2.

// DHT11 dht11(2);

// void setup() {
//     // Initialize serial communication to allow debugging and data readout.
//     // Using a baud rate of 9600 bps.
//     Serial.begin(9600);
    
//     // Uncomment the line below to set a custom delay between sensor readings (in milliseconds).
//     // dht11.setDelay(500); // Set this to the desired delay. Default is 500ms.
// }

// void loop() {
//     int temperature = 0;
//     int humidity = 0;

//     // Attempt to read the temperature and humidity values from the DHT11 sensor.
//     int result = dht11.readTemperatureHumidity(temperature, humidity);

//     // Check the results of the readings.
//     // If the reading is successful, print the temperature and humidity values.
//     // If there are errors, print the appropriate error messages.
//     if (result == 0) {
//         Serial.print("Temperature: ");
//         Serial.print(temperature);
//         Serial.print(" °C\tHumidity: ");
//         Serial.print(humidity);
//         Serial.println(" %");
//     } else {
//         // Print error message based on the error code.
//         Serial.println(DHT11::getErrorString(result));
//     }
// }
