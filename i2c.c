#include <reg51.h>
#include "i2c.h"
#include "delay.h"

sbit scl = P2^3;
sbit sda = P2^2;
sbit a7  = B^7;

void i2c_clock(){
	delay_us(2);
	scl = 1;
	delay_us(2);
	scl = 0;
}

void i2c_start(){
	scl = 0;
	sda = 1;
	delay_us(2);
	scl = 1;
	delay_us(2);
	sda = 0;
	delay_us(2);
	scl = 0;
}
void i2c_stop(){
	scl = 0;
	delay_us(2);
	sda = 0;
	delay_us(2);
	scl = 1;
	delay_us(2);
	sda = 1;
}

void i2c_write(unsigned char dat){
	unsigned char i;
	B = dat; // save the dat on B Register
	
  for(i=0; i<8; i++) {
		sda = a7;
		i2c_clock();
		B = B<<1;
  } 
  sda = 1;
}

unsigned char i2c_read(){
	unsigned char i;
	sda = 1; //sda as Input;
	B = 0;
	for(i=0; i<8; i++){
		delay_us(2);
		scl = 1;
		delay_us(3);
		B = B<<1;
		B = B | sda;
		scl = 0;
	}
	sda = 0;
	return B;
}
void i2c_ack(){
	sda = 0;
	i2c_clock();
	sda = 1;
}

void i2c_no_ack(){
	sda = 1;
	i2c_clock();
	scl = 1;
}