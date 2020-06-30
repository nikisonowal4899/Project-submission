#include <reg51.h>
#include <stdlib.h>
#include "lcd.h"
#include "delay.h"
#include "eeprom.h"

sbit ko1 = P3^0;
sbit ko2 = P3^1;
sbit ko3 = P3^2;
sbit ko4 = P3^3;

sbit ki1 = P1^0;
sbit ki2 = P1^1;
sbit ki3 = P1^2;
sbit ki4 = P1^3;
// keyboard connections

sbit motor_an = P3^4;
sbit motor_ct = P3^5;
// motor driver circuit connection

//Global variables
#define pw_address 0xE0

unsigned char keyboard_data[13];
unsigned char temp_array[13];
unsigned char data_index; // pointer to the position of the keyboard_data array
unsigned char screen_flag; // tells the programm about the current screen
unsigned char security_breach; // no. of times incorrect attempts have been made 

//Functions
void keyboard_scan();
void keyboard_input(unsigned char);
void home_screen();
unsigned char match_password();
unsigned char confirm_password();
void motor_fn(unsigned char);

// -*-*-*-*-* MAIN FUNCTION START -*-*-*-*-*-*-*- //
void main() {
	P3 = 0x00; // OUTPUT
	P2 = 0x00; // Port 2 as OUTPUT to LCD
	P1 = 0xFF; // keyboard input
	
	lcd_init();
	home_screen();
	
	for(data_index = 0; data_index < 13; data_index++) {
		temp_array[data_index] = '\0';
		keyboard_data[data_index] = '\0';
	}
	
	data_index = 0;
	motor_fn('c');
	
	while(1) {
		keyboard_scan();
	}
	 
}
// -*-*-*-*-* MAIN FUNCTION END -*-*-*-*-*-*-*- //

void keyboard_scan() {
	// make each output lines low one by one
	
	ko1 = 0; ko2 = 1; ko3 = 1; ko4 = 1; // make row 1 low
	if(ki1 == 0) { // '1' is pressed
		delay(220);
		keyboard_input('1');
		delay(750);
		while(ki1 == 0);
	}
	if(ki2 == 0) { // '2' is pressed
		delay(220);
		keyboard_input('2');
		delay(750);
		while(ki2 == 0);
	}
	if(ki3 == 0) { // '3' is pressed
		delay(220);
		keyboard_input('3');
		delay(750);
		while(ki3 == 0);
	}
	if(ki4 == 0) { // 'A' is pressed
		delay(220);
		keyboard_input('A');
		delay(750);
		while(ki4 == 0);
	}
	// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
	
	ko1 = 1; ko2 = 0; ko3 = 1; ko4 = 1; // make row 2 low
	if(ki1 == 0) { // '4' is pressed
		delay(220);
		keyboard_input('4');
		delay(750);
		while(ki1 == 0);
	}
	if(ki2 == 0) { // '5' is pressed
		delay(220);
		keyboard_input('5');
		delay(750);
		while(ki2 == 0);
	}
	if(ki3 == 0) { // '6' is pressed
		delay(220);
		keyboard_input('6');
		delay(750);
		while(ki3 == 0);
	}
	if(ki4 == 0) { // 'B' is pressed // 'B' will be used as Display password
		delay(220);
		keyboard_input('B');
		delay(750);
		while(ki4 == 0);
		keyboard_input('b');
	}
	// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
	
	ko1 = 1; ko2 = 1; ko3 = 0; ko4 = 1; // make row 3 low
	if(ki1 == 0) { // '7' is pressed
		delay(220);
		keyboard_input('7');
		delay(750);
		while(ki1 == 0);
	}
	if(ki2 == 0) { // '8' is pressed
		delay(220);
		keyboard_input('8');
		delay(750);
		while(ki2 == 0);
	}
	if(ki3 == 0) { // '9' is pressed
		delay(220);
		keyboard_input('9');
		delay(750);
		while(ki3 == 0);
	}
	if(ki4 == 0) { // 'C' is pressed
		delay(220);
		keyboard_input('C');
		delay(750);
		while(ki4 == 0);
	}
	// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
	
	ko1 = 1; ko2 = 1; ko3 = 1; ko4 = 0; // make row 4 low
	if(ki1 == 0) { // '*' is pressed
		delay(220);
		keyboard_input('*');
		delay(750);
		while(ki1 == 0);
	}
	if(ki2 == 0) { // '0' is pressed
		delay(220);
		keyboard_input('0');
		delay(750);
		while(ki2 == 0);
	}
	if(ki3 == 0) { // '#' is pressed
		delay(220);
		keyboard_input('#');
		delay(750);
		while(ki3 == 0);
	}
	if(ki4 == 0) { // 'D' is pressed
		delay(220);
		keyboard_input('D');
		delay(750);
		while(ki4 == 0);
	}
}

