#pragma once

#ifndef PITOIC_H
#define PITOIC_H

#include <SPICREATE.h>
#include <Arduino.h>

class PITOIC
{
    int CS;
    float constantVal;
    int deviceHandle{-1};
    SPICREATE::SPICreate *PITOICSPI;

public:
    uint16_t PitoRawData;
    float WindSpeed;
    void begin(SPICREATE::SPICreate *targetSPI, int cs, uint32_t freq = 8000000, float constant = 1.);
    void Get(uint8_t *rx, int PlessureVal = 101325, float TemperatureVal = 14.5); // Oshima Nov. Average Temp.
    // *** LITTLE ENDIAN ***
    // (uint32_t)rx[0] << 8 | (uint32_t)rx[1] means PITO raw data
};

void PITOIC::begin(SPICREATE::SPICreate *targetSPI, int cs, uint32_t freq, float alpha)
{
    CS = cs;
    PITOICSPI = targetSPI;
    constantVal = alpha;
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

    deviceHandle = PITOICSPI->addDevice(&if_cfg, cs);
    return;
}

void PITOIC::Get(uint8_t *rx, int PlessureVal, float TemperatureVal)
{
    spi_transaction_t comm = {};
    comm.flags = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
    comm.length = (2) * 8;
    comm.cmd = NULL;

    comm.tx_buffer = NULL;
    comm.rx_buffer = NULL;
    comm.user = (void *)CS;

    spi_transaction_ext_t spi_transaction = {};
    spi_transaction.base = comm;
    spi_transaction.command_bits = 0;

    PITOICSPI->transmit(NULL, rx, 2, deviceHandle);

    PitoRawData = rx[0] << 8 | rx[1];
    float AirDensity = PlessureVal / ((TemperatureVal + 273.5) * 2.87);
    double Speed_Calc_Buf = (PitoRawData / 16384 * 100 - 10) * 250 - 10000;
    if (Speed_Calc_Buf < 0)
    {
        Speed_Calc_Buf = Speed_Calc_Buf * -1;
    }
    Speed_Calc_Buf = sqrt(Speed_Calc_Buf * 2 / AirDensity);
    WindSpeed = (float)Speed_Calc_Buf * constantVal;
    return;
}

#endif
