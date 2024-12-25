//start of A1 task
#include "common.h"
#include "A1Task.h"
#include "SerialTask.h"

//local defines
#define WAKEUP_TIMEOUT 1000 //wakeup timer timeout at 1s
#define WAKEUP_TIMER_ID 0   //wakeup timer id.

//local variables
static SemaphoreHandle_t Sem;
static QueueHandle_t Queue;
static TimerHandle_t Wake_Timer;
static interTaskMsg_t A1Taskmsg, incomingMsg;

//local function prototypes
static void A1_Timer_Timeout(TimerHandle_t xTimer);

//global functions.
void A1_task(void *p)
{
	//(void)p;
	
	Queue = xQueueCreate(10, sizeof(interTaskMsg_t));
	Sem = xSemaphoreCreateMutex();
	
	//timer params: name for debug, timer period ticks, pdFalse = one-shot not auto loading timer, 0 means no timer id, function to run if timer fires
	//timer id should be used if multiple timers are calling the same callback function
	Wake_Timer = xTimerCreate((const char*)"A1 Wake Up Timer", WAKEUP_TIMEOUT, pdFALSE, WAKEUP_TIMER_ID, A1_Timer_Timeout);
	
	//start timer
	//if timer command queue is full, setting last param to max delay will make this task blocked. see configTIMER_QUEUE_LENGTH in freeRTOSConfig.h
	xTimerStart(Wake_Timer, portMAX_DELAY);
	
	//loop.	
	while((xQueueReceive(Queue, &incomingMsg, portMAX_DELAY))) //portMAX_DELAY as last param means this will wait without timing out
	{
		A1Taskmsg.Id = 1;
		memcpy(A1Taskmsg.data, "Hello1\n\r", 10);
		
		//write to queue
		ST_SendMsg(&A1Taskmsg);
		
		
	}//end while
}


/*   
Timeout call back:  


*/
static void A1_Timer_Timeout(TimerHandle_t xTimer)
{
	if( xTimer == Wake_Timer)
	{
		//prepare a message
		A1Taskmsg.Id = 0;
		memcpy(A1Taskmsg.data, "A1 Timeout", 11);
		
		//post it to A1 task queue
		xQueueSendToBack(Queue, &A1Taskmsg, 0);
		
		//reset timer
		xTimerReset(xTimer, portMAX_DELAY); //delay until timer command queue space opens for this command
	}
}
