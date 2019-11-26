/*
 * ldr_interface.c
 *
 *  Created on: 24 Nov 2019
 *      Author: IHA
 */

//#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define LDR_GPIO GPIO_NUM_4
#define ESP_INTR_FLAG_DEFAULT 0

static SemaphoreHandle_t _semaphoreToGive = NULL;

static void IRAM_ATTR pumpStateIsrHandler(void *arg) {
	xSemaphoreGiveFromISR(_semaphoreToGive, NULL);
}

void ldrInit(SemaphoreHandle_t semaphoreToGive) {
	_semaphoreToGive = semaphoreToGive;
	gpio_config_t io_conf;
//interrupt of rising edge
	io_conf.intr_type = GPIO_INTR_ANYEDGE;
//bit mask of the pins, use GPIO4/5 here
	io_conf.pin_bit_mask = (1ULL << LDR_GPIO);
//set as input mode
	io_conf.mode = GPIO_MODE_INPUT;
//disable pull-down mode
	io_conf.pull_down_en = 0;
//disable pull-up mode
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
//hook isr handler for specific gpio pin
	gpio_isr_handler_add(LDR_GPIO, pumpStateIsrHandler,
	NULL);
}

bool ldrSeeLight(void) {
	return (gpio_get_level(LDR_GPIO) == 0);
}
