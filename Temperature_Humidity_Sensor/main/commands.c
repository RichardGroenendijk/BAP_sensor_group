/*
 * commands.c
 *
 *  Created on: 3 May 2022
 *      Author: richard
 */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "commands.h"
#include "freertos/FreeRTOS.h"

esp_err_t create_command(uint8_t *command_ptr, size_t length)
{
	esp_err_t err = 0;

	if(length >= 2)
	{
		int command = *command_ptr << 8 *(command_ptr+1);
	}
	else
		err = -1;

	printf("command = ");
	return err;
}

esp_err_t SHT35_soft_reset(i2c_port_t i2c_num, uint8_t device_address)
{
	uint8_t write_buffer[2] = {0x30, 0xA2};

	return i2c_master_write_to_device(i2c_num, device_address, *write_buffer, 2, 1);
}

// #define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
