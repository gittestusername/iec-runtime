#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include "plcmodel.h"

static void plc_task_execute(void *task);
void plc_task_create(PLC_TASK_LIST *task_list, PLC_CONFIG *config);
void plc_task_start(PLC_TASK_LIST *task_list, PLC_CONFIG *config);
void plc_task_delete(PLC_TASK_LIST *task_list, PLC_CONFIG *config);
#endif
