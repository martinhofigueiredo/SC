#include <Arduino.h>

const int butpin = 2;
const int ledPin = LED_BUILTIN;
const long interval = 200; // define interval to toggle LED
unsigned long last_time = 0;
bool status = LOW;
bool flag = HIGH;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(butpin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(butpin), pause, FALLING);
}

void pause(){
  Serial.print("ISR CALL ");
  flag = !flag;
  if(flag){
    Serial.println("Enable");
  }
  else{
    Serial.println("Disable");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();
  if (current_time - last_time >= interval && flag) {
    // save the last time you blinked the LED
    last_time = current_time;
    status = !status;
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, status);
  }
  Serial.print("Status - ");
  Serial.print(status);
  Serial.print(" | Flag - ");
  if(flag){
    Serial.println("Enable");
    }
   else{
    Serial.println("Disable");
    }
  }