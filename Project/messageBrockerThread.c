#include "cmsis_os.h"
#include "utility.c"
#include "displayctrl.h"
#include <stdio.h>
#include <stdlib.h>

#define MESSAGE_QUEUE_SIZE 64

osThreadId tidConsumeThread;  // consume thread

osMessageQDef(messageQueue, MESSAGE_QUEUE_SIZE, uint32_t); 	// declare a message queue
osMessageQId messageQueue;            									    // declare an ID for the message queue

void sendMessage(Message*);
void consumeMessageThread(void const *argument);

osThreadDef(consumeMessageThread, osPriorityNormal, 1, 0);
 

void sendMessage(Message* message) {
		osMessagePut(messageQueue, (uint32_t) message, osWaitForever);  // Send Message
}

void consumeMessageThread(void const *argument) {
		Message* message;
		osEvent  evt;

		
		// Start thread of consuming messages.
		for (;;) {
				evt = osMessageGet(messageQueue, osWaitForever);  // wait for message
				if (evt.status == osEventMessage) {
						message = (Message*)evt.value.v;  // we can get valid message.
						
					printf("Thread id %02d:\t", message->threadId);
						
						unsigned int count;
						count = message->count;
						
						for (int i = 0; i < count; i++) {
								printf("%s\t", message->lights[i].color);
						}
						
						printf("\n");
						
						free(message);
						message = NULL;
						
						// Draw required traffic's light part
						//DrawTrafficLight(message->threadId, message->lightId, message->color);
				}
		}
}


// Initialize message queue for sending/recieving message with traffic light state info
void InitMessageQueue() {
		messageQueue = osMessageCreate(osMessageQ(messageQueue), NULL);
}

// Start consumer thread - should be executed after all initialization operations
void StartConsumer() {
	tidConsumeThread = osThreadCreate(osThread(consumeMessageThread), NULL);
}
