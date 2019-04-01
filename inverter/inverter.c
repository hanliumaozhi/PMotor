
#include "inverter.h"


TIM_HandleTypeDef* motor_pwm;
GPIO_TypeDef* DRV8301_PORT;
uint16_t DRV8301_PIN;
SPI_HandleTypeDef* DRV8301;
ADC_HandleTypeDef* a_adc;
ADC_HandleTypeDef* b_adc;

GPIO_TypeDef* POWER_PORT;
uint16_t POWER_PIN;

volatile uint16_t so_1_raw_val;
volatile uint16_t so_2_raw_val;
volatile uint32_t c_statue_word;

float so_1_voltage;
float so_2_voltage;

float so_1_current;
float so_2_current;

uint32_t a_channel_val;
uint32_t b_channel_val;
uint32_t c_channel_val;

float pwm_interval;
float total_counter;



void inverter_setup(TIM_HandleTypeDef* motor_tim, SPI_HandleTypeDef* drv8301, GPIO_TypeDef* port, uint16_t pin)
{
	motor_pwm = motor_tim;
	DRV8301 = drv8301;
	DRV8301_PORT = port;
	DRV8301_PIN = pin;
	drv8301_setup(drv8301, port, pin);
}

void inverter_init(ADC_HandleTypeDef* a, ADC_HandleTypeDef* b, GPIO_TypeDef* power_port, uint16_t power_pin, float pwm_interval_)
{
	a_adc = a;
	b_adc = b;
	
	POWER_PORT = power_port;
	POWER_PIN = power_pin;
	
	HAL_GPIO_WritePin(POWER_PORT, POWER_PIN, GPIO_PIN_SET);
	HAL_Delay(500);
	
	drv8301_init();
	
	HAL_Delay(500);
	
	//HAL_TIM_Base_Start(motor_pwm);
	HAL_TIM_PWM_Start(motor_pwm, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(motor_pwm, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(motor_pwm, TIM_CHANNEL_3);
	
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_1, MAX_PWM);
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_2, MAX_PWM);
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_3, MAX_PWM);
	
	pwm_interval = pwm_interval_;
	total_counter = (float)MAX_PWM;
	
	//init adc
	a_adc->Instance->CR2 |= 0x00000001;
	b_adc->Instance->CR2 |= 0x00000001;
}

void inverter_zero_current()
{
	//pass
}

void inverter_get_voltage()
{
	//pass
}

void inverter_sample_current()
{
	a_adc->Instance->CR2 |= 0x40000000; 
	do {
		c_statue_word = a_adc->Instance->SR;
	} while ((c_statue_word & 2) == 0);
	so_1_raw_val = a_adc->Instance->DR;
	so_2_raw_val = b_adc->Instance->DR;
}

void inverter_get_current(float *a, float *b, float *c)
{
	so_1_voltage = ((float)so_1_raw_val) / 4095.0f * 3.3f;
	so_2_voltage = ((float)so_2_raw_val) / 4095.0f * 3.3f;
	
	// for G = 10v/v;
	so_1_current = 33.0f - 20.0f*so_1_voltage;
	so_2_current = 33.0f - 20.0f*so_2_voltage;
	
	*a = so_1_current;
	*b = so_2_current;
	*c = -so_1_current - so_2_current;
}

void inverter_set_pwm(float a_t, float b_t, float c_t)
{
	a_channel_val = (uint32_t) ((a_t / pwm_interval)*total_counter);
	b_channel_val = (uint32_t) ((b_t / pwm_interval)*total_counter);
	c_channel_val = (uint32_t) ((c_t / pwm_interval)*total_counter);
	a_channel_val = MAX_PWM - a_channel_val;
	b_channel_val = MAX_PWM - b_channel_val;
	c_channel_val = MAX_PWM - c_channel_val;
	
	if (a_channel_val < MIN_LOW_COUNTER) {
		a_channel_val = MIN_LOW_COUNTER;
	}
	if ( b_channel_val < MIN_LOW_COUNTER){
		b_channel_val = MIN_LOW_COUNTER;
	}
	if (c_channel_val < MIN_LOW_COUNTER){
		c_channel_val = MIN_LOW_COUNTER;
	}
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_1, a_channel_val);
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_2, b_channel_val);
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_3, c_channel_val);
}


void inverter_set_pwm_percentage(float a_t, float b_t, float c_t)
{
	a_channel_val = (uint32_t)(a_t*total_counter);
	b_channel_val = (uint32_t)(b_t*total_counter);
	c_channel_val = (uint32_t)(c_t*total_counter);
	a_channel_val = MAX_PWM - a_channel_val;
	b_channel_val = MAX_PWM - b_channel_val;
	c_channel_val = MAX_PWM - c_channel_val;
	
	if (a_channel_val < MIN_LOW_COUNTER) {
		a_channel_val = MIN_LOW_COUNTER;
	}
	if (b_channel_val < MIN_LOW_COUNTER) {
		b_channel_val = MIN_LOW_COUNTER;
	}
	if (c_channel_val < MIN_LOW_COUNTER) {
		c_channel_val = MIN_LOW_COUNTER;
	}
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_1, a_channel_val);
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_2, b_channel_val);
	__HAL_TIM_SET_COMPARE(motor_pwm, TIM_CHANNEL_3, c_channel_val);
}