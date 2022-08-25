#include <FS.h>
#include <SD_MMC.h>

#ifndef SDIOLOGWRAPPER_H
#define SDIOLOGWRAPPER_H

namespace SDIOLogWrapper
{
    bool isSDOpend = 0;
    File logFile;
    TaskHandle_t xWriteSDHandle;
    const char *fileName = "/logger.csv";
    QueueHandle_t xQueue;
    int SaveInterval = 128;

    void setSaveFileName(const char *);

    int initSD();
    void deinitSD();

    void openFile();
    void writeFile(const char mData[]);
    void closeFile();

    int makeQueue(int uxQueueLength);
    int appendQueue(char xData[]);
    int countWaitingQueue();
    void deleteQueue();

    void writeSDfromQueue(void *parameters);

    void setSaveInterval(int interval);
    void writeTaskCreate(int TaskExecuteCore);
    void writeTaskDelete();

    void setSaveFileName(const char *_fileName)
    {
        fileName = _fileName;
    }

    int initSD()
    {
        if (isSDOpend)
        {
            return 2;
        }
        if (!SD_MMC.begin())
        {
            return 1;
        }
        isSDOpend = 1;
        return 0;
    }

    void deinitSD()
    {
        SD_MMC.end();
        isSDOpend = 0;
    }

    void openFile()
    {
        printf("file:%s", fileName);
        logFile = SD_MMC.open(fileName, FILE_APPEND);
    }

    void writeFile(const char mData[])
    {
        logFile.println(mData);
    }

    void closeFile()
    {
        logFile.close();
    }

    int makeQueue(int uxQueueLength)
    {
        xQueue = xQueueCreate(uxQueueLength, sizeof(char *));

        if (xQueue == NULL)
        {
            return 1;
        }
        return 0;
    }

    void deleteQueue()
    {
        vQueueDelete(xQueue);
    }

    IRAM_ATTR int countWaitingQueue()
    {
        return uxQueueMessagesWaiting(xQueue);
    }

    IRAM_ATTR int appendQueue(char *xData)
    {
        if (uxQueueSpacesAvailable(xQueue) < 1)
        {
            return 1;
        }
        else
        {
            xQueueSend(xQueue, &xData, 0);
        }
        return 0;
    }

    IRAM_ATTR void writeSDfromQueue(void *parameters)
    {
        for (;;)
        {
            char *data;
            int logCounter = 0;
            if (xQueueReceive(xQueue, &data, 0) == pdTRUE)
            {
                logFile.print(data);
                delete[] data;
                logCounter++;
                if (logCounter == SaveInterval)
                {
                    logCounter = 0;
                    logFile.close();
                    logFile = SD_MMC.open(
                        fileName, FILE_APPEND);
                }
            }
            else
            {
                delay(1);
            }
        }
    }

    void setSaveInterval(int interval)
    {
        SaveInterval = interval;
    }

    void writeTaskCreate(int TaskExecuteCore)
    {
        xTaskCreateUniversal(writeSDfromQueue, "Queue2SD", 8192, NULL, configMAX_PRIORITIES, &xWriteSDHandle, TaskExecuteCore);
    }

    void writeTaskDelete()
    {
        vTaskDelete(xWriteSDHandle);
    }
};
#endif