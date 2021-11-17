/*
 * LCD.h
 *
 * Created: 11/10/2021 9:55:36 AM
 *  Author: ESMAEILY
 */ 


#ifndef LCD_H_
#define LCD_H_

#define WAIT_TYPE 1									//Checking busy flag = 1, Waiting a constant time = 0 
#define CONST_DELAY 2								//time in ms
#define RS 0
#define RW 1
#define E 2
#define LCD_DBUS_PORT PORTA
#define LCD_DBUS_DDRx DDRA
#define LCD_DBUS_PINx PINA
#define LCD_CBUS_PORT PORTB
#define LCD_CBUS_DDRx DDRB


void comnwrt(unsigned char command);
void datawrt(unsigned char data);
void dispdata(char a[]);
#if(WAIT_TYPE)
	void wait_flag();
#endif
void lcd_init();
void lprintf(const char *fmt, ...);
void lputunum(unsigned int v);
void lputnum(int v);
static void putdigit0(unsigned char c);
static void putdigit(unsigned char c, unsigned char *flag);
void lputhexbyte(unsigned int v);
void lputhex(unsigned int v);


#endif /* LCD_H_ */
