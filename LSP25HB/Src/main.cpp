#include <Arduino.h>
#include <SPICREATE.h>
#include <LPS25HB.h>
#include <MissionConfig.h>

SPICREATE::SPICreate SPIC1;
LPS lps25;
uint8_t rx[3];
uint8_t t = 0;

void setup()
{
  delay(100);
  // pinMode(33, true);
  // digitalWrite(33, HIGH);
  delay(100);
  Serial.begin(115200);
  delay(100);
  SPIC1.begin(HSPI, SCK1, MISO1, MOSI1);
  delay(100);
  Serial.println("Serial Set Done");
  delay(100);
  lps25.begin(&SPIC1, LPSCS, 5000000);
  delay(100);
  Serial.println("Set Done");
}

void loop()
{
  if (Serial.available())
  {
    if (Serial.read() == 'l')
    {
      t++;
    }
  }
  if (t > 0)
  {
    lps25.Get(rx);
    for (uint8_t i = 0; i < 3; i++)
    {
      Serial.print(rx[i]);
      Serial.print(",\t");
    }
    Serial.println(lps25.Plessure);
  }

  delay(1000);
  // put your main code here, to run repeatedly:
}