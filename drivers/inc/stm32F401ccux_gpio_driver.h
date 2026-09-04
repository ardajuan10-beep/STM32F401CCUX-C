/*
 * stm32F401ccux_gpio_driver.h
 *
 *  Created on: Feb 16, 2026
 *      Author: ardaj
 */

#ifndef INC_STM32F401CCUX_GPIO_DRIVER_H_
#define INC_STM32F401CCUX_GPIO_DRIVER_H_

#include "stm32F401ccux.h"

// Configuration structure for GPIOx peripheral
typedef struct
{
	uint8_t				GPIO_PinNumber; 			// which pin 0-15
	uint8_t				GPIO_PinMode;				// possible values from @GPIO MODES
	uint8_t				GPIO_PinSpeed;				// possible values from @GPIO Output type
	uint8_t				GPIO_PinPuPdControl;		// possible values from @GPIO Pin Pull Up Down
	uint8_t				GPIO_PinOPType;				// possible values from @GPIO Output Speed
	uint8_t				GPIO_PinAltFunMode;			// possible values from @GPIO Output Speed
}GPIO_PinConfig_t;

//Handle Structure for GPIOx peripheral
typedef struct
{
	GPIO_RegDef_t		*pGPIOx;			//holds the base address of the GPIO port to which the pin belong
	GPIO_PinConfig_t	GPIO_PinConfig;		//this holds the gpio pin setting configuration
}GPIO_Handle_t;

// GPIO PIN NUMBERS
// @GPIO Pin Numbers
#define GPIO_PIN_NO_0			0
#define GPIO_PIN_NO_1			1
#define GPIO_PIN_NO_2			2
#define GPIO_PIN_NO_3			3
#define GPIO_PIN_NO_4			4
#define GPIO_PIN_NO_5			5
#define GPIO_PIN_NO_6			6
#define GPIO_PIN_NO_7			7
#define GPIO_PIN_NO_8			8
#define GPIO_PIN_NO_9			9
#define GPIO_PIN_NO_10			10
#define GPIO_PIN_NO_11			11
#define GPIO_PIN_NO_12			12
#define GPIO_PIN_NO_13			13
#define GPIO_PIN_NO_14			14
#define GPIO_PIN_NO_15			15

// GPIO MODES
// @GPIO MODES
#define GPIO_MODE_IN			0
#define GPIO_MODE_OUT			1
#define GPIO_MODE_ALTFN			2
#define GPIO_MODE_ANALOGUE		3
#define GPIO_MODE_IT_FT			4		//custom: falling edge trigger
#define GPIO_MODE_IT_RT 		5		//custom: rising edge trigger
#define GPIO_MODE_IT_RFT 		6		//custom: rising and falling edge trigger

// GPIO pin output types
// @GPIO Output type
#define GPIO_OP_TYPE_PP			0		//Push Pull
#define GPIO_OP_TYPE_OD			1		//Open Drain

// GPIO pin output speeds
// @GPIO Output Speed
#define GPIO_SPEED_LOW			0		//Low speed
#define GPIO_SPEED_MEDIUM		1		//medium speed
#define GPIO_SPEED_FAST			2		//high speed
#define GPIO_SPEED_HIGH			3		//very high speed

// GPIO pin pull up pull down
// @GPIO Pin Pull Up Down
#define GPIO_NO_PUPD			0		//no pull up or pull down
#define GPIO_PIN_PU				1		//pull up
#define GPIO_PIN_PD				2		//pull down

// peripheral clock setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

// init and deinit
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

// read write
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

//irq configuration and ISR handling
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F401CCUX_GPIO_DRIVER_H_ */
