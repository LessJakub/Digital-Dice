/*
 * DigitalDice.cpp
 *
 * Created: 2021-12-30 00:12:14
 * Author : JakubLeśniak
 */ 
#define F_CPU 16500000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "max7219.h"
//#include "mpu6050.h"
//#include "I2CProtocol.h"
//#include "I2CMaster.h"
//#include "TWI_USI_Master.h"
#include "mpu6050v2.h"
#include "Timer.h"



void init(void)
{
	initTimer();
}

volatile int lasttime;
volatile uint8_t max_value = 6;
volatile uint8_t min_value = 1;

uint8_t getRandom()
{
	//return TCNT0%max_value + min_value;
	return min_value + (uint8_t)(TCNT0/256.0 * ((max_value - min_value) + 1));
}

uint8_t checkControllAngle()
{
	
	
}

void displayValueOnCube(uint8_t value)
{
	displayNumberFromMemory(value , 0);
	_delay_ms(1);
	displayNumberFromMemory(value, 1);
}

void clearCube()
{
	clearMax7219(0);
	_delay_ms(1);
	clearMax7219(1);
}

int main(void)
{	
	init();
	initMax7219(0);
	initMax7219(1);
	
	mpu6050_init();
	
	
	int16_t ax = 0;
	int16_t ay = 0;
	int16_t az = 0;
	int16_t gx = 0;
	int16_t gy = 0;
	int16_t gz = 0;
	
	double ax_buffer = ax/MPU6050_ACCEL_LSB_16;
	double ay_buffer = ay/MPU6050_ACCEL_LSB_16;
	double az_buffer = az/MPU6050_ACCEL_LSB_16;
	
	double gx_buffer = gx/MPU6050_GYRO_LSB_2000;
	double gy_buffer = gy/MPU6050_GYRO_LSB_2000;
	double gz_buffer = gz/MPU6050_GYRO_LSB_2000;
	
	uint8_t blinking = 0;
	uint8_t control_max_values = 0;
	uint8_t control_min_values = 0;

	float start_control_seconds = -1.0;
	float start_display_seconds = getSeconds();
	float control_seconds = 0.0;
	float display_seconds = 0.0;
	
	int16_t controll_angle = -4;
	int16_t controll_error = 1;
	
	uint8_t rolling = 0;
	uint8_t reset = 0;
	
	int display_value = 0;
	
	while(1) {

		mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);

		ax_buffer = ax/MPU6050_ACCEL_LSB_16; 
		ay_buffer = ay/MPU6050_ACCEL_LSB_16;
		az_buffer = az/MPU6050_ACCEL_LSB_16;
		
		gx_buffer = gx/MPU6050_GYRO_LSB_2000;
		gy_buffer = gy/MPU6050_GYRO_LSB_2000;
		gz_buffer = gz/MPU6050_GYRO_LSB_2000;

		if(ax_buffer <= controll_angle + controll_error && ax_buffer >= controll_angle - controll_error && az_buffer > 0)
		{
			control_seconds = getSeconds();
			if(start_control_seconds == -1.0) start_control_seconds = control_seconds;

			if(control_seconds - start_control_seconds >= 5.0)
			{
				start_control_seconds = -1.0;
				if(control_max_values == 0)
				{
					control_max_values = 1;
					blinking = 1;
					display_value = max_value;
				}
				else if (control_max_values == 1)
				{
					control_max_values = 0;
					blinking = 0;
					display_value = (int)(max_value + min_value)/2;
				}
			}
		}
		else if(az_buffer < 0.0 && control_max_values != 1)
		{
			display_value = getRandom();
		}
		else
		{
			start_control_seconds = -1.0;
		}
		
		if(control_max_values == 1 )
		{
			if(ay_buffer >= 5 && (max_value + 1) < 10) max_value++;
			else if(ay_buffer <= -5 && (max_value - 1) > min_value) max_value--;
			display_value = max_value;
		}
		
		//uint8_t i = getRandom();

		//displayValue(display_value, 0);
		//displayValue(display_value, 1);
		//display_value = abs(ay_buffer);
		//display_value = abs(ax_buffer);
		displayValueOnCube(display_value);
		
		if(blinking)
		{
			//_delay_ms(10);
			clearCube();
			displayValueOnCube(display_value);
			//_delay_ms(100);
		}
		
		
		
		//_delay_ms(50);

		}
}

