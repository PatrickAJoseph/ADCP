
#ifndef INC_TICK_H_
#define INC_TICK_H_

#include <msp430.h>
#include <stdint.h>
#include "task_manager.h"

#define TICK_INTERVAL_MS    10

uint32_t tick_count;
uint16_t tick_flag;

void Tick_Init();
uint32_t get_tick_count();
uint32_t get_tick_ms();
void Tick_Reset();
uint16_t get_tick_flag();
void clear_tick_flag();


#endif /* INC_TICK_H_ */
