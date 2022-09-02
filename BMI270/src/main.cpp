#include <Arduino.h>
#include <bmi270.h>
#include <SPICREATE.h>
#include <MPU9250.h>


#define BMISCK 14
#define BMIMISO 12
#define BMIMOSI 13
#define BMICS 33
#define SPIFREQ 100000

BMI bmi270;

SPICREATE::SPICreate SPIC;


void setup() {
  Serial.begin(115200);
  SPIC.begin(HSPI, BMISCK, BMIMISO, BMIMOSI);
  bmi270.begin(&SPIC, BMICS, SPIFREQ);
  
}

void loop() {
  uint8_t a;
  a = bmi270.WhoImI();
  Serial.println(a);
  delay(1);
  int16_t bmi_data[6];
  bmi270.Get(bmi_data);
  Serial.write("AX:");
  Serial.println(bmi_data[0]);
  Serial.print("AY:");
  Serial.println(bmi_data[1]);
  Serial.print("AZ:");
  Serial.println(bmi_data[2]);
  Serial.print("GX:");
  Serial.println(bmi_data[3]);
  Serial.print("GY:");
  Serial.println(bmi_data[4]);
  Serial.print("GZ:");
  Serial.println(bmi_data[5]);
  
  delay(1000);
}