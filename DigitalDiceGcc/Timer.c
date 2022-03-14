/* 
* Timer.cpp
*
* Created: 2022-01-05 18:54:58
* Author: JakubLeśniak
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Timer.h"
#include "mpu6050v2.h"

volatile int intr_count = 0;

ISR (TIMER0_OVF_vect)      //Interrupt vector for Timer0
{
	intr_count++;
	TCNT0 = 0x00;
}


void initTimer()
{
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TCCR0B |= (1<<CS00)|(1<<CS02);
	sei();
	TCNT0=0;
	TIMSK|=(1<<TOIE0);
}

float getSeconds()
{
	return (intr_count * 63.18)/3.0;
}