
/*
 Author : Yuta Takayasu
 Date : 2022/5/14
    Library for operating barometer with ESP32.
    It depends on SPICREATE.h and SPICREATE.h
*/

#pragma once

#ifndef MPU_H
#define MPU_H
#include <SPICREATE.h>
#include <Arduino.h>

#define MPU_Data_Adress 0x3B
#define MPU_GYRO_CONFIG 0x1B
#define MPU_WhoAmI_Adress 0x75
#define MPU_ACC_CONFIG 0x1C
#define MPU_16G 0b00011000
#define MPU_8G 0b00010000
#define MPU_4G 0b00001000
#define MPU_2G 0b00000000
#define MPU_2000dps 0b00011000
#define MPU_1000dps 0b00010000
#define MPU_500dps 0b00001000
#define MPU_250dps 0b00000000

#define MPU_2500deg 0x18

class MPU
{
    int CS;
    int deviceHandle{-1};
    SPICREATE::SPICreate *MPUSPI;

public:
    float AccelNorm;
    void begin(SPICREATE::SPICreate *targetSPI, int cs, uint32_t freq = 8000000);
    uint8_t WhoAmI();
    void CheckReg();
    void Get(int16_t *rx);
};

void MPU::begin(SPICREATE::SPICreate *targetSPI, int cs, uint32_t freq)
{
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);
    delay(10);
    digitalWrite(cs, LOW);
    CS = cs;
    MPUSPI = targetSPI;
    spi_device_interface_config_t if_cfg = {};

    // if_cfg.spics_io_num = cs;
    if_cfg.pre_cb = NULL;
    if_cfg.post_cb = NULL;
    if_cfg.cs_ena_pretrans = 0;
    if_cfg.cs_ena_posttrans = 0;

    if_cfg.clock_speed_hz = freq;

    if_cfg.mode = SPI_MODE3;
    if_cfg.queue_size = 1;

    if_cfg.pre_cb = csReset;
    if_cfg.post_cb = csSet;

    deviceHandle = MPUSPI->addDevice(&if_cfg, cs);

    // Init
    delay(100);
    MPUSPI->setReg(0x6A, 0x10, deviceHandle); // set I2C disable
    delay(1);
    MPUSPI->setReg(MPU_ACC_CONFIG, MPU_16G, deviceHandle); // set range 16G
    delay(1);
    MPUSPI->setReg(MPU_GYRO_CONFIG, MPU_2000dps, deviceHandle); // set range 2000 deg/s
    delay(1);

    CheckReg();
    delay(1);

    return;
}
uint8_t MPU::WhoAmI()
{
    return MPUSPI->readByte(0x80 | 0x75, deviceHandle);
}

void MPU::CheckReg()
{
    // if (WhoAmI() != 113)
    // {
    //     while (1)
    //     {
    //         Serial.print("Wrong Who Am I MPU ");
    //         Serial.println(WhoAmI());
    //         delay(1000);
    //     }
    // }
    return;
}
void IRAM_ATTR MPU::Get(int16_t *rx)
{
    uint8_t rx_buf[14];
    spi_transaction_t comm = {};
    comm.flags = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
    comm.length = (14) * 8;
    comm.cmd = MPU_Data_Adress | 0x80;

    comm.tx_buffer = NULL;
    comm.rx_buffer = rx_buf;
    comm.user = (void *)CS;

    spi_transaction_ext_t spi_transaction = {};
    spi_transaction.base = comm;
    spi_transaction.command_bits = 8;
    MPUSPI->pollTransmit((spi_transaction_t *)&spi_transaction, deviceHandle);

    rx[0] = (rx_buf[0] << 8 | rx_buf[1]);
    rx[1] = (rx_buf[2] << 8 | rx_buf[3]);
    rx[2] = (rx_buf[4] << 8 | rx_buf[5]);
    rx[3] = (rx_buf[8] << 8 | rx_buf[9]);
    rx[4] = (rx_buf[10] << 8 | rx_buf[11]);
    rx[5] = (rx_buf[12] << 8 | rx_buf[13]);
    AccelNorm = 0;
    for (uint8_t i = 0; i < 3; i++)
    {
        double AccelBuf = rx[i] * 16. / 32767.;
        AccelNorm = AccelNorm + AccelBuf * AccelBuf;
    }
    AccelNorm = sqrt(AccelNorm);
    return;
}
#endif