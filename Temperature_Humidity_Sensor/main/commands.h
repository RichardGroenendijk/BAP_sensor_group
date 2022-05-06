/*
 * commands.h
 *
 *  Created on: 3 May 2022
 *      Author: Richard & Tom
 */

#ifndef MAIN_COMMANDS_H_
#define MAIN_COMMANDS_H_

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "i2c_data.h"
#include "communication.h"

esp_err_t i2c_master_init(void);
esp_err_t SHT35_single_measurement(int16_t *temp_ptr, uint16_t *hum_ptr);
esp_err_t SHT35_read_out_status_register(uint8_t *data, size_t read_size);
esp_err_t SHT35_read_and_print_status_register(void);
esp_err_t SHT35_single_shot_data_acquisition(uint8_t *data, size_t read_size, _Bool clock_stretching, char repeatability);
esp_err_t SHT35_read_measurements_periodic_mode(uint8_t *data, size_t read_size);
esp_err_t SHT35_heater(_Bool heater_enabled);
esp_err_t SHT35_periodic_data_acquisition(int measurements_per_minute, char repeatability);
esp_err_t SHT35_soft_reset(void);
esp_err_t SHT35_break_command(void);
esp_err_t SHT35_ART_command(void);
esp_err_t SHT35_clear_status_register(void);

#endif /* MAIN_COMMANDS_H_ */
