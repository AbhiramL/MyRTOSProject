//start of serial task
#include<string.h>
#include "SerialTask.h"

static SemaphoreHandle_t SerialTask_Sem;
static QueueHandle_t Queue;
struct io_descriptor *ioSer; //for serial uart port writing
static interTaskMsg_t SerTaskmsg;

void SER_Task(void *p)
{
	//(void)p;
	//Init
	Queue = xQueueCreate(10, sizeof(interTaskMsg_t));	
	SerialTask_Sem = xSemaphoreCreateMutex();

	ioSer = NULL;
	
	//loop
	while(xQueueReceive(Queue, &SerTaskmsg, portMAX_DELAY))
	{
		usart_async_get_io_descriptor(&USART_0, &ioSer);
		usart_async_enable(&USART_0);
		
		//analyze the task msg
		switch(SerTaskmsg.Id)
		{
			case 1:
				//io_write(ioSer, (uint8_t*)"A1 Task:", sizeof("A1 Task:"));
				io_write(ioSer, (uint8_t*)SerTaskmsg.data, strlen((const char *)SerTaskmsg.data));	
				break;
			
			case 2:
				//io_write(ioSer, (uint8_t*)"A2 Task:", sizeof("A2 Task:"));
				io_write(ioSer, (uint8_t*)SerTaskmsg.data, strlen((const char *)SerTaskmsg.data));
				break;		
				
			default:
				io_write(ioSer, (uint8_t*)"Unknown task:", sizeof("Unknown task:"));
				//io_write(ioSer, (uint8_t*)SerTaskmsg.data, strlen((const char *)SerTaskmsg.data));
				break;	
		}//end switch

		
	}//end while
		
}

void ST_SendMsg(interTaskMsg_t* SerTaskmsg) //no static, we want A1 and A2 to call this
{
	//get semaphore
	if(xSemaphoreTake(SerialTask_Sem, portMAX_DELAY))
	{
		//write to serial task queue
		xQueueSendToBack(Queue, SerTaskmsg, 0);
		
		xSemaphoreGive(SerialTask_Sem);
		
	}
	
}