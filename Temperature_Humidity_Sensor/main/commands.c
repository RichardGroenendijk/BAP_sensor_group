/*
 * commands.c
 *
 *  Created on: 3 May 2022
 *      Author: richard
 */

#include "commands.h"

esp_err_t SHT35_heater(char enable)
{
	uint8_t write_buffer[2] = {0};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	write_buffer[0] = 0x30;
	if(enable == 'E')
		write_buffer[1] = 0x6D;
	else if(enable == 'D')
		write_buffer[1] = 0x66;
	else
		return ESP_ERR_INVALID_ARG;

	return i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_periodic_data_acquisition(int measurements_per_minute, char repeatability)
{
	esp_err_t err = ESP_OK;

	uint8_t write_buffer[2] = {0};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	switch(measurements_per_minute)
	{
		case 30:
			write_buffer[0] = 0x20;
			switch(repeatability)
			{
				case 'H':
					write_buffer[1] = 0x32;
					break;
				case 'M':
					write_buffer[1] = 0x24;
					break;
				case 'L':
					write_buffer[1] = 0x2F;
					break;
				default :
					err = ESP_ERR_INVALID_ARG;
			}
			break;

		case 60:
			write_buffer[0] = 0x21;
			switch(repeatability)
			{
				case 'H':
					write_buffer[1] = 0x30;
					break;
				case 'M':
					write_buffer[1] = 0x26;
					break;
				case 'L':
					write_buffer[1] = 0x2D;
					break;
				default :
					err = ESP_ERR_INVALID_ARG;
			}
			break;

		case 120:
			write_buffer[0] = 0x22;
			switch(repeatability)
			{
				case 'H':
					write_buffer[1] = 0x36;
					break;
				case 'M':
					write_buffer[1] = 0x20;
					break;
				case 'L':
					write_buffer[1] = 0x2B;
					break;
				default :
					err = ESP_ERR_INVALID_ARG;
			}
			break;

		case 240:
			write_buffer[0] = 0x23;
			switch(repeatability)
			{
				case 'H':
					write_buffer[1] = 0x34;
					break;
				case 'M':
					write_buffer[1] = 0x22;
					break;
				case 'L':
					write_buffer[1] = 0x29;
					break;
				default :
					err = ESP_ERR_INVALID_ARG;
			}
			break;

		case 600:
			write_buffer[0] = 0x27;
			switch(repeatability)
			{
				case 'H':
					write_buffer[1] = 0x37;
					break;
				case 'M':
					write_buffer[1] = 0x21;
					break;
				case 'L':
					write_buffer[1] = 0x2A;
					break;
				default :
					err = ESP_ERR_INVALID_ARG;
			}
			break;

		default :
			err = ESP_ERR_INVALID_ARG;
	}

	if(err != ESP_OK)
		return err;

	err = i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

	return err;
}


/* Simple single 16-bit commands */
esp_err_t SHT35_soft_reset(void)
{
	uint8_t write_buffer[2] = {0x30, 0xA2};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	return i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_break_command(void) // stop periodic data acquisition mode
{
	uint8_t write_buffer[2] = {0x30, 0x93};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	return i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_ART_command(void) // accelerated response time
{
	uint8_t write_buffer[2] = {0x2B, 0x32};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	return i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_clear_status_register(void)
{
	uint8_t write_buffer[2] = {0x30, 0x41};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	return i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}



// #define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
