/*
 * stm32F401ccux_spi_driver.h
 *
 *  Created on: Sep 4, 2026
 *      Author: ardaj
 */

#ifndef INC_STM32F401CCUX_SPI_DRIVER_H_
#define INC_STM32F401CCUX_SPI_DRIVER_H_

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
