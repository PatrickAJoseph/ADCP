/*
 * EEPROM.c
 *
 *  Created on: 06-Nov-2024
 *      Author: hp
 */

#include "EEPROM.h"

void EEPROM_write_byte(uint16_t address, uint8_t data)
{
    I2C_start_write(0x50);
    I2C_write_first_byte(address >> 8);
    I2C_write_byte(address & 0x00FF);
    I2C_write_byte_and_stop(data);
    delay_ms(15);
}

uint8_t EEPROM_read_byte(uint16_t address)
{
    uint8_t _byte = 0;
    I2C_start_write(0x50);
    I2C_write_first_byte(address >> 8);
    I2C_write_byte(address & 0x00FF);
    _byte = I2C_read_byte_after_write();
    delay_ms(15);
    return(_byte);
}

void EEPROM_read_bytes(uint16_t address, void* data, uint16_t size)
{
    I2C_start_write(0x50);
    I2C_write_first_byte(address >> 8);
    I2C_write_byte(address & 0x00FF);

    I2C_start_repeated_read();
    I2C_read_bytes(data,size);
}

void EEPROM_write_bytes(uint16_t address, void* data, uint16_t size)
{
    uint16_t index = 0;

    while(size)
    {
        EEPROM_write_byte(address+index,*((uint8_t*)data + index));
        size--;
        index++;
    }
}


