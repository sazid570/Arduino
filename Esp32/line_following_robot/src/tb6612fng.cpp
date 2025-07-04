#include<Arduino.h>

#define pwma 32
#define pwmb 14
#define motor1a 25
#define motor2a 26
#define motor1b 33
#define motor2b 27




void setup(){
    Serial.begin(115200);
    Serial.println("Tb6612fng motor driver test");
    // pinMode(pwma,OUTPUT);
    // pinMode(pwmb,OUTPUT);
    pinMode(27,OUTPUT);
    pinMode(26,OUTPUT);
    

}

void loop(){
    digitalWrite(27, HIGH);
    digitalWrite(26, LOW);
    Serial.println("Motor 1 ON");
    delay(2000);
    digitalWrite(27, LOW);
    digitalWrite(26, HIGH);  
    Serial.println("Motor 1 OFF");
    delay(2000);

}