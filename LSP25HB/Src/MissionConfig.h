#ifndef MissionConfig
#define MissionConfig

// UART Setting
#define WBRX 34
#define WBTX 33
#define RMRX 35
#define RMTX 32
#define UARTSpeed 115200

// SPI Setting
#define SCK1 18
#define MISO1 19
#define MOSI1 23
#define MPUCS 5
#define LPSCS 17
#define PITOCS 16
#define FLASHCS0 25
#define FLASHCS1 26
#define FLASHCS2 27
#define SPI_freq 5000000
#define PITO_freq 300000
#define FLASH_freq 12000000

// LED Setting
#define LEDR_Port 22
#define LEDG_Port 21
#define LEDB_Port 0
#define PWM_freq 12800

//
#define MPU_WhoAmI_Value 113
#define LPS_WhoAmI_Value 189

// Mission Board Mode
#define Wait_Mode_0 0
#define Prepare_Mode 1
#define Log_Mode 2
#define Wait_Mode_1 3
#define Delete_Mode 4
#define ReadWait_Mode 5
#define ReadData_Mode 6

// Parameter
#define QueueSize 256
#define Flash_Max 0x1000000

#endif