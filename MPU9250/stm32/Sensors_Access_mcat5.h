/*
 * Sensors_Access.h
 *
 *  Created on: 2020/05/07
 *      Author: tomoki
 */

#ifndef SENSORS_ACCESS_MCAT_H_
#define SENSORS_ACCESS_MCAT_H_
#include "stm32f7xx_hal.h"
#include "main.h"
void ICM_CS_Low();
void ICM_CS_High();
void LPS_CS_Low();
void LPS_CS_High();
void MPU_CS_Low();
void MPU_CS_High();

void SPI_Transmit(uint8_t value, SPI_HandleTypeDef spi);
void SPI_TransmitRecieve(uint8_t RecieveData[],int Size, SPI_HandleTypeDef spi);

void ICM_Write_Byte(uint8_t Adress,uint8_t Value);
void ICM_Read_Bytes(uint8_t Adress,uint8_t ReceiveData[], int Size);
void LPS_Write_Byte(uint8_t Adress,uint8_t Value);
void LPS_renderRead_Bytes(uint8_t Adress,uint8_t ReceiveData[], int Size);
void LPS_Read_Bytes(uint8_t Adress,uint8_t ReceiveData[], int Size);
void MPU_Write_Byte(uint8_t Adress,uint8_t Value);
void MPU_Read_Bytes(uint8_t Adress,uint8_t ReceiveData[],int Size);

uint8_t ICM_WhoAmI();
uint8_t LPS_WhoAmI();
uint8_t MPU_WhoAmI();

void ICM_Initialization();
void ICM_Rawand6axis(uint8_t RecieveData[14],int16_t ICM_Raw_Data[6]);
void ICM_6axis_Raw(int16_t ICM_Raw_Data[6]);

void ICM_Calibration(int16_t Gyro_Calibrate_Data[3]);

void LPS_WakeUp();
uint32_t LPS_RawandPressure(uint8_t RecieveData[3]);
uint32_t LPS_Pressure_Raw();
int LPS_Tempreture_Raw();

void MPU_Initialization();
void MPU_6axis_Raw(uint8_t Recieve_Data[14],int16_t MPU_Raw_Data[6]);
int MPU_Compass_Raw(uint8_t Receive_Data[7],int16_t MPU_Raw_Data[3]);
void MPU_6axis(uint16_t MPU_Data[6]);

#endif /* SENSORS_ACCESS_MCAT4_H_ */
