/*
 * max7219.c
 *
 * Created: 2021-11-01 15:54:21
 *  Author: JakubLeśniak
 */ 
//#include <stdint.h>
#include <avr/pgmspace.h>
//#include <avr/io.h>
#include <util/delay.h>

#include "max7219.h"

#define NUM_MEM_SIZE 8*10

uint8_t const NUM_MEM[] PROGMEM = { 0, 0, 60, 66, 66, 60, 0, 0,
									0, 0, 0, 4, 126, 0, 0, 0,
									0, 0, 100, 82, 74, 68, 0, 0,
									0, 0, 36, 66, 74, 52, 0, 0,
									0, 0, 16, 24, 116, 18, 0, 0,
									0, 0, 36, 74, 74, 50, 0, 0,
									0, 0, 56, 84, 82, 32, 0, 0,
									0, 0, 68, 34, 18, 12, 0, 0,
									0, 0, 52, 74, 74, 52, 0, 0,
									0, 0, 36, 74, 74, 52, 0, 0};


#define DISPLAY_SIZE 8

uint8_t numberMemory[DISPLAY_SIZE];

void setup_SPI(void)
{
	#if  defined(__ATtiny85__)
		DDRB = (1<<PB1)|(1<<PB2)|(1<<PB3);
		PORTB &= ~((1 << PB0)|(1 << PB1)|(1 << PB2));
		USICR = (1<<USIWM0);
	#endif
	
	#if defined(__AVR_ATmega8A__)||defined(__AVR_ATmega8__)
		 // SCK MOSI CS/LOAD/SS
		DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI);
		DDRD | (1 << PIN_SS_1) | (1 << PIN_SS_2);
		 // SPI Enable, Master mode
		 SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);
	#endif
}

void spiSendByteMax7219(unsigned char databyte)
{
	#if defined(__AVR_ATmega8A__)||defined(__AVR_ATmega8__)
		// Copy data into the SPI data register
		SPDR = databyte;
		// Wait until transfer is complete
		while (!(SPSR & (1 << SPIF)));
	#endif
}
/*
void writeDataMax7219(unsigned char data_register,unsigned char data)
{
	//setup_SPI();
	//MAX7219_LOAD0;
	//#define MAX7219_LOAD1           PORTB |= (1<<PIN_SS)
	//#define MAX7219_LOAD0           PORTB &= ~(1<<PIN_SS)
	PORTB &= ~(1<<PIN_SS);
	
	if (device == 0)
	{
		PORTD &= ~(1<<PIN_SS_1);
	}
	else
	{
		PORTD &= ~(1<<PIN_SS_2);
	}
	
	
	//while ((PINB & (1 << PIN_SS)));
	spiSendByteMax7219(data_register);
	// Send the data to be stored
	spiSendByteMax7219(data);
	//MAX7219_LOAD1;
	
	PORTB |= (1<<PIN_SS);
	if (device == 0)
	{
		PORTD |= (1<<PIN_SS_1);
	}
	else
	{
		PORTD |= (1<<PIN_SS_2);
	}
	
	//while (!(PINB & (1 << PIN_SS)));
}
*/
void writeDataMax7219OnDevice(unsigned char data_register, char unsigned data, uint8_t device)
{
	#if defined(__ATtiny85__)
		setup_SPI();
	#endif
	
	//Clear BUS from all data.
	PORTB &= ~(1 << PIN_MOSI);
	
	//Start SS
	PORTB &= ~(1<<PIN_SS);
	if(device == 0)
	{
		//Set device connected to SS_2 to inactive.
		PORTD |= (1 << PIN_SS_2);
		//Wait for pin to go high
		while (!(PIND & (1 << PIN_SS_2)));
		//Select device connected to SS_1
		PORTD &= ~(1<<PIN_SS_1);
	}
	else
	{
		//Set device connected to SS_2 to inactive.
		PORTD |= (1 << PIN_SS_1);
		//Wait for pin to go high
		while (!(PIND & (1 << PIN_SS_1)));
		//Select device connected to SS_2
		PORTD &= ~(1<<PIN_SS_2);
	}

	//while ((PINB & (1 << PIN_SS)));
	spiSendByteMax7219(data_register);
	// Send the data to be stored
	spiSendByteMax7219(data);
	//MAX7219_LOAD1;
	
	//Set all devices to inactive
	PORTB |= (1<<PIN_SS);
	PORTD |= (1<<PIN_SS_1);
	PORTD |= (1<<PIN_SS_2);
	PORTB &= ~(1 << PIN_MOSI);

}
/*
void writeDataMax7219OnDeviceTwo(unsigned char data_register, char unsigned data)
{
	//setup_SPI();
	//MAX7219_LOAD0;
	//#define MAX7219_LOAD1           PORTB |= (1<<PIN_SS)
	//#define MAX7219_LOAD0           PORTB &= ~(1<<PIN_SS)
	
	PORTB &= ~(1 << PIN_MOSI);
	
	PORTB &= ~(1<<PIN_SS);
	PORTD &= ~(1<<PIN_SS_2);
	
	//while ((PINB & (1 << PIN_SS)));
	spiSendByteMax7219(data_register);
	// Send the data to be stored
	spiSendByteMax7219(data);
	//MAX7219_LOAD1;
	
	PORTB |= (1<<PIN_SS);
	PORTD |= (1<<PIN_SS_2);
	
	PORTB &= ~(1 << PIN_MOSI);

}
*/
void setColMax7219(uint8_t colno, uint8_t value, uint8_t device) 
{
	writeDataMax7219OnDevice(MAX7219_COLUMN_ZERO + colno, value, device);
}

