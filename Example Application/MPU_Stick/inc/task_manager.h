
#ifndef INC_TASK_MANAGER_H_
#define INC_TASK_MANAGER_H_

#include "system.h"
#include <stdint.h>

#define TASK_STATE_SUSPENDED    0
#define TASK_STATE_ACTIVE       1

#define TASK_MANAGER_NUMBER_OF_TASKS    2

/* Application specific task list. */

#define TASK_TRANSMIT_HANDLER       0
#define TASK_RECEIVE_HANDLER        1

typedef struct
{
    void (*function)(void);
    uint8_t count;
    uint8_t pending_executions;
    uint8_t execution_interval;
    uint8_t state;
}Task_t;


Task_t tasks[TASK_MANAGER_NUMBER_OF_TASKS];

void Task_Manager_Init();
void Task_Manager_Update_Task_States();
void Task_Manager_Execute_Tasks();

#endif /* INC_TASK_MANAGER_H_ */
