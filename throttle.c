//******************************************************************************
// This program calibrates the ESCs. ESCs should be flashed with SimonK
// firmware. Throttle is calibrated to 1060us stop, 1860us full throttle, as
// described in https://github.com/sim-/tgy/.
// ESCs are attached to PB6 (PWM 0), PB7 (PWM1), PC4 (PWM6), and PC5 (PWM7).
// OE for the level shifter is active low, and attached to PA0.
//******************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

void init(void)
{
	//Set system clock to 80MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Set PWM clock source to 10MHz
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	//Enable PWM0, port B, and port C (and A)
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	//Setup pin multiplexer
	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinConfigure(GPIO_PB7_M0PWM1);
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinConfigure(GPIO_PC5_M0PWM7);

	//Set pin to PWM
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_3);

	//Count down (left aligned), no sync
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	//Set period to 4ms (Each count is 0.1us)
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 40000);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 40000);

	//Set all pulse widths to 0
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);
	
	//Enable generators 0 and 3 (controlling outputs 0, 1 and 6, 7)
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);

	//Enable pin outputs
	PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT), true);
	
	//Bring OE Line high
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_0, 0xFF);
}

int main(void)
{
	init();
	uint32_t val;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 10600);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 10600);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 10600);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 10600);
	SysCtlDelay(100000000);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
	for (val = 10600; val < 18640; val++)
	{
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, val);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, val);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, val);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, val);
		SysCtlDelay(2666);
	}
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 10600);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 10600);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, 10600);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 10600);
	/*for (val = 18640; val >= 10600; val--)
	{
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, val);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, val);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, val);
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, val);
		SysCtlDelay(26666);
	}*/
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0xFF);
	while(1)
	{
	}
}
