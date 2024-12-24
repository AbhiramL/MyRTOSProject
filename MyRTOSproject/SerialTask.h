#ifndef SERIALTASK_H
#define SERIALTASK_H
#include "common.h"

void SER_Task(void *p);
void ST_SendMsg(interTaskMsg_t* SerTaskmsg);
#endif