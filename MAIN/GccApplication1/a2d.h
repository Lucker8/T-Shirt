#ifndef A2D_H_
#define A2D_H_


#define ADC_PIN0 0x00
#define ADC_PIN1 0x01
#define ADC_PIN2 0x02
#define ADC_PIN3 0x03
#define ADC_PIN4 0x04
#define ADC_PIN5 0x05
#define ADC_PIN6 6
#define ADC_PIN7 0x07
#define ZERO_OFFSET 0.826
#define SLOPE 0.0315
#define V_IN 4.8

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


#define BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU/(16UL*BAUDRATE))-1)

unsigned long millis(void);
uint16_t adc_read(uint8_t adc_channel);
double get_humidity(uint16_t adc_data);
double get_temp_lm(uint16_t adc_data);
void register_setup();


#endif
