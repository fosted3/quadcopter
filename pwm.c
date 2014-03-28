#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

void init_pwm(void)
{
	//Set PWM clock source to 10MHz
	SysCtlPWMClockSet(SYSCTL_PWMDIV_8);

	//Enable PWM0, port B, and port C
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	//Setup pin multiplexer
	GPIOPinConfigure(GPIO_PB6_M0PWM0);
	GPIOPinConfigure(GPIO_PB7_M0PWM1);
	GPIOPinConfigure(GPIO_PC4_M0PWM6);
	GPIOPinConfigure(GPIO_PC5_M0PWM7);

	//Set pin to PWM
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

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
}

void clamp(uint16_t l, uint16_t *v, uint16_t h)
{
	if (*v > h)
	{
		*v = h;
	}
	else if (*v < l)
	{
		*v = l;
	}
}

void set_pwm(uint16_t m0, uint16_t m1, uint16_t m2, uint16_t m3)
{
	//Error checking
	clamp(PWM_MIN, &m0, PWM_MAX);
	clamp(PWM_MIN, &m1, PWM_MAX);
	clamp(PWM_MIN, &m2, PWM_MAX);
	clamp(PWM_MIN, &m3, PWM_MAX);
	
	//Set PWM
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, m0);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, m1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6, m2);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, m3);
}
