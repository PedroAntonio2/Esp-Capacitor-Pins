#ifndef GENERATE_PWM_H
#define GENERATE_PWM_H

void setup_pwm(ledc_timer_bit_t duty_resolution, uint16_t freq, ledc_mode_t speed_mode, 
                ledc_timer_t timer_num, ledc_channel_t channel, ledc_channel_t gpio_num);
void setup_pwm_for_touch(uint8_t channel, uint16_t freq, uint8_t pin);

#endif