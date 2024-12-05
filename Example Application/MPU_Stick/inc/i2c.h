
#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>

#define CLK_FREQ   8000000UL
#define CLOCK_FREQ CLK_FREQ

#ifndef CLK_FREQ
#error  "Master clock frequency not defined in module: i2c.h"
#endif


void I2C_GPIO_Init();
void I2C_Init(uint32_t I2C_clock_frequency);

void I2C_start_write(uint8_t device_address);
void I2C_write_byte(uint8_t value);
void I2C_write_byte_and_stop(uint8_t value);
void I2C_write_first_byte(uint8_t value);
void I2C_stop_write();

void I2C_start_read(uint8_t device_address);
void I2C_start_repeated_read();
uint8_t I2C_read_byte();
void I2C_stop_read();
uint8_t I2C_read_byte_after_write();
void I2C_read_bytes(void* data, uint16_t size);
uint8_t I2C_read_last_byte();

#endif /* I2C_H_ */
