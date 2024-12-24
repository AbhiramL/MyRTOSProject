//start of serial task
#include "common.h"
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
		//analyze the task msg
		//switch(SerTaskmsg->Id){}		
		
		if(xSemaphoreTake(UART_IO_Sem, 0))
		{
			usart_async_get_io_descriptor(&USART_0, &ioSer);
			usart_async_enable(&USART_0);
			
			memcpy(SerTaskmsg.data, "Serial Task\n", 10);
			io_write(ioSer, (uint8_t*)SerTaskmsg.data, sizeof(SerTaskmsg.data));	
			
			//release the semaphore...
			xSemaphoreGive(UART_IO_Sem);
			
			//then, sleep.
			os_sleep(2000);
		}
		else
		{
			os_sleep(500);			
		}
	}
		
}