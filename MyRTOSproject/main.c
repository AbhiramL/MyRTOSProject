#include "atmel_start.h"
#include "rtos_start.h"
#include "common.h"
#include "A1Task.h"
#include "A2Task.h"
#include "SerialTask.h"

#define TASK_STACK_SIZE (1024 / sizeof(portSTACK_TYPE))

#define A1_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define A2_TASK_PRIORITY (A1_TASK_PRIORITY + 1)
#define SER_TASK_PRIORITY (A2_TASK_PRIORITY + 1)

static TaskHandle_t      xA1Task, xA2Task, xSerTask;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	xTaskCreate(A1_task, "A1", TASK_STACK_SIZE, NULL, A1_TASK_PRIORITY, xA1Task);
	xTaskCreate(A2_task, "A2", TASK_STACK_SIZE, NULL, A2_TASK_PRIORITY, xA2Task);
	xTaskCreate(SER_Task, "SERIAL", TASK_STACK_SIZE, NULL, SER_TASK_PRIORITY, xSerTask);
	
	//Begin the scheduler....
	vTaskStartScheduler();
	
	return 0;
}
