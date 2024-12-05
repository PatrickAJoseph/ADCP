
#ifndef SPI_H_
#define SPI_H_

#include <msp430.h>
#include <stdint.h>

/*
 * @brief:              Function to initialize GPIOs for SPI operation @ USCI B1 interface.
 * @parameters:         None
 * @return:             None
 *
 * */

void SPI_GPIO_Init();

/*
 * @brief:              Function to initialize the USCI B1 peripheral in SPI mode.
 *                      4-bit mode, 8-bits/frame, CPOL = 0, CPHA = 1.
 *
 * @parameters:         None.
 * @return:             None.
 *
 * */

void SPI_Init();

/*
 * @brief:                          Function to transfer N number of bytes between master and slave.
 * @parameter transmit_bytes:       Pointer to array containing bytes to be transferred to slave.
 * @parameter receive_bytes:        Pointer to array to hold bytes received from slave.
 * @parameter n_bytes:              Number of bytes to be transferred between master and slave.
 *
 * */

void SPI_Transfer(uint8_t* transmit_buffer, uint8_t* receive_buffer,uint8_t transfer_count);


#endif /* SPI_H_ */
