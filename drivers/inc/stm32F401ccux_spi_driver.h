/*
 * stm32F401ccux_spi_driver.h
 *
 *  Created on: Sep 4, 2026
 *      Author: ardaj
 */

#ifndef INC_STM32F401CCUX_SPI_DRIVER_H_
#define INC_STM32F401CCUX_SPI_DRIVER_H_

#include "stm32F401ccux.h"

// Configuration structure for SPIx peripheral
typedef struct
{
	uint8_t			SPI_DeviceMode; //master or slave
	uint8_t			SPI_BusConfig;	// full duplex, half duplex, simplex
	uint8_t			SPI_SclkSpeed; // required serial clock speed
	uint8_t			SPI_DFF; // 8 bits or 16 bits data format
	uint8_t			SPI_CPOL; // idle state of clock when no data is being transferred, 0 for low
	uint8_t			SPI_CPHA; // 0 -> data is sampled at the first edge of the clock, 1 -> sampled at second edge
	uint8_t			SPI_SSM; //hardware or software slave select managemetn
}SPI_Config_t;

//Handle Structure for SPIx peripheral
typedef struct
{
	SPI_RegDef_t	*pSPIx;
	SPI_Config_t	SPI_Config;
}SPI_Handle_t;

// SPI Device Modes -> bit 2 of CR1 register
#define SPI_DeviceMode_Master		1
#define SPI_DeviceMode_Slave		0

// SPI Bus config -> bit 10 (for receive only simplex) bits 14 (receive or transmit for half duplex) and 15 (half or full duplex) of CR1 register
#define SPI_BusConfig_FD			1 //full duplex
#define SPI_BusConfig_HD			2 //half duplex
//#define SPI_BusConfig_S_TX		3 //simplex TX only //this is equivalent to full duplex communication with one less cable
#define SPI_BusConfig_S_RX			3 //simplex RX only // this is almost the same as full duplex but we need to enable bit 10 to be able to force clock on master mode without mosi line

// SPI clock speed -> bits 3,4,5 of CR1
#define SPI_SCLKSpeedDiv_2			0 //divides the peripheral clock by 2
#define SPI_SCLKSpeedDiv_4			1 //divides the peripheral clock by 4
#define SPI_SCLKSpeedDiv_8			2 //divides the peripheral clock by 8
#define SPI_SCLKSpeedDiv_16			3 //divides the peripheral clock by 16
#define SPI_SCLKSpeedDiv_32			4 //divides the peripheral clock by 32
#define SPI_SCLKSpeedDiv_64			5 //divides the peripheral clock by 64
#define SPI_SCLKSpeedDiv_128		6 //divides the peripheral clock by 128
#define SPI_SCLKSpeedDiv_256		7 //divides the peripheral clock by 256

// SPI Data Format -> bit 11 of CR1
#define SPI_DFF_16bits				1
#define SPI_DFF_8bits				0

// SPI CPHA -> bit 0 of CR1
#define SPI_CPHA_High				1
#define SPI_CPHA_Low				0

// SPI CPOL -> bit 1 of CR1
#define SPI_CPOL_High				1
#define SPI_CPOL_Low				0

//SPI SSM (Software Slave management) -> bit 9 of CR1
#define SPI_SSM_EN					1 // software slave management
#define SPI_SSM_DI					0 // hardware slave management


/*********************************************************************
 * 				APIs supported by this driver
 * 				Check function definitions for more info
 *****************************************************************
 */

//Peripheral Clock Setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

//Init and De-init
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

//blocking send and receive
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

//IRQ Configuration and ISR Handling
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);


#endif /* INC_STM32F401CCUX_SPI_DRIVER_H_ */
