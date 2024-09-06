#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "capacitive_touch.h"

#define TOUCH_LIMIT  (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

uint16_t read_capacitive_pin_T0(){   
    uint16_t filtered_value;
    touch_pad_read_filtered(0, &filtered_value);
    return filtered_value;
}

void capaticitive_pin_init(){
    touch_pad_config(0, TOUCH_LIMIT);
}
