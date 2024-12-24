//start of A2 task
#include "common.h"
#include "A2Task.h"


static interTaskMsg_t A2Taskmsg;
struct io_descriptor *ioA2;


void A2_task(void *p)
{
	//(void)p;
	while(1)
	{
		if(xSemaphoreTake(UART_IO_Sem, 0))
		{
			//send queue message
			usart_async_get_io_descriptor(&USART_0, &ioA2);
			usart_async_enable(&USART_0);
			
			memcpy(A2Taskmsg.data, "A2 Task\n", 10);
			io_write(ioA2, (uint8_t*)A2Taskmsg.data, sizeof(A2Taskmsg.data));			
			
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
