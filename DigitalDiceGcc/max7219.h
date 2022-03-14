/*
 * max7219.h
 *
 * Created: 2021-11-01 15:54:36
 *  Author: JakubLeśniak
 */ 



#ifndef MAX7219_H_
#define MAX7219_H_

//#include <stdint.h>

void initMax7219(uint8_t device);
void setup_SPI(void);
void spiSendByteMax7219(unsigned databyte);
void writeDataMax7219OnDevice(unsigned data_register, unsigned data, uint8_t device);
void setColMax7219(uint8_t colno, uint8_t value, uint8_t device);
void clearMax7219(uint8_t device);
void fillMax7219(uint8_t device);
void blinkMax7219(uint8_t device);


void blankNumber(void);
void readNumberFromMemory(uint8_t number);
void displayNumberFromMemory(uint8_t number, uint8_t device);
void displayValue(int value, uint8_t device);
void drawHeart(void);
void drawWave(void);


#endif /* MAX7219_H_ */