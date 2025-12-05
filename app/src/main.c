/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/spi.h>

#include "max7219.h"

#include <app_version.h>

#define TEST_EN 1
#define TEST_DIS 0

LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

int main (void){

	//Edit desired output on LED matrix
	uint8_t output[8]= {
		0b10000000,
		0b10100000,
		0b10001000,
		0b10000010,
		0b10000010,
		0b10001000,
		0b10100000,
		0b10000000

	};

	printk("MAX7219 LED matrix driver application.\n");

	 const struct device *led_matrix0 = DEVICE_DT_GET(DT_ALIAS(led_matrix));
	 const struct led_matrix_api *led_matr_api = (const struct led_matrix_api *)led_matrix0-> api;

	 led_matr_api->test(led_matrix0, TEST_EN);		//Enable test mode, all LEDs on
	 k_sleep(K_MSEC(1000));
	 led_matr_api->test(led_matrix0, TEST_DIS);		//Disable test mode, all LEDs off
	 k_sleep(K_MSEC(1000));

	 led_matr_api->write(led_matrix0, output);		//Write the array you have specified to the LED matrix

	 return 0;

}