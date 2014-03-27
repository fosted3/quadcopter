#ifndef i2c_h_
#define i2c_h_
#include <stdint.h>

void i2c_init(void);
void i2c_write(uint8_t addr, uint8_t reg, uint8_t data);
void i2c_read(uint8_t addr, uint8_t reg, uint8_t *data);

#endif
