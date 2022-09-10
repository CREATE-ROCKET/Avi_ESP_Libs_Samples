#include <Arduino.h>
#include <SPICREATE.h>
#include <S25FL512S.h>

#define flashCS 27
#define SCK1 33
#define MISO1 25
#define MOSI1 26

SPICREATE::SPICreate SPIC1;
Flash flash1;

#define PAGE_LENGTH 512 // You can change this number to an aliquot part of 512.

// SPIflash S25FL512S 用サンプルコード

void setup()
{
  Serial.begin(115200);
  SPIC1.begin(VSPI, SCK1, MISO1, MOSI1);
  flash1.begin(&SPIC1, flashCS, 100000);
  // put your setup code here, to run once:
}

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();
    if (cmd == 'e')
    {
      flash1.erase();
    }
    if (cmd == 'r')
    {
      Serial.println("r is pushed");
      uint8_t rx[512];
      flash1.read(0x000, rx);
      for (int i = 0; i < 512; i++)
      {
        if (i % 16 == 0)
        {
          Serial.println();
        }
        Serial.print(rx[i]);
      }
      Serial.println();
      flash1.read(0x200, rx);
      for (int i = 0; i < 512; i++)
      {
        if (i % 16 == 0)
        {
          Serial.println();
        }
        Serial.print(rx[i]);
      }
      Serial.println();
      flash1.read(0x400, rx);
      for (int i = 0; i < 512; i++)
      {
        if (i % 16 == 0)
        {
          Serial.println();
        }
        Serial.print(rx[i]);
      }
      Serial.println();
      Serial.println();
    }
    if (cmd == 't')
    {
      uint8_t rx[512];
      flash1.read(0x000, rx);
      for (int i = 0; i < 512; i++)
      {
        Serial.print(rx[i]);
      }
      Serial.println();

      delay(100);

      uint8_t tx[512] = {};
      for (int i = 0; i < 512; i++)
      {
        tx[i] = i / 2; // integer
        // Serial.print(i / 2);
      }
      Serial.println();
      flash1.write(0x200, tx);

      delay(100);

      flash1.read(0x200, rx);
      for (int i = 0; i < 512; i++)
      {
        Serial.print(rx[i]);
      }
      Serial.println();
    }
  }
  delay(100);
}