#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Wire.h>
#include <SPI.h>
#include "display.h"

Display disp;

QueueHandle_t xQueue1;

/*  Task to control display - refreshing 50 hz 
*
*/
void displayUpdate(void * parameter){
  const TickType_t xFrequency = 20;
  TickType_t xLastWakeTime = xTaskGetTickCount();

  uint32_t tmp_disp_number = 54637598;

  for(;;){
    xQueueReceive(xQueue1, &(tmp_disp_number), 0);

    disp.Loop(tmp_disp_number);

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}


/*  Task to serial data scanning
*
*/
void serialUpdate(void * parameter){
  uint32_t counter = 0;
  const uint32_t max_counter = 10;
  uint32_t number_read;
  uint8_t data_read[4];
  for(;;){
    while(Serial.available() >= 4){
      Serial.readBytes(data_read, 4);
      number_read = (uint32_t)data_read[3] | data_read[2] << 8 | data_read[1] << 16 | data_read[0] << 24;
      xQueueSend( xQueue1, ( void * ) &number_read, ( TickType_t ) 0 );
    }

    if(Serial.available()){ // clear buffer if there are less than 3 bytes for "max_counter" ticks time
      if(++counter >= max_counter){
        while(Serial.read() != -1){}
        counter = 0;
      }
    }else{
      counter = 0;
    }

    
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  xQueue1 = xQueueCreate( 1, sizeof(uint32_t));

  xTaskCreatePinnedToCore(
    displayUpdate,       // Function that should be called
    "Display update",    // Name of the task (for debugging)
    2000,                // Stack size (bytes)
    NULL,                // Parameter to pass
    1,                   // Task priority
    NULL,                 // Task handle
    CONFIG_ARDUINO_RUNNING_CORE
  );

  xTaskCreate(
    serialUpdate,       // Function that should be called
    "Serial update",    // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    2,               // Task priority
    NULL             // Task handle
  );
}

void loop() {
  delay(1);
}
