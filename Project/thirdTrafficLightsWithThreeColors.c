#include "cmsis_os.h"  
#include "utility.c"

volatile int thirdThreadState;
extern volatile int firstThreadState;
extern unsigned int blinkTimeDelay;
unsigned int thirdTrafficLightsId;
int thirdStatesCount;
int thirdLightsCount;
TrafficLightState thirdThreadStates[3] = {
	{
		{
			{0, RED_COLOR, DIM_STATE},
			{1, YELLOW_COLOR, DIM_STATE},
			{2, GREEN_COLOR, DIM_STATE} 
		}, 4000
	},
	{
		{
			{0, RED_COLOR, BRIGHT_STATE},
			{1, YELLOW_COLOR, BRIGHT_STATE},
			{2, GREEN_COLOR, BRIGHT_STATE} 
		}, 1000
	},
	{
		{
			{0, RED_COLOR, BLINK_STATE},
			{1, YELLOW_COLOR, BLINK_STATE},
			{2, GREEN_COLOR, BLINK_STATE} 
		}, 3000
	}
};

void thirdTrafficLightThread (void const *argument);              // thread function
osThreadId thirdThreadId;                             						// thread id
osThreadDef(thirdTrafficLightThread, osPriorityNormal, 1, 32000);       // thread object

osSemaphoreDef(thirdSemaphore); 
osSemaphoreId thirdSemaphoreId;	

extern osSemaphoreId firstSemaphoreId;

extern void processLights(Ligth const* lights, int const count, unsigned char threadId, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay);
extern int customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, unsigned int* state);

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
	thirdTrafficLightsId = 2;
	
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
				processLights(thirdThreadStates[stateCopy].lights, thirdLightsCount, thirdTrafficLightsId, &thirdThreadState, stateCopy, thirdThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
			case 1: {
				processLights(thirdThreadStates[stateCopy].lights, thirdLightsCount, thirdTrafficLightsId, &thirdThreadState, stateCopy, thirdThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
			case 2: {
				processLights(thirdThreadStates[stateCopy].lights, thirdLightsCount, thirdTrafficLightsId, &thirdThreadState, stateCopy, thirdThreadStates[stateCopy].delay, blinkTimeDelay);
				
				osSemaphoreWait(firstSemaphoreId, osWaitForever);
	
				firstThreadState = 2;
	
				osSemaphoreRelease(firstSemaphoreId);
				
				break;
			}
		}
			
		osSemaphoreWait(thirdSemaphoreId, osWaitForever);
	
		thirdThreadState = (thirdThreadState + 1) % thirdStatesCount;
	
		osSemaphoreRelease(thirdSemaphoreId);
	}
}