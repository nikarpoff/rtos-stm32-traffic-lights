#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f3xx.h"                  // Device header
#include "RTE_COMPONENTS.h"
#include <stdio.h>

extern volatile int firstThreadState;
extern volatile int secondThreadState;
extern volatile int thirdThreadState;

extern osSemaphoreId firstSemaphoreId;
extern osSemaphoreId secondSemaphoreId;
extern osSemaphoreId thirdSemaphoreId;


 
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
				// Check is PC4 (k1) pressed
				if ((GPIOC->IDR & GPIO_IDR_4) == 0) {  
						// k1 pressed -> k2 cannot be checked
						osSemaphoreWait(semaphore_input_id, osWaitForever); // wait for ability to check k1 (when k2 is not pressed)
						
						// Debonce delay
						osDelay(50);
						
						if ((GPIOC->IDR & GPIO_IDR_4) != 0) {
								// k2 now available
								osSemaphoreRelease(semaphore_input_id);
								continue;
						}
				
						do {
								// Increase states
								osSemaphoreWait(firstSemaphoreId, osWaitForever);
								firstThreadState++;
								osSemaphoreRelease(firstSemaphoreId);
							
								osSemaphoreWait(secondSemaphoreId, osWaitForever);
								secondThreadState++;
								osSemaphoreRelease(secondSemaphoreId);
							
							
								osSemaphoreWait(thirdSemaphoreId, osWaitForever);
								secondThreadState++;
								osSemaphoreRelease(thirdSemaphoreId);

								// Sleep for 100 ms
								osDelay(100);
						} while ((GPIOC->IDR & GPIO_IDR_4) == 0);
			
						// k1 unpressed						
						// k2 now available
						osSemaphoreRelease(semaphore_input_id);
				}
		
				osDelay(50);
		}
}

/*----------------------------------------------------------------------------
 *      Thread 2 'Input_K2_Thread': Thread to check is there K2 pressed
 *---------------------------------------------------------------------------*/
void Input_K2_Thread (void const *argument) {	
		while (1) {
				// Check is PB7 (k1) pressed
				if ((GPIOC->IDR & GPIO_IDR_6) == 0) {  
						// k2 pressed -> k1 cannot be checked
						osSemaphoreWait(semaphore_input_id, osWaitForever); // wait for ability to check k2 (when k1 is not pressed)
						
						// Debounce delay
						osDelay(50);
						
						if ((GPIOC->IDR & GPIO_IDR_6) != 0) {
								// k2 now available
								osSemaphoreRelease(semaphore_input_id);
								continue;
						}
				
						do {
								// Set states to initial
								osSemaphoreWait(firstSemaphoreId, osWaitForever);
								firstThreadState = 0;
								osSemaphoreRelease(firstSemaphoreId);
							
								osSemaphoreWait(secondSemaphoreId, osWaitForever);
								secondThreadState = 0;
								osSemaphoreRelease(secondSemaphoreId);
							
							
								osSemaphoreWait(thirdSemaphoreId, osWaitForever);
								thirdThreadState = 0;
								osSemaphoreRelease(thirdSemaphoreId);
							
								// Sleep for 100 ms
								osDelay(100);
						} while ((GPIOC->IDR & GPIO_IDR_6) == 0);
			
						// k2 unpressed
						// k1 now available
						osSemaphoreRelease(semaphore_input_id);
			}
		
			osDelay(50);
  }
}
