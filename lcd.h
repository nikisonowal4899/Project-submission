#ifndef _LCD_H
#define _LCD_H

void lcd_init(); // LCD initialisation
void lcd_write(unsigned char, unsigned char); // will receive Data to be send and Register select
void latch_enable(); // a clock pulse for triggering Data// better to change with a tik_tok function
void lcd_print(unsigned char); // will receive memory address of the string as argument
void reset_screen();

#endif