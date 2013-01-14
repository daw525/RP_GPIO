#include <stdio.h>
#include <unistd.h>	/* Contains sleep() function */
#include "RP_GPIO.h"

int main(int argc, char **argv) {
	int i = 0;

	/* Must export the pin before use */
	RP_GPIO_export(17, ON);
	RP_GPIO_direction(17, OUT);

	for (i = 0; i < 10; i++) {
		RP_GPIO_write(17, ON);
		sleep(1);
		RP_GPIO_write(17, OFF);
		sleep(1);
	}

	/* Unexport the pin when finished */
	RP_GPIO_export(17, OFF);

	return (0);
}
