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

#include "board.h"
#include "capacitive_touch.h"
#include "wifi.h"

uint16_t filtered_value;
bool touch_detected = false;
bool last_touch_detected = false;

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
        vTaskDelay(150 / portTICK_PERIOD_MS);
    }
}

static void detect_human_touch(void *pvParameter){
    while(1){
        if(xMutex_data != NULL){
            if( xSemaphoreTake(xMutex_data, ( TickType_t ) 10) == pdTRUE){
                //Touch detection(400 is the threshold value, arbitrarily chosen)
                if(filtered_value <= 400){
                    touch_detected = true;
                }
                else{
                    touch_detected = false;
                }
                xSemaphoreGive(xMutex_data);
            }
        }
        vTaskDelay(150 / portTICK_PERIOD_MS);
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
        if(last_touch_detected != touch_detected){
            if(touch_detected){
                send_data_to_topic("20211610015/touch", "1");
            }
            else{
            send_data_to_topic("20211610015/touch", "0");
            }
        }
        last_touch_detected = touch_detected;
        vTaskDelay(75 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{   

    //Mutex initialization
    xMutex_data = xSemaphoreCreateBinary();
    xSemaphoreGive(xMutex_data);

    //Initialization of necessary peripherals/components
    board_init();

    //Task creation
    xTaskCreate(&read_sensor, "Read T0 capacitive Pin", 4096, NULL, 5, NULL);
    xTaskCreate(&detect_human_touch, "Detect Pin touch", 2048, NULL, 5, NULL);
    xTaskCreate(&blink_led, "Blink LED according to touch", 2048, NULL, 5, NULL);
}
