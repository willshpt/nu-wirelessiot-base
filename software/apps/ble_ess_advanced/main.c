// BLE Service example app
//
// Creates a BLE environmental sensing service

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = 0xAABB,
  .adv_name          = "ESSTestWill", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t environmental_sensing_service = {{
  .uuid128 = {0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,
              0x00,0x10,0x00,0x00,0x1A,0x18,0x00,0x00}
}};

static simple_ble_char_t temperature_char = {.uuid16 = 0x2A6E};
static simple_ble_char_t uv_index_char = {.uuid16 = 0x2A76};
static simple_ble_char_t wind_chill_char = {.uuid16 = 0x2A79};
static simple_ble_char_t rainfall_char = {.uuid16 = 0x2A78};
static simple_ble_char_t humidity_char = {.uuid16 = 0x2A6F};
static int16_t temperature_val = -200;
static uint8_t uv_val = 1;
static int8_t wind_chill_val = -8;
static uint16_t rainfall_val = 279;
static uint16_t humidity_val = 94;


/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
  printf("Got write to a characteristic!\n");
}

int main(void) {
  printf("Board started. Initializing BLE: \n");
  temperature_val = -200;
  uv_val = 1;
  wind_chill_val = -8;
  rainfall_val = 279;
  humidity_val = 94;
  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&environmental_sensing_service);
  simple_ble_add_characteristic(1, 0, 0, 0,
      sizeof(temperature_val), (uint8_t*)&temperature_val,
      &environmental_sensing_service, &temperature_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
      sizeof(wind_chill_val), (uint8_t*)&wind_chill_val,
      &environmental_sensing_service, &wind_chill_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
      sizeof(uv_val), (uint8_t*)&uv_val,
      &environmental_sensing_service, &uv_index_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
      sizeof(rainfall_val), (uint8_t*)&rainfall_val,
      &environmental_sensing_service, &rainfall_char);
  simple_ble_add_characteristic(1, 0, 0, 0,
      sizeof(humidity_val), (uint8_t*)&humidity_val,
      &environmental_sensing_service, &humidity_char);


  // Start Advertising
  /*int8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {0x11, 0x21, 0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,
              0x00,0x10,0x00,0x00,0x1A,0x18,0x00,0x00};

  simple_ble_adv_raw(ble_data, 18);
  printf("Started BLE advertisements\n");*/
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}

