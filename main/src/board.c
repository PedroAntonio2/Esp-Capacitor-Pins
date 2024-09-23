#include <stdio.h>
#include "esp_rom_gpio.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "capacitive_touch.h"
#include "board.h"
#include "wifi.h"
#include "generate_PWM.h"
//void setup_led(ledc_timer_bit_t duty_resolution, uint16_t freq, ledc_mode_t speed_mode, ledc_timer_t timer_num, ledc_channel_t channel, ledc_channel_t gpio_num);

//16, 17, 18, 19, 21, 22, 23

#define TOUCH0 16
#define TOUCH1 25
#define TOUCH2 18
#define TOUCH3 19
#define TOUCH4 21
#define TOUCH5 22
#define TOUCH6 23

const int touch_pins[] = {TOUCH0, TOUCH1, TOUCH2, TOUCH3, TOUCH4, TOUCH5, TOUCH6};
const int capacitive_pins[] = {0, 1, 2, 3, 4, 5, 6}; 
const int frequencies[] = {261, 293, 329, 349, 392, 440, 493};

void board_init() {
  //LEDs configuration
  for (int i = 0; i < 7; i++) {
    setup_pwm_for_touch(i, frequencies[i], touch_pins[i]);
    capaticitive_pin_init(capacitive_pins[i]);
  }
  //esp_rom_gpio_pad_select_gpio(LED);
  //gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  //Touch sensor configuration
  

  printf("Finished LED and Touch sensor");

  printf("Starting Wifi init");
  //initialise_wifi();
}