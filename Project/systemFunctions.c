#include "cmsis_os.h"
#include "utility.c"
#include <stdlib.h>

extern void sendMessage(Message* message);
//void blinkTheColor(void const* argument);
int customDelay(unsigned int microDelay, unsigned int totalDelay, int stateCopy, volatile int* state);
void processLights(Ligth const* lights, unsigned int const count, unsigned char threadId/*, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay*/);
int getLightColorFromState(unsigned int color, unsigned int type);
//void processBlinkLight(void* argument);

/*void blinkTheColor(void const* argument) {
	BlinkyInfo* blinkyInfo = (BlinkyInfo*)argument;
	
	if (argument == NULL) {
		return;
	}
	
	unsigned int microDelay;
	microDelay = 100;
	
	unsigned int currentDelayTime;
	currentDelayTime = 0;
	
	int brightHexColor;
	int dimHexColor;
			
	brightHexColor = getLightColorFromState(blinkyInfo->light.color, BRIGHT_STATE);
	dimHexColor = getLightColorFromState(blinkyInfo->light.color, DIM_STATE);
	
	Message message;
	message.threadId = blinkyInfo->threadId;
	message.lightId = blinkyInfo->light.color;
	
	while (currentDelayTime < blinkyInfo->totalTimeDelay) {
		message.color = brightHexColor;
		
		sendMessage(message);
		
		if (customDelay(microDelay, blinkyInfo->blinkTimeDelay, blinkyInfo->stateIdCopy, blinkyInfo->stateId) == 1) {
			break;
		}
		
		currentDelayTime = currentDelayTime + blinkyInfo->blinkTimeDelay;
		
		message.color = dimHexColor;
		
		sendMessage(message);
		
		if (customDelay(microDelay, blinkyInfo->blinkTimeDelay, blinkyInfo->stateIdCopy, blinkyInfo->stateId) == 1) {
			break;
		}
		
		currentDelayTime = currentDelayTime + blinkyInfo->blinkTimeDelay;
	}
}*/

int customDelay(unsigned int microDelay, unsigned int totalDelay, int stateCopy, volatile int* state) {
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

void processLights(Ligth const* lights, unsigned int const count, unsigned char threadId/*, volatile int* stateId, int stateIdCopy, unsigned int totalTimeDelay, unsigned int blinkTimeDelay*/) {
	Message* message = (Message*)malloc(sizeof(Message));
	message->lights = lights;
	message->count = count;
	message->threadId = threadId;
	
	sendMessage(message);
	
	/*for (int i = 0; i < count; i++) {
		Ligth light;
		light = *(lights + i);
		
		if (light.type == BLINK_STATE) {
			BlinkyInfo blinkyInfo;
			blinkyInfo.light = light;
			blinkyInfo.threadId = threadId;
			blinkyInfo.stateIdCopy = stateIdCopy;
			blinkyInfo.stateId = stateId;
			blinkyInfo.blinkTimeDelay = blinkTimeDelay;
			blinkyInfo.totalTimeDelay = totalTimeDelay;
			
			processBlinkLight((void*)&blinkyInfo);
		}
		else {
			int hexColor;
			
			hexColor = getLightColorFromState(light.color, light.type);
			
			Message message;
			message.color = hexColor;
			message.lightId = light.lightId;
			message.threadId = threadId;
			
			sendMessage(message);
		}
	}*/
}

/*void processBlinkLight(void * argument) {
	osThreadDef(blinkTheColor, osPriorityNormal, 1, 32);
	osThreadCreate(osThread(blinkTheColor), argument);
}*/

int getLightColorFromState(unsigned int color, unsigned int type) {
	if (color == RED_COLOR && type == BRIGHT_STATE) return 0xF800;
	else if (color == RED_COLOR && type == DIM_STATE) return 0x6800;
	else if (color == YELLOW_COLOR && type == BRIGHT_STATE) return 0xFC00;
	else if (color == YELLOW_COLOR && type == DIM_STATE) return 0xBC63;
	else if (color == GREEN_COLOR && type == BRIGHT_STATE) return 0x07E0;
	else if (color == GREEN_COLOR && type == DIM_STATE) return 0x0340;
	
	return 0x0000;
}