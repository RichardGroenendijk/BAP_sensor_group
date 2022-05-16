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

/* For dma mode */
//#define TIMES              256
//#define GET_UNIT(x)        ((x>>3) & 0x1)
//
//#define ADC_RESULT_BYTE     4
//#define ADC_CONV_LIMIT_EN   0
//#define ADC_CONV_MODE       ADC_CONV_ALTER_UNIT     //ESP32C3 only supports alter mode
//#define ADC_OUTPUT_TYPE     ADC_DIGI_OUTPUT_FORMAT_TYPE2
//
//static uint32_t adc1_chan_mask = ADC1_CHANNEL_0;
////static uint16_t adc1_chan_mask = BIT(2) | BIT(3);
//static uint32_t adc2_chan_mask = BIT(0);
//
//esp_err_t continuous_adc_init();

#define ADC_ATTEN_CH1	ADC_ATTEN_11db

bool adc_calibration_init(esp_adc_cal_characteristics_t *adc1_chars);

#endif /* MAIN_ADC_COMMUNICATION_H_ */
