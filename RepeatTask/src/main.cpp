#include <Arduino.h>

TaskHandle_t sayHelloWorldTask;

IRAM_ATTR void sayHelloWorld(void *parameters)
{
  portTickType xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    Serial.println("Hello World!");

    vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();
    switch (cmd)
    {
    case 'a':
      xTaskCreatePinnedToCore(sayHelloWorld, "sayHello", 1024, NULL, 1, &sayHelloWorldTask, APP_CPU_NUM);
      break;

    case 'b':
      vTaskDelete(sayHelloWorldTask);
      break;
    }
  }
}