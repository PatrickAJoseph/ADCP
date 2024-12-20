
#include "system.h"

void System_Clock_Init()
{
    P2SEL1 |= BIT6 | BIT7;                       // P2.6~P2.7: crystal pins

    FRCTL0 = FRCTLPW | NWAITS_2 ;

    do
    {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

    __bis_SR_register(SCG0);                     // disable FLL
    CSCTL3 |= SELREF__XT1CLK;                    // Set XT1 as FLL reference source
    CSCTL0 = 0;                                  // clear DCO and MOD registers
    CSCTL1 |= DCORSEL_7;                         // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 731;                       // DCOCLKDIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                     // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));   // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
                                                 // default DCOCLKDIV as MCLK and SMCLK source.
}

void delay_ms(uint16_t delay)
{
    while(delay)
    {
        __delay_cycles(CLOCK_TICK_INTERVAL_MS);
        delay--;
    }
}
