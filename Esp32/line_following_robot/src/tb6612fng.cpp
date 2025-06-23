#include <Arduino.h>

void setup(){
    Serial.begin(115200);
    Serial.println("TB6612FNG Motor Driver Test");
}

void loop(){
    Serial.println("Hello world");
    delay(1000); 

}