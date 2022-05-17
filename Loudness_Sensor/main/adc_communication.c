/*
 * adc_communication.c
 *
 *  Created on: 13 May 2022
 *      Author: Richard & Tom
 */

#include "adc_communication.h"

static const char *TAG = "ADC";

esp_err_t adc_config_init(void)
{
	esp_err_t err;

	err = adc1_config_width(ADC_WIDTH_BIT_DEFAULT);

	if (err == ESP_OK)
		err = adc1_config_channel_atten(ADC1_CHANNEL, ADC_ATTEN);

	return err;
}

bool adc_calibration_init(esp_adc_cal_characteristics_t *adc1_chars)
{
	esp_err_t ret;
    bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
    	ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    }
    else if (ret == ESP_ERR_INVALID_VERSION) {
    	ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    }
    else if (ret == ESP_OK) {
    	cali_enable = true;
    	esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH_BIT_DEFAULT, 0, adc1_chars);
    }
    else {
    	ESP_LOGE(TAG, "Invalid arg");
    }

    return cali_enable;
}

void print_results(uint16_t *raw_ptr, uint16_t *voltage_ptr)
{
	ESP_LOGI(TAG, "Raw data:     %d", *raw_ptr);
	ESP_LOGI(TAG, "Calc voltage: %d mV", *voltage_ptr);
}

float average_array(uint16_t *array, size_t array_size)
{
	float average = 0.0;

	for (int i = 0; i < array_size; i++)
		average += array[i];

	return average/array_size;
}

//esp_err_t continuous_adc_init(void)
//{
//	esp_err_t err;
//
//    adc_digi_init_config_t adc_dma_config = {
//        .max_store_buf_size = 1024,
//        .conv_num_each_intr = 256,
//        .adc1_chan_mask = adc1_chan_mask,
//        .adc2_chan_mask = adc2_chan_mask,
//    };
//
//    err = adc_digi_initialize(&adc_dma_config);
//    if (err != ESP_OK)
//    	return err;
//
//    adc_digi_configuration_t dig_cfg = {
//        .conv_limit_en = ADC_CONV_LIMIT_EN,
//        .conv_limit_num = 250,
//        .sample_freq_hz = 10 * 1000,
//        .conv_mode = ADC_CONV_MODE,
//        .format = ADC_OUTPUT_TYPE,
//    };
//
//    err = adc_digi_controller_configure(&dig_cfg);
//    return err;
//}
