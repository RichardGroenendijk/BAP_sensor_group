#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"


#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/adc.h"

//#include "adc.h"

#define TIMES              256
#define GET_UNIT(x)        ((x>>3) & 0x1)

#define ADC_RESULT_BYTE     4
#define ADC_CONV_LIMIT_EN   0
#define ADC_CONV_MODE       ADC_CONV_ALTER_UNIT     //ESP32C3 only supports alter mode
#define ADC_OUTPUT_TYPE     ADC_DIGI_OUTPUT_FORMAT_TYPE2

static uint32_t adc1_chan_mask = ADC1_CHANNEL_0;
static uint32_t adc2_chan_mask = BIT(0);




//static const char *TAG = "ADC DMA";



//esp_err_t event_handler(void *ctx, system_event_t *event)
//{
//    return ESP_OK;
//}

void app_main(void)
{
    adc_digi_init_config_t adc_dma_config = {
        .max_store_buf_size = 1024,
        .conv_num_each_intr = 256,
        .adc1_chan_mask = adc1_chan_mask,
        .adc2_chan_mask = adc2_chan_mask,
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_digi_initialize(&adc_dma_config));

    adc_digi_configuration_t dig_cfg = {
        .conv_limit_en = ADC_CONV_LIMIT_EN,
        .conv_limit_num = 250,
        .sample_freq_hz = 10 * 1000,
        .conv_mode = ADC_CONV_MODE,
        .format = ADC_OUTPUT_TYPE,
    };

    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_digi_controller_configure(&dig_cfg));

    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_digi_start());


    uint32_t length_max = 256;
    int timeout_ms = 50;
    uint8_t data = 0;
    uint8_t *bufpoint = data;
    //uint32_t data_length = 0;
    uint32_t out_length = 0;
    esp_err_t ret;

    while(1){
    ret = adc_digi_read_bytes(bufpoint, length_max, &out_length, timeout_ms);
    printf("out length = %d\n data = %d\n", out_length, bufpoint);
    for (int i = 0; i < out_length; i += ADC_RESULT_BYTE) {
        adc_digi_output_data_t *p = (void*)&bufpoint[i];
        printf("Unit: %d, Channel: %d, Value: %x", 1, p->type2.channel, p->type2.data);
        //ESP_LOGI("Unit: %d, Channel: %d, Value: %x", 1, p->type1.channel, p->type1.data);
    }
    vTaskDelay(100);
    //printf("test");
    }
    adc_digi_stop();

    ret = adc_digi_deinitialize();










}

