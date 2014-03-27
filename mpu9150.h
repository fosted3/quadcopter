#ifndef mpu9150_h_
#define mpu9150_h_

#include "mpu9150_defs.h"
#include <stdint.h>

void setup_compass(void);
void mpu9150_write(uint8_t reg, uint8_t data);
uint8_t mpu9150_read(uint8_t reg);
extern void i2c_write(uint8_t addr, uint8_t reg, uint8_t data);
extern uint8_t i2c_read(uint8_t addr, uint8_t reg);

#endif
