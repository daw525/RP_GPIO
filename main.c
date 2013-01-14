/*******************************************************************************
 * main.c
 *
 * Sample program using RP_GPIO library to blink an LED connected to pin 17 on
 * the Raspberry Pi.
 *
 ******************************************************************************/
#include <stdio.h>
#include <unistd.h>	/* Contains sleep() function */
#include "RP_GPIO.h"

int main(int argc, char **argv) {
	int i = 0;

	/* 1. Export the pin before use */
	RP_GPIO_export(17, ON);

	/* 2. Configure the pin direction */
	RP_GPIO_direction(17, OUT);

	for (i = 0; i < 10; i++) {
		/* 3. Toggle pin */
		RP_GPIO_write(17, ON);
		sleep(1);
		RP_GPIO_write(17, OFF);
		sleep(1);
	}

	/* 4. Unexport the pin when finished */
	RP_GPIO_export(17, OFF);

	return (0);
}
