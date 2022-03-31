#include <Arduino.h>

const int ledPin = LED_BUILTIN; // define which LED to use
const long interval = 200; // define interval to toggle LED
unsigned long last_time = 0;
bool status = LOW;


// initialization
void setup() {
    pinMode (ledPin, OUTPUT); // set the ledPin as output
}
// run
void loop() {
  
unsigned long current_time = millis();
if (current_time - last_time >= interval) {
    // save the last time you blinked the LED
    last_time = current_time;
    status = !status;
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, status);
  }
};