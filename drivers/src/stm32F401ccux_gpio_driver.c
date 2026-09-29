/*
 * stm32F401ccux_gpio_driver.c
 *
 *  Created on: Feb 16, 2026
 *      Author: ardaj
 */


#include "stm32F401ccux_gpio_driver.h"

// peripheral clock setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
	if (EnorDi == ENABLE){
		if (pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}else if (pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		}else if (pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		}else if (pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		}else if (pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		}else if (pGPIOx == GPIOH){
			GPIOH_PCLK_EN();
		}
	}else{
		if (pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}else if (pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		}else if (pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		}else if (pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		}else if (pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		}else if (pGPIOx == GPIOH){
			GPIOH_PCLK_DI();
		}
	}
}

// init and deinit
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){

	// declare temp reg var and bit mask variable
	uint32_t temp_reg;
	uint32_t mask;

	//enable peri clocks for the chosen port (A, B, C, D...)
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//1. configure the mode of gpio pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOGUE){ //normal modes
		// read the mode register into temp register
		temp_reg = pGPIOHandle->pGPIOx->MODER;
		// create the bit mask. Depending on the pin number make 2 consecutive bits 0, rest as 1s
		mask = ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		// make the consecutive 2 bits of the temp_reg as 0. untouch other bits
		temp_reg &= mask;
		// write the mode to the mode register depending on pin number
		temp_reg = temp_reg | (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER = temp_reg;

	}else{ //interrupt mode

	}

	//2. configure the speed
	// read the speed register into temp register
	temp_reg = pGPIOHandle->pGPIOx->OSPEEDR;
	// create the bit mask. Depending on the pin number make 2 consecutive bits 0, rest as 1s
	mask = ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	// make the consecutive 2 bits of the temp_reg as 0. untouch other bits
	temp_reg &= mask;
	// write the speed to the speed register depending on pin number
	pGPIOHandle->pGPIOx->OSPEEDR = temp_reg | (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	//3. configure the pupd settings
	// read the pull up register into temp register
	temp_reg = pGPIOHandle->pGPIOx->PUPDR;
	// create the bit mask. Depending on the pin number make 2 consecutive bits 0, rest as 1s
	mask = ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	// make the consecutive 2 bits of the temp_reg as 0. untouch other bits
	temp_reg &= mask;
	// write the pull up config to the speed register depending on pin number
	pGPIOHandle->pGPIOx->PUPDR = temp_reg | (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	//4. configure the optype
	// read the op type register into temp register
	temp_reg = pGPIOHandle->pGPIOx->OTYPER;
	// create the bit mask. Depending on the pin number make 1 bit 0, rest as 1s
	mask = ~(0x1 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	// make 1 bit of the temp_reg as 0. untouch other bits
	temp_reg &= mask;
	// write the op type config to the op type register depending on pin number
	pGPIOHandle->pGPIOx->OTYPER = temp_reg | (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	//5. configure the alt functionality
	// if the mode is selected as alternate functionality mode
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		// declare temp reg var and bit mask variable as arrays of size 2
		uint32_t temp_reg_arr[2];
		uint32_t mask_arr[2];
		// read the alt functionality register into temp register
		temp_reg_arr[0] = pGPIOHandle->pGPIOx->AFR[0];
		temp_reg_arr[1] = pGPIOHandle->pGPIOx->AFR[1];
		// create the bit mask. Depending on the pin number make 4 consecutive bit 0, rest as 1s
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber < 8){
			mask_arr[0] = ~(0xF << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 4));
			mask_arr[1] = 0xFFFFFFFF;
		}else{
			mask_arr[0] = 0xFFFFFFFF;
			mask_arr[1] = ~(0xF << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8) * 4));
		}
		// make 4 bit of the temp_reg as 0. untouch other bits
		temp_reg_arr[0] &= mask_arr[0];
		temp_reg_arr[1] &= mask_arr[1];

		// write the alt fun config to the alt fun register depending on pin number
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber < 8){
			pGPIOHandle->pGPIOx->AFR[0] = temp_reg_arr[0] | (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 4));
		}else{
			pGPIOHandle->pGPIOx->AFR[1] = temp_reg_arr[1] | (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << ((pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8) * 4));
		}
	}

}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){

	if (pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}else if (pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}else if (pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	}else if (pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	}else if (pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	}else if (pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}

}

// read write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){

}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){

}
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value){

}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value){

}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){

}

//irq configuration and ISR handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi){

}
void GPIO_IRQHandling(uint8_t PinNumber){

}
