#include <stdio.h>
#include "esp_rom_gpio.h"
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "driver/touch_pad.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "board.h"
#include "capacitive_touch.h"
#include "wifi.h"

uint16_t filtered_value = 500;
uint8_t current_pin = 0;
bool touch_detected = false;
bool last_touch_detected = false;
const char* notas[] = {"Dó", "Si", "Mi", "Fá", "Sol", "Lá", "Ré"};

ledc_mode_t speed_mode;
ledc_timer_t timer_num;

SemaphoreHandle_t xMutex_data;

static void read_sensor(void *pvParameter){
    while(1){
        if(xMutex_data != NULL){
            if( xSemaphoreTake(xMutex_data, ( TickType_t ) 10) == pdTRUE){
                filtered_value = 500;
                if(current_pin == 6){
                    current_pin = 0;
                }
                else{
                    current_pin++;
                }
                if(current_pin == 1){
                    current_pin++;
                }
                filtered_value = read_capacitive_pin(current_pin);
                if(filtered_value <= 400){
                    //printf("%d, %d  \n", current_pin,filtered_value);
                    touch_detected = true;
                }
                else{
                    touch_detected = false;
                }
                xSemaphoreGive(xMutex_data);
            }
        }
        vTaskDelay(12 / portTICK_PERIOD_MS);
    }
}

static void update_pwm(void *pvParameter){
    while(1){
        if(touch_detected){
            switch (current_pin) {
        case 0:
        case 1:
        case 2:
        case 3:
            speed_mode = LEDC_HIGH_SPEED_MODE;
            break;
        case 4:
        case 5:
        case 6:
            speed_mode = LEDC_LOW_SPEED_MODE;
            break;
        default:
            return;
    }
            ledc_set_duty(speed_mode, current_pin, 1000);
            ledc_update_duty(speed_mode, current_pin);
            printf("%d, %d LED on\n", current_pin, filtered_value);
        }
        else{
            //gpio_set_level(LED, false);
            ledc_set_duty(speed_mode, current_pin, 0);
            ledc_update_duty(speed_mode, current_pin);
        }
        
        if(last_touch_detected != touch_detected){
            if(touch_detected){
                send_data_to_topic("20211610015/touch", notas[current_pin]);
            }
            else{
            send_data_to_topic("20211610015/touch", "0");
            }
        }
        
        last_touch_detected = touch_detected;
        vTaskDelay(4 / portTICK_PERIOD_MS);
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
    xTaskCreate(&read_sensor, "Read T0 capacitive Pin and detect touch", 4096, NULL, 5, NULL);
    xTaskCreate(&update_pwm, "Change PWM according to touch", 2048, NULL, 5, NULL);
}