void keyboard_input(unsigned char dat) {
	
	if(dat == '#') { // Enter is pressed // Enter key will have many purpose		
		if(data_index != 0 && screen_flag == 'H') { // Password matching purpose 
			unsigned char check = match_password();
			reset_screen();
			
			if(check == '1') {
				lcd_print(0x10); // password match
				motor_fn('f'); // opening the door
				delay(9000);
				motor_fn('c'); // motor stopped // Now the door is in open position // door is to be closed manually by pressing 'A' on keyboard
				screen_flag = 'T'; // door open position
			} else if(check == '0') {
				lcd_print(0x20); // wrong password
				screen_flag = 'X';
			} 
		}
		else if(screen_flag == 'C') { // change password Menu
				reset_screen();
				lcd_print(0x30); // old password?			
				lcd_write(0xC0, 0);
				lcd_print(0xC0); // arrow
				lcd_write(0x0F, 0); // cursor on blinking on
				screen_flag = 'O';
			} 
		else if(screen_flag == 'O' && data_index != 0) {
				if(match_password() == '1') { // old password entered is correct, proceed to enter New password
					reset_screen();
					lcd_print(0x40); //New password
					lcd_write(0xC0, 0);
					lcd_print(0xC0); //Arrow
					lcd_write(0x0F, 0); // cursor on blinking on
					screen_flag = 'N';
					for(data_index=0; data_index<13; data_index++) keyboard_data[data_index] = '\0'; // clearing the keyboard_data array
					data_index = 0;
				} else {
					reset_screen();
					lcd_write(0x0C, 0); // cursor off blinking off
					lcd_print(0x20); // wrong password
					lcd_write(0xC0, 0);
					lcd_print(0x50); // Go to home
					screen_flag = 'W';
				}
			} 
		else if(screen_flag == 'N') {
				unsigned char i;
				reset_screen();
				lcd_print(0x60); // conf. password
				lcd_write(0xC0, 0);
				lcd_print(0xC0); // arrow
		 		
				//copy Data from keyboard_array to temp_array for confirming purpose later
				for(i=0; i< data_index; i++) {
					temp_array[i] = keyboard_data[i];
				}
				data_index = 0;
				screen_flag = 'F';
			} 
		else if(screen_flag == 'F') {
			unsigned char temp_address = pw_address;
				if(confirm_password() == '1') { //password matched// change the password
					reset_screen();
					
					eeprom_write_string(keyboard_data, pw_address); // storing the new password in EEPROM
					
					lcd_write(0x0C, 0); // cursor off blinking off
					lcd_print(0x70); // Done !!
					lcd_write(0xC0, 0);
					lcd_print(0x80); // press Home
					screen_flag = 'J'; // J for Null function
				} else {
					reset_screen();
					lcd_write(0x0C, 0); // cursor off blinking off
					lcd_print(0xD0); // incorrect
					lcd_write(0xC0, 0);
					lcd_print(0x80); // press home
					screen_flag = 'J'; // Null function
				}
				for(data_index=0; data_index<13; data_index++) temp_array[data_index] = '\0'; // clearing the temp array
			}
			return ;
	} 
	
	else if(dat == 'A') { // close the door if it is already open
		if(screen_flag == 'T') {
			motor_fn('r');
			delay(9000);
			motor_fn('c');
			home_screen();
		}		
	}
	
	else if((dat == 'B' || dat == 'b') && (screen_flag == 'H' || screen_flag == 'O' || screen_flag == 'N' || screen_flag == 'F')){
		unsigned char index;
		if(dat == 'B') { // show passwod
			lcd_write(0xC4, 0); // set cursor to the beginning
			for(index=0; index<data_index; index++) lcd_write(keyboard_data[index], 1);
		}
		else {
			lcd_write(0xC4, 0); // set cursor to the beginning 
			for(index=0; index<data_index; index++) lcd_write(0xC8, 1); // print katakana 'ne'
		}
		return;
	}

	else if(dat == 'D' && screen_flag != 'T') { // Home key is pressed  
		home_screen();
		
		return ;
	} 
		
	else if(dat == '*') { // Backspace is pressed
		if(data_index > 0 && (screen_flag == 'H' || screen_flag == 'O' || screen_flag == 'N' || screen_flag == 'F')) {
			lcd_write(0x10, 0); //Moving LCD cursor backward
			lcd_write(' ', 1); // Printing a Space
			lcd_write(0x10, 0); // Moving cursor backward again
			data_index--;
		}	
		return ;
	} 
	
	else if(dat == 'C') { // Setting Menu is Called // Setting menu can be called only from Home screen
		if(screen_flag == 'H') {
			reset_screen();
			lcd_write(0x0C, 0); // Cursor off Blinking off
			lcd_print(0x90); // change password
			lcd_write(0xC0, 0); // Go to second row
			lcd_print(0xA0); //press enter
			screen_flag = 'C';
			data_index = 0;
		}		
		return ;
	}
	
	else {
		if(data_index < 12 && (screen_flag == 'H' || screen_flag == 'O' || screen_flag == 'N' || screen_flag == 'F')) { // Store the input in Keyboard Array
			lcd_write(0xC8, 1); // katakana 'Ne' character
			keyboard_data[data_index] = dat;
			data_index++;
		}
	}
}

