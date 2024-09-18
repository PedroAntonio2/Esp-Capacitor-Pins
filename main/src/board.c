#include <stdio.h>
#include "esp_rom_gpio.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#include "capacitive_touch.h"
#include "board.h"
#include "wifi.h"
#include "generate_PWM.h"
//void setup_led(ledc_timer_bit_t duty_resolution, uint16_t freq, ledc_mode_t speed_mode, ledc_timer_t timer_num, ledc_channel_t channel, ledc_channel_t gpio_num);

#define LED 2

void board_init() {
  int wifi_status = 0;
  
  //LEDs configuration
  setup_pwm(LEDC_TIMER_13_BIT, 261, LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, LEDC_CHANNEL_0, LED);
  //esp_rom_gpio_pad_select_gpio(LED);
  //gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  //Touch sensor configuration
  capaticitive_pin_init();

  printf("Finished LED and Touch sensor");

  printf("Starting Wifi init");
  initialise_wifi();
}