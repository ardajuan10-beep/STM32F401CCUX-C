/*
 * stm32F401ccux_spi_driver.c
 *
 *  Created on: Sep 4, 2026
 *      Author: ardaj
 */


#include "stm32F401ccux_spi_driver.h"

//Function initialisations
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

//Peripheral Clock Setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		if (pSPIx == SPI1){
			SPI1_PCLK_EN();
		}else if (pSPIx == SPI2_I2S2){
			SPI2_PCLK_EN();
		}else if (pSPIx == SPI3_I2S3){
			SPI3_PCLK_EN();
		}else if (pSPIx == SPI4){
			SPI4_PCLK_EN();
		}
	}else{
		if (pSPIx == SPI1){
			SPI1_PCLK_DI();
		}else if (pSPIx == SPI2_I2S2){
			SPI2_PCLK_DI();
		}else if (pSPIx == SPI3_I2S3){
			SPI3_PCLK_DI();
		}else if (pSPIx == SPI4){
			SPI4_PCLK_DI();
		}
	}

}

// Init
void SPI_Init(SPI_Handle_t *pSPIHandle){
	//enable clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	// initialise the prepared register as 0
	uint32_t prep_reg = 0;

	// if it is master mode set CR1 reg bit 2
	if(pSPIHandle->SPI_Config.SPI_DeviceMode == SPI_DeviceMode_Master){
		prep_reg |= (1 << SPI_CR1_MSTR);
	}

	// if it is half duplex mode set CR1 reg bit 15
	if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BusConfig_HD){
		prep_reg |= (1 << SPI_CR1_BIDI_MODE);
	}

	//bit 14 is for transmit and recive for half duplex. I guess we will change it during send and receive operations...

	// if it is simplex receive only set CR1 reg bit 10
	if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BusConfig_S_RX){
		prep_reg |= (1 << SPI_CR1_RX_ONLY);
	}

	// write to bits 3..5 of CR1 for SPI speed
	prep_reg |= (pSPIHandle->SPI_Config.SPI_SclkSpeed << SPI_CR1_BR);

	// if it is 16 bit data format set CR1 reg bit 11
	if(pSPIHandle->SPI_Config.SPI_DFF == SPI_DFF_16bits){
		prep_reg |= (1 << SPI_CR1_DFF);
	}

	// if CPHA is high set CR1 reg bit 0
	if(pSPIHandle->SPI_Config.SPI_CPHA == SPI_CPHA_High){
		prep_reg |= (1 << SPI_CR1_CPHA);
	}

	// if CPOL is high set CR1 reg bit 1
	if(pSPIHandle->SPI_Config.SPI_CPOL == SPI_CPOL_High){
		prep_reg |= (1 << SPI_CR1_CPOL);
	}

	// if SSM is enabled, set CR1 reg bit 9
	if(pSPIHandle->SPI_Config.SPI_SSM == SPI_SSM_EN){
		prep_reg |= (1 << SPI_CR1_SSM);
	}

	// read control register
	uint32_t CR1_read = pSPIHandle->pSPIx->CR1;

	// make bits 2, 15, 10, 3..5, 11, 0, 1, and 9 0es -> make bits 0..5, 9..11, 15 0es
	CR1_read &= 0x71C0;

	// write to the CR1 without affecting the other bits
	pSPIHandle->pSPIx->CR1 = CR1_read | prep_reg;
}

// send data (blocking)
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
	// while there is data to send (length of data is larger than 0
	while (Len > 0){
		// while tx buffer is full. (wait until tx buffer is empty.)
		while(!(pSPIx->SR & (1 << SPI_SR_TXE)));

		//if data frame format is 8. (if the DFF bit of CR1 is 0)
		if(!(pSPIx->CR1 & (1 << SPI_CR1_DFF))){
			// write the next element to the buffer
			pSPIx->DR = *pTxBuffer;

			//increment input data pointer
			pTxBuffer++;

			//decrement Len
			Len--;

		//if data frame format is 16, and if length is at least 2
		}else if(Len >= 2){
			// write the next 2 elements to the DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);//*pTxBuffer + (*(pTxBuffer + 1) << 8);

			//increment input data pointer by 2 bytes as the dff is 16 bits
			(uint16_t*)pTxBuffer++;

			//decrement Len by 2 as the dff is 16 bits
			Len = Len - 2;

		// if it comes here, than it means the length is less than 2 and dff is 16 bits.
		// then we have to terminate writing
		}else{
			return;
		}
	}
}


