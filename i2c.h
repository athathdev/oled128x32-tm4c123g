#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void InitI2C0(void);
void I2CSendSingleByte(uint8_t slave_addr, char data);
void I2CSendMultipleBytes(uint8_t slave_addr, uint8_t numOfBytes, char by[]);

#endif 
