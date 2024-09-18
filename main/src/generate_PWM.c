#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "generate_PWM.h"

void setup_pwm(ledc_timer_bit_t duty_resolution, uint16_t freq, ledc_mode_t speed_mode, ledc_timer_t timer_num, ledc_channel_t channel, ledc_channel_t gpio_num){
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = duty_resolution,
        .freq_hz = freq,
        .speed_mode = speed_mode,
        .timer_num = timer_num,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .channel = channel,
        .duty = 0,
        .gpio_num = gpio_num,
        .speed_mode = speed_mode,
        .hpoint = 0,
        .timer_sel = timer_num
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}