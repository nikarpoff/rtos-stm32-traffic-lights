#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os.h"    

void delay(volatile uint32_t count){
	while(count--)
	__NOP();
}

int main(void)
{
	osKernelInitialize(); /* initialize CMSIS-RTOS          */
	
	volatile uint32_t StartUpCounter = 0, HSEStatus = 0;
	SET_BIT(RCC -> CR,RCC_CR_HSEON);
	
	do {
		HSEStatus = RCC->CR & RCC_CR_HSERDY;
		StartUpCounter++;
	} while((HSEStatus == 0) && (StartUpCounter != 0x5000));
	
	if ((RCC->CR & RCC_CR_HSERDY) != RESET) 
	{
		FLASH->ACR = FLASH_ACR_LATENCY_1;
		
		CLEAR_BIT(RCC -> CR,RCC_CR_PLLON);

		RCC->CFGR2 |= (uint32_t)RCC_CFGR2_PREDIV_DIV4;
		RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_PLLMUL2);
		SET_BIT(RCC -> CR,RCC_CR_PLLON); // ???????? PLL

		while((RCC->CR & RCC_CR_PLLRDY) == 0){}

		RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

		while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL){}
	}
	else
	{ 
		while(1){} 
	}
	
	SystemCoreClockUpdate();

	SET_BIT(RCC -> CFGR, RCC_CFGR_MCO_PLL);
	SET_BIT(RCC -> AHBENR,RCC_AHBENR_GPIOAEN);

	SET_BIT(GPIOA -> MODER,GPIO_MODER_MODER8_1);

	CLEAR_BIT(GPIOA -> AFR[1], GPIO_AFRH_AFRH0_Msk);

	SET_BIT(RCC->AHBENR,RCC_AHBENR_GPIOEEN);

	SET_BIT(GPIOE->MODER,GPIO_MODER_MODER11_0|GPIO_MODER_MODER13_0);

	SET_BIT(GPIOE->OTYPER,GPIO_OTYPER_OT_13);

	SET_BIT(GPIOE->PUPDR, GPIO_PUPDR_PUPDR13_0);
	
	osKernelStart(); /* start thread execution         */
	
	InitDisplay();
	
	
	while(1){
		
		GPIOE->BSRR = GPIO_ODR_11|GPIO_ODR_13;
		
		osDelay(50);
		
		ProcessDisplay();
		
		//delay(10);
		GPIOE->BRR = GPIO_ODR_11|GPIO_ODR_13;
		//delay(10);
		
		osDelay(50);
	}
}
