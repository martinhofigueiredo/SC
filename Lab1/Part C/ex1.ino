/** AMGP@FE.UP.PT 
 *  SC - 2021/2022
 */
 
#include <Arduino_FreeRTOS.h>

// Include mutex support
#include <semphr.h>

//Declaring a SemaphoreHandle_t
SemaphoreHandle_t mutex;

int var_global = 0;
void TaskInc( void *pvParameters );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Create a mutex
  mutex = xSemaphoreCreateMutex();
  if (mutex != NULL) {
    Serial.println("Mutex created");
  }

    
  xTaskCreate(TaskInc,"Task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskInc,"Task2", 128, NULL, 1, NULL);
  
  // Start Scheduler that will manage tasks
  vTaskStartScheduler();
}

void TaskInc( void *pvParameters )
{
  int var_local = var_global;
  while (1)
  {
    //Take mutex
    if (xSemaphoreTake(mutex, 10) == pdTRUE)
    {
      var_local++;
      vTaskDelay( random(80, 200)/ portTICK_PERIOD_MS ); 
      var_global = var_local;
  
      Serial.println(var_global);
  
      //Give mutex
      xSemaphoreGive(mutex);  
    }
    else
    {
      //Do stuffs that do not require access to critical sections
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:

}