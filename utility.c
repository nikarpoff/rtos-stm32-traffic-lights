#include "cmsis_os.h"
#include <stdint.h>

#define RED_COLOR 1
#define YELLOW_COLOR 2
#define GREEN_COLOR 3

#define BRIGHT_STATE 1
#define DIM_STATE 2
#define BLINK_STATE 3

typedef struct {
	unsigned char threadId;
	unsigned char lightId;
	unsigned int color;
} Message;

typedef struct {
	unsigned char lightId;
	unsigned int color;
	unsigned int type;
} Ligth;

typedef struct {
	Ligth lights[3];
} TrafficLightState;

typedef struct {
	Ligth light;
	osMessageQueueId_t queueId;
	unsigned char threadId;
	unsigned int stateIdCopy;
	unsigned int* stateId;
	unsigned int blinkTimeDelay;
	unsigned int totalTimeDelay;
} BlinkyInfo;