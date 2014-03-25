//******************************************************************************
// This program tests the radio functionality of the board. The NRF24L01+ is
// attached to SSI1, on pins PD0-PD3. In addition to the standard SPI pins, the
// NRF also has an interrupt output (NRF_IRQ, on PE4), and an enable pin
// (NRF_EN, on PE5). NRF_EN is active high, NRF_IRQ is active low.
//******************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "nrf_defs.h"

void init(void)
{
	//Set system clock to 80MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Enable SSI1, port D, and port E
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	//Configure pin multiplexing
	GPIOPinConfigure(GPIO_PD0_SSI1CLK);
	GPIOPinConfigure(GPIO_PD1_SSI1FSS);
	GPIOPinConfigure(GPIO_PD2_SSI1RX);
	GPIOPinConfigure(GPIO_PD3_SSI1TX);

	//Set NRF_EN as output, NRF_IRQ as input
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);

	//Configure port D for SSI
	GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	//Configure SSI settings (1MHz SCK, Polarity & Phase 0, 8 bits / frame
	SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);

	//Enable SSI1
	SSIEnable(SSI1_BASE);
}

void enable_nrf(void)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x10);
}

void disable_nrf(void)
{
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
}

void read_reg(uint8_t reg, uint8_t* buf, uint8_t len)
{
	uint8_t i = 0;
	for (i = 0; i < len; i++)
	{
		SSIDataGet(SSI1_BASE, &buf[i]);
	}
}

void write_reg(uint8_t reg, uint8_t* buf, uint8_t len)
{
	uint8_t i = 0;
	for (i = 0; i < len; i++)
	{
		SSIDataPut(SSI1_BASE, &buf[i]);
	}
	while (SSIBusy(SSI1_BASE))
	{
	}
}

void configure()
{

}

int main(void)
{
	while (1)
	{
	}
}
