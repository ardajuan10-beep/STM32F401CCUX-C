/*
 * stm32F401ccux.h
 *
 *  Created on: Dec 20, 2025
 *      Author: ardaj
 */

#ifndef STM32F401CCUX_H_
#define STM32F401CCUX_H_

//library that contains shorthand notations for the data  types uint32_t and many more
#include <stdint.h>

#define FLAG_SET 					1
#define FLAG_RESET 					0

/*///////////////////////////////////////////////////////////////////
// General BASE ADDRESSES
/////////////////////////////////////////////////////////////////////*/
#define FLASH_BASEADDR				0x08000000U
#define SRAM1_BASEADDR				0x20000000U //64KB
#define ROM_BASEADDR				0x1FFF0000U //30KB
/*/////////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////////////////////////////////
// PERIPHERAL BUS BASE ADDRESSES
/////////////////////////////////////////////////////////////////////*/
#define APB1PERIPH_BASEADDR			0x40000000U
#define APB2PERIPH_BASEADDR			0x40010000U
#define AHB1PERIPH_BASEADDR			0x40020000U
#define AHB2PERIPH_BASEADDR			0x50000000U
/*/////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////
// PERIPHERAL BASE ADDRESSES
/////////////////////////////////////////////////////////////////////*/
//AHB1 BUS
#define GPIOA_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1000U)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1C00U)
#define RCC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x3800)

// APB1 Peripheral base addresses
#define SPI2_I2S2_BASEADDR			(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_I2S3_BASEADDR			(APB1PERIPH_BASEADDR + 0x3C00)
#define USART2_BASEADDR				(APB1PERIPH_BASEADDR + 0x4400)
#define I2C1_BASEADDR				(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR				(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR				(APB1PERIPH_BASEADDR + 0x5C00)

// APB2 Base Address
#define USART1_BASEADDR				(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR				(APB2PERIPH_BASEADDR + 0x1400)
#define SPI1_BASEADDR				(APB2PERIPH_BASEADDR + 0x3000)
#define SPI4_BASEADDR				(APB2PERIPH_BASEADDR + 0x3400)
#define SYSCFG_BASEADDR				(APB2PERIPH_BASEADDR + 0x3800)
#define EXTI_BASEADDR				(APB2PERIPH_BASEADDR + 0x3C00)
/*/////////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////////////////////////////////
// Structural register descriptions for peripherals
/////////////////////////////////////////////////////////////////////*/
// Structural register description for GPIO
typedef struct
{
	volatile uint32_t 				MODER;
	volatile uint32_t 				OTYPER;
	volatile uint32_t 				OSPEEDR;
	volatile uint32_t 				PUPDR;
	volatile uint32_t 				IDR;
	volatile uint32_t 				ODR;
	volatile uint32_t 				BSRR;
	volatile uint32_t 				LCKR;
	volatile uint32_t 				AFR[2];
}GPIO_RegDef_t;

// Structural register description for RCC
typedef struct
{
	volatile uint32_t 				CR;			// 0x40023800
	volatile uint32_t 				PLLCFGR;	// 0x40023804
	volatile uint32_t 				CFGR;		// 0x40023808
	volatile uint32_t 				CIR;		// 0x4002380C
	volatile uint32_t 				AHB1RSTR;	// 0x40023810
	volatile uint32_t 				AHB2RSTR;	// 0x40023814
	volatile uint32_t 				dummy_18;	// 0x40023818
	volatile uint32_t 				dummy_1C;	// 0x4002381C
	volatile uint32_t 				APB1STR;	// 0x40023820
	volatile uint32_t 				APB2STR;	// 0x40023824
	volatile uint32_t 				dummy_28;	// 0x40023828
	volatile uint32_t 				dummy_2C;	// 0x4002382C
	volatile uint32_t 				AHB1ENR;	// 0x40023830
	volatile uint32_t 				AHB2ENR;	// 0x40023834
	volatile uint32_t 				dummy_38;	// 0x40023838
	volatile uint32_t 				dummy_3C;	// 0x4002383C
	volatile uint32_t 				APB1ENR;	// 0x40023840
	volatile uint32_t 				APB2ENR;	// 0x40023844
}RCC_RegDef_t;

// Structural register description of I2C
typedef struct
{
	volatile uint32_t 				CR1;
	volatile uint32_t 				CR2;
	volatile uint32_t 				OAR1;
	volatile uint32_t 				OAR2;
	volatile uint32_t 				DR;
	volatile uint32_t 				SR1;
	volatile uint32_t 				SR2;
	volatile uint32_t 				CCR;
	volatile uint32_t 				TRISE;
	volatile uint32_t 				FLTR;
}I2C_RegDef_t;
/*/////////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////////////////////////////////
// Register Description Pointers equal to Peripheral Base ADR
/////////////////////////////////////////////////////////////////////*/
// GPIO Base address pointers to the I2C struct
#define GPIOA						((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB						((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC						((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD						((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE						((GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOH						((GPIO_RegDef_t*) GPIOH_BASEADDR)

