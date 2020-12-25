#ifndef A2D_H_
#define A2D_H_


#define ADC_PIN0 0x00
#define ADC_PIN1 0x01
#define ADC_PIN2 0x02
#define ADC_PIN3 0x03
#define ADC_PIN4 0x04
#define ADC_PIN5 0x05
#define ADC_PIN6 6
#define ADC_PIN7 7
#define ZERO_OFFSET 0.826
#define SLOPE 0.0315
#define V_IN 4.8

#define accel_i2c_adr_a0 0x38
#define accel_i2c_adr_a0_read 0x39
#define accel_i2c_adr_a0_write 0x38
#define accel_i2c_adr_a1_read 0x3B
#define accel_i2c_adr_a1_write 0x3A
#define accel_i2c_adr_a1 0x1D
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define F_SETUP 0x09
#define XYZ_DATA_CFG 0x0E
#define CTRL_REG1 0x2A
#define CTRL_REG2 0x2B

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
void register_setup(void);
void get_data_accel(int *x, int *y, int *z);
void MMA8451_init(void);

#endif
