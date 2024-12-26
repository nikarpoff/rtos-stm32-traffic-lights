#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os.h"
#include "LCD_ILI9325.h"

unsigned int blinkTimeDelay = 500;
unsigned int microDelay = 100;

void ConfigureApp();
extern void InitMessageQueue();
extern void StartConsumer();
extern int Init_Input (void);
extern int initFirstTrafficLight();
extern int initSecondTrafficLight();
extern int initThirdTrafficLight();
extern void InitDisplay(uint16_t tlNumber, uint16_t lghtsNumber);

int main(void)
{
	osKernelInitialize();
	
	ConfigureApp();
	InitDisplay(3, 3);
	
	InitMessageQueue();
	StartConsumer();
	
	int result;
	
	result = Init_Input();
	
	if (result != 0 ) 
		return -1;
	
	result = initFirstTrafficLight();
	
	if (result != 0 ) 
		return -1;
	
	result = initSecondTrafficLight();
	
	if (result != 0 ) 
		return -1;
	
	result = initThirdTrafficLight();
	
	if (result != 0 ) 
		return -1;
	
	osKernelStart();
	
	
	while(1) { }
}


void ConfigureApp() {
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
	
	
	// Enable clock for GPIOC
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	// Set PC4 (K1) as input
	GPIOC->MODER &= ~GPIO_MODER_MODER4;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR4;
	
	
	// Set PC6 (K1) as input
	GPIOC->MODER &= ~GPIO_MODER_MODER6;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR6;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER0_Msk;
  GPIOC->MODER |= GPIO_MODER_MODER0_0;

  GPIOC->OTYPER |= GPIO_OTYPER_OT_0;
}
