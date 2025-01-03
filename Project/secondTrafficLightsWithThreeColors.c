#include "cmsis_os.h"  
#include "utility.c"

volatile int secondThreadState;
extern volatile int thirdThreadState;
extern unsigned int blinkTimeDelay;
unsigned int secondTrafficLightsId;
int secondStatesCount;
int secondLightsCount;
TrafficLightState secondThreadStates[4] = {
	{
		{
			{0, RED_COLOR, BLINK_STATE},
			{1, YELLOW_COLOR, DIM_STATE},
			{2, GREEN_COLOR, BRIGHT_STATE} 
		}, 4000
	},
	{
		{
			{0, RED_COLOR, DIM_STATE},
			{1, YELLOW_COLOR, BLINK_STATE},
			{2, GREEN_COLOR, BRIGHT_STATE} 
		}, 1000
	},
	{
		{
			{0, RED_COLOR, DIM_STATE},
			{1, YELLOW_COLOR, BRIGHT_STATE},
			{2, GREEN_COLOR, BRIGHT_STATE} 
		}, 3000
	},
	{
		{
			{0, RED_COLOR, BRIGHT_STATE},
			{1, YELLOW_COLOR, BLINK_STATE},
			{2, GREEN_COLOR, BLINK_STATE} 
		}, 2000
	}
};

void secondTrafficLightThread (void const *argument);              // thread function
osThreadId secondThreadId;                             						// thread id
osThreadDef(secondTrafficLightThread, osPriorityNormal, 1, 32000);       // thread object

osSemaphoreDef(secondSemaphore); 
osSemaphoreId secondSemaphoreId;	

extern osSemaphoreId thirdSemaphoreId;

extern void processLights(Ligth const* lights, int const count, unsigned char threadId, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay);
extern int customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, unsigned int* state);

int initSecondTrafficLight() {
	secondSemaphoreId = osSemaphoreCreate(osSemaphore(secondSemaphore), 1);
	
	if (!secondSemaphoreId) 
		return -1;
	
	secondThreadId = osThreadCreate(osThread(secondTrafficLightThread), NULL);
  
	if (!secondThreadId) 
		return -1;
	
	secondLightsCount = 3;
	secondStatesCount = 4;
	secondThreadState = 0;
	secondTrafficLightsId = 1;
	
	return 0;
}

void secondTrafficLightThread (void const *argument) {
	int stateCopy;
	
	while (1) {
		osSemaphoreWait(secondSemaphoreId, osWaitForever);
	
		stateCopy = secondThreadState;
	
		osSemaphoreRelease(secondSemaphoreId);
		
		switch (stateCopy) {
			case 0: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
			case 1: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
			case 2: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
			case 3: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay);
				
				osSemaphoreWait(thirdSemaphoreId, osWaitForever);
	
				thirdThreadState = 2;
	
				osSemaphoreRelease(thirdSemaphoreId);
				
				break;
			}
		}
			
		osSemaphoreWait(secondSemaphoreId, osWaitForever);
	
		secondThreadState = (secondThreadState + 1) % secondStatesCount;
	
		osSemaphoreRelease(secondSemaphoreId);
	}
}