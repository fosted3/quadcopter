#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"


void i2c_init(void)
{
	//Enable I2C0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	//Set PB2 and PB3 as I2C
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	//Set 400kHz fast mode
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
}

void i2c_write(uint8_t addr, uint8_t reg, uint8_t data)
{
	I2CMasterSlaveAddrSet(I2C0_BASE, addr, false);
	I2CMasterDataPut(I2C0_BASE, reg);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_BASE));
	I2CMasterDataPut(I2C0_BASE, data);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_BASE));
}

void i2c_read(uint8_t addr, uint8_t reg, uint8_t *data)
{
	I2CMasterSlaveAddrSet(I2C0_BASE, addr, false);
	I2CMasterDataPut(I2C0_BASE, reg);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	while(I2CMasterBusy(I2C0_BASE));
	I2CMasterSlaveAddrSet(I2C0_BASE, addr, true);
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
	while(I2CMasterBusy(I2C0_BASE));
	*data = I2CMasterDataGet(I2C0_BASE);
}
