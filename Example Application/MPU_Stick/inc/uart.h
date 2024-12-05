
#ifndef UART_H_
#define UART_H_

#include "system.h"
#include <msp430.h>
#include <stdint.h>

#define UART_Clock_MHz      SMCLK_FREQ
#define UART_BUFFER_SIZE    9

extern volatile uint8_t UART_RX_byte;
extern volatile uint8_t UART_buffer[UART_BUFFER_SIZE];

extern volatile union UART_status
{
  uint8_t all;

  struct segments
  {
    uint8_t tx_ready: 1;
    uint8_t rx_ready: 1;
    uint8_t rx_buffer_full: 1;
  }segments;

}UART_status;

void UART_GPIO_Init();
void UART_Init(uint32_t baud_rate);
void UART_Transmit(uint8_t* bytes, uint8_t n_bytes);
void UART_Receive(uint8_t* bytes, uint8_t n_bytes);
void UART_flush();
void UART_puts(char* s);
void UART_putu(uint32_t n);
void UART_puti(int32_t n);
void UART_putb(uint8_t _byte);
void UART_newline();

#endif /* UART_H_ */
