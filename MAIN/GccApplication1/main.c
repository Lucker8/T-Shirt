/*
 * GccApplication1.c
 *
 * Created: 11/1/2020 5:56:11 PM
 * Author : petko
 */

#define F_CPU 16000000UL

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "i2cmaster.h"
#include "lcd.h"
#include "usart.h"
#include "a2d.h"

#define B1 		0x10 //~(1<<4)
#define B2  	0x20 //~(1<<5)
#define B3 		0x40 //~(1<<6)
#define B4 		0x80 //~(1<<7)

void gr_screen(int mode);

int main(void)
{
	double temperature,true_humidity, max_temp,min_temp,max_hum,min_hum,t_s;
	long unsigned timer_c;
	int state=0;

	i2c_init();
 	MMA8451_init();
	LCD_init();
	register_setup();
	DDRD = 0x0F; //I/O board:PD4..7 as inputs, for buttons. 0-3 output
	DDRC = 0x3B; //I/O 2 input for temp, 6 input for hum
	PORTC = 0x30; //try this if fail 0x30 // Enable internal pull at 4-6 and 2 inputs
	PORTD = 0xF0; // internal pull up at 4-7
	gr_screen(0);  //0 hello, 1 goodbye

	_delay_ms(200);

	temperature=get_temp_lm(adc_read(2)); 	//setting up starting temperature
	max_temp = temperature;
	min_temp = temperature;
	t_s = temperature;

	true_humidity=get_humidity(adc_read(6))/(1.0546-0.00216*temperature);			//setting up starting humidity
	max_hum=true_humidity;
	min_hum=true_humidity;

	while(1)
	{
													//MAIN MENU
		timer_c=micros();
		LCD_set_cursor(0,0);
		printf("MAIN MENU");
		LCD_set_cursor(0,1);
		printf("B1-Temperature");
		LCD_set_cursor(0,2);
		printf("B2-Humidity");
		LCD_set_cursor(0,3);
		printf("B3-Heart rate");

		if (((PIND&B1)==B1))
		{
													//TEMP MENU
			LCD_clear();
			while((PIND&B4)==B4)
			{
				if (state==1)
				{
					LCD_clear();
					state=0;
				}
				if(state==2)
				{
					LCD_clear();
					state=3;
				}
				LCD_set_cursor(0,0);
				printf("Temperature");
				LCD_set_cursor(16,3);
				printf("Back");

				if(micros()-timer_c>5000)//takes measurement every 500ms
					{
					timer_c=micros();
					temperature = get_temp_lm(adc_read(2));
					if (temperature<min_temp) min_temp=temperature;
					if (temperature>max_temp) max_temp=temperature;

					}
				if(abs(t_s-temperature)>=0.5 && state!=3)
					{
						state=1;LCD_clear();
						while(abs(t_s-temperature)>=0.5)
						{
							LCD_set_cursor(0,0);
							printf("Temp :%02.1f\337C",temperature);
							LCD_set_cursor(0,1);
							printf("!ALERT!");
							LCD_set_cursor(0,2);
							printf("Temp diff. high!");
							LCD_set_cursor(0,3);
							printf("B1 to ignore OR wait!");
							if(micros()-timer_c>5000)//takes measurement every 500ms
								{
								timer_c=micros();
								temperature = get_temp_lm(adc_read(2));
								}
							if(((PIND&B1)==B1))
								{
								state=2;
								LCD_clear();
								break;
								}
								
						}
					}
				LCD_set_cursor(0,1);
				printf("Current: %02.1f\337C",temperature);
				LCD_set_cursor(0,2);
				printf("Max: %.1f\337C",max_temp);
				LCD_set_cursor(0,3);
				printf("Min: %.1f\337C",min_temp);
				}

			LCD_clear();
			state = 0;
			_delay_ms(100);

		}



		if (((PIND&B2)==B2))
		{
																//HUMIDITY MENU
			_delay_ms(200);
			LCD_clear();
			while((PIND&B4)==B4)
			{
				if (state==1)
				{
					LCD_clear();
					state=0;
				}
				if(state==2)
				{
					LCD_clear();
					state=3;
				}
				LCD_set_cursor(0,0);
				printf("Humidity");
				LCD_set_cursor(16,3);
				printf("Back");
				if(micros()-timer_c>5000)//takes measurement every 500ms
				{
					timer_c=micros();
					if (max_hum>100)
					{
						continue;
					}
					else
					{
						true_humidity =get_humidity(adc_read(6))/(1.0546-0.00216*temperature);
						if(max_hum<true_humidity) max_hum=true_humidity;
						if(min_hum>true_humidity) min_hum=true_humidity;

						if((true_humidity<25 || true_humidity>65) && state!=3)
							{
								state=1;LCD_clear();
								while((true_humidity<25 || true_humidity>65))
								{
									LCD_set_cursor(0,0);
									printf("Humidity: %02.1f %%",true_humidity);
									LCD_set_cursor(0,1);
									printf("!ALERT!");
									LCD_set_cursor(0,2);
									printf("HUMIDITY");
									if(micros()-timer_c>5000)//takes measurement every 500ms
										{
											timer_c=micros();
											true_humidity = get_humidity(adc_read(6))/(1.0546-0.00216*temperature);
										
											LCD_set_cursor(0,3);
											printf("B1 to ignore OR wait!");
										}
									if(((PIND&B1)==B1))
										{
											state=2;
											LCD_clear();
											break;
										}
										
								}
							}
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
						printf("Current Hum.: %.2f",true_humidity);
						LCD_set_cursor(0,2);
						printf("Min Hum.: %.2f",min_hum);
						LCD_set_cursor(0,3);
						printf("Max Hum.: %.2f",max_hum);
					}
				}
			
			LCD_clear();
			state = 0;
			_delay_ms(100);
		}

		if(((PIND&B3)==B3))
		{									//HEART RATE MENU
			_delay_ms(200);
			timer_c=micros();
			LCD_clear();
			while((PIND&B4)==B4)
			{	
				
				LCD_set_cursor(0,0);
				printf("Heart Rate");
				LCD_set_cursor(16,3);
				printf("Back");
				/*
				if(micros()-timer_c>5000)//takes measurement every 500ms
				{
					timer_c=micros();
					PORTD|=(1<<3);
					_delay_ms(500);
					PORTD&=0xF0;
				}*/
				
				LCD_set_cursor(0,1);
				printf("0"); //replace with get_hr
			}
			LCD_clear();
			_delay_ms(50);
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
