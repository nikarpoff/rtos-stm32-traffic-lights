#include "cmsis_os.h"  
#include "utility.c"

volatile int firstThreadState;
extern volatile int secondThreadState;
extern unsigned int blinkTimeDelay;
extern unsigned int microDelay;
unsigned int firstTrafficLightsId;
int firstStatesCount;
int firstLightsCount;
TrafficLightState firstThreadStates[3] = {
	{
		{
			{0, RED_COLOR, BRIGHT_STATE},
			{1, YELLOW_COLOR, DIM_STATE},
			{2, GREEN_COLOR, BLINK_STATE} 
		}, 2000
	},
	{
		{
			{0, RED_COLOR, DIM_STATE},
			{1, YELLOW_COLOR, DIM_STATE},
			{2, GREEN_COLOR, BRIGHT_STATE} 
		}, 3000
	},
	{
		{
			{0, RED_COLOR, BRIGHT_STATE},
			{1, YELLOW_COLOR, BRIGHT_STATE},
			{2, GREEN_COLOR, BRIGHT_STATE} 
		}, 1500
	},
};

void firstTrafficLightThread (void const *argument);              // thread function
osThreadId firstThreadId;                             						// thread id
osThreadDef(firstTrafficLightThread, osPriorityNormal, 1, 32000);       // thread object

osSemaphoreDef(firstSemaphore); 
osSemaphoreId firstSemaphoreId;	

extern osSemaphoreId secondSemaphoreId;

extern void processLights(Ligth const* lights, int const count, unsigned char threadId, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay);
extern int customDelay(unsigned int microDelay, unsigned int totalDelay, int stateCopy, volatile int* state);

int initFirstTrafficLight() {
	firstSemaphoreId = osSemaphoreCreate(osSemaphore(firstSemaphore), 1);
	
	if (!firstSemaphoreId) 
		return -1;
	
	firstThreadId = osThreadCreate(osThread(firstTrafficLightThread), NULL);
  
	if (!firstThreadId) 
		return -1;
	
	firstLightsCount = 3;
	firstStatesCount = 3;
	firstThreadState = 0;
	firstTrafficLightsId = 0;
	
	return 0;
}

void firstTrafficLightThread (void const *argument) {
	int stateCopy;
	
	while (1) {
		osSemaphoreWait(firstSemaphoreId, osWaitForever);
	
		stateCopy = firstThreadState;
	
		osSemaphoreRelease(firstSemaphoreId);
		
		switch (stateCopy) {
			case 0: {
				processLights(firstThreadStates[stateCopy].lights, firstLightsCount, firstTrafficLightsId, &firstThreadState, stateCopy, firstThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
			case 1: {
				processLights(firstThreadStates[stateCopy].lights, firstLightsCount, firstTrafficLightsId, &firstThreadState, stateCopy, firstThreadStates[stateCopy].delay, blinkTimeDelay);
				
				osSemaphoreWait(secondSemaphoreId, osWaitForever);
	
				secondThreadState = 1;
	
				osSemaphoreRelease(secondSemaphoreId);
				
				break;
			}
			case 2: {
				processLights(firstThreadStates[stateCopy].lights, firstLightsCount, firstTrafficLightsId, &firstThreadState, stateCopy, firstThreadStates[stateCopy].delay, blinkTimeDelay);
				break;
			}
		}
			
		osSemaphoreWait(firstSemaphoreId, osWaitForever);
	
		firstThreadState = (firstThreadState + 1) % firstStatesCount;
	
		osSemaphoreRelease(firstSemaphoreId);
		
		customDelay(microDelay, firstThreadStates[stateCopy].delay, stateCopy, &firstThreadState);
	}
}