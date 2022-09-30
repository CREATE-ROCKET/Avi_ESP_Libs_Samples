#include <Arduino.h>
#include <SPICREATE.h>
#include <S25FL512S.h>

#define flashCS 27
#define SCK1 33
#define MISO1 25
#define MOSI1 26

SPICREATE::SPICreate SPIC1;
Flash flash1;

// SPIflash S25FL512S 用サンプルコード

void setup()
{
  Serial.begin(115200);
  SPIC1.begin(VSPI, SCK1, MISO1, MOSI1);
  flash1.begin(&SPIC1, flashCS, 100000);
  // put your setup code here, to run once:
  Serial.println("Launched");
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

    if (cmd == 't')
    {
      uint8_t rx[256];
      flash1.read(0x1000000, rx);
      for (int i = 0; i < 256; i++)
      {
        Serial.print(rx[i]);
      }
      Serial.println();

      delay(100);

      uint8_t tx[256];
      for (int i = 0; i < 256; i++)
      {
        tx[i] = 0xFF - i;
      }
      flash1.write(0x1000000, tx);

      delay(100);

      flash1.read(0x1000000, rx);
      for (int i = 0; i < 256; i++)
      {
        Serial.print(rx[i]);
      }
      Serial.println();
    }
  }
  delay(100);
}