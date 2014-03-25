//******************************************************************************
// This program tests the ADC functionality of the board. There is a 74HC4051
// analog mux that multiplexes 8 analog signals onto AIN0 (attached to PE3)
// OE enables output, and S0:2 switch signals. OE, S0, S1, and S2 are mapped to 
// PA0, PA6, PF0, and PA7 respectively. OE is active low.
//******************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

void init(void)
{
	//Set system clock to 80MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	
	//Enable ADC0, port A, port E, and port F
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	//Set A0, A6, A7, and F0 as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	
	//Set E3 as AIN
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	
	//Setup ADC paramaters (single sample, single ended, sequence 3(?), cpu interrupt)
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	
	//Clear interrupt
	ADCIntClear(ADC0_BASE, 3);
	
	//Write low to all outputs
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_6 | GPIO_PIN_7, 0x00);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);
}

uint32_t adc_sample(void)
{
	uint32_t rv[1];
	ADCProcessorTrigger(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE, 3, false))
	{
	}
	ADCIntClear(ADC0_BASE, 3);
	ADCSequenceDataGet(ADC0_BASE, 3, rv);
	return rv[0];
}

uint32_t read_channel(uint32_t chan)
{
	// S2 S1 S0
	// A7 F0 A6
	// port a
	// ((chan | 0x01) << 6) | ((chan | 0x02) << 5)
	// port f
	// ((chan | 0x04) >> 1)
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, ((chan | 0x01) << 6) | ((chan | 0x02) << 5));
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, (chan | 0x04) >> 1);
	SysCtlDelay(50); //stabilize that shit
		
}

int main(void)
{
}
