/*
 * stm32F401ccux_i2c_driver.h
 *
 *  Created on: Dec 20, 2025
 *      Author: ardaj
 */

#ifndef STM32F401CCUX_I2C_DRIVER_H_
#define STM32F401CCUX_I2C_DRIVER_H_

#include "stm32F401ccux.h"

// Configuration structure for I2Cx peripheral
typedef struct
{
	uint32_t		I2C_SCLSpeed;
	uint8_t			I2C_DeviceAddress;
	uint8_t			I2C_ACKControl;
	uint16_t		I2C_FMDutyCycle;
}I2C_Config_t;

//Handle Structure for I2Cx peripheral
typedef struct
{
	I2C_RegDef_t	*pI2Cx;
	I2C_Config_t	I2C_Config;
}I2C_Handle_t;

//I2C_SCLSpeed
#define I2C_SCL_SPEED_SM 	100000
#define I2C_SCL_SPEED_FM4K 	400000
#define I2C_SCL_SPEED_FM2K 	200000

// I2C ack control
#define I2C_ACK_ENABLE		1
#define I2C_ACK_DISABLE		0

//I2C FMDutyCycle
#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1

//I2C Related status flag definitions
#define I2C_FLAG_SB			(1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR		(1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF		(1 << I2C_SR1_BTF)
#define I2C_FLAG_STOPF		(1 << I2C_SR1_STOPF)
#define I2C_FLAG_RxNE		(1 << I2C_SR1_RxNE)
#define I2C_FLAG_TxE		(1 << I2C_SR1_TxE)
#define I2C_FLAG_BERR		(1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO		(1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF			(1 << I2C_SR1_AF)
#define I2C_FLAG_OVR		(1 << I2C_SR1_OVR)
#define I2C_FLAG_TIMEOUT	(1 << I2C_SR1_TIMEOUT)


/*********************************************************************
 * 				APIs supported by this driver
 * 				Check function definitions for more info
 *****************************************************************
 */

//Peripheral Clock Setup
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

//Master
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr);

//Init and De-init
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

//IRQ Configuration and ISR Handling
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

//Other Peripheral Control APIs
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

// Application Callback
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);


#endif /* STM32F401CCUX_I2C_DRIVER_H_ */
