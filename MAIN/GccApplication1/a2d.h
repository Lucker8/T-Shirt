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
#define V_REF 4.8

#define accel_i2c_adr_a0_read 0x39 //sa0=0 8bit final value r
#define accel_i2c_adr_a0_write 0x38 //8bit final value w
#define accel_i2c_adr_a1_read 0x3B //sa0=1 r
#define accel_i2c_adr_a1_write 0x3A //sa0=1n w
#define accel_i2c_adr_a1 0x1D //the slave addres sa0=0 for sa0=1 do 0x1D
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define F_SETUP 0x09 //FIFO SETUP default 0
#define XYZ_DATA_CFG 0x0E //dynamic range settings default 0
#define CTRL_REG1 0x2A //ODR=800 Hz-standby ai=0x2B
#define CTRL_REG2 0x2B //sleep enable,osmodes ai=0x2c
#define CTRL_REG3 0x2C //wake ai=0x2D
#define CTRL_REG4 0x2D //interrup enable
#define CTRL_REG5 0x2E //interrupt pin

#define OFF_X 0x30 //x offset adjust
#define OFF_Y 0x31 // y offset adjust
#define OFF_Z 0x0D //z offset adjust

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


#define BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU/(16UL*BAUDRATE))-1)

unsigned long micros(void);
uint16_t adc_read(uint8_t adc_channel);
double get_humidity(uint16_t adc_data);
double get_temp_lm(uint16_t adc_data);
void register_setup(void);
void get_data_accel(int *x, int *y, int *z);
void MMA8451_init(void);
void i2c_write_reg(char device, char reg, char data);
char i2c_read_reg(char device, char reg);

#endif
