#include "cmsis_os.h"
#include "utility.c"

void blinkTheColor(void* argument);
void customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, unsigned int* state);

void blinkTheColor(void* argument) {
	BlinkyInfo* blinkyInfo = (BlinkyInfo*)argument;
	
	if (argument == NULL) {
		return;
	}
	
	unsigned int microDelay;
	microDelay = 100;
	
	unsigned int currentDelayTime;
	currentDelayTime = 0;
	
	Message message;
	message.threadId = blinkyInfo->threadId;
	message.color = blinkyInfo->light.color;
	
	while (currentDelayTime < blinkyInfo->totalTimeDelay) {
		message.type = BRIGHT_STATE;
		
		osMessageQueuePut(blinkyInfo->trafficLightsId, &message, 0, microDelay);
		
		if (!customDelay(microDelay, blinkyInfo->blinkTimeDelay, blinkyInfo->stateIdCopy, blinkyInfo->stateId) {
			break;
		}
		
		currentDelayTime = currentDelayTime + blinkyInfo->blinkTimeDelay;
		
		message.type = DIM_STATE;
		
		osMessageQueuePut(blinkyInfo->trafficLightsId, &message, 0, microDelay);
		
		if (!customDelay(microDelay, blinkyInfo->blinkTimeDelay, blinkyInfo->stateIdCopy, blinkyInfo->stateId) {
			break;
		}
		
		currentDelayTime = currentDelayTime + blinkyInfo->blinkTimeDelay;
	}
}

bool customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, unsigned int* state) {
	unsigned int currentDelayTime;
	currentDelayTime = 0;
	
	while (currentDelayTime < totalDelay) {
		if (stateCopy != *state) {
			return false;
		}
		
		osDelay(microDelay);
		
		currentDelayTime = currentDelayTime + microDelay;
	}
	
	return true;
}