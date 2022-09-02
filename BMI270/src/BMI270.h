#pragma once

#ifndef BMI_H
#define BMI_H
#include <SPICREATE.h>
#include <Arduino.h>

#define BMI_Data_Adress 0x0C
#define BMI_WhoAmI_Adress 0x00 //default 0x24
#define BMI_ACC_CONFIG 0x40
#define BMI_ACC_RANGE 0x41
#define BMI_GYRO_CONFIG 0x42
#define BMI_GYRO_RANGE 0x43
#define BMI_PWR_CTRL 0x7D
#define BMI_PWR_CONF 0x7C
#define BMI_16G 0x03 
#define BMI_8G 0x02
#define BMI_4G 0x01
#define BMI_2G 0x00
#define BMI_2000dps 0x00
#define BMI_1000dps 0x01
#define BMI_500dps  0x02
#define BMI_250dps  0x03
#define BMI_125dps  0x04
#define BMI_2500deg 0x00       
#define BMI_SET_PWR 0b00000110
#define BMI_SET_PMODE 0x03

class BMI
{
    int CS;
    int deviceHandle{-1};
    SPICREATE::SPICreate *BMISPI;

public:
    void begin(SPICREATE::SPICreate *targetSPI, int cs, uint32_t freq = 8000000);
    uint8_t WhoImI();
    void Get(int16_t *rx);
};

void BMI::begin(SPICREATE::SPICreate *targetSPI, int cs, uint32_t freq)
{
    CS = cs;
    BMISPI = targetSPI;
    spi_device_interface_config_t if_cfg = {};

    //if_cfg.spics_io_num = cs;
    if_cfg.pre_cb = NULL;
    if_cfg.post_cb = NULL;
    if_cfg.cs_ena_pretrans = 0;
    if_cfg.cs_ena_posttrans = 0;

    if_cfg.clock_speed_hz = freq;

    if_cfg.mode = SPI_MODE0;
    if_cfg.queue_size = 1;

    if_cfg.pre_cb = csReset;
    if_cfg.post_cb = csSet;

    deviceHandle = BMISPI->addDevice(&if_cfg, cs);

    //Init
    BMISPI->setReg(BMI_PWR_CONF, BMI_SET_PMODE, deviceHandle);
    delay(1);
    BMISPI->setReg(BMI_PWR_CTRL, BMI_SET_PWR, deviceHandle); //enable acuisition accelaration and gyroscope
    BMISPI->setReg(BMI_ACC_RANGE, BMI_16G, deviceHandle);
    BMISPI->setReg(BMI_GYRO_RANGE, BMI_2000dps, deviceHandle);
    return;
}
uint8_t BMI::WhoImI()
{
    // return BMISPI->readByte(0x00, deviceHandle);
    uint8_t rx_buf[2];
    spi_transaction_t comm = {};
    comm.flags = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
    comm.length = (2) * 8;
    comm.cmd = BMI_WhoAmI_Adress | 0x80;

    comm.tx_buffer = NULL;
    comm.rx_buffer = rx_buf;
    comm.user = (void *)CS;

    spi_transaction_ext_t spi_transaction = {};
    spi_transaction.base = comm;
    spi_transaction.command_bits = 8;
    BMISPI->pollTransmit((spi_transaction_t *)&spi_transaction, deviceHandle);

    return rx_buf[1];
}
void BMI::Get(int16_t *rx)
{
    uint8_t rx_buf[13];
    spi_transaction_t comm = {};
    comm.flags = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
    comm.length = (13) * 8;
    comm.cmd = BMI_Data_Adress | 0x80;

    comm.tx_buffer = NULL;
    comm.rx_buffer = rx_buf;
    comm.user = (void *)CS;

    spi_transaction_ext_t spi_transaction = {};
    spi_transaction.base = comm;
    spi_transaction.command_bits = 8;
    BMISPI->pollTransmit((spi_transaction_t *)&spi_transaction, deviceHandle);

    rx[0] = (rx_buf[2] << 8 | rx_buf[1]);
    rx[1] = (rx_buf[4] << 8 | rx_buf[3]);
    rx[2] = (rx_buf[6] << 8 | rx_buf[5]);
    rx[3] = (rx_buf[8] << 8 | rx_buf[7]);
    rx[4] = (rx_buf[10] << 8 | rx_buf[9]);
    rx[5] = (rx_buf[12] << 8 | rx_buf[11]);
    return;
}
#endif