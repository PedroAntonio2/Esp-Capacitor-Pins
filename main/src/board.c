#include <stdio.h>
#include "esp_rom_gpio.h"
#include "driver/gpio.h"

#include "capacitive_touch.h"
#include "board.h"
#include "wifi.h"

#define LED 2

void board_init() {
  int wifi_status = 0;
  
  //LED configuration
  esp_rom_gpio_pad_select_gpio(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  //Touch sensor configuration
  capaticitive_pin_init();

  printf("Finished LED and Touch sensor");

  printf("Starting Wifi init");
  wifi_status = initialise_wifi();
  if(wifi_status == 1){
    printf("Wifi init success");
  }
  else{
    printf("Wifi init failed");
  }
}