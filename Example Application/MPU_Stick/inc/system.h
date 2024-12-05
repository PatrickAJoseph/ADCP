
#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include <msp430.h>

#include "uart.h"
#include "LED.h"
#include "MPU9250.h"
//#include "configuration.h"
#include "motion_processing.h"
#include "ADCP_frames.h"
#include "ADCP.h"
#include "ADCP_Transmit.h"
#include "ADCP_Receive.h"
#include "tick.h"
#include "EEPROM.h"
#include "task_manager.h"


#define MCLK_FREQ   24000000UL
#define SMCLK_FREQ   24000000UL

#define MCLK_FREQ_MHz   24UL
#define SMCLK_FREQ_MHz  24UL

#define CLOCK_TICK_INTERVAL_MS  (1000*MCLK_FREQ_MHz)

void System_Clock_Init();
void delay_ms(uint16_t delay);

#endif /* INC_SYSTEM_H_ */
