#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f3xx.h"                  // Device header
#include "RTE_COMPONENTS.h"
#include <stdio.h>

 
void Input_K1_Thread (void const *argument);                // thread function
osThreadId tid_Input_K1_Thread;                             // thread id
osThreadDef(Input_K1_Thread, osPriorityNormal, 1, 0);       // thread object

void Input_K2_Thread (void const *argument);                // thread function
osThreadId tid_Input_K2_Thread;                             // thread id
osThreadDef(Input_K2_Thread, osPriorityNormal, 1, 0);       // thread object

osSemaphoreDef(semaphore_input);
osSemaphoreId semaphore_input_id;														// behavior k1/k2 sem


int Init_Input (void) {
		semaphore_input_id = osSemaphoreCreate(osSemaphore(semaphore_input), 1);
		if (!semaphore_input_id) return(-1);
	
		tid_Input_K1_Thread = osThreadCreate(osThread(Input_K1_Thread), NULL);
		if (!tid_Input_K1_Thread) return(-1);
	
		tid_Input_K2_Thread = osThreadCreate(osThread(Input_K2_Thread), NULL);
		if (!tid_Input_K2_Thread) return(-1);
  
		return(0);
}


/*----------------------------------------------------------------------------
 *      Thread 1 'Input_K1_Thread': Thread to check is there K1 pressed
 *---------------------------------------------------------------------------*/
void Input_K1_Thread (void const *argument) {	
		while (1) {
				// Check is PB7 (k1) pressed
				if (!(GPIOB->IDR & (1 << 7))) {  
						// k1 pressed -> k2 cannot be checked
						osSemaphoreWait(semaphore_input_id, osWaitForever); // wait for ability to check k1 (when k2 is not pressed)
						
						// DO SMTH
				
						do {
								// Sleep for 100 ms
								osDelay(100);
						} while (!(GPIOB->IDR & (1 << 7)));
			
						// k1 unpressed
						
						// DO SMTH
						
						// k2 now available
						osSemaphoreRelease(semaphore_input_id);
				}
		
				osDelay(20);
		}
}

/*----------------------------------------------------------------------------
 *      Thread 2 'Input_K2_Thread': Thread to check is there K2 pressed
 *---------------------------------------------------------------------------*/
void Input_K2_Thread (void const *argument) {	
		while (1) {
				// Check is PB7 (k1) pressed
				if (!(GPIOB->IDR & (1 << 7))) {  
						// k2 pressed -> k1 cannot be checked
						osSemaphoreWait(semaphore_input_id, osWaitForever); // wait for ability to check k2 (when k1 is not pressed)
						
						// DO SMTH
				
						do {
								// Sleep for 100 ms
								osDelay(100);
						} while (!(GPIOB->IDR & (1 << 7)));
			
						// k2 unpressed
						
						// DO SMTH
						
						// k1 now available
						osSemaphoreRelease(semaphore_input_id);
			}
		
			osDelay(20);
  }
}


/**
uint16_t scankeypad(void) {
		uint16_t key_code=0;
		uint8_t tmpbuf=0;
		int i;
		for(i=0; i<4; i++) 
		{
				LL_GPIO_ResetOutputPin(GPIOC,1<<(4+i));
				__NOP(); __NOP(); __NOP(); __NOP();
				tmpbuf= (uint8_t)LL_GPIO_ReadInputPort(GPIOC);
				tmpbuf= ~tmpbuf & 0x0F;
				key_code |=tmpbuf<<(4*i);
				LL_GPIO_SetOutputPin(GPIOC,1<<(4+i));
		}
		return key_code;
}
*/

/**
void InitKpd4(void) 
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);//GPIOC
	//PC0,PC1,PC2,PC3 In
	LL_GPIO_InitTypeDef gpio_initstruct;
	gpio_initstruct.Pin        = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3;
	gpio_initstruct.Mode       = LL_GPIO_MODE_INPUT;
	gpio_initstruct.Pull       = LL_GPIO_PULL_UP;
	if (LL_GPIO_Init(GPIOC, &gpio_initstruct) != SUCCESS)
		while (1){} 
	
	//PC4,PC5,PC6,PC7 Out
	gpio_initstruct.Pin        = LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
	gpio_initstruct.Mode       = LL_GPIO_MODE_OUTPUT;
	gpio_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_initstruct.Pull       = LL_GPIO_PULL_UP;
	if (LL_GPIO_Init(GPIOC, &gpio_initstruct) != SUCCESS)
		while (1){} 
	LL_GPIO_SetOutputPin (GPIOC, LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7);
}
*/