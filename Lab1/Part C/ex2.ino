/** AMGP@FE.UP.PT 
 *  SC - 2021/2022
 */
#include <Arduino_FreeRTOS.h>
#include <MultiFunctionShield.h>        

// Include mutex support
#include <semphr.h>

//Declaring a SemaphoreHandle_t
SemaphoreHandle_t mutex;

int var_switching_time = 500;

void TaskBlink1( void *pvParameters );
void TaskSerial( void *pvParameters );
void TaskButtonS2( void *pvParameters );
void TaskBuzz(void *pvParameters);


void setup() {  
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Create a mutex
  mutex = xSemaphoreCreateMutex();

  xTaskCreate(
             TaskBlink1,  // Function to be called
             "Task1",     // Name of task for debuging
             128,         // Stack size
             NULL,        // Parameter to pass to function
             1,           // Task priority (0 to configMAX_PRIORITIES -1)
             NULL);       // Task handle
             
  xTaskCreate(TaskSerial,"Task1", 128, NULL, 1, NULL);   
  xTaskCreate(TaskButtonS2,"Task2", 128, NULL, 2, NULL);  
  xTaskCreate(TaskButtonS1,"Task3", 128, NULL, 2, NULL); 
 // xTaskCreate(TaskBuzz,"Task4", 128, NULL, 1, NULL); 
  
  // Start Scheduler that will manage tasks
  vTaskStartScheduler();
}


void TaskBlink1(void *pvParameters) 
{
  pinMode(LED_4_PIN, OUTPUT);
  while(1)
  {
    digitalWrite(LED_4_PIN, HIGH);   
    vTaskDelay( var_switching_time / portTICK_PERIOD_MS ); 
    digitalWrite(LED_4_PIN, LOW);    
    vTaskDelay( var_switching_time / portTICK_PERIOD_MS ); 
  }
}


void TaskSerial(void *pvParameters)  
{
  int incomingValue = 0; // for incoming serial data
  while(1)
  {
    // send data only when you receive data:
    if (Serial.available() > 0) 
    {
      //Take mutex
      if (xSemaphoreTake(mutex, 10) == pdTRUE)
      {
        // read the incoming byte:
        incomingValue = Serial.parseInt();
        if(incomingValue!=0)
          var_switching_time = incomingValue;

        //Give mutex
        xSemaphoreGive(mutex);  
      }
    }
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); 
  }
}

void TaskButtonS2(void *pvParameters)  
{
  MultiFunctionShield MFS;
  MFS.begin();
  pinMode (BUTTON_2_PIN, INPUT);    // insert J2 on board for pull-ups!
  
  while(1)
  {
    if (!digitalRead(BUTTON_2_PIN))
    {
        //Take mutex
        if (xSemaphoreTake(mutex, 10) == pdTRUE)
        {
         var_switching_time *= 2;
         MFS.Display(var_switching_time);
         
         //Give mutex
         xSemaphoreGive(mutex);
        }
    }
    vTaskDelay (300/ portTICK_PERIOD_MS);
  }
}


void TaskButtonS1(void *pvParameters)  
{
  MultiFunctionShield MFS;
  MFS.begin();
  pinMode (BUTTON_1_PIN, INPUT);    // insert J2 on board for pull-ups!
  
  while(1)
  {
    if (!digitalRead(BUTTON_1_PIN))
    {
        //Take mutex
        if (xSemaphoreTake(mutex, 10) == pdTRUE)
        {
           var_switching_time /= 2;
           MFS.Display(var_switching_time);
           
           //Give mutex
           xSemaphoreGive(mutex);
        }
    }
    vTaskDelay (300/ portTICK_PERIOD_MS);
  }
}


 
void TaskBuzz(void *pvParameters) 
{
  //Declaring LED pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  while(1)
  {
      //Fading the LED
      for(int i=0; i<255; i++)
      {
        analogWrite(BUZZER_PIN, i);
        vTaskDelay (var_switching_time/ portTICK_PERIOD_MS);
      }
    
      for(int i=255; i>0; i--)
      {
        analogWrite(BUZZER_PIN, i);
        vTaskDelay (var_switching_time/ portTICK_PERIOD_MS);
      }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
}