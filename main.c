#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"

#include "i2c.h"
#include "ssd1306.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void UARTPutString(uint32_t base, char *s){
	int i;
	for(i = 0; s[i] != '\0'; i++)
		UARTCharPut(base, s[i]);
}

void println(uint32_t base, char *s){
	//UARTCharPut(base, '\n');
	UARTPutString(base, s);
	UARTCharPut(base, '\n');
}

//void ATCommand(char *s){
//	int i;
void InitLeds(){
	// Setup the system clock to run at 50 Mhz from PLL with crystal reference
  SysCtlClockSet(SYSCTL_SYSDIV_1|SYSCTL_USE_OSC|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);
	// Enable and configure the GPIO port for the LED operation.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);
}

void InitUART0(){
	//init uart0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void InitUART2(){
	//init uart2
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTFIFODisable(UART2_BASE);
}



int main(void){
	InitLeds();
	InitI2C0();
	//GPIOPinWrite(GPIO_PORTF_BASE, RED_LED, RED_LED);
	InitializeDisplay();
	//GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, GREEN_LED);
	int i, j, n;
	clear_display();
	for(i = 0; i < 128; i++){
		for(j = 0; j < 32; j++){
			//sendCharXY('i', i, j);
			printBigNumber('2', i, j);
			for(n = 0; n < 1000000; n++);
		}
	}
	
	while(1){
	}
}
