#include "impedance_controller.h"

float k_p_;
float k_b_;

float torque_;
int is_first_;

float mech_postion__;
float mech_velocity__;
float elec_velocity__;
float elec_position__;

uint32_t counter_pre_;
uint32_t counter_;

float toa_;

float pwm_interval_;

float distance_val_;

float pi_f_ = 3.14159265359f;
float pi_2_f_ = 6.28318530718f;

void impedance_controller_setup(float k_p, float k_d, float toa, float pwm_interval)
{
	k_p_ = k_p;
	k_b_ = k_d;
	is_first_ = 1;
	toa_ = toa;
	pwm_interval_ = pwm_interval;
}

void IC_running(float position_ref)
{
	//for simple
	/*if (is_first_)
	{
		get_mech_position(&mech_postion__, 0.00005);
		get_mech_velocity(&mech_velocity__);
		get_elec_position(&elec_position__);
		counter_pre_  = __HAL_TIM_GET_COUNTER(&htim2);
	}
	else
	{
		
	}*/
	
	get_mech_position(&mech_postion__, pwm_interval_);
	get_mech_velocity(&mech_velocity__);
	get_elec_position(&elec_position__);
	
	get_distance(position_ref, mech_postion__, &distance_val_);
	torque_ = k_p_*distance_val_ - k_b_*(mech_velocity__);
	
	current_regulator_set_current((torque_ / toa_), 0);
	current_commutate(elec_position__);
}


void get_distance(float ref_position, float current_postion, float* goal_distance)
{
	if (ref_position - current_postion > pi_f_){
		*goal_distance = (ref_position - (current_postion + pi_2_f_));
	}
	
	if (ref_position - current_postion < -pi_f_){
		*goal_distance = ((ref_position+pi_2_f_) - current_postion);
	}
}