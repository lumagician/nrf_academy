/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 10*60*1000

// devicetree node aliases
#define LED0_NODE DT_ALIAS(led0)

#define SW0_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led1);
}

static struct gpio_callback button1_cb_data;

void main(void)
{
	int ret;

	if (!device_is_ready(led1.port)) {
		return;
	}

	if (!device_is_ready(button1.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	ret = gpio_pin_configure_dt(&button1, GPIO_INPUT);
	if (ret < 0) {
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret < 0) {
		return;
	}

	gpio_init_callback(&button1_cb_data, button_pressed, BIT(button1.pin));
	gpio_add_callback(button1.port, &button1_cb_data);


	while (1) {
		// sleep for a long time
		k_msleep(SLEEP_TIME_MS);
	}
}

