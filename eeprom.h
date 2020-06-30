#ifndef _EEPROM_H
#define _EEPROM_H

void eeprom_write_byte(unsigned char, unsigned char);
unsigned char eeprom_read_byte(unsigned char);
void eeprom_write_string(unsigned char*, unsigned char);

#endif