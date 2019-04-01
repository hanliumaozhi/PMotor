
#ifndef INVERTER_H
#define INVERTER_H

#include "tim.h"
#include "drv8301.h"

#define PWM_INTERVAL 0.00005f
#define MAX_PWM 500
#define MIN_LOW_COUNTER 45

extern volatile uint16_t so_1_raw_val;
extern volatile uint16_t so_2_raw_val;

extern float so_1_voltage;
extern float so_2_voltage;

extern float so_1_current;
extern float so_2_current;

extern ADC_HandleTypeDef* a_adc;
extern ADC_HandleTypeDef* b_adc;


void inverter_setup(TIM_HandleTypeDef* motor_tim, SPI_HandleTypeDef* drv8301, GPIO_TypeDef* port, uint16_t pin);
void inverter_init(ADC_HandleTypeDef* a, ADC_HandleTypeDef* b, GPIO_TypeDef* power_port, uint16_t power_pin, float pwm_interval_);
void inverter_zero_current();

void inverter_get_voltage();

void inverter_sample_current();
void inverter_get_current(float *a, float *b, float *c);

void inverter_set_pwm(float a_t, float b_t, float c_t);

void inverter_set_pwm_percentage(float a_t, float b_t, float c_t);
#endif // !INVERTER_H
