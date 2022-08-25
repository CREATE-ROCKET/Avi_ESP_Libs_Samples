#include <Arduino.h>
#include <SDIOLogWrapper.hpp>

#define LOGGINGINTERVAL 1

#define QUEUESTRINGSIZE 256

xTaskHandle xlogHandle;

IRAM_ATTR void logging(void *parameters)
{
  portTickType xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    char *bfChar = new char[QUEUESTRINGSIZE];
    bfChar[0] = '\0';
    uint8_t xQueueWaitCount = SDIOLogWrapper::countWaitingQueue();
    sprintf(bfChar, "%s%d,aaaaaaaaaaaa", bfChar, xQueueWaitCount);

    for (int i = 0; i < 16; i++)
    {
      sprintf(bfChar, "%s%s,", bfChar, "aaaaaaaaaaaa");
    }

    sprintf(bfChar, "%s\n", bfChar);

    if (SDIOLogWrapper::appendQueue(bfChar) == 1)
    {
      vTaskDelete(&xlogHandle);
      Serial.println("queue filled!");
      ESP.restart();
    }

    vTaskDelayUntil(&xLastWakeTime, LOGGINGINTERVAL / portTICK_PERIOD_MS);
  }
}

void setup()
{
  // io init
  Serial.begin(115200);

  SDIOLogWrapper::makeQueue(128);
  SDIOLogWrapper::setSaveFileName("/aiueo.csv");
  SDIOLogWrapper::setSaveInterval(100);
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (Serial.available())
  {
    char cmd = Serial.read();

    if (cmd == 'r')
    {
      ESP.restart();
    }

    if (cmd == 'a')
    {
      Serial.printf("SD init result: %d\n", SDIOLogWrapper::initSD());
      SDIOLogWrapper::openFile();
      SDIOLogWrapper::writeTaskCreate(APP_CPU_NUM);
      xTaskCreateUniversal(logging, "logging", 8192, NULL, configMAX_PRIORITIES, &xlogHandle, PRO_CPU_NUM);
    }

    if (cmd == 'g')
    {
      vTaskDelete(xlogHandle);
      delay(1000);
      SDIOLogWrapper::writeTaskDelete();
      SDIOLogWrapper::closeFile();
      SDIOLogWrapper::deinitSD();
    }
  }
  delay(100);
}