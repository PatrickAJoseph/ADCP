
#ifndef EEPROM_H_
#define EEPROM_H_

#include "i2c.h"

void EEPROM_write_byte(uint16_t address, uint8_t data);

uint8_t EEPROM_read_byte(uint16_t address);

void EEPROM_read_bytes(uint16_t address, void* data, uint16_t size);

void EEPROM_write_bytes(uint16_t address, void* data, uint16_t size);


#endif /* EEPROM_H_ */
