#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

void init(void)
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
	ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
}

int main(void)
{
	init();
	while(1)
	{
		ROM_SysCtlDelay(2666666);
		ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
		ROM_SysCtlDelay(2666666);
		ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
	}
}
