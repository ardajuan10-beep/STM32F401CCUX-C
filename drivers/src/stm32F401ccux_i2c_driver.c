/*
 * i2c_driver.c
 *
 *  Created on: Dec 12, 2025
 *      Author: ardaj
 */


#include "stm32F401ccux_i2c_driver.h"

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseR(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseW(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){
	//we are writing to the start register here which is 8.
	pI2Cx->CR1 |= (1<< I2C_CR1_STOP);
}

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
	//we are writing to the start register here which is 8.
	pI2Cx->CR1 |= (1<< I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseW(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){
	// Shift slave addr by one to make space for read write bit
	SlaveAddr = SlaveAddr << 1;
	// Least significant bit of the address field is set to 0 for WRITE
	// This is done by bitwise anding Slave ADDR with 11111110
	SlaveAddr &= ~(1);
	// Write the 8 bit unsigned int to the data register
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ExecuteAddressPhaseR(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){
	// Shift slave addr by one to make space for read write bit
	SlaveAddr = SlaveAddr << 1;
	// Least significant bit of the address field is set to 1 for Read
	// This is done by adding 1 to the slave address (we know that the lsb is 0)
	SlaveAddr = SlaveAddr + 1;
	// Write the 8 bit unsigned int to the data register
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFFlag(I2C_RegDef_t *pI2Cx){
	// Read SR1 and SR2 registers
	// This will automatically clear the ADDR Flag
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;
	// below is to avoid the unused variable warning
	(void)dummyRead;
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
	//we are writing to the peripheral enable bit (0) of control register 1.
	pI2Cx->CR1 |= (1<< I2C_CR1_PE);
}

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		if (pI2Cx == I2C1){
			I2C1_PCLK_EN();
		}else if (pI2Cx == I2C2){
			I2C2_PCLK_EN();
		}else if (pI2Cx == I2C3){
			I2C3_PCLK_EN();
		}
	}else{
		if (pI2Cx == I2C1){
			I2C1_PCLK_DI();
		}else if (pI2Cx == I2C2){
			I2C2_PCLK_DI();
		}else if (pI2Cx == I2C3){
			I2C3_PCLK_DI();
		}
	}

}

uint32_t RCC_GetPLLOutputClock(){
	return 0;
}


uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, SystemClk, ahbp;
	uint8_t clksrc, temp, apb1p;

	// we are reading bits 2 and 3 (clk source) of RCC_CFGR
	clksrc = ((RCC->CFGR >> 2) & 0x3);
	//then determine clock based on clock source
	if (clksrc == 0){ //HSI clock
		SystemClk = 16000000;
	}
	else if (clksrc == 1){ //HSE clock
		SystemClk = 8000000;
	}
	else if (clksrc == 2){ //PLL clock
		SystemClk = RCC_GetPLLOutputClock();
	}

	// we are reading the bits 4-7 (AHB Prescaler) of RCC_CFGR
	temp = ((RCC->CFGR >> 4) & 0xF);
	// depending on the value of bits 4-7 we determine the division factor
	if (temp < 8){
		ahbp = 1; //divided by 1
	}else if (temp == 0b1000){
		ahbp = 2; //divided by 2
	}else if (temp == 0b1001){
		ahbp = 4;
	}else if (temp == 0b1010){
		ahbp = 8;
	}else if (temp == 0b1011){
		ahbp = 16;
	}else if (temp == 0b1100){
		ahbp = 64;
	}else if (temp == 0b1101){
		ahbp = 128;
	}else if (temp == 0b1110){
		ahbp = 256;
	}else if (temp == 0b1111){
		ahbp = 512;
	}

	// we are reading the bits 12-10 (APB1 Prescaler) of RCC_CFGR
	temp = ((RCC->CFGR >> 10) & 0b0111);
	// depending on the value of bits 12-10 find the division factor
	if (temp < 0b100){
		apb1p = 1; //divided by 1
	}else if (temp == 0b100){
		apb1p = 2; //divided by 2
	}else if (temp == 0b101){
		apb1p = 4;
	}else if (temp == 0b110){
		apb1p = 8;
	}else if (temp == 0b111){
		apb1p = 16;
	}

	// find the clock frequency feeding the APB1 Peripherals
	pclk1 = (SystemClk / ahbp) / apb1p;

	return pclk1;
}



uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName){
	if (pI2Cx->SR1 & FlagName){
		return FLAG_SET;
	}else{
		return FLAG_RESET;
	}
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr){
	// 1. Generate START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in SR1
	//  Note: Until SB is cleared SCL will be stretched (pulled to Low)
	// When the SB flag is set (start condition is generated), it will go to the next phase
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));


	// 3. Send the address of the slave with r/nw bit set to W(0) (total 8 bits)
	I2C_ExecuteAddressPhaseW(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that the address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Clear the ADDR flag according to its software sequence (Reading SR1 and SR2 registers is required)
	//  Note: Until ADDR is cleared SCL will be stretched (pulled to Low)
	I2C_ClearADDRFFlag(pI2CHandle->pI2Cx);

	//6. Send the data until Len becomes 0
	while(Len > 0){
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TxE)); // Wait till TxE is set
		pI2CHandle->pI2Cx->DR = *pTxBuffer; // put the input data into data register
		pTxBuffer++; //increment input data pointer
		Len--; // decrement Len
	}

	//7. When Len becomes zero wait for TXE = 1 and BTF = 1 before generating the STOP condition
	// Note: TxE=1 and BTF=1 means that both SR and DR are empty and next transition should begin
	// When BFT=1 SCL will be stretched (pulled to Low)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TxE)); // Wait till TxE is set
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF)); // Wait till BTF is set

	//8. Generate STOP condition and master need not to wait for completion of stop condition
	// Note: Generating stop automatically clears the BTF
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint8_t Len, uint8_t SlaveAddr){
	// 1. Generate START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in SR1
	//  Note: Until SB is cleared SCL will be stretched (pulled to Low)
	// When the SB flag is set (start condition is generated), it will go to the next phase
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	// 3. Send the address of the slave with r/nw bit set to R(0) (total 8 bits)
	I2C_ExecuteAddressPhaseR(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that the address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//procedure ro read only 1 byte from slave
	if(Len == 1){
		//Disable Acking
		pI2CHandle->pI2Cx->CR1 &= ~(1 << 10); // Clear Bit 10 (ACK)

		//Clear the ADDR Flag by reading SR1 and SR2
		I2C_ClearADDRFFlag(pI2CHandle->pI2Cx);

		//wait until RXNE becomes 1 (SR1 register)
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RxNE));

		//generate stop condition
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//read data into buffer
		*pTxBuffer = pI2CHandle->pI2Cx->DR;
	}

	if (Len > 1){
		// clear address flag
		I2C_ClearADDRFFlag(pI2CHandle->pI2Cx);

		//read data until len becomes 0
		for(uint32_t i = Len; i>0; i--){
			//wait until RXNE becomes 1 (SR1 register)
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RxNE));

			//if last 2 bytes are remaining
			if(i==2){

				//clear the ack bit
				pI2CHandle->pI2Cx->CR1 &= ~(1 << 10); // Clear Bit 10 (ACK)

				//generate stop condition
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			//read data into buffer
			*pTxBuffer = pI2CHandle->pI2Cx->DR;

			//increment the buffer address
			pTxBuffer++;
		}
	}

	//re-enable acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
		pI2CHandle->pI2Cx->CR1 |= (1 << 10);  // Set CBit 10 (ACK)
	}

}

