#ifndef UTILS_H
#define UTILS_H

#include "tim.h"

void set_us_delay(TIM_HandleTypeDef* sleep_tim);
void delay_us(uint32_t us);
#endif // !UTILS_H