// I2C Base address pointers to the I2C struct
#define I2C1						((I2C_RegDef_t*) I2C1_BASEADDR)
#define I2C2						((I2C_RegDef_t*) I2C2_BASEADDR)
#define I2C3						((I2C_RegDef_t*) I2C3_BASEADDR)

// RTC Base address pointers to the RCC struct
#define RCC							((RCC_RegDef_t*) RCC_BASEADDR)
/*/////////////////////////////////////////////////////////////////////*/


/*///////////////////////////////////////////////////////////////////
// CLOCK ENABLES
/////////////////////////////////////////////////////////////////////*/
// Clock Enable Macros for GPIOx peripherals
// PCLK is peripheral clock
#define GPIOA_PCLK_EN()				(RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN()				(RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()				(RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()				(RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()				(RCC->AHB1ENR |= (1<<4))
#define GPIOH_PCLK_EN()				(RCC->AHB1ENR |= (1<<7))

//Clock Enables for I2C
#define I2C1_PCLK_EN()				(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()				(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()				(RCC->APB1ENR |= (1 << 23))

//Clock Enable for SPI
#define SPI1_PCLK_EN()				(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()				(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()				(RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()				(RCC->APB2ENR |= (1 << 13))

//Clock Enable for USART
#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()			(RCC->APB1ENR |= (1 << 17))
#define USART6_PCLK_EN()			(RCC->APB2ENR |= (1 << 5))

//Clock Enable for SYSCFG Peripheral
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1 << 14))

/*/////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////
// CLOCK DISABLES
/////////////////////////////////////////////////////////////////////*/
#define GPIOA_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOH_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 7))

//Clock Enables for I2C
#define I2C1_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 23))

//Clock Enable for SPI
#define SPI1_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 13))

//Clock Enable for USART
#define USART1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 17))
#define USART6_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 5))

//Clock Enable for SYSCFG Peripheral
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 14))

/*/////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////
//I2C REGISTER BITS
/////////////////////////////////////////////////////////////////////*/
#define I2C_CR1_PE					0
#define I2C_CR1_NOSTRETCH			7
#define I2C_CR1_START				8
#define I2C_CR1_STOP				9
#define I2C_CR1_ACK					10
#define I2C_CR1_SWRST				15

#define I2C_CR2_LAST				12
#define I2C_CR2_DMAEN				11
#define I2C_CR2_ITBUFEN				10
#define I2C_CR2_ITEVTEN				9
#define I2C_CR2_ITERREN				8
#define I2C_CR2_FREQ				0, 1, 2, 3, 4, 5

#define I2C_SR1_SMBALERT			15
#define I2C_SR1_TIMEOUT				14
#define I2C_SR1_PECERR				12
#define I2C_SR1_OVR					11
#define I2C_SR1_AF					10
#define I2C_SR1_ARLO				9
#define I2C_SR1_BERR				8
#define I2C_SR1_TxE					7
#define I2C_SR1_RxNE				6
#define I2C_SR1_STOPF				4
#define I2C_SR1_ADD10				3
#define I2C_SR1_BTF					2
#define I2C_SR1_ADDR				1
#define I2C_SR1_SB					0

#define I2C_SR2_PEC					8, 9, 10, 11, 12, 13, 14, 15
#define I2C_SR2_DUALF				7
#define I2C_SR2_SMBHOST				6
#define I2C_SR2_SMBDEFAULT			5
#define I2C_SR2_GENCALL				4
#define I2C_SR2_TRA					2
#define I2C_SR2_BUSY				1
#define I2C_SR2_MSL					0

#define I2C_CCR_F_S					15
#define I2C_CCR_DUTY				14
#define I2C_CCR_CCR					0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
/*/////////////////////////////////////////////////////////////////////*/

/*///////////////////////////////////////////////////////////////////
//Generic Macros
/////////////////////////////////////////////////////////////////////*/
#define ENABLE 						1
#define DISABLE 					0
#define SET							ENABLE
#define RESET						DISABLE
#define GPIO_PIN_SET				SET
#define GPIO_PIN_RESET				RESET



#include "stm32F401ccux_i2c_driver.h"
#include "stm32F401ccux_gpio_driver.h"



#endif /* STM32F401CCUX_H_ */
