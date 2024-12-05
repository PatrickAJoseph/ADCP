
#include "spi.h"

/*
 * @brief:              Function to initialize GPIOs for SPI operation @ USCI B1 interface.
 * @parameters:         None
 * @return:             None
 *
 * */

void SPI_GPIO_Init()
{
    /* For SPI configuration UCB1.
     *
     * P4.4 --> Chip Select (Output).
     * P4.5 --> Serial Clock (Output).
     * P4.6 --> MOSI (Output).
     * P4.7 --> MISO (Input).
     *
     * */

    P4DIR |= (BIT4 | BIT5 | BIT6);
    P4DIR &= ~BIT7;

    //P4SEL0 |= BIT4;     P4SEL1 &= ~BIT4;
    P4SEL0 |= BIT5;     P4SEL1 &= ~BIT5;
    P4SEL0 |= BIT6;     P4SEL1 &= ~BIT6;
    P4SEL0 |= BIT7;     P4SEL1 &= ~BIT7;
}


/*
 * @brief:              Function to initialize the USCI B1 peripheral in SPI mode.
 *                      4-bit mode, 8-bits/frame, CPOL = 0, CPHA = 1.
 *
 * @parameters:         None.
 * @return:             None.
 *
 * */

void SPI_Init()
{
    /* SPI peripheral initialization. */

    P4OUT |= BIT4;

    UCB1CTLW0 = UCSWRST;            //  Set software reset flag to configure peripheral.

    UCB1CTLW0 |= UCSYNC;

    UCB1CTLW0 |= UCSYNC;            //  Put USCI in synchronous mode.
    UCB1CTLW0 |= UCMST;             //  Put USCI in master mode.
    UCB1CTLW0 |= UCMSB;             //  MSB bit first.
    UCB1CTLW0 &= ~UCCKPL;            //  Clock polarity is high during inactive state.
    UCB1CTLW0 |= UCCKPH;           //  Data is transmitted by master on falling edge of clock
                                   //  and serial data in line sampled on rising edge of clock.
    UCB1CTLW0 &= ~UC7BIT;           //  USCI in 8-bit mode.
    UCB1CTLW0 |= UCMODE_2;          //  4-pin SPI mode, Chip select active low.
    UCB1CTLW0 |= UCSSEL__SMCLK;     //  SMCLK configured as 24 MHz.
    UCB1BRW  = 24;                  //  SPI clock frequency: 1 MHz.
    UCB1CTLW0 |= UCSTEM;            //  STE pin generates enable signal for slave devices.
    UCB1IE   = (UCTXIE | UCRXIE);  //  Enable transmit & receive interrupts.
    UCB1IFG  = 0;                  //  Clear all interrupt flags.
    UCB1CTLW0 &= ~UCSWRST;          //  Take peripheral out of reset state.
}

/*
 * @brief:                          Function to transfer N number of bytes between master and slave.
 * @parameter transmit_bytes:       Pointer to array containing bytes to be transferred to slave.
 * @parameter receive_bytes:        Pointer to array to hold bytes received from slave.
 * @parameter n_bytes:              Number of bytes to be transferred between master and slave.
 *
 * */

#define SPI_TRANSFER_TIMEOUT_COUNT_LIMIT       100

void SPI_Transfer(uint8_t* transmit_buffer, uint8_t* receive_buffer,uint8_t transfer_count)
{
    uint8_t index = 0;
    uint16_t timeout_counter = 0;

    P4OUT &= ~BIT4;

    while(transfer_count)
    {

        while( !(UCB1IFG & UCTXIFG) && (timeout_counter < SPI_TRANSFER_TIMEOUT_COUNT_LIMIT) )              //  Wait until data is shifted from transmit buffer into transmit shift register.
        {
            timeout_counter++;
        }

        if(timeout_counter >= SPI_TRANSFER_TIMEOUT_COUNT_LIMIT)
        {
            UCB1IFG &= ~UCTXIFG;                        //  Clear interrupt flag.
            P4OUT |= BIT4;
            break;
        }

        timeout_counter = 0;

        UCB1TXBUF = transmit_buffer[index];         //  Load data into transmit buffer.
        //UCB1IE &= UCTXIE;
        UCB1IFG &= ~UCTXIFG;                        //  Clear interrupt flag.

        while( !(UCB1IFG & UCRXIFG) && ( timeout_counter < SPI_TRANSFER_TIMEOUT_COUNT_LIMIT ) )              //  Wait until transmit/receive operation is completed.
        {
            timeout_counter++;
        }

        if(timeout_counter >= SPI_TRANSFER_TIMEOUT_COUNT_LIMIT)
        {
            UCB1IFG &= ~UCRXIFG;                        //  Clear receive interrupt flag.
            P4OUT |= BIT4;
            break;
        }

        UCB1IFG &= ~UCRXIFG;                        //  Clear receive interrupt flag.
        receive_buffer[index] = (uint8_t)UCB1RXBUF; // Load data from receive buffer.

        index++;
        transfer_count--;
    }

    P4OUT |= BIT4;
}


