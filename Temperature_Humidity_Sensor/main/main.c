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
#include "i2c_data.h"
#include "communication.h"

static const char *TAG = "i2c-simple-example";

/**
 * @brief Read a sequence of bytes from a MPU9250 sensor registers
 */
static esp_err_t SHT35_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
}

/**
 * @brief Write a byte to a MPU9250 sensor register
 */
static esp_err_t SHT35_register_write_byte(int write_buffer)
{
	uint8_t data[2] = {(write_buffer >> 8), (write_buffer&0xFF)};
	uint8_t *pointer = data;
	uint8_t lengthofdata = sizeof(data);
	printf(" the size of the data %u",lengthofdata);


    int ret;
    /*uint8_t write_buf[2] = {reg_addr, data};*/

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, SHT35_SENSOR_ADDR, pointer, lengthofdata, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

    return ret;
}

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

static void SHT35_single_measurement_test();
static void periodic_measurements_test();
static void periodic_measurements_test2();
static void single_shot_measurements_test();
static void status_register_test();

void app_main(void)
{
//	periodic_measurements_test2();

//	single_shot_measurements_test();

//	status_register_test();

	SHT35_single_measurement_test();

}

static void SHT35_single_measurement_test()
{
	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully");

	vTaskDelay(pdMS_TO_TICKS(10));

	int16_t temperature;
	uint16_t humidity;

	ESP_ERROR_CHECK(SHT35_single_measurement(&temperature, &humidity));

	print_sensor_values(&temperature, &humidity);
}

static void periodic_measurements_test()
{
	printf("weeeejohhhh\n");

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

//    printf("%d\n", I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

    vTaskDelay(pdMS_TO_TICKS(100));
    ESP_ERROR_CHECK(SHT35_clear_status_register());

    vTaskDelay(pdMS_TO_TICKS(100));

    ESP_ERROR_CHECK(SHT35_periodic_data_acquisition(60,'H'));

	while(true)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));

		uint8_t data[6] = {0};
		size_t len=sizeof(data);
		uint8_t *Temp_Hum = data;
		ESP_ERROR_CHECK(SHT35_read_measurements_periodic_mode(Temp_Hum, len));

		float temperature = -45 + 175.0*((data[0] << 8) + data[1])/(65536.0 - 1);
		float humidity = 100.0*((data[3] << 8) + data[4])/(65536.0 - 1);
		printf("Temperature: %f\n", temperature);
		printf("Humidity:    %f\n", humidity);
	}
}

static void periodic_measurements_test2()
{
	printf("weeeejohhhh\n");

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

//    vTaskDelay(pdMS_TO_TICKS(1000));
//    ESP_ERROR_CHECK(SHT35_soft_reset());

    vTaskDelay(pdMS_TO_TICKS(1000));

    ESP_ERROR_CHECK(SHT35_periodic_data_acquisition(60,'H'));

	while(true)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));

		uint8_t data[6] = {0};
		size_t len=sizeof(data);
		uint8_t *temp_hum = data;
		ESP_ERROR_CHECK(SHT35_read_measurements_periodic_mode(temp_hum, len));

		int16_t temperature;
		uint16_t humidity;
		ESP_ERROR_CHECK(process_raw_temp_hum_values(temp_hum, len, &temperature, &humidity));

		print_sensor_values(&temperature, &humidity);
	}
}

static void single_shot_measurements_test()
{
	printf("weeeejohhhh\n");

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

//	ESP_ERROR_CHECK(SHT35_soft_reset());
//	ESP_ERROR_CHECK(SHT35_heater(false));


    vTaskDelay(pdMS_TO_TICKS(10));

    while(true)
    {
		vTaskDelay(pdMS_TO_TICKS(1000));

		uint8_t data[6] = {0};
		size_t len=sizeof(data);
		uint8_t *temp_hum = data;
		ESP_ERROR_CHECK(SHT35_single_shot_data_acquisition(temp_hum, len, false, 'H'));

		int16_t temperature;
		uint16_t humidity;
		ESP_ERROR_CHECK(process_raw_temp_hum_values(temp_hum, len, &temperature, &humidity));

		print_sensor_values(&temperature, &humidity);
    }
}

static void status_register_test()
{
	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully");

	vTaskDelay(pdMS_TO_TICKS(10));

	ESP_ERROR_CHECK(SHT35_heater(false));

	vTaskDelay(pdMS_TO_TICKS(10));

	SHT35_read_and_print_status_register();
}


