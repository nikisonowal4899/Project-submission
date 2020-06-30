#include "eeprom.h"
#include "i2c.h"
#include "delay.h"

#define EEPROM_ID 0xA0

void eeprom_write_byte(unsigned char address, unsigned char eeprom_data){
	i2c_start();
		i2c_write(EEPROM_ID);
	i2c_ack();
		i2c_write(address);
	i2c_ack();
		i2c_write(eeprom_data);
	i2c_ack();
		i2c_stop();
	delay(6);
}

void eeprom_write_string(unsigned char* string_ptr, unsigned char address) {
	unsigned char temp;
	unsigned char i=0;
	
	do {
		temp = string_ptr[i];
		eeprom_write_byte(address, temp);
		address++; i++;
	} while(temp != '\0');
}

unsigned char eeprom_read_byte(unsigned char address) {
	unsigned char eeprom_data;
	i2c_start();
		i2c_write(EEPROM_ID);
	i2c_ack();
		i2c_write(address);
	i2c_ack();
		
	i2c_start();
	i2c_write(EEPROM_ID | 0x01);
		i2c_ack();
	eeprom_data = i2c_read();
		i2c_no_ack();
		i2c_stop();
	delay(4);
	
	return eeprom_data;
	
}