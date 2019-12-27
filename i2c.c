#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

void InitI2C0(){
	//enable i2c0 module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	
	//reset module
  SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
	
	//enable GPIO that contains I2C0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	
	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	
	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
	
	//clear I2C FIFOs
  HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

void I2CSendSingleByte(uint8_t slave_addr, char data){
	// Tell the master module what address it will place on the bus when
  // communicating with the slave.
	I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
	
	//put the data to be sent in the FIFO
	I2CMasterDataPut(I2C0_BASE, data);
	
	//Initialize sending data from the MCU
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	
	//wait while MCU is done transferring
	while(I2CMasterBusy(I2C0_BASE));
}

void I2CSendMultipleBytes(uint8_t slave_addr, uint8_t numOfBytes, char by[]){
	uint8_t i;
	// Tell the master module what address it will place on the bus when
  // communicating with the slave.
	I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
	
	//put the data to be sent in the FIFO
	I2CMasterDataPut(I2C0_BASE, by[0]);
	
	//Initialize sending data from the MCU
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	
	//wait while MCU is done transferring
	while(I2CMasterBusy(I2C0_BASE));
	for(i = 1; i < numOfBytes - 1; i++){
		//put next piece of data into I2C FIFO
    I2CMasterDataPut(I2C0_BASE, by[i]);
		//send next data that was just placed into FIFO
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     
    // Wait until MCU is done transferring.
    while(I2CMasterBusy(I2C0_BASE));
	}
	//put last byte of data
	I2CMasterDataPut(I2C0_BASE, by[numOfBytes - 1]);
	//send next data that was just placed into FIFO
  I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
  // Wait until MCU is done transferring.
  while(I2CMasterBusy(I2C0_BASE));
}
