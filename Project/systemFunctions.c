#include "cmsis_os.h"
#include "utility.c"

void blinkTheColor(void* argument);
int customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, unsigned int* state);

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
		
		osMessagePut(blinkyInfo->queueId, *(uint32_t*)&message, microDelay);
		
		if (customDelay(microDelay, blinkyInfo->blinkTimeDelay, blinkyInfo->stateIdCopy, blinkyInfo->stateId) == 1) {
			break;
		}
		
		currentDelayTime = currentDelayTime + blinkyInfo->blinkTimeDelay;
		
		message.type = DIM_STATE;
		
		osMessagePut(blinkyInfo->queueId, *(uint32_t*)&message, microDelay);
		
		if (customDelay(microDelay, blinkyInfo->blinkTimeDelay, blinkyInfo->stateIdCopy, blinkyInfo->stateId) == 1) {
			break;
		}
		
		currentDelayTime = currentDelayTime + blinkyInfo->blinkTimeDelay;
	}
}

int customDelay(unsigned int microDelay, unsigned int totalDelay, unsigned int stateCopy, unsigned int* state) {
	unsigned int currentDelayTime;
	currentDelayTime = 0;
	
	while (currentDelayTime < totalDelay) {
		if (stateCopy != *state) {
			return 1;
		}
		
		osDelay(microDelay);
		
		currentDelayTime = currentDelayTime + microDelay;
	}
	
	return 0;
}