void clearMax7219(uint8_t device)
{
	for (uint8_t a = 0; a < 3; a++) {
		for (uint8_t i = 0; i < 8; i++) {
			writeDataMax7219OnDevice(MAX7219_COLUMN_ZERO + i, 0, device);
		}
	}
}
/*
void clearMax7219Two(void)
{
	for (uint8_t a = 0; a < 3; a++) {
		for (uint8_t i = 0; i < 8; i++) {
			writeDataMax7219OnDevice(MAX7219_COLUMN_ZERO + i, 0);
		}
	}
}
*/
void fillMax7219(uint8_t device)
{
	//for (uint8_t a = 0; a < 3; a++) {
		for (uint8_t i = 0; i < 8; i++) {
			//writeDataMax7219(MAX7219_COLUMN_ZERO + i, 0b1111111, device);
			writeDataMax7219OnDevice(MAX7219_COLUMN_ZERO + i, 0b1111111, device);
		}
	//}
}

void blinkMax7219(uint8_t device)
{
	fillMax7219(device);
	_delay_ms(100);
	clearMax7219(device);
}

void initMax7219(uint8_t device) 
{
		clearMax7219(device);
		
		writeDataMax7219OnDevice(MAX7219_SHUTDOWN, 1,device);
		writeDataMax7219OnDevice(MAX7219_DECODE_MODE, 0,device);
		writeDataMax7219OnDevice(MAX7219_SCAN_LIMIT, 7,device);
		writeDataMax7219OnDevice(MAX7219_INTENSITY, 1,device);
		writeDataMax7219OnDevice(MAX7219_DISPLAYTEST, 0,device);

		_delay_ms(20);
}
/*
void initMax7219Two(void)
{
	clearMax7219Two();
	
	writeDataMax7219OnDeviceTwo(MAX7219_SHUTDOWN, 1);
	writeDataMax7219OnDeviceTwo(MAX7219_DECODE_MODE, 0);
	writeDataMax7219OnDeviceTwo(MAX7219_SCAN_LIMIT, 7);
	writeDataMax7219OnDeviceTwo(MAX7219_INTENSITY, 1);
	writeDataMax7219OnDeviceTwo(MAX7219_DISPLAYTEST, 0);
}
*/

void blankNumber(void)
{
	for(uint8_t i = 0; i < DISPLAY_SIZE; i++)
		numberMemory[i] = 0;
}

void readNumberFromMemory(uint8_t number)
{
	uint8_t memoryIterator = number * 8;
	uint8_t numberIterator = 0;
	
	blankNumber();
	
	while(numberIterator < 8 && memoryIterator < NUM_MEM_SIZE)
	{
		numberMemory[numberIterator] = pgm_read_byte(&NUM_MEM[memoryIterator]);
		numberIterator++;
		memoryIterator++;
	}
}

void displayNumberFromMemory(uint8_t number, uint8_t device)
{
	if(number*8 >= NUM_MEM_SIZE)
	{
		fillMax7219(device);
		return;
	}
	
	//clearMax7219();
	readNumberFromMemory(number);
	for(uint8_t c = 0; c < DISPLAY_SIZE; c++)
	{
		setColMax7219(c, numberMemory[c], device);
		//setColMax7219(c, numberMemory[c]);
		//setColMax7219(c, numberMemory[c]);
	}
		
}

uint8_t POW(uint8_t x, uint8_t y)
{
	uint8_t tmp = 1;
	if(y >= 1)
	{
		tmp = x;
		for(uint8_t i = 1; i < y; i++)
		{
			uint8_t tmpS = tmp * x;
			if(tmpS > 0)
			tmp = tmpS;
		}
	}
	
	return tmp;
}

void displayValue(int value, uint8_t device)
{
	clearMax7219(device);
	int tmpValue = abs(value);
	//if(value < 0) tmpValue = tmpValue * (-1);
	
	for (int n = 7; n >= 0; n--)
	{
		int x = pow(10, n);
		int tmp = 0;
		if(tmpValue > x && x >= 0)
		{
			tmp = tmpValue/(x + 0.1);
			tmpValue = tmpValue%x;
		}
		if(tmp < 9 && tmp !=0)
		{
			setColMax7219(7 - n,(uint8_t)(1 << 8 -(tmp)), device);
		}
		else if (tmp != 0)
		{
			setColMax7219(7 - n, 0b10000001, device);
		}
	}
	if(value < 0)
	{
		setColMax7219(0, 0xFF, device);
	}
}