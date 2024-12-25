//start of A2 task
#include "common.h"
#include "SerialTask.h"
#include "A2Task.h"

//local defines
#define WAKEUP_TIMEOUT 1500 //wakeup timer timeout at 1s
#define WAKEUP_TIMER_ID 0   //wakeup timer id.

static SemaphoreHandle_t sem;
static QueueHandle_t Queue;
static TimerHandle_t Wake_Timer;
static interTaskMsg_t A2Taskmsg, incomingMsg;

//local function prototypes
static void A2_Timer_Timeout(TimerHandle_t xTimer);

void A2_task(void *p)
{
	//(void)p;
	
	Queue = xQueueCreate(10, sizeof(interTaskMsg_t));
	sem = xSemaphoreCreateMutex();
	
	//timer params: name for debug, timer period ticks, pdFalse = one-shot not auto loading timer, 0 means no timer id, function to run if timer fires
	//timer id should be used if multiple timers are calling the same callback function
	Wake_Timer = xTimerCreate((const char*)"A2 Wake Up Timer", WAKEUP_TIMEOUT, pdFALSE, WAKEUP_TIMER_ID, A2_Timer_Timeout);
	
	//start timer
	xTimerStart(Wake_Timer, portMAX_DELAY); 
	//if timer command queue is full, setting last param to max delay will make this task blocked. see configTIMER_QUEUE_LENGTH in freeRTOSConfig.h
	
	while((xQueueReceive(Queue, &incomingMsg, portMAX_DELAY))) //portMAX_DELAY as last param means this will wait without timing out
	{
		A2Taskmsg.Id = 2;
		memcpy(A2Taskmsg.data, "Hello2\n\r", 10);
		
		//write to queue
		ST_SendMsg(&A2Taskmsg);		
					
	}//end while
}

static void A2_Timer_Timeout(TimerHandle_t xTimer)
{
	if( xTimer == Wake_Timer)
	{
		//prepare a message
		A2Taskmsg.Id = 0;
		memcpy(A2Taskmsg.data, "A2 Timeout", 11);
		
		//post it to A1 task queue
		xQueueSendToBack(Queue, &A2Taskmsg, 0);
		
		//reset timer
		xTimerReset(xTimer, portMAX_DELAY); //delay until timer command queue space opens for this command
	}
}
