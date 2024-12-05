
#include "task_manager.h"



void Task_Manager_Init()
{
    uint8_t i = 0;

    for( i = 0 ; i < TASK_MANAGER_NUMBER_OF_TASKS ; i++ )
    {
        tasks[i].count = 0;
        tasks[i].pending_executions = 0;
        tasks[i].state = TASK_STATE_SUSPENDED;
    }

    tasks[TASK_TRANSMIT_HANDLER].function                =   &ADCP_Transmit_Handler;
    tasks[TASK_TRANSMIT_HANDLER].execution_interval      =   10;
    tasks[TASK_TRANSMIT_HANDLER].state                   =   TASK_STATE_ACTIVE;

    tasks[TASK_RECEIVE_HANDLER].function                 =   &ADCP_Receive_Handler;
    tasks[TASK_RECEIVE_HANDLER].execution_interval       =   1;
    tasks[TASK_RECEIVE_HANDLER].state                    =   TASK_STATE_ACTIVE;
}


void Task_Manager_Update_Task_States()
{
    uint8_t i = 0;

    for( i = 0 ; i < TASK_MANAGER_NUMBER_OF_TASKS ; i++ )
    {
        tasks[i].count++;

        if( tasks[i].count >= tasks[i].execution_interval )
        {
            tasks[i].count = 0;

            if(tasks[i].pending_executions != 255)
            {
                tasks[i].pending_executions++;
            }
        }
    }
}

void Task_Manager_Execute_Tasks()
{
    uint8_t i = 0;

    for( i = 0 ; i < TASK_MANAGER_NUMBER_OF_TASKS ; i++ )
    {
        if( tasks[i].pending_executions )
        {
            tasks[i].pending_executions--;
            tasks[i].function();
        }
    }
}


