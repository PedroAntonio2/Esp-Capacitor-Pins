#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "capacitive_touch.h"

#define TOUCH_LIMIT  (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

uint16_t read_capacitive_pin(uint8_t pin){   
    uint16_t filtered_value;
    //Perform a filtered reading on the capacitive pin
    //Supposedly, the filtered value will ignore the noise and give a more accurate reading
    touch_pad_read_filtered(pin, &filtered_value);
    return filtered_value;
}

void capaticitive_pin_init(uint8_t pad){
    //Touch sensor configuration
    ESP_ERROR_CHECK(touch_pad_init());
    ESP_ERROR_CHECK(touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V));
    touch_pad_config(pad, TOUCH_LIMIT);
    ESP_ERROR_CHECK(touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD));
}
