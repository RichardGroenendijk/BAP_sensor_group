/*
 * adc_communication.h
 *
 *  Created on: 13 May 2022
 *      Author: Richard & Tom
 */

#ifndef MAIN_ADC_COMMUNICATION_H_
#define MAIN_ADC_COMMUNICATION_H_

#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include "esp_event.h"
#include <stdlib.h>
#include <math.h>

#define ADC_ATTEN		ADC_ATTEN_6db
#define ADC1_CHANNEL	ADC1_CHANNEL_0
#define SAMPLES			100
#define THRESHOLD		700
#define MEASURE_PERIOD	10

esp_err_t adc_config_init(void);
bool adc_calibration_init(esp_adc_cal_characteristics_t *adc1_chars);
void print_results(uint16_t *raw_ptr, uint16_t *voltage_ptr);
void print_results_stat(float *mean_ptr, float *median_ptr, float *std_dev_ptr);
void single_one_sec_measurement(esp_adc_cal_characteristics_t *adc1_chars, float *mean, float *median, float *std_dev);
float mean_array(uint16_t *array, size_t array_size);
float std_dev_array(uint16_t *array, size_t size, float mean);
float median_array(uint16_t *array, size_t array_size);
int compare_func(const void * a, const void * b);

#endif /* MAIN_ADC_COMMUNICATION_H_ */
