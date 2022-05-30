#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "loudness_sensor.h"

void while_loop(esp_adc_cal_characteristics_t *adc1_chars)
{
	uint16_t adc_raw;
	uint16_t voltage;

	while (true)
	{
		adc_raw = adc1_get_raw(ADC1_CHANNEL);
		voltage = esp_adc_cal_raw_to_voltage(adc_raw, adc1_chars);

		print_results(&adc_raw, &voltage);
        vTaskDelay(pdMS_TO_TICKS(10));
	}
}

void rolling_average(esp_adc_cal_characteristics_t *adc1_chars)
{
	uint16_t adc_raw[SAMPLES] = {0};
	uint16_t voltage[SAMPLES] = {0};
	float rolling_average;

	while (true)
	{
		for (int i = 0; i < (SAMPLES); i++)
		{
			adc_raw[i] = adc1_get_raw(ADC1_CHANNEL);
			voltage[i] = esp_adc_cal_raw_to_voltage(adc_raw[i], adc1_chars);

			rolling_average = mean_array(voltage, SAMPLES);

			if (rolling_average > THRESHOLD)
				ESP_LOGI("ADC", "Rolling average: %f", rolling_average);

			vTaskDelay(pdMS_TO_TICKS(MEASURE_PERIOD));
		}
	}
}

void continuous_measurements(esp_adc_cal_characteristics_t *adc1_chars)
{
	float mean, median, std_dev;

	while (true)
	{
		single_one_sec_measurement(adc1_chars, &mean, &median, &std_dev);

		print_results_stat(&mean, &median, &std_dev);
	}
}

void app_main(void)
{
	esp_adc_cal_characteristics_t adc1_chars;
	bool cali_enable = adc_calibration_init(&adc1_chars);
	if (cali_enable)
		ESP_LOGI("ADC", "Calibrated correctly");

	ESP_ERROR_CHECK_WITHOUT_ABORT(adc_config_init());

//	rolling_average(&adc1_chars);

	continuous_measurements(&adc1_chars);

}

