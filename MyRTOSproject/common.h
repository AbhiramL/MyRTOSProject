#ifndef COMMON_H
#define COMMON_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "hal_rtos.h"
#include "driver_init.h"
#include "utils.h"
#include "rtos_port.h"
#include "semphr.h"

#include <string.h>

typedef struct
{
	uint8_t Id;
	uint8_t data[10];
}interTaskMsg_t;

//semaphore/queue declared 
extern SemaphoreHandle_t UART_IO_Sem;

#endif 

