/*
 * max7219.h
 *
 * Created: 2021-11-01 15:54:36
 *  Author: JakubLeśniak
 */ 



#ifndef MAX7219_H_
#define MAX7219_H_

//#include <stdint.h>

#define SCL_CLK 100000L
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

#if defined(__AVR_ATmega8A__)||defined(__AVR_ATmega8__)
#define PIN_MOSI PORTB3
#define PIN_SS PORTB2
#define PIN_SS_1 PORTD6
#define PIN_SS_2 PORTD7
#define PIN_SCK PORTB5
#endif

#if defined(__ATtiny85__)
#define PIN_MOSI PORTB0
//Can be inncorect, copy error
#define PIN_SS PORTB3
#define PIN_SCK PORTB2
#endif

#define MAX7219_COLUMN_ZERO		0x01
#define MAX7219_DECODE_MODE		0x09
#define MAX7219_INTENSITY		0x0a
#define MAX7219_SCAN_LIMIT		0x0b
#define MAX7219_SHUTDOWN		0x0c
#define MAX7219_DISPLAYTEST		0x0f

#define ON                        1
#define OFF                       0

#define MAX7219_LOAD1           PORTB |= (1<<PIN_SS)
#define MAX7219_LOAD0           PORTB &= ~(1<<PIN_SS)

#define MAX7219_COLUMN_ZERO		0x01
#define MAX7219_DECODE_MODE		0x09
#define MAX7219_INTENSITY		0x0a
#define MAX7219_SCAN_LIMIT		0x0b
#define MAX7219_SHUTDOWN		0x0c
#define MAX7219_DISPLAYTEST		0x0f


void initMax7219(uint8_t device);
void setup_SPI(void);
void spiSendByteMax7219(unsigned char databyte);
void writeDataMax7219(unsigned char data_register, char unsigned data);
void writeDataMax7219OnDevice(unsigned char data_register, char unsigned data, uint8_t device);
//void writeDataMax7219OnDeviceTwo(unsigned char data_register, char unsigned data);
void setColMax7219(uint8_t colno, uint8_t value, uint8_t device);
void clearMax7219(uint8_t device);
//void clearMax7219Two(void);
void fillMax7219(uint8_t device);
void blinkMax7219(uint8_t device);


void blankNumber(void);
void readNumberFromMemory(uint8_t number);
void displayNumberFromMemory(uint8_t number, uint8_t device);
void displayValue(int value, uint8_t device);
void drawHeart(void);
void drawWave(void);
#endif /* MAX7219_H_ */