void I2C_Init(I2C_Handle_t *pI2CHandle){

	//1. Set the clock in SM
		//1.0 enable clock
		//1.1 Configure the mode in CCR register (bit 15) (FM(fast) or SM(slow))
		//1.2 If in fast mode, select the duty cycle of Fast mode SCL in CCR register (14th bit)
			//For fat mode you have 2 options 1-> Tlow = 2*Thigh 2-> Tlow = 1.8*Thigh
		//1.3 Program FREQ field of CR2 with the value of PCLK1 (hopefully 16 MHz)
		//1.4 Calculate and Program CCR value in CCR field of CCR register

	uint32_t tempreg = 0;

	//enable clock for i2c peripheral
	I2C_PeriClockControl(pI2CHandle->pI2Cx,ENABLE);

	//enable peripheral
	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);

	//Automatic acking is bit 10 of CR1
	// Enable or Disable ACK in the Control Register 1
	if (pI2CHandle->I2C_Config.I2C_ACKControl == ENABLE) {
	    pI2CHandle->pI2Cx->CR1 |= (1 << 10);  // Set Bit 10 (ACK)
	} else {
	    pI2CHandle->pI2Cx->CR1 &= ~(1 << 10); // Clear Bit 10 (ACK)
	}

	//configure the FREQ field of CR2
	tempreg = 0;
	// Freq field is the APB1 peripheral clock in MHz
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	//write it to control register 2. We only need the first 5 bits
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0b111111);

	//program OAR1 Register
	tempreg = 0;
	// wirte to the ADD[7:1] bitfield of the OAR1 register
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	// this bit has to be kept true by software all the time apparently
	tempreg |= (1 << 14);
	// write it to OAR1 Register
	pI2CHandle->pI2Cx->OAR1 = (tempreg & 0b100000011111110);

	//program I2C_CCR register
	tempreg = 0;
	uint16_t ccr_value = 0;
	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
		// if the reqired speed is lower than Standard mode speed then we need to use standar mode calculation
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		// ccr bitfield is the first 12 bits only
		tempreg |= ccr_value & 0xFFF;
	}else{
		// else it means we are in fast mode
		// set register 15 as this must be set for fast mode
		tempreg |= (1 << 15);
		// for fast mode, the duty cycle can be set as 1 or 0
		// for each duty cycle the formula will be different
		// duty cycle is bit 14
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		else{
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}
		// ccr bitfield is the first 12 bits only
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	//TRISE Configuration
	tempreg = 0;
	//for standard mode
	if (pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM) {
		//uint8_t trise;
		//In SM: tris[0:5] = PCLK1 * (max rise time in SM which is 1000 ns) + 1
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;
	//for fast mode
	}else{
		//In FM: tris[0:5] = PCLK1 * (max rise time in SM which is 300 ns) + 1
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);

	//enable I2C peripheral
	I2C_PeripheralControl(pI2CHandle->pI2Cx, ENABLE);


	//pI2CHandle->pI2Cx->CR1 |= (1 << 10);  // Set Bit 10 (ACK)

	//acking can only be enabled after peripheral is enabled.
	// Enable or Disable ACK in the Control Register 1
	if (pI2CHandle->I2C_Config.I2C_ACKControl == ENABLE) {
		pI2CHandle->pI2Cx->CR1 |= (1 << 10);  // Set Bit 10 (ACK)
	} else {
		pI2CHandle->pI2Cx->CR1 &= ~(1 << 10); // Clear Bit 10 (ACK)
	}
}
