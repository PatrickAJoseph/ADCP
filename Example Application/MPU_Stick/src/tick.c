
#include "tick.h"

void Tick_Init()
{
    tick_count = 0;
    tick_flag = 0;

    TB0CCTL0 |= CCIE;          //  Enable capture-compare interrupt.

    P1DIR |= BIT1;

    /* Clock divider for Timer B is set to 8 * 8. SMCLK = 1 MHz  --> Timer B base frequency = 375kHz. */

    TB0CCR0 = 3750 - 1;                                           //  Set blink interval to 10 milli-second.
    TB0CTL |= (TBSSEL__SMCLK | MC__UP | ID__8 );                   //  Clock source is SMCLK and counting mode is UP.
                                                                   //  Set clock divider to 8.
    TB0EX0 |= TBIDEX__8;                                           //   Divide clock further by 8.
}

uint32_t get_tick_count()
{
    return(tick_count);
}

uint32_t get_tick_ms()
{
    return(tick_count * TICK_INTERVAL_MS);
}

void Tick_Reset()
{
    tick_count = 0;
    tick_flag = 0;
}

uint16_t get_tick_flag()
{
    return(tick_flag);
}

void clear_tick_flag()
{
    tick_flag = 0;
}


#pragma vector = TIMER0_B0_VECTOR

__interrupt void TIMER_B_ISR()
{
    tick_count++;
    tick_flag = 1;
    Task_Manager_Update_Task_States();
    TB0CTL &= ~TBIFG;
    //P1OUT ^= BIT1;
}
