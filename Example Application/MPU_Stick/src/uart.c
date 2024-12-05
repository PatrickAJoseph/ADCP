
#include "uart.h"
#include "ADCP.h"

volatile uint8_t UART_buffer[UART_BUFFER_SIZE] = {0};
volatile uint8_t UART_buffer_position = 0;
volatile union UART_status UART_status = {.all = 0};
volatile uint8_t UART_RX_byte = 0;

void UART_GPIO_Init()
{
  P1DIR |= BIT7;                        //  UCA0 TX pin configured as an output.
  P1SEL0 |= BIT7;  P1SEL1 &= ~BIT7;     //  Select function to UCA0 TX.

  P1DIR &= ~BIT6;                       //  UCA0 RX pin configured as an input.
  P1SEL0 |= BIT6;  P1SEL1 &= ~BIT6;     //  Select function to UCA0 RX.

  UART_status.segments.rx_ready = 0;
  UART_status.segments.tx_ready = 0;
  UART_status.segments.rx_buffer_full = 0;

  UART_buffer_position = 0;
}

void UART_Init(uint32_t baud_rate)
{
  /* Universal serial communication unit 0: Control word register 0. */

  UCA0CTLW0 |= UCSWRST;             //  Perform a software reset.
  UCA0CTLW0 |= UCSSEL__SMCLK;       //  Peripheral clock source: Sub-master clock.

  /* Configure UART settings. */

  uint32_t BRDIV = (((UART_Clock_MHz)/16)/baud_rate);
  UCA0BR0 = (uint8_t)( BRDIV & 0x00FF );
  UCA0BR1 = (uint8_t)( BRDIV >> 8 );

  /* Enable over-sampling: 16 bits per peripheral clock.
   * First modulation stage selected for over-sampling by 16 mode.  */

  UCA0MCTLW |= ( 0x4900 | UCOS16 | UCBRF_1 );

  /* Perform software reset for UCA0 peripheral. */

  UCA0CTLW0 &= ~UCSWRST;

  /* Enable receive interrupts. */

  UCA0IE |= UCRXIE;

  /* Enable global interrupts. */

  //__bis_SR_register(GIE);
}


void UART_Transmit(uint8_t* bytes, uint8_t n_bytes)
{
  uint16_t count = 0;

  while(n_bytes)
  {
    while(!(UCA0IFG&UCTXIFG));      //  Wait until transmit buffer is empty.
    UCA0TXBUF = bytes[count];       //  Load target transmit byte into buffer.
    n_bytes--;                      //  Decrement number of bytes to be transmitted.
    count++;                        //  Increment buffer position.
  }
}

void UART_Receive(uint8_t* bytes, uint8_t n_bytes)
{
  uint8_t count = 0;

  while(n_bytes)
  {
    bytes[count]  = 0;
    while(!UART_status.segments.rx_ready);
    bytes[count] = (uint8_t)UART_RX_byte;
    UART_status.segments.rx_ready = 0;
    count++;
    n_bytes--;
  }
}

void UART_flush()
{
  uint8_t i = 0;

  UART_status.segments.rx_buffer_full = 0;

  while(i < UART_BUFFER_SIZE)
  {
      UART_buffer[i] = 0;
      i++;
  }

  UART_buffer_position = 0;
}

void UART_puts(char* s)
{
  while( *s != '\0' )
  {
    while( !( UCA0IFG & UCTXIFG ) );
    UCA0TXBUF = (uint8_t)(*s);
    s++;
  }
}


void UART_putu(uint32_t n)
{
  uint8_t buffer[10];
  uint8_t position = 0;

  do
  {
    buffer[position] = ((uint8_t)'0' + (uint8_t)((uint32_t)(n%10)));
    position++;
    n /= 10;
  }while(n);

  do
  {
    position--;
    while( !( UCA0IFG & UCTXIFG ) );
    UCA0TXBUF = buffer[position];
  }while(position);

}

void UART_putb(uint8_t _byte)
{
    while( !( UCA0IFG & UCTXIFG ) );
    UCA0TXBUF = _byte;
}

void UART_newline()
{
    UART_putb((uint8_t)'\n');
    UART_putb((uint8_t)'\r');
}

void UART_puti(int32_t n)
{
  if( n < 0 )
  {
      n = -n;
      while( !( UCA0IFG & UCTXIFG ) );
      UCA0TXBUF = (uint8_t)'-';
  }

  UART_putu((uint32_t)n);
}



#pragma vector=USCI_A0_VECTOR

__interrupt void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
      case USCI_NONE: break;

      /* Receive interrupt. */

      case USCI_UART_UCRXIFG:
      {
        //while(!(UCA0IFG&UCTXIFG));
        UART_RX_byte = UCA0RXBUF;
        UCA0IFG &= ~UCRXIFG;
        UART_buffer[UART_buffer_position] = UART_RX_byte;


        UART_buffer_position++;

        if( ( UART_RX_byte == 0x0A ) && ( UART_buffer_position != UART_BUFFER_SIZE ) )
        {
            UART_flush();
            UART_buffer_position = 0;
        }
        else if(UART_buffer_position == UART_BUFFER_SIZE)
        {
            ADCP_Packet_t tempPacket = {0};

            tempPacket.byte[0] = UART_buffer[0];
            tempPacket.byte[1] = UART_buffer[1];
            tempPacket.byte[2] = UART_buffer[2];
            tempPacket.byte[3] = UART_buffer[3];
            tempPacket.byte[4] = UART_buffer[4];
            tempPacket.byte[5] = UART_buffer[5];
            tempPacket.byte[6] = UART_buffer[6];
            tempPacket.byte[7] = UART_buffer[7];
            tempPacket.byte[8] = UART_buffer[8];

            if( tempPacket.byte[8] == 0x0A )
            {
                if( ADCP_Receive_Queue_Status != ADCP_QUEUE_FULL )
                {
                    ADCP_Receive_Enqueue(tempPacket);
                }
                else
                {
                    ADCP_flags.flags.backup_packet_loaded = 1;
                    backup_packet = tempPacket;
                }

                //ADCP_Receive_Handler();

                UART_buffer_position = 0;
            }
            else
            {
                UART_buffer_position = 0;
                UART_flush();
            }

            UART_status.segments.rx_buffer_full = 1;
        }

        UART_status.segments.rx_ready = 1;
      }
      break;

      /* Transmit ready interrupt. Set when buffer register is empty. */

      case USCI_UART_UCTXIFG: break;

      /* Start bit interrupt. */

      case USCI_UART_UCSTTIFG: break;

      /* Transmit ready interrupt. Set when contents of internal buffer
       * is sent and buffer register is empty. */

      case USCI_UART_UCTXCPTIFG: break;
      default: break;
    }
}
