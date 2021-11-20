#define F_CPU 1000000UL
#include <avr/io.h>
#include "LCD.h"

int main(){
	lcdinit();
	lprintf("%%");
	while(1);
	return 0;
}