unsigned char match_password() {
	unsigned char password_array[13];	
	unsigned char temp_address = pw_address, temp, index = 0;
	
	do { //copying the password from the EEPROM
		temp = eeprom_read_byte(temp_address);
		password_array[index] = temp;
		index++; temp_address++;
	} while(temp != '\0'); 
	index--; // index is the Size of the password array
	
	if(index == data_index) {
		for(index=0; index<data_index; index++) {
			if(keyboard_data[index] != password_array[index]) return '0';
		}
		return '1';
	} else return '0';
}

unsigned char confirm_password() {
	unsigned char i=0;
	while(temp_array[i] != '\0') i++; //Acquiring Size of temp array
	if(i == data_index) {
		for(i=0; i<data_index; i++) { // Matching each character
			if(keyboard_data[i] != temp_array[i]) return '0';
		}
		return '1';
	} else return '0';
}

void home_screen() { // the default lcd screen
	unsigned char i;
	for(i=0; i<13; i++) { // clearing the keyboard Data array
			keyboard_data[i] = '\0';
		}
	data_index = 0;
	reset_screen();
	lcd_print(0xB0); // enter password
	lcd_write(0xC0, 0); //Jump to second line position 0
	lcd_print(0xC0); // arrow
	lcd_write(0x0F, 0); // cursor ON blinking ON
	screen_flag = 'H';	
}	

void motor_fn(unsigned char fn) {
	if(fn == 'f') { //motor forward direction
		motor_an = 1;
		motor_ct = 0;
	} else if (fn == 'r') { // motor reverse direction
		motor_an = 0;
		motor_ct = 1;
	} else if (fn == 'c') { // motor turn off
		motor_an = 0;
		motor_ct = 0;
	}
}