#include <stdio.h>
#include "esp_rom_gpio.h"
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "driver/touch_pad.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "capacitive_touch.h"

#define LED 2

#define TOUCH_LIMIT  (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

uint16_t filtered_value;
bool touch_detected = false;

SemaphoreHandle_t xMutex_data;

static void read_sensor(void *pvParameter){
    while(1){
        if(xMutex_data != NULL){
            if( xSemaphoreTake(xMutex_data, ( TickType_t ) 10) == pdTRUE){
                filtered_value = read_capacitive_pin_T0();
                printf("%d\n", filtered_value);
                xSemaphoreGive(xMutex_data);
            }
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void detect_human_touch(void *pvParameter){
    while(1){
        if(xMutex_data != NULL){
            if( xSemaphoreTake(xMutex_data, ( TickType_t ) 10) == pdTRUE){
                if(filtered_value <= 400){
                    touch_detected = true;
                }
                else{
                    touch_detected = false;
                }
                xSemaphoreGive(xMutex_data);
            }
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void blink_led(void *pvParameter){
    while(1){
        if(touch_detected){
            gpio_set_level(LED, true);
            printf("LED on\n");
        }
        else{
            gpio_set_level(LED, false);
            printf("LED off\n");
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{   
    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    xMutex_data = xSemaphoreCreateBinary();
    xSemaphoreGive(xMutex_data);

    ESP_ERROR_CHECK(touch_pad_init());
    ESP_ERROR_CHECK(touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V));
    capaticitive_pin_init();
    ESP_ERROR_CHECK(touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD));

    xTaskCreate(&read_sensor, "Read T0 capacitive Pin", 4096, NULL, 5, NULL);
    xTaskCreate(&detect_human_touch, "Detect Pin touch", 2048, NULL, 5, NULL);
    xTaskCreate(&blink_led, "Blink LED according to touch", 2048, NULL, 5, NULL);
}
