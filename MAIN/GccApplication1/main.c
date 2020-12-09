/*
 * GccApplication1.c
 *
 * Created: 11/1/2020 5:56:11 PM
 * Author : petko
 */ 

#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"
#include "ds1621.h"
#include "lcd.h"
#include "std.h"
#include "usart.h"
#include "a2d.h"

#define B1 0x3E
#define B2 0x3D
#define B3 0x3B
#define B4 0x37

void gr_screen(int mode);

int main(void)

{		
	double temperature,humidity, max_temp,min_temp,max_hum,min_hum;
	long unsigned timer_c;
	//MMA8451_init();
	i2c_init();
	LCD_init();
	ds1621_init();
	//uart_init();
	//io_redirect(); 
	//ADMUX = (1<<REFS0);//set prescaler to 128 and turn on the ADC module
	//ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN); 
	register_setup();
	DDRD = 0xFF; //I/O board:PD4…7 as outputs, for LEDs
	DDRC = 0xF0; //I/O board PC0…3 as inputs, for buttons
	PORTC = 0x3F; // Enable internal pull at PC0..3 inputs
	PORTD = 0x00; // Set output LEDs to off
	
	gr_screen(0);     //0 hello, 1 goodbye
	humidity=get_humidity(adc_read(6));
	max_hum=humidity;
	min_hum=humidity;

	temperature=get_temperature();
	max_temp = temperature;
	min_temp = temperature;
	
	
	//timer_c=millis();
	
	
	
	while(1)
	{	
													//MAIN MENU
		timer_c=millis();
		LCD_set_cursor(0,0);
		printf("MAIN MENU");
		LCD_set_cursor(0,1);
		printf("B1-Temperature");
		LCD_set_cursor(0,2);
		printf("B2-Humidity");
		LCD_set_cursor(0,3);
		printf("B3-Heart rate");
		
		if (PINC==B1){
													//TEMP MENU	
			LCD_clear();
			while(PINC!=B4)
			{
				LCD_set_cursor(0,0);
				printf("Temperature");
				LCD_set_cursor(16,3);
				printf("Back");
				//LCD_set_cursor(0,2);
				
				
				if(millis()-timer_c>5000)//takes measurement every 500ms
				{	
					timer_c=millis();
					temperature = get_temperature();
					if (temperature<min_temp) min_temp=temperature;
					if (temperature>max_temp) max_temp=temperature;
					
					
				}
				
				
				LCD_set_cursor(0,1);
				printf("Current: %.1f\337C",temperature);
				LCD_set_cursor(0,2);
				printf("Max: %.1f\337C",max_temp); 
				LCD_set_cursor(0,3);
				printf("Min: %.1f\337C",min_temp);
				//printf("%.2f",get_temp_lm(adc_read(0)));
			}
			
			LCD_clear();//make it into a function
			_delay_ms(100);
		
		}
		
		
		
		if (PINC==B2)
		{	
																//HUMIDITY MENU
			_delay_ms(200);
			LCD_clear();
			ADMUX = (1<<REFS0);//set prescaler to 128 and turn on the ADC module
			ADCSRA = (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
			while(PINC!=B4)
			{	
				LCD_set_cursor(0,0);
				printf("Humidity");
				LCD_set_cursor(16,3);
				printf("Back");
				if(millis()-timer_c>5000)//takes measurement every 500ms
				{	
					timer_c=millis();
					if (max_hum>100)
					{
						continue;
					}
					else
					{			
						humidity = get_humidity(adc_read(6));
						if(max_hum<humidity) max_hum=humidity;
						if(min_hum>humidity) min_hum=humidity;
					}
				}
				if (max_hum>100)
				{
					LCD_clear();
					LCD_set_cursor(0,1);
					printf("No sensor present!");
					_delay_ms(1000);
					break;
				}
					else
				{
					LCD_set_cursor(0,1);
					printf("Current humidity: %.2f",humidity);
					LCD_set_cursor(0,2);
					printf("Min Humidity: %.2f",max_hum);
					LCD_set_cursor(0,3);
					printf("Max Humidity: %.2f",min_hum);
					
				}

				
			}
			LCD_clear();
			_delay_ms(100);
		}
	
		if(PINC==B3)
		{									//HEART RATE MENU
			_delay_ms(200);
			LCD_clear();
			while(PINC!=B4)
			{
				LCD_set_cursor(0,0);
				printf("Heart Rate");
				LCD_set_cursor(16,3);
				printf("Back");
				/*if(millis()-t>5000)//takes measurement every 500ms
				{
					timer_c=millis();
					
					
				}*/
				LCD_set_cursor(0,1);
				printf("0"); //replace with get_hr
			}
			LCD_clear();
			_delay_ms(50);		
		}
		
		if(PINC==(B4&B3&B2))
		{	
			LCD_clear();
			_delay_ms(50);
			gr_screen(1);
			break;
		}
	
	}
	return 0;
	
}



void gr_screen(int mode)
{
	char hello_l[5]={'H','E','L','L','O'},bye_l[7]={'G','O','O','D','B','Y','E'};
	if(!mode)
	{
		for(int i=0;i<5;i++)
		{
			LCD_set_cursor(i+6,1);
			printf("%c",hello_l[i]);
			_delay_ms(100);
		}
		_delay_ms(500);
		for(int i=0;i<5;i++)
		{
			LCD_set_cursor(i+6,1);
			printf(" ");
			_delay_ms(100);
		}
	}
	else
	{
		for(int i=0;i<7;i++)
		{
			LCD_set_cursor(i+6,1);
			printf("%c",bye_l[i]);
			_delay_ms(100);
		}
		_delay_ms(500);
		for(int i=0;i<7;i++)
		{
			LCD_set_cursor(i+6,1);
			printf(" ");
			_delay_ms(100);
		}
		
	}

}