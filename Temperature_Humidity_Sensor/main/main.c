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

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void periodic_measurements_test()
{
	printf("weeeejohhhh\n");

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    printf("%d\n", I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

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

void periodic_measurements_test2()
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


void single_shot_measurements_test()
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

void status_register_test()
{
	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully");

	vTaskDelay(pdMS_TO_TICKS(10));

	ESP_ERROR_CHECK(SHT35_heater(false));

	vTaskDelay(pdMS_TO_TICKS(10));

	SHT35_read_and_print_status_register();
}

void app_main(void)
{
//	periodic_measurements_test2();

//	single_shot_measurements_test();

	status_register_test();

	/* GPIO18 is SCL
	 * GPIO19 is SDA
	 *
	 * I2C_MS_MODE Set this bit to configure the I2C controller as an I2C Master. Clear this bit to configure
		the I2C controller as a slave. (R/W)


		INITIALIZATION
		I2C_CTR_REG->I2C_MS_MODE = 1; Set master mode
		I2C_CTR_REG->I2C_CONF_UPGATE = 1 Synchronize registers
		I2C_COMD0_REG->I2C_COMMAND0 =  (opcode=Rstart=0/6?)000 0000 0000
		I2C_COMD1_REG->I2C_COMMAND1 =  (opcode=Write=1)X?1 0000 0001
		I2C_COMD2_REG->I2C_COMMAND2 =  (opcode=STOP=2)000 0000 0000
		I2C_SLAVE_ADDR_REG->I2C_SLAVE_ADDR = 0x44+(1) for write/!read slave adress of SHT35
		No clock stretching and the lowest repeatability (power saving)


	 */
















    /*nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    int level = 0;
    while (true) {
        gpio_set_level(GPIO_NUM_4, level);
        level = !level;
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }*/

    /*ESP_LOGI(TAG, "SEXXXXXXXXXXXXXXXXXXXXXXX");*/

}

