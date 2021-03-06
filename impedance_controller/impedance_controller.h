#ifndef IMPEDANCE_CONTROLLER_H
#define IMPEDANCE_CONTROLLER_H

#include "current_regulator.h"
#include "as5048a.h"


void impedance_controller_setup(float k_p, float k_d, float toa, float pwm_interval);
void IC_running(float position_ref);
void get_distance(float ref_position, float current_postion, float* goal_distance);
#endif // !IMPEDANCE_CONTROLLER_H
