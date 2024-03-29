#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
//#include "nrf_defs.h"

void init(void)
{
	uint32_t temp;
	//Set system clock to 80MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Enable SSI1, port D, and port E
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	//Configure pin multiplexing
	GPIOPinConfigure(GPIO_PD0_SSI1CLK);
	//GPIOPinConfigure(GPIO_PD1_SSI1FSS);
	GPIOPinConfigure(GPIO_PD2_SSI1RX);
	GPIOPinConfigure(GPIO_PD3_SSI1TX);

	//Set NRF_EN as output, NRF_IRQ as input
	//GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4);
	//GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);

	//Configure port D for SSI
	GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1);

	//Configure SSI settings (1MHz SCK, Polarity & Phase 0, 8 bits / frame
	SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);

	//Enable SSI1
	SSIEnable(SSI1_BASE);
	while(SSIDataGetNonBlocking(SSI1_BASE, &temp))
    {
    }
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
}

int main(void)
{
	init();
	//char *pcChars = "SPI";
	uint8_t i;
	while(1)
	{
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
		for (i = 0; i < 3; i++)
		{
			SSIDataPut(SSI1_BASE, 0xA0);
		}
		while(SSIBusy(SSI1_BASE))
    	{
    	}
    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0xFF);
    	SysCtlDelay(26666);
	}
}
