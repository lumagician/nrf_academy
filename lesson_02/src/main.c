/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>


// define an isr for int_button
void pin_isr(const struct device *dev, struct gpio_callback *cb, gpio_port_pins_t pins);

// get gpio pointers from devicetree macros
struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_NODELABEL(led1), gpios);
struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_NODELABEL(button0), gpios);
struct gpio_dt_spec int_button = GPIO_DT_SPEC_GET(DT_NODELABEL(button1), gpios);

// gpio isr callback 
static struct gpio_callback pin_cb_data;

void main(void)
{
	int ret;

	// wait till the devices are ready
	if (!device_is_ready(led.port)) {
		return;
	}

	if (!device_is_ready(button.port)) {
		return;
	}

	if (!device_is_ready(int_button.port)) {
		return;
	}
	printk("all devices ready!\n");


	// configure led pin as an output
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	// configure button1 pin as an input
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return;
	}

	// configure button2 pin as an interrupt input
	ret = gpio_pin_interrupt_configure_dt(&int_button, GPIO_INT_EDGE_BOTH);
	if (ret < 0) {
		return;
	}



	// init callback function for int_button interrupts
	gpio_init_callback(&pin_cb_data, pin_isr, BIT(int_button.pin));

	// add callback function
	gpio_add_callback(int_button.port, &pin_cb_data);

	printk("Initialization done!\n");

	while (1) {

		k_msleep(1000);
		printk("looping\n");
	
	}
}

// isr implementation

void pin_isr(const struct device *dev, struct gpio_callback *cb, gpio_port_pins_t pins) {
	printk("isr was called!\n");
	gpio_pin_toggle_dt(&led);
	printk("gpio was toggled\n");
	return;
}