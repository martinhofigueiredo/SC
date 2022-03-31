#include <Arduino.h>
#define TIME1 1
const int butpin = 2;
const int ledPin = LED_BUILTIN;
bool flag = HIGH;
bool status = LOW;
uint8_t time1 = TIME1;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(butpin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 34268; //preload timer
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1);
  interrupts();
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

ISR(TIMER1_OVF_vect){
 task_led();  
}
void task_led(void){
    TCNT1 = 34268; //preload timer
    status = !status;
    if(flag){
      digitalWrite(ledPin, status);}
  }

  
void loop() {
  // put your main code here, to run repeatedly:
  
  }