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

	uint8_t output[8]= {
		0b10000000;
		0b10000000;
		0b10000000;
		0b10000000;
		0b10000000;
		0b10000000;
		0b10000000;
		0b10000000;

	};

	printk("MAX7219 LED matrix driver application.\n");

	const struct device *led_matrix0 = DEVICE_DT_GET(DT_ALIAS(led-matrix));
	const struct led_matrix_api *led_matr_api = (const struct led_matrix_api *)led_matrix0-> api;

	led_matr_api->test(led_matrix0, TEST_EN);
	k_sleep(K_MSEC(1000));
	led_matr_api->test(led_matrix0, TEST_DIS);
	k_sleep(K_MSEC(1000));

	led_matr_api->write(led_matrix0, output);

	return 0;

}




// int main(void)
// {
// 	int ret;
// 	unsigned int period_ms = BLINK_PERIOD_MS_MAX;
// 	const struct device *sensor, *blink;
// 	struct sensor_value last_val = { 0 }, val;

// 	printk("Zephyr Example Application %s\n", APP_VERSION_STRING);

// 	sensor = DEVICE_DT_GET(DT_NODELABEL(example_sensor));
// 	if (!device_is_ready(sensor)) {
// 		LOG_ERR("Sensor not ready");
// 		return 0;
// 	}

// 	blink = DEVICE_DT_GET(DT_NODELABEL(blink_led));
// 	if (!device_is_ready(blink)) {
// 		LOG_ERR("Blink LED not ready");
// 		return 0;
// 	}

// 	ret = blink_off(blink);
// 	if (ret < 0) {
// 		LOG_ERR("Could not turn off LED (%d)", ret);
// 		return 0;
// 	}

// 	printk("Use the sensor to change LED blinking period\n");

// 	while (1) {
// 		ret = sensor_sample_fetch(sensor);
// 		if (ret < 0) {
// 			LOG_ERR("Could not fetch sample (%d)", ret);
// 			return 0;
// 		}

// 		ret = sensor_channel_get(sensor, SENSOR_CHAN_PROX, &val);
// 		if (ret < 0) {
// 			LOG_ERR("Could not get sample (%d)", ret);
// 			return 0;
// 		}

// 		if ((last_val.val1 == 0) && (val.val1 == 1)) {
// 			if (period_ms == 0U) {
// 				period_ms = BLINK_PERIOD_MS_MAX;
// 			} else {
// 				period_ms -= BLINK_PERIOD_MS_STEP;
// 			}

// 			printk("Proximity detected, setting LED period to %u ms\n",
// 			       period_ms);
// 			blink_set_period_ms(blink, period_ms);
// 		}

// 		last_val = val;

// 		k_sleep(K_MSEC(100));
// 	}

// 	return 0;
// }

