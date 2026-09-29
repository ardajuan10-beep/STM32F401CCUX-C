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

//SPI2_NSS -> PB9 // not used (for slave config only)
//SPI2_SCK -> PB10
//SPI2_MISO -> PB14 //not used
//SPI2_MOSI -> PB15

void static I2C1_GPIO_Inits(void){
	GPIO_Handle_t I2CPins;



	// set for SCL
	// sets the starting address of the registers as GPIOB
	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; // open drain
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; // pull up resistor
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	// set for SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);
}

void static SPI2_GPIO_Inits(void){
	GPIO_Handle_t SPIPins;

	// set for SCK SPI2
	// sets the starting address of the registers as GPIOB
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; //push pull
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; // no pull up or pull down resistor
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&SPIPins);

	// set for MOSI SPI2
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);
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


void static SPI2_Inits(void){
	SPI_Handle_t SPI2_Handle;

	// sets the starting address of the registers as I2C1
	SPI2_Handle.pSPIx = SPI2_I2S2;
	SPI2_Handle.SPI_Config.SPI_BusConfig = SPI_BusConfig_FD;
	SPI2_Handle.SPI_Config.SPI_DeviceMode = SPI_DeviceMode_Master;
	SPI2_Handle.SPI_Config.SPI_SclkSpeed = SPI_SCLKSpeedDiv_2;
	SPI2_Handle.SPI_Config.SPI_SSM = SPI_SSM_DI; // does not care, this is for slave

	// init the SPI2
	SPI_Init(&SPI2_Handle);
}

int I2C_test(void){
	//initialise GPIO for I2C 1
	I2C1_GPIO_Inits();
	//initialise I2C1 parameters like speed, ack control, I2C clock
	I2C1_Inits();


	//initialise ODR register of the temp sensor
	// ODR register is 0x37
	// we are writing bx1101 1101 -> 1 (Disables sleep mode) 10111 (10 Hz) 01 (normal mode)
	uint8_t odr_init[] = {0x37, 0xDD};
	I2C_MasterSendData(&I2C1_Handle, &odr_init[0], 2, SLAVE_ADDR);

	while(1){
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




}
