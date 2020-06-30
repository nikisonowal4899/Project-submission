#include <reg51.h>
#include "lcd.h"
#include "delay.h"
#include "eeprom.h"

sbit rs = P2^0;
sbit en = P2^1;
sbit d4 = P2^4;
sbit d5 = P2^5;
sbit d6 = P2^6;
sbit d7 = P2^7;
//LCD hardware connection //

sbit a0 = B^0; 
sbit a1 = B^1;
sbit a2 = B^2;
sbit a3 = B^3;
sbit a4 = B^4;
sbit a5 = B^5;
sbit a6 = B^6;
sbit a7 = B^7;
//B Register variables

void lcd_init() { //function for initialization of the LCD
	
	delay(5); // some long delay until the lcd powers up
	lcd_write(0x33, 0); // set lcd to 8-Bit mode initially
	delay(2); // some delay
	lcd_write(0x32, 0); // function set to 8-Bit mode again
	delay(2); // some delay
	lcd_write(0x28, 0); // set to 4-Bit mode
	delay(2);
	lcd_write(0x0c, 0); //display ON, cursor OFF
	delay(1);
	lcd_write(0x06, 0); //set entry mode to Auto increment
	delay(1);
}

void lcd_write(unsigned char Data, unsigned char select_reg) {
	rs = select_reg;
	
	B = Data; // storing the Data in the B Register
	
	//First send the upper Nibble
	d4 = a4;
	d5 = a5;
	d6 = a6;
	d7 = a7;
	delay(1); // little delay before firing the enable pulse
	latch_enable();
	
	//send the lower nibble
	d4 = a0;
	d5 = a1;
	d6 = a2;
	d7 = a3;
	delay(1); // little delay before firing the enable pulse
	latch_enable();
	
	delay(1); //some delay before returning
}

void lcd_print(unsigned char eeprom_address) {
	// Any string to be printed is to be read from the external memory compulsorily
	// the fucntion receives the address of the beginning byte of the string
	// a string cannot be longer than 16 characters
	// all unsigned characters
		
	unsigned char temp1, i;
	
	for(i=0; i<16; i++) {
		temp1 = eeprom_read_byte(eeprom_address);
		if(temp1 == '\0') break;
		lcd_write(temp1, 1);
		eeprom_address++;
	}
}

void latch_enable() {
	en = 1;
	delay_us(1); //delay
	en = 0;
	delay_us(1); //delay
}

void reset_screen() {
	lcd_write(0x01, 0); // clear screen
	delay(2);
	lcd_write(0x02, 0); // go to home pos
	delay(2);
}