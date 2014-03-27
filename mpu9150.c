#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

void mpu9150_write(uint8_t reg, uint8_t data)
{
	i2c_write(MPU9150_ADDR, reg, data);
}

uint8_t mpu9150_read_single(uint8_t reg)
{
	return i2c_read(MPU9150_ADDR, reg);
}

uint16_t mpu9150_read_double(uint8_t lreg, uint8_t hreg)
{
	uint16_t rv;
	rv = (((uint16_t)i2c_read(MPU9150_ADDR, hreg)) << 8) & 0xFF00;
	rf |= i2c_read(MPU9150_ADDR, lreg);
	return rv;
}

void mpu9150_setup()
{
	i2c_write(MPU9150_CMPS_ADDR, 0x0A, 0x00);
	i2c_write(MPU9150_CMPS_ADDR, 0x0A, 0x0F);
	i2c_write(MPU9150_CMPS_ADDR, 0x0A, 0x00);
	mpu9150_write(MPU9150_I2C_MST_CTRL, 0x40);
	mpu9150_write(MPU9150_I2C_SLV0_ADDR, 0x8C);
	mpu9150_write(MPU9150_I2C_SLV0_REG, 0x02);
	mpu9150_write(MPU9150_I2C_SLV0_CTRL, 0x88);
	mpu9150_write(MPU9150_I2C_SLV1_ADDR, 0x0C);
	mpu9150_write(MPU9150_I2C_SLV1_REG, 0x0A);
	mpu9150_write(MPU9150_I2C_SLV1_CTRL, 0x81);
	mpu9150_write(MPU9150_I2C_SLV1_DO, 0x01);
	mpu9150_write(MPU9150_I2C_MST_DELAY_CTRL, 0x03);
	mpu9150_write(0x01, 0x80);
	mpu9150_write(MPU9150_I2C_SLV4_CTRL, 0x04);
	mpu9150_write(MPU9150_I2C_SLV1_DO, 0x00);
	mpu9150_write(MPU9150_USER_CTRL, 0x00);
	mpu9150_write(MPU9150_I2C_SLV1_DO, 0x01);
	mpu9150_write(MPU9150_USER_CTRL, 0x20);
	mpu9150_write(MPU9150_I2C_SLV4_CTRL, 0x13);
}
