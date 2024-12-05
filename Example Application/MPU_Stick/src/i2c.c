
#include "i2c.h"

void I2C_GPIO_Init()
{
    P1REN |= (BIT2 | BIT3);
    P1OUT |= (BIT2 | BIT3);
    P1SEL0 |= (BIT2 | BIT3);     //  Configure P1.2 as SDA, P1.3 as SCL.
    P1SEL1 &= ~( BIT2 | BIT3);
}

void I2C_Init(uint32_t I2C_clock_frequency)
{
    /*  Refer to SLAU445I: 24.3.1 */

     UCB0CTLW0    |= UCSWRST;                         //  Perform a software reset.
     UCB0CTLW0   |= (UCMODE_3 | UCMST |
                     UCSSEL__SMCLK  | UCSYNC );      //  UCMODE_3: Auto baud-rate detection, UCMST: Master mode.
     UCB0BRW     = 60;                               //  I2C clock rate: SMCLK/96 = 24MHz/60 = 400 kHz.
     UCB0I2CSA   = 0;                     //  Configure slave ID.
     UCB0TBCNT = 255;
     UCB0CTLW0   &= ~UCSWRST;                        //  Take UCB0 module out of reset mode.
     //UCB0IE      |= UCNACKIE;                         //  Enable I2C Ack. Failure Interrupt.
}

void I2C_start_write(uint8_t device_address)
{
    //UCB0CTLW0    |= UCSWRST;                                //  Perform a software reset.

    //UCB0I2CSA    =      (uint16_t)device_address; //   Load slave address.
    //UCB0IFG      &=  ~( UCTXIFG | UCRXIFG | UCSTTIFG);     //   Clear existing receive and transmit interrupts.
    //UCB0IE       |=     ( UCTXIE | UCSTTIE );              //   Disable receive interrupt and enable transmit interrupt.
    //UCB0IE       &=      ~UCRXIE;              //   Disable receive interrupt.

    //UCB0CTLW0    &= ~UCSWRST;                                //  Perform a software reset.

    //UCB0CTLW0    |=     ( UCTR | UCTXSTT );      //   Enable transmit ready bit and enable start condition.

    /* Configure registers. */

    UCB0I2CSA    =      (uint16_t)device_address; //   Load slave address.
    UCB0IFG      &=  ~( UCTXIFG | UCRXIFG );     //   Clear existing receive and transmit interrupts.
    //UCB0IE       |=     ( UCTXIE );              //   Disable receive interrupt and enable transmit interrupt.
    //UCB0IE       &=      ~UCRXIE;                //   Disable receive interrupt.

    UCB0CTLW0    |=     ( UCTR | UCTXSTT );      //   Enable transmit ready bit and enable start condition.

    /* After transmission of the start bit, transmit interrupt will be generated . */


    while( !(UCB0IFG & UCTXIFG0) );
    //UCB0IFG &= ~UCTXIFG0;
    //while( UCB0CTLW0 & UCTXSTT );
    //__no_operation();
}


void I2C_write_byte(uint8_t value)
{
    UCB0IFG &= ~UCTXIFG0;
    UCB0TXBUF = value;
    while( !(UCB0IFG & UCTXIFG0) );

    //UCB0CTLW0 |= UCTXSTT;
}

void I2C_write_first_byte(uint8_t value)
{
    UCB0IFG &= ~UCTXIFG0;
    UCB0TXBUF = value;
    while (UCB0CTL1 & UCTXSTT);
    while( !(UCB0IFG & UCTXIFG0) );
    //UCB0CTLW0 |= UCTXSTT;
}

void I2C_write_byte_and_stop(uint8_t value)
{
    UCB0IFG &= ~UCTXIFG0;
    UCB0TXBUF = value;
    while( !(UCB0IFG & UCTXIFG0) );
    UCB0CTLW0 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);
}

void I2C_stop_write()
{
    UCB0CTLW0 |= UCTXSTP;   //  Send stop command.
}

void I2C_start_read(uint8_t device_address)
{
}

void I2C_start_repeated_read()
{
    UCB0CTLW0 &= ~UCTR;                 //  Disable transmitter mode.
    UCB0CTLW0 |= UCTXSTT;               //  Send repeated start.
    //UCB0CTLW0 &= ~UCTR;                 //  Disable transmitter mode.
    //UCB0CTLW0 |= UCTXSTT;               //  Send repeated start.
    while (UCB0CTL1 & UCTXSTT);
    UCB0IFG &= ~UCTXIFG0;
}

uint8_t I2C_read_byte()
{
    while( !( UCB0IFG & UCRXIFG ) );
    uint8_t _byte = UCB0RXBUF;
    UCB0IFG &= ~UCRXIFG;
    return(_byte);
}

void I2C_read_bytes(void* data, uint16_t size)
{
    uint16_t index = 0;

    while(size)
    {
        if( size != 1 )
        {
            (*(uint8_t*)(data + index)) = I2C_read_byte();
        }
        else
        {
            (*(uint8_t*)(data + index)) = I2C_read_last_byte();
        }

        size--;
        index++;
    }
}

uint8_t I2C_read_last_byte()
{
    UCB0CTLW0 |= UCTXSTP;
    while( !( UCB0IFG & UCRXIFG ) );
    uint8_t _byte = UCB0RXBUF;
    UCB0IFG &= ~UCRXIFG;
    return(_byte);
}

uint8_t I2C_read_byte_after_write()
{
    UCB0CTLW0 &= ~UCTR;                 //  Disable transmitter mode.
    UCB0CTLW0 |= UCTXSTT;               //  Send repeated start.
    while (UCB0CTL1 & UCTXSTT);
    //__no_operation();
    UCB0IFG &= ~UCTXIFG0;
    UCB0CTLW0 |= UCTXSTP;
    while( !( UCB0IFG & UCRXIFG ) );
    uint8_t _byte = UCB0RXBUF;
    UCB0IFG &= ~UCRXIFG;
    while (UCB0CTL1 & UCTXSTP);
    //__no_operation();
    return(_byte);
}

void I2C_stop_read()
{
    UCB0CTLW0 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);
}
