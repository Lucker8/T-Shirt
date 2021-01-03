/*
 * GccApplication1.c
 *
 * Created: 11/1/2020 5:56:11 PM
 * Author : petko
 */


#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ds1621.h"
#include "lcd.h"
#include "a2d.h"
#include "i2cmaster.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

uint16_t adc_read(uint8_t adc_channel)//the user inputs from 0-7 for the desired pin
{
  switch (adc_channel)
  {
    case 0:
      adc_channel = ADC_PIN0;
      break;
    case 1:
      adc_channel=ADC_PIN1;
      break;
    case 2:
      adc_channel=ADC_PIN2;
      break;
    case 3:
      adc_channel=ADC_PIN3;
      break;
    case 4:
      adc_channel=ADC_PIN4;
      break;
    case 5:
      adc_channel=ADC_PIN5;
      break;
    case 6:
      adc_channel=ADC_PIN6;
      break;
    case 7:
      adc_channel=ADC_PIN7;
      break;
    default:
      printf("Pin Error adc read");
      return 0;
  }


	ADMUX |= adc_channel; // set the desired channel
	//start a conversion
	ADCSRA |= (1<<ADSC);
	// now wait for the conversion to complete
	while ( (ADCSRA & (1<<ADSC)) );
	ADMUX &= 0xf0; // clear any previously used channel, but keep internal reference

	// now we have the result, so we return it to the calling function as a 16 bit unsigned int
	return ADC;
}

double get_humidity(uint16_t adc_data)//returns the data from humidity sensor in RH%
{
	double rh,volts;//10 bits
	volts = (adc_data*V_REF)/1024;//10 bits

	rh=(volts-ZERO_OFFSET)/SLOPE;
	return rh;
}

double get_temp_lm(uint16_t adc_data)//temp reading for lm35
{
  double volts = (adc_data*V_REF)/1024;//10 bits
  return volts*100;
}

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;


ISR (TIMER0_OVF_vect) // timer0 overflow interrupt
{
	timer0_millis += MILLIS_INC;
	timer0_fract += FRACT_INC;
	if (timer0_fract >= FRACT_MAX) {
		timer0_fract -= FRACT_MAX;
		timer0_millis += 1;
	}
	timer0_overflow_count++;
}

unsigned long micros()
{
	unsigned long m;
	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	sei();
	return m;
}

void register_setup()
{
	cli();
	TCCR0B |= 0x03; //Sets prescaler for timer0 to 64
	TIMSK0 |= 0x01; //Enables interupt on timer0 overflow
	EECR = 0x00;
	EICRA |= (1<<ISC11) | (1<<ISC10); //sets int1 to trigger on rising edge
	EIMSK |= (1<<INT1); //enables int1 external interrupt
	ADMUX = (1<<REFS0);//set prescaler to 128 and turn on the ADC module
	ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
	sei();
}


void MMA8451_init()
{
	i2c_write_reg(accel_i2c_adr_a0, CTRL_REG1, 0x02);
	i2c_write_reg(accel_i2c_adr_a0, CTRL_REG2, 0x02);
	i2c_write_reg(accel_i2c_adr_a0, XYZ_DATA_CFG, 0x02);
	i2c_write_reg(accel_i2c_adr_a0, F_SETUP, 0x00);
	i2c_write_reg(accel_i2c_adr_a0, CTRL_REG1, 0x03);
}

void get_data_accel(int *x, int *y, int *z)
{
	i2c_start(accel_i2c_adr_a0_write);
	i2c_write(OUT_X_MSB);
	i2c_start(accel_i2c_adr_a0_read);
	*x = (i2c_readAck()<<8);
	*y = (i2c_readAck()<<8);
	*z = (i2c_readNak()<<8);
	//i2c_stop();
}

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
