//start of A1 task
#include "common.h"
#include "A1Task.h"
#include "SerialTask.h"

static interTaskMsg_t A1Taskmsg;
struct io_descriptor *ioA1;

void A1_task(void *p)
{
	//(void)p;
	while(1)
	{
		if(xSemaphoreTake(UART_IO_Sem, 0))
		{
			//make queue message
			A1Taskmsg.Id = 1;
			memcpy(A1Taskmsg.data, "Hello1\n\r", 10);
			
			//write to queue
			ST_SendMsg(&A1Taskmsg);

			os_sleep(500);
						
			//release the semaphore...
			xSemaphoreGive(UART_IO_Sem);
			
			os_sleep(500);
		}
		else
		{
			os_sleep(500);
			
		}
	}//end while
}
