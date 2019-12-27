#include "ssd1306.h"
#include "i2c.h"
#include <stdint.h>
#include "data.h"

#define slaveAddress 0x3c

void ssd1306_command(uint8_t c){
	uint8_t control = 0x00;
	char arr[2] = {control, c};
//	I2CSendSingleByte(slaveAddress, control);
//	I2CSendSingleByte(slaveAddress, c);
	I2CSendMultipleBytes(slaveAddress, 2, arr);
}

void  ssd1306_data(uint8_t c){
	char arr[2] = {0x40, c};
//	I2CSendSingleByte(slaveAddress, 0x40);
//	I2CSendSingleByte(slaveAddress, c);
	I2CSendMultipleBytes(slaveAddress, 2, arr);
}

void setColAddress(){
	ssd1306_command(SSD1306_COLUMNADDR); // 0x21 COMMAND
	ssd1306_command(0); // Column start address
	ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address
}

void setPageAddress(){
	ssd1306_command(SSD1306_PAGEADDR); // 0x22 COMMAND
	ssd1306_command(0); // Start Page address
	ssd1306_command((SSD1306_LCDHEIGHT/8)-1);// End Page address
}

void  InitializeDisplay(){
	// Init sequence for 128x64 OLED module
	ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE

	ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	ssd1306_command(0x80);                 // the suggested ratio 0x80
	
	ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
	ssd1306_command(0x3F);
	
	ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	ssd1306_command(0x0);                                   // no offset
	
	ssd1306_command(SSD1306_SETSTARTLINE);// | 0x0);        // line #0
	
	ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
	ssd1306_command(0x14);  // using internal VCC
	
	//ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
	//ssd1306_command(0x00);          // 0x00 horizontal addressing
	
	ssd1306_command(SSD1306_SEGREMAP | 0x1); // rotate screen 180
	
	//ssd1306_command(SSD1306_COMSCANDEC); // rotate screen 180
	
	ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
	ssd1306_command(0x12);
	
	ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
	ssd1306_command(0xCF);
	
	ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
	ssd1306_command(0xF1);
	
	ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
	ssd1306_command(0x40);
	
	ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
	
	ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
	
	ssd1306_command(SSD1306_DISPLAYON);                     //switch on OLED
}

void displayOn(void){
	ssd1306_command(0xaf);        //display on
}

void displayOff(void){
	ssd1306_command(0xae);		//display off
}

void reset_display(void){
	displayOff();
	clear_display();

	
	displayOn();
}

void clear_display(void)
{
	unsigned char i,k;
	for(k=0;k<8;k++)
	{
		setXY(k,0);
		{
			for(i=0;i<128;i++)     //clear all COL
			{
				SendChar(0);         //clear all COL
				//delay(10);
			}
		}
	}
}

void SendChar(unsigned char data){
	char arr[2] = {0x40, data};
//	I2CSendSingleByte(slaveAddress, 0x40);
//	I2CSendSingleByte(slaveAddress, data);
	I2CSendMultipleBytes(slaveAddress, 2, arr);
}

void setXY(unsigned char row,unsigned char col){
	ssd1306_command(0xb0+row);                //set page address
	ssd1306_command(0x00+(8*col&0x0f));       //set low col address
	ssd1306_command(0x10+((8*col>>4)&0x0f));  //set high col address
}

//==========================================================//
// Prints a display char (not just a byte) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15)
// and 8 ROWS (0-7).
void sendCharXY(unsigned char data, int X, int Y)
{
	setXY(X, Y);
	char arr[9];
	uint8_t i;
	arr[0] = 0x40;
	
	for(i = 0; i < 8; i++)
		arr[i + 1] = myFont[data - 0x20][i];
	
	I2CSendMultipleBytes(slaveAddress, 9, arr);
}

//==========================================================//
// Prints a display big number (96 bytes) in coordinates X Y,
// being multiples of 8. This means we have 16 COLS (0-15)
// and 8 ROWS (0-7).
void printBigNumber(char string, int X, int Y)
{
	setXY(X,Y);
	int salto=0;
	for(int i = 0; i < 96; i++)
	{
		if(string == ' ') {
			SendChar(0);
		} else
		SendChar(bigNumbers[string-0x30][i]);
		
		if(salto == 23) {
			salto = 0;
			X++;
			setXY(X,Y);
			} else {
			salto++;
		}
	}
}
