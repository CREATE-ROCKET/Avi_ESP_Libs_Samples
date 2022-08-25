#include <Arduino.h>
#include <SPICREATE.h>
#include <PITOIC.h>

#define PITOCS 27
#define SCK1 33
#define MISO1 25
#define MOSI1 26

SPICREATE::SPICreate SPIC1;
PITOIC pitoic;

//ピトー管IC HSCDRRN160MDSA3 用 サンプルコード

void setup()
{
  Serial.begin(115200);
  SPIC1.begin(VSPI, SCK1, MISO1, MOSI1);
  pitoic.begin(&SPIC1, PITOCS, 6000000);
  // put your setup code here, to run once:
}

void loop()
{
  uint8_t rx[2];
  pitoic.Get(rx);
  Serial.print(rx[0]);
  Serial.print(",");
  Serial.print(rx[1]);
  Serial.println();
  delay(500);
  // put your main code here, to run repeatedly:
}