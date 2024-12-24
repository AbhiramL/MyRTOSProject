//start of A1 task
#include "common.h"
#include "A1Task.h"


static interTaskMsg_t A1Taskmsg;
struct io_descriptor *ioA1;


void A1_task(void *p)
{
	//(void)p;

			usart_async_get_io_descriptor(&USART_0, &ioA1);
			usart_async_enable(&USART_0);

	
	while(1)
	{
		if(xSemaphoreTake(UART_IO_Sem, 0))
		{
			//send queue message
			
			memcpy(A1Taskmsg.data, "A1 Task\n", 10);
			
			io_write(ioA1, (uint8_t*)A1Taskmsg.data, sizeof(A1Taskmsg.data));

			os_sleep(500);
						
			//release the semaphore...
			xSemaphoreGive(UART_IO_Sem);
			
			os_sleep(500);
		}
		else
		{
			os_sleep(500);
			
		}
		
	}
	
}
