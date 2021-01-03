/*
 * std.c
 *
 * Created: 19-02-2020 14:28:59
 *  Author: mikkel
 */ 
#include "std.h"
#include "i2cmaster.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void MMA8451_init()
{
	i2c_write_reg(accel_i2c_adr_a0, CTRL_REG1, 0x02);
	i2c_write_reg(accel_i2c_adr_a0, CTRL_REG2, 0x02);
	i2c_write_reg(accel_i2c_adr_a0, XYZ_DATA_CFG, 0x02);
	i2c_write_reg(accel_i2c_adr_a0, F_SETUP, 0x00);
	i2c_write_reg(accel_i2c_adr_a0, CTRL_REG1, 0x03);
}
/*
void register_setup()
{
	cli();
	//Timer0 setup
	TCCR0B |= 0x03; //Sets prescaler for timer0 to 64
	TIMSK0 |= 0x01; //Enables interupt on timer0 overflow
	
	//Port setup
	DDRD = 0x87; //I/O board:PD4?7 as inputs
	DDRC = 0xF0; //I/O board PC0?3 as inputs, for buttons
	DDRB = 0xF0;
	PORTB = 0x00;
	PORTC = 0x30; // Enable internal pull at PC0..3 inputs
	PORTD = 0x00; // Set output LEDs to off
	
	ADMUX = 0x40; // sets Vref to Avcc
	ADCSRA = 0x87; // Enables ADC and sets prescaler to 128
	
	EECR = 0x00;
	
	EICRA |= (1<<ISC11) | (1<<ISC10); //sets int1 to trigger on rising edge
	EIMSK |= (1<<INT1); //enables int1 external interrupt
	
	sei();
}*/



unsigned int read_adc(char channel)
{
	ADMUX &= 0xf0; // clears preivesly selected channel
	ADMUX |= channel; //sets adc multiplexer to channel
	ADCSRA |= (1<<ADSC); // Starts conversion
	while(ADCSRA & (1<<ADSC)); //"aits for conversion to complete
	return ADC; //Returns value from conversion
}

void get_data_accel(int *x, int *y, int *z)
{
	
	int raw;
	i2c_start(accel_i2c_adr_a0_write); //start writing/interrupt sleep
	i2c_write(OUT_X_MSB); 
	i2c_start(accel_i2c_adr_a0_read);
	*x = (i2c_readAck()<<8); //the accel. auto incr. after write recieved
	*y = (i2c_readAck()<<8);//therefore multiple bytes can be written after ack received
	*z = (i2c_readNak()<<8);
	//i2c_stop();
}
/*
void accel_calibrate()
{
	char x, y, z;
	i2c_start(accel_i2c_adr_a0_write);
	i2c_write(OUT_X_MSB);
	i2c_start(accel_i2c_adr_a0_read);
	x = i2c_readAck();
	y = i2c_readAck();
	z = i2c_readNak();
	i2c_stop();
	i2c_start(accel_i2c_adr_a0_write);
	i2c_write(OFF_X);
	i2c_write(-x);
	i2c_stop();
	i2c_start(accel_i2c_adr_a0_write);
	i2c_write(OFF_Y);
	i2c_write(-y);
	i2c_stop();
	i2c_start(accel_i2c_adr_a0_write);
	i2c_write(OFF_Z);
	i2c_write(4096-z);
	i2c_stop();
}*/

void i2c_write_reg(char device, char reg, char data)
{
	i2c_start(device + I2C_WRITE);
	i2c_write(reg);
	i2c_write(data);
	i2c_stop();
}
char i2c_read_reg(char device, char reg)
{
	i2c_start(device + I2C_WRITE);
	i2c_write(reg);
	i2c_start(device + I2C_READ);
	char data = i2c_readNak();
	i2c_stop();
	return data;
}

