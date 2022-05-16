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
#include "adc_communication.h"

//#include "adc.h"



//static const char *TAG = "ADC DMA";



//esp_err_t event_handler(void *ctx, system_event_t *event)
//{
//    return ESP_OK;
//}

void app_main(void)
{
	printf("hallo\n");
	esp_adc_cal_characteristics_t adc1_chars;
	bool cali_enable = adc_calibration_init(&adc1_chars);
	printf("Bool = %d\n", cali_enable);

//	ESP_ERROR_CHECK_WITHOUT_ABORT(continuous_adc_init());
//	ESP_ERROR_CHECK_WITHOUT_ABORT(adc_digi_start());
//
//	printf("Test1\n");
//
//	esp_err_t ret;
//	uint32_t ret_num = 0;
//	uint8_t result[TIMES] = {0};
//	memset(result, 0xcc, TIMES);
//
//	ret = adc_digi_read_bytes(result, TIMES, &ret_num, ADC_MAX_DELAY);
//	printf("ret_num = %u\n", ret_num);
//
//	for (int i = 0; i < TIMES; i++)
//	{
//		printf("result[%u] = %u\n", i, result[i]);
//	}

//    uint32_t length_max = 256;
//    int timeout_ms = 50;
//    uint8_t data = 0;
//    uint8_t *bufpoint = data;
//    //uint32_t data_length = 0;
//    uint32_t out_length = 0;
//    esp_err_t ret;
//
//    ret = adc_digi_read_bytes(bufpoint, length_max, &out_length, timeout_ms);

//    while(1){
//    ret = adc_digi_read_bytes(bufpoint, length_max, &out_length, timeout_ms);
//    printf("out length = %d\n data = %d\n", out_length, bufpoint);
//    for (int i = 0; i < out_length; i += ADC_RESULT_BYTE) {
//        adc_digi_output_data_t *p = (void*)&bufpoint[i];
//        printf("Unit: %d, Channel: %d, Value: %x", 1, p->type2.channel, p->type2.data);
//        //ESP_LOGI("Unit: %d, Channel: %d, Value: %x", 1, p->type1.channel, p->type1.data);
//    }
//    vTaskDelay(100);
//    //printf("test");
//    }
//    adc_digi_stop();
//
//    ret = adc_digi_deinitialize();










}

