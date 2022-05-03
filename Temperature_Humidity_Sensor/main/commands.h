/*
 * commands.h
 *
 *  Created on: 3 May 2022
 *      Author: richard
 */

#ifndef MAIN_COMMANDS_H_
#define MAIN_COMMANDS_H_

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "i2c_data.h"

esp_err_t SHT35_read_measurements_periodic_mode(uint8_t *data, size_t read_size);
esp_err_t SHT35_heater(char enable);
esp_err_t SHT35_periodic_data_acquisition(int measurements_per_minute, char repeatability);
esp_err_t SHT35_soft_reset(void);
esp_err_t SHT35_break_command(void);
esp_err_t SHT35_ART_command(void);
esp_err_t SHT35_clear_status_register(void);

#endif /* MAIN_COMMANDS_H_ */
