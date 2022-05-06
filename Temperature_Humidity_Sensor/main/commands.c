/*
 * commands.c
 *
 *  Created on: 3 May 2022
 *      Author: Richard & Tom
 */

#include "commands.h"

esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t SHT35_single_measurement(int16_t *temp_ptr, uint16_t *hum_ptr)
{
	uint8_t raw_data[6] = {0};
	size_t raw_data_len=sizeof(raw_data);
	uint8_t *raw_data_ptr = raw_data;

	esp_err_t err = SHT35_single_shot_data_acquisition(raw_data_ptr, raw_data_len, false, 'H');

	if(err != ESP_OK)
		return err;

	err = process_raw_temp_hum_values(raw_data_ptr, raw_data_len, temp_ptr, hum_ptr);

	return err;
}

esp_err_t SHT35_read_out_status_register(uint8_t *data, size_t read_size)
{
	uint8_t write_buffer[2] = {0xF3, 0x2D};
	uint8_t *buffer_ptr = write_buffer;
	size_t write_size = 2;

	if(read_size > 3)
		return ESP_ERR_INVALID_ARG;

	return i2c_master_write_read_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, write_size, data, read_size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_read_and_print_status_register(void)
{
	uint8_t reg_data[3] = {0};
	size_t reg_len=sizeof(reg_data);
	uint8_t *reg_ptr = reg_data;

	esp_err_t err = SHT35_read_out_status_register(reg_ptr, reg_len);
	if(err != ESP_OK)
		return err;

	err = print_status_register(reg_ptr, reg_len);

	return err;
}

esp_err_t SHT35_single_shot_data_acquisition(uint8_t *data, size_t read_size, _Bool clock_stretching, char repeatability)
{
	esp_err_t err = ESP_OK;

	uint8_t write_buffer[2] = {0};
	uint8_t *buffer_ptr = write_buffer;
	size_t write_size = 2;

	if(read_size > 6)
		err = ESP_ERR_INVALID_ARG;

	if(clock_stretching)
	{
		write_buffer[0] = 0x2C;
		switch(repeatability)
		{
			case 'H':
				write_buffer[1] = 0x06;
				break;
			case 'M':
				write_buffer[1] = 0x0D;
				break;
			case 'L':
				write_buffer[1] = 0x10;
				break;
			default:
				err = ESP_ERR_INVALID_ARG;
		}
	}
	else
	{
		write_buffer[0] = 0x24;
		switch(repeatability)
		{
			case 'H':
				write_buffer[1] = 0x00;
				break;
			case 'M':
				write_buffer[1] = 0x0B;
				break;
			case 'L':
				write_buffer[1] = 0x16;
				break;
			default:
				err = ESP_ERR_INVALID_ARG;
		}
	}

	if(err != ESP_OK)
		return err;

	return i2c_master_write_read_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, write_size, data, read_size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_read_measurements_periodic_mode(uint8_t *data, size_t read_size)
{
	uint8_t write_buffer[2] = {0xE0, 0x00};
	uint8_t *buffer_ptr = write_buffer;
	size_t write_size = 2;

	if(read_size > 6)
		return ESP_ERR_INVALID_ARG;

	return i2c_master_write_read_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, write_size, data, read_size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

esp_err_t SHT35_heater(_Bool heater_enabled)
{
	uint8_t write_buffer[2] = {0};
	uint8_t *buffer_ptr = write_buffer;
	size_t size = 2;

	write_buffer[0] = 0x30;
	if(heater_enabled)
		write_buffer[1] = 0x6D;
	else
		write_buffer[1] = 0x66;

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
				default:
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
				default:
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
				default:
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
				default:
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
				default:
					err = ESP_ERR_INVALID_ARG;
			}
			break;

		default:
			err = ESP_ERR_INVALID_ARG;
	}

	if(err != ESP_OK)
		return err;

	err = i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, buffer_ptr, size, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

	return err;
}


/* Simple single 16-bit commands */
esp_err_t SHT35_soft_reset(void) // WERKT NIET MEER !!!!
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

