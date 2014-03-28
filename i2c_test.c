#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_i2c.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
//#include "mpl3115a2.h"
//#include "mpu9150.h"
#include "i2c.h"

/*void uart_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}*/

void main_init(void)
{
	//Set system clock to 80MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	
	//Enable GPIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
}

void init()
{
	main_init();
	i2c_init();
	//uart_init();
}

int main(void)
{
	init();
	uint8_t data;
	uint8_t addr = 0xA0;
	uint8_t reg = 0x00;
	while (1)
	{
		for (reg = 0; reg < 8; reg++)
		{
			i2c_write(addr, reg, 0xAA);
			//UARTprintf("Register %i reads %i\n", reg, data);
			SysCtlDelay(1000);
		}
		//SysCtlDelay(10000);
	}
	return 0;
}
