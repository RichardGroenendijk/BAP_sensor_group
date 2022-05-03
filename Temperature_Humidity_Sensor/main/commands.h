/*
 * commands.h
 *
 *  Created on: 3 May 2022
 *      Author: richard
 */

#ifndef MAIN_COMMANDS_H_
#define MAIN_COMMANDS_H_

#include "freertos/FreeRTOS.h"

esp_err_t create_command(uint8_t *command, size_t length);

#endif /* MAIN_COMMANDS_H_ */
