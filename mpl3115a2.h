#ifndef mpl3115a2_h_
#define mpl3115a2_h_

#include "mpl3115a2_defs.h"
#include <stdint.h>

void mpl3115a2_setup(void);
void mpl3115a2_write(uint8_t reg, uint8_t data);
uint8_t mpl3115a2_read(uint8_t reg);
extern void i2c_write(uint8_t addr, uint8_t reg, uint8_t data);
extern void i2c_read(uint8_t addr, uint8_t reg, uint8_t &data);

#endif
