/*
 * I2C_test.c
 *
 *  Created on: Apr 19, 2026
 *      Author: ardaj
 */

#include <stdint.h>
#include <string.h>
#include "stm32F401ccux.h"

#define MY_ADDRESS  			0x61
#define SLAVE_ADDR				0x47

//I2C1_SCL -> PB6
//I2C1_SDA -> PB7


void static I2C1_GPIO_Inits(void){
	GPIO_Handle_t I2CPins;



	// set for SCL
	// sets the starting address of the registers as GPIOB
	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	// set for SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);
}

I2C_Handle_t I2C1_Handle;
void static I2C1_Inits(void){

	// sets the starting address of the registers as I2C1
	I2C1_Handle.pI2Cx = I2C1;
	I2C1_Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	// address does not matter as we are master
	I2C1_Handle.I2C_Config.I2C_DeviceAddress = MY_ADDRESS;
	// we are not using FM so it is not important
	I2C1_Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	// set I2C speed as standard mode
	I2C1_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
	// init the I2C
	I2C_Init(&I2C1_Handle);
}

int I2C_test(void){
	//initialise GPIO for I2C 1
	I2C1_GPIO_Inits();
	//initialise I2C1 parameters like speed, ack control, I2C clock
	I2C1_Inits();
	//enable I2C peripheral
	//I2C_PeripheralControl(I2C1, ENABLE); // already done inside init
	//acking can only be enabled after peripheral is enabled. I WILL ADDRESS THIS LATER. THIS IS NOT IDEAL
	//I2C1_Handle.pI2Cx->CR1 |= (1 << 10);  // Set Bit 10 (ACK) // already done inside init

	//initialise ODR register
	// ODR register is 0x37
	// we are writing bx1101 1101 -> 1 (Disables sleep mode) 10111 (10 Hz) 01 (normal mode)
	uint8_t odr_init[] = {0x37, 0xDD};
	I2C_MasterSendData(&I2C1_Handle, &odr_init[0], 2, SLAVE_ADDR);

	//Read temp value
	// Temp registers
	uint8_t TEMP_REG_XLSB = 0x1D;
	uint8_t TEMP_REG_LSB = 0x1E;
	uint8_t TEMP_REG_MSB = 0x1F;
	// Temp data
	uint8_t TEMP_DATA_XLSB = 0x1D;
	uint8_t TEMP_DATA_LSB = 0x1E;
	uint8_t TEMP_DATA_MSB = 0x1F;
	//read xlsb
	I2C_MasterSendData(&I2C1_Handle, &TEMP_REG_XLSB, 1, SLAVE_ADDR);
	I2C_MasterReceiveData(&I2C1_Handle, &TEMP_DATA_XLSB, 1, SLAVE_ADDR);
	//read lsb
	I2C_MasterSendData(&I2C1_Handle, &TEMP_REG_LSB, 1, SLAVE_ADDR);
	I2C_MasterReceiveData(&I2C1_Handle, &TEMP_DATA_LSB, 1, SLAVE_ADDR);
	//read msb
	I2C_MasterSendData(&I2C1_Handle, &TEMP_REG_MSB, 1, SLAVE_ADDR);
	I2C_MasterReceiveData(&I2C1_Handle, &TEMP_DATA_MSB, 1, SLAVE_ADDR);

	//convert to real temperature value
	// 1. Combine the registers into a 32-bit integer
	int32_t raw_temp = (TEMP_DATA_MSB << 16) | (TEMP_DATA_LSB << 8) | TEMP_DATA_XLSB;
	// 2. Sign-extend the 24-bit value to 32 bits
	if (raw_temp & 0x800000) {
	    raw_temp |= 0xFF000000;
	}
	// 3. Convert to Celsius
	float temperature_celsius = (float)raw_temp / 65536.0f;
	while(1);


}
