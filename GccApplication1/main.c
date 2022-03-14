/*
 * GccApplication1.c
 *
 * Created: 2022-01-10 00:42:56
 * Author : JakubLeśniak
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "max7219.h"

void init()
{
	// SCK MOSI CS/LOAD/SS
	//Set SCK, MOSI, SS as outputs
	DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI)|(1 << PIN_SS);
	DDRD |= (1 << PIN_SS_1) | (1 << PIN_SS_2);

	// SPI Enable, Master mode
	//Enable SPI as master.
	SPCR |= (1 << SPE) | (1 << MSTR)| (1<<SPR1);
}

int main(void)
{
    /* Replace with your application code */
	init();
	initMax7219(0);
	initMax7219(1);
	//writeDataMax7219(0x01 + 1, 0xFF);
	
	#if MPU6050_GETATTITUDE == 0
	int16_t ax = 0;
	int16_t ay = 0;
	int16_t az = 0;
	int16_t gx = 0;
	int16_t gy = 0;
	int16_t gz = 0;
	double axg = 0;
	double ayg = 0;
	double azg = 0;
	double gxds = 0;
	double gyds = 0;
	double gzds = 0;
	double roll = 0.0f;
	double pitch = 0.0f;
	double yaw = 0.0f;
	
	//double ax_buffer = ax/MPU6050_ACCEL_LSB_16;
	//double ay_buffer = ay/MPU6050_ACCEL_LSB_16;
	//double az_buffer = az/MPU6050_ACCEL_LSB_16;
	
	float lastTime = 0.0f;
	#endif
	
	mpu6050_init();
	
	int i = 0;
    while (1) 
    {
		
		#if MPU6050_GETATTITUDE == 0
		//mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
		//mpu6050_getConvData(&axg, &ayg, &azg, &gxds, &gyds, &gzds);
		
		
		//displayValue(gx ,1);
		#endif
		displayNumberFromMemory(i, 1);
		i++;
		if(i >= 10) i = 0;


		_delay_ms(500);
		
    }
}

