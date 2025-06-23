#include<Arduino.h>
#include <wifi.h>


const char* ssid = "TP-Link_6F55";
const char* password = "10639115";

WiFiServer server(80); // Create a server that listens on port 80

String header;

String output23state = "off"; // Variable to store the LED state
const int output23 = 23; // GPIO pin for the built-in LED

void setup() {
  // Initialize serial communication at a baud rate of 115200
  Serial.begin(115200);
  Serial.println("Welcome to webserver on ESP32!");

  pinMode(output23,OUTPUT); // Set the built-in LED pin as output
  digitalWrite(output23, LOW); // Initialize the LED to be off
  
  // connect to the wifi
  WiFi.begin(ssid, password); // Connect to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) { // Wait for the connection to be established
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  //print the credentials 
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start the server
}

void loop(){
  WiFiClient client = server.available();

  if (client) { // Check if a client has connected
    Serial.println("New Client.");
    String currentLine = ""; // Variable to store the current line of the request

    while (client.connected()) { // Loop while the client is connected
      if (client.available()) { // If there is data available to read
        char c = client.read(); // Read a character from the client
        Serial.write(c); // Print the character to the serial monitor
        header += c; // Append the character to the header string

        if (c == '\n') { // If we reach the end of a line
          if (currentLine.length() == 0) { // If the current line is empty, it means we have received the end of the request
            // Respond to the request
            if (header.indexOf("GET /23/on") >= 0) {
              output23state = "on"; // Set LED state to on
              digitalWrite(output23, HIGH); // Turn on the LED
            } else if (header.indexOf("GET /23/off") >= 0) {
              output23state = "off"; // Set LED state to off
              digitalWrite(output23, LOW); // Turn off the LED
            }

            // Send HTTP response with styled button
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head>");
            client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>");
            client.println("body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; }");
            client.println(".btn { display: inline-block; padding: 20px 40px; font-size: 2em; border: none; border-radius: 8px; margin: 20px 0; width: 80vw; max-width: 400px; transition: background 0.2s; color: #fff; cursor: pointer; }");
            client.println(".on { background: #4CAF50; }");
            client.println(".off { background: #f44336; }");
            client.println("@media (max-width: 500px) { .btn { font-size: 1.2em; padding: 15px 10px; } }");
            client.println("</style>");
            client.println("</head>");
            client.println("<body>");
            client.print("<h2>LED is now <span style='color:");
            client.print(output23state == "on" ? "#4CAF50" : "#f44336");
            client.print(";'>");
            client.print(output23state);
            client.println("</span></h2>");
            if (output23state == "off") {
              client.println("<a href=\"/23/on\"><button class=\"btn on\">Turn On</button></a>");
            } else {
              client.println("<a href=\"/23/off\"><button class=\"btn off\">Turn Off</button></a>");
            }
            client.println("</body>");
            client.println("</html>");

            break; // Break out of the while loop after sending response
          } else { 
            currentLine = ""; // Clear current line for next iteration
          }
        } else if (c != '\r') { 
          currentLine += c; // Append character to current line
        }
      }
    }

    header = ""; // Clear header for next request
    client.stop(); // Close the connection with the client
    Serial.println("Client disconnected.");
  }
  
}
