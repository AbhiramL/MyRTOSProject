//start of A2 task
#include "common.h"
#include "SerialTask.h"
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
			A2Taskmsg.Id = 2;
			memcpy(A2Taskmsg.data, "Hello2\n\r", 10);
			
			//write to queue
			ST_SendMsg(&A2Taskmsg);		
			
			//then, sleep.
			os_sleep(500);
						
			//release the semaphore...
			xSemaphoreGive(UART_IO_Sem);

			//then, sleep.
			os_sleep(500);			
		}
		else
		{
			os_sleep(500);
		}
	}//end while
}
