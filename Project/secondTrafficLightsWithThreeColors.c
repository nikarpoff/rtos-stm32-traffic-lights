#include "cmsis_os.h"  
#include "utility.c"

volatile int secondThreadState;
extern volatile int thirdThreadState;
extern unsigned int blinkTimeDelay;
extern unsigned int microDelay;
unsigned int secondTrafficLightsId;
unsigned int secondStatesCount;
unsigned int secondLightsCount;
TrafficLightState secondThreadStates[3] = {
	{
		{
			{"RB"},
			{"YD"},
			{"GD"}
		}, 1000
	},
	{
		{
			{"RD"},
			{"YB"},
			{"GD"}
		}, 1500
	},
	{
		{
			{"RD"},
			{"YD"},
			{"GB"}
		}, 2500
	},
};

void secondTrafficLightThread (void const *argument);              // thread function
osThreadId secondThreadId;                             						// thread id
osThreadDef(secondTrafficLightThread, osPriorityNormal, 1, 0);       // thread object

osSemaphoreDef(secondSemaphore); 
osSemaphoreId secondSemaphoreId;	

extern osSemaphoreId thirdSemaphoreId;

extern void processLights(Ligth const* lights, unsigned int const count, unsigned char threadId/*, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay*/);
extern int customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, volatile int* state);

int initSecondTrafficLight() {
	secondSemaphoreId = osSemaphoreCreate(osSemaphore(secondSemaphore), 1);
	
	if (!secondSemaphoreId) 
		return -1;
	
	secondThreadId = osThreadCreate(osThread(secondTrafficLightThread), NULL);
  
	if (!secondThreadId) 
		return -1;
	
	secondLightsCount = 3;
	secondStatesCount = 3;
	secondThreadState = 0;
	secondTrafficLightsId = 2;
	
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
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId/*, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay*/);
				break;
			}
			case 1: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId/*, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay*/);
				break;
			}
			case 2: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId/*, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay*/);
				
				osSemaphoreWait(thirdSemaphoreId, osWaitForever);
	
				//thirdThreadState = 0;
	
				osSemaphoreRelease(thirdSemaphoreId);
				
				break;
			}
			case 3: {
				processLights(secondThreadStates[stateCopy].lights, secondLightsCount, secondTrafficLightsId/*, &secondThreadState, stateCopy, secondThreadStates[stateCopy].delay, blinkTimeDelay*/);
				
				break;
			}
		}
		
		int result = customDelay(microDelay, secondThreadStates[stateCopy].delay, stateCopy, &secondThreadState);
		
		if (result != 0) continue;
			
		osSemaphoreWait(secondSemaphoreId, osWaitForever);
	
		secondThreadState = (secondThreadState + 1) % secondStatesCount;
	
		osSemaphoreRelease(secondSemaphoreId);
	}
}