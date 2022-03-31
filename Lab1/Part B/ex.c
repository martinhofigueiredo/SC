#include <Arduino_FreeRTOS.h>
#include <MultiFunctionShield.h >

TaskHandle_t xPrint;
bool emer;

void TaskBlink1( void *pvParameters ){
 pinMode(LED_BUILTIN, OUTPUT);
 int base = 250;
 while(1){
	 int speed = (emer ? base : 2*base); //speed is 1x if emer is false and 2x if emer is false
	 digitalWrite(LED_BUILTIN, HIGH);
	 vTaskDelay(speed/portTICK_PERIOD_MS);
	 digitalWrite(LED_BUILTIN, LOW);
	 vTaskDelay(speed/portTICK_PERIOD_MS);
 }
}

void TaskPrint( void *pvParameters ){
  
  long sec = 0;
  while(1){
	sec++;
	Serial.print("Segundos: ");
	Serial.println(sec);
	vTaskDelay (1000/portTICK_PERIOD_MS);
  }
}

void TaskBuzz( void *pvParameters ){
  MultiFunctionShield MFS;
  MFS.begin();
  pinMode(BUTTON_3_PIN,INPUT);
  pinMode(BUZZER_PIN,OUTPUT);  
  while(1){
	if(!digitalRead(BUTTON_3_PIN)){
		vTaskSuspend(xPrint);
		Serial.println("EMERGENCY");
    digitalWrite(BUZZER_PIN, LOW);
    emer = 1;
	  }
	else{
		vTaskResume(xPrint);
    digitalWrite(BUZZER_PIN, HIGH);
	  emer = 0;
	  }
  }
}

void TaskDisplay( void *pvParameters ){
  MultiFunctionShield MFS;
  MFS.begin();
  pinMode(BUTTON_1_PIN, INPUT);
  long counter = 0;
  while(1){
	if(!digitalRead(BUTTON_1_PIN)){
		counter++;
		MFS.Display(counter);
	}
	vTaskDelay (1000/portTICK_PERIOD_MS);
  }
}


 
void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
	Serial.println("Start");
	xTaskCreate(TaskBlink1, "Blink", 128, NULL, 1, NULL);
	xTaskCreate(TaskDisplay, "Display", 128, NULL, 1, NULL);
	xTaskCreate(TaskPrint, "Print", 128, NULL, 1, &xPrint);
	xTaskCreate(TaskBuzz, "Buzz", 128, NULL, 1, NULL);
	vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}