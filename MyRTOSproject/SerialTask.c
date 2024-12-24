//start of serial task
#include<string.h>
#include "SerialTask.h"

QueueHandle_t serial_queue;
struct io_descriptor *ioSer;

static interTaskMsg_t SerTaskmsg;

void SER_Task(void *p)
{
	//(void)p;
	//Init
	serial_queue = xQueueCreate(10, sizeof(interTaskMsg_t));
	ioSer = NULL;
	
	while(xQueueReceive(serial_queue, &SerTaskmsg, portMAX_DELAY))
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
			
		//then, sleep.
		os_sleep(500);
		
	}//end while
		
}

void ST_SendMsg(interTaskMsg_t* SerTaskmsg)
{
	//write to serial task queue
	xQueueSendToBack(serial_queue, SerTaskmsg, 0);
}