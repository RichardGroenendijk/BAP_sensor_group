/*
 * loudness_sensor.c
 *
 *  Created on: 13 May 2022
 *      Author: Richard & Tom
 */

#include "loudness_sensor.h"

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

void print_results_stat(float *mean_ptr, float *median_ptr, float *std_dev_ptr)
{
	ESP_LOGI(TAG, "Mean:	%.2f", *mean_ptr);
	ESP_LOGI(TAG, "Median:	%.2f", *median_ptr);
	ESP_LOGI(TAG, "Std dev:	%.2f", *std_dev_ptr);
}

void single_one_sec_measurement(esp_adc_cal_characteristics_t *adc1_chars, float *mean, float *median, float *std_dev)
{
	uint16_t adc_raw[SAMPLES] = {0};
	uint16_t voltage[SAMPLES] = {0};

	for (int i = 0; i < SAMPLES; i++)
	{
		adc_raw[i] = adc1_get_raw(ADC1_CHANNEL);
		voltage[i] = esp_adc_cal_raw_to_voltage(adc_raw[i], adc1_chars);
		vTaskDelay(pdMS_TO_TICKS(MEASURE_PERIOD));
	}

	float raw_mean = mean_array(adc_raw, SAMPLES);

	ESP_LOGI(TAG, "Raw mean: %.2f", raw_mean);

	*mean = mean_array(voltage, SAMPLES);
	*median = median_array(voltage, SAMPLES);
	*std_dev = std_dev_array(voltage, SAMPLES, *mean);
}

float mean_array(uint16_t *array, size_t array_size)
{
	float mean = 0.0;

	for (int i = 0; i < array_size; i++)
		mean += array[i];

	return mean/array_size;
}

float std_dev_array(uint16_t *array, size_t size, float mean)
{
	float sum_squared_diff = 0;
	for (int i = 0; i < size; i++)
		sum_squared_diff += (array[i] - mean)*(array[i] - mean);

	return sqrt(sum_squared_diff/size);
}

float median_array(uint16_t *array, size_t size)
{
	qsort(array, size, sizeof(uint16_t), compare_func);
//	for (int i = 0; i < size; i++)
//	{
//		ESP_LOGI(TAG, "Element %i: %u", i, array[i]);
//	}

	if (size % 2 != 0)
		return (float)array[size/2];
	else
		return (float)(array[(size-1)/2] + array[size/2])/2.0;
}

int compare_func(const void * a, const void * b)
{
   // qsort() passes in `void*` types because it can't know the actual types being sorted
   // convert those pointers to pointers to int and deref them to get the actual int values

   uint16_t val1 = *(uint16_t*)a;
   uint16_t val2 = *(uint16_t*)b;

   // qsort() expects the comparison function to return:
   //
   //    a negative result if val1 < val2
   //    0 if val1 == val2
   //    a positive result if val1 > val2

   return ( val1 - val2 );
}
