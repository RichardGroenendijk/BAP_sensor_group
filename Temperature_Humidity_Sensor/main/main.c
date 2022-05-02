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

static const char *TAG = "i2c-simple-example";

#define I2C_MASTER_SCL_IO           18     				       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           19      				   /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0          				   /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SHT35_SENSOR_ADDR                 0x44


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

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
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

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void app_main(void)
{
	printf("weeeejohhhh\n");

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

	int write_buffer = 0x212D; /*MSB+LSB*/

	SHT35_register_write_byte(write_buffer);
	printf("test\n");

	uint8_t data[6] = {0};
	size_t len=sizeof(data);
	uint8_t *Temp_Hum = data;
	ESP_ERROR_CHECK(SHT35_register_read(SHT35_SENSOR_ADDR, Temp_Hum, len));







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

