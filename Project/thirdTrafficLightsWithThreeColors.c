#include "cmsis_os.h"  
#include "utility.c"

volatile int thirdThreadState;
extern volatile int firstThreadState;
extern unsigned int blinkTimeDelay;
extern unsigned int microDelay;
unsigned int thirdTrafficLightsId;
unsigned int thirdStatesCount;
unsigned int thirdLightsCount;
TrafficLightState thirdThreadStates[3] = {
	{
		{
			{"RB"},
			{"YD"},
			{"GD"}
		}, 2000
	},
	{
		{
			{"RD"},
			{"YB"},
			{"GD"}
		}, 1000
	},
	{
		{
			{"RD"},
			{"YD"},
			{"GB"}
		}, 500
	},
};

void thirdTrafficLightThread (void const *argument);              // thread function
osThreadId thirdThreadId;                             						// thread id
osThreadDef(thirdTrafficLightThread, osPriorityNormal, 1, 0);       // thread object

osSemaphoreDef(thirdSemaphore); 
osSemaphoreId thirdSemaphoreId;	

extern osSemaphoreId firstSemaphoreId;

extern void processLights(Ligth const* lights, unsigned int const count, unsigned char threadId/*, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay*/);
extern int customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, volatile int* state);

int initThirdTrafficLight() {
	thirdSemaphoreId = osSemaphoreCreate(osSemaphore(thirdSemaphore), 1);
	
	if (!thirdSemaphoreId) 
		return -1;
	
	thirdThreadId = osThreadCreate(osThread(thirdTrafficLightThread), NULL);
  
	if (!thirdThreadId) 
		return -1;
	
	thirdLightsCount = 3;
	thirdStatesCount = 3;
	thirdThreadState = 0;
	thirdTrafficLightsId = 3;
	
	return 0;
}

void thirdTrafficLightThread (void const *argument) {
	int stateCopy;
	
	while (1) {
		osSemaphoreWait(thirdSemaphoreId, osWaitForever);
	
		stateCopy = thirdThreadState;
	
		osSemaphoreRelease(thirdSemaphoreId);
		
		switch (stateCopy) {
			case 0: {
				processLights(thirdThreadStates[stateCopy].lights, thirdLightsCount, thirdTrafficLightsId/*, &thirdThreadState, stateCopy, thirdThreadStates[stateCopy].delay, blinkTimeDelay*/);
				break;
			}
			case 1: {
				processLights(thirdThreadStates[stateCopy].lights, thirdLightsCount, thirdTrafficLightsId/*, &thirdThreadState, stateCopy, thirdThreadStates[stateCopy].delay, blinkTimeDelay*/);
				break;
			}
			case 2: {
				processLights(thirdThreadStates[stateCopy].lights, thirdLightsCount, thirdTrafficLightsId/*, &thirdThreadState, stateCopy, thirdThreadStates[stateCopy].delay, blinkTimeDelay*/);
				
				osSemaphoreWait(firstSemaphoreId, osWaitForever);
	
				//firstThreadState = 0;
	
				osSemaphoreRelease(firstSemaphoreId);
				
				break;
			}
		}
		
		int result = customDelay(microDelay, thirdThreadStates[stateCopy].delay, stateCopy, &thirdThreadState);
		
		if (result != 0) continue;
			
		osSemaphoreWait(thirdSemaphoreId, osWaitForever);
	
		thirdThreadState = (thirdThreadState + 1) % thirdStatesCount;
	
		osSemaphoreRelease(thirdSemaphoreId);
	}
}