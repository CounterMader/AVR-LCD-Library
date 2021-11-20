/*
 * LCD.c
 *
 * Created: 11/10/2021 9:57:55 AM
 *  Author: ESMAEILY
 */ 
#include <avr/io.h>
#include <stdarg.h>
#include "LCD.h"
#if(!WAIT_TYPE)
	#include <util/delay.h>
	#ifndef F_CPU
	#define F_CPU 8000000UL
	#endif
#endif

void comnwrt(unsigned char command){
	LCD_DBUS_PORT = command;
	LCD_CBUS_PORT &= ~((1 << RS) | (1 << RW));
	LCD_CBUS_PORT |= (1 << E);
	LCD_CBUS_PORT &= ~(1 << E);
#if(WAIT_TYPE)
	wait_flag();
#else
	_delay_ms(CONST_DELAY);
#endif
}

void datawrt(unsigned char data){
	LCD_DBUS_PORT = data;
	LCD_CBUS_PORT &= ~(1 << RW);
	LCD_CBUS_PORT |= (1 << RS);
	LCD_CBUS_PORT |= (1 << E);
	LCD_CBUS_PORT &= ~(1 << E);
#if(WAIT_TYPE)
	wait_flag();
#else
	_delay_ms(CONST_DELAY);
#endif
}

void dispdata(char a[]){
	unsigned char i = 0;
	while(a[i] != '\0'){
		datawrt(a[i]);
		i++;
	}
}

#if(WAIT_TYPE)
void wait_flag(){
	LCD_DBUS_DDRx = 0;
	LCD_CBUS_PORT &= ~(1 << RS);
	LCD_CBUS_PORT |= (1 << RW);
	LCD_CBUS_PORT |= (1 << E);
	while(LCD_DBUS_PINx & 0x80){
		LCD_CBUS_PORT &= ~(1 << E);
		LCD_CBUS_PORT |= (1 << E);
	}
	LCD_CBUS_PORT &= ~(1 << E);
	LCD_CBUS_PORT &= ~(1 << RW);
	LCD_DBUS_DDRx = 0xFF;
}
#endif
void lcd_init(){
	LCD_DBUS_DDRx |= 0xFF;
	LCD_CBUS_DDRx |= (1 << RS) | (1 << RW) | (1 << E);
	comnwrt(0x3C);
	comnwrt(0x0F);
	comnwrt(0x01);
	//comnwrt(0x02);
}

void lprintf(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case '%':
				{
					dispdata(*fmt);
					fmt++;
					continue;
				}
				case 's':
				{
					char* str = va_arg(ap, char *);
					dispdata(str);
					fmt++;
					continue;
				}

				case 'c':
				{
					char c = va_arg(ap, int);
					datawrt(c);
					fmt++;
					continue;
				}
				#ifdef CONFIG_32BIT
				case 'p':
				fmt--;
				#endif
				case 'l': /* assume an x is following */
				{
					long l = va_arg(ap, unsigned long);
					/* TODO: not 32-bit safe */
					lputhex((unsigned short)(l >> 16));
					lputhex((unsigned short)l);
					fmt += 2;
					continue;
				}

				case '2': /* assume an x is following */
				{
					char c = va_arg(ap, int);
					lputhexbyte(c);
					fmt += 2;
					continue;
				}

				#ifndef CONFIG_32BIT
				case 'p':
				#endif
				case 'x':
				case 'd':
				case 'u':
				{
					unsigned int v = va_arg(ap, int);

					if (*fmt == 'x' || *fmt == 'p')
					lputhex(v);
					else if (*fmt == 'd')
					lputnum(v);
					else if (*fmt == 'u')
					lputunum(v);

					fmt++;
					continue;
				}
			}
		}
		datawrt(*fmt);
		fmt++;
	}

	va_end(ap);
}

void lputunum(unsigned int v)
{
	unsigned char n = 0;
	putdigit((v / 10000) % 10, &n);
	putdigit((v / 1000) % 10, &n);
	putdigit((v / 100) % 10, &n);
	putdigit((v / 10) % 10, &n);
	putdigit0(v % 10);
}

void lputnum(int v)
{
	if (v < 0) {
		datawrt('-');
		v = -v;
	}
	lputunum(v);
}

static void putdigit0(unsigned char c)
{
	datawrt("0123456789ABCDEF"[c & 15]);
}
static void putdigit(unsigned char c, unsigned char *flag)
{
	if (c || *flag) {
		*flag |= c;
		putdigit0(c);
	}
}
void lputhexbyte(unsigned int v)
{
	putdigit0(v >> 4);
	putdigit0(v);
}
void lputhex(unsigned int v)
{
	putdigit0(v >> 12);
	putdigit0(v >> 8);
	putdigit0(v >> 4);
	putdigit0(v);
}
