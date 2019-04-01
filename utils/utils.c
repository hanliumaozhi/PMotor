#include "utils.h"

TIM_HandleTypeDef* sleep_tim_;

void set_us_delay(TIM_HandleTypeDef* sleep_tim)
{
	sleep_tim_ = sleep_tim;
}

void delay_us(uint32_t us)
{
	uint32_t pre_counter = __HAL_TIM_GET_COUNTER(sleep_tim_);
	uint32_t post_counter;
	do {
		post_counter =  __HAL_TIM_GET_COUNTER(sleep_tim_);
	} while ((post_counter - pre_counter) < us);
}