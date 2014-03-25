#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

void init(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
}

int main(void)
{
	init();
	while(1)
	{
		SysCtlDelay(2666666);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
		SysCtlDelay(2666666);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
	}
}
