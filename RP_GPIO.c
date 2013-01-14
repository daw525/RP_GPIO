/*******************************************************************************
 * RP_GPIO.c
 * GPIO library for Raspberry Pi, using the /sys/ file system.
 * 
 * In order to use this library without running as root:
 *
 * 	1. 	The following line must be added to /etc/rc.local
 *
 *			chown -R pi:pi /sys/class/gpio 
 *
 * 	2. 	A text file called 99-gpio.rules must be created 
 *		in /etc/udev/rules.d with the following contents 
 *
 *	SUBSYSTEM=="gpio", ACTION=="add", PROGRAM="/bin/sh -c 'chown -R pi:pi /sys%p'"
 *
 * - History ------------------------------------------------------------------
 *
 * Version 1 (2012/12/22) - File created
 * Version 2 (2013/01/14) - Comments added
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "RP_GPIO.h"

//#define DEBUG_ON (1) /* Uncomment to allow debug to stdout */

static int pinValid(int);

/******************************************************************************
 * Function 	: pinValid	
 * Purpose		: Helper function to provide error checking on pin numbers
 *				  passed to other functions. Designed for ras-pi HW rev. 1.
 * Parameters	: Integer value.
 * Returns		: 1 if valid GPIO pin, else 0.
 ******************************************************************************/
static int pinValid(int pin) {
	int found=0;
	int i=0;
	static int raspi_gpio[17] = {0,1,4,7,8,9,10,11,14,15,17,18,21,22,23,24,25};
	while (!found && i<17) {
		found = (pin == raspi_gpio[i]);
		i++;
	}
	return found;
}

/******************************************************************************
 * Function 	: RP_GPIO_export	
 * Purpose		: Export a pin to make it available for reading or writing, 
 *                also unexport a pin when it is no longer required.
 * Parameters	: (1) Integer value of pin number to export.
 *				  (2) RP_GPIO_STATE - ON to export, OFF to unexport.
 * Returns		: 0 on success, -1 on failure.
 ******************************************************************************/
int RP_GPIO_export(int pin, RP_GPIO_STATE state) {
	int countout = 10000;
	struct stat sb;
	char b[50];	
	int result;
	FILE *f;
	if (!pinValid(pin)) return -1;
	if (state == ON) {
		f = fopen("/sys/class/gpio/export","w");
	} else {
		f = fopen("/sys/class/gpio/unexport","w");
	}
	if (NULL == f) return -1;
	sprintf(b,"%d",pin);
	result = fputs(b,f);
	if (0 > result) return -1;
	if (0 != fclose(f)) return -1;

	if (state == ON) {
/*	Once the pin has been exported, it takes a little time to become
	available for use (micro seconds). This is a small polling loop with 
	timeout that waits for the pin to become ready */
		sprintf(b,"/sys/devices/virtual/gpio/gpio%d",pin);
		if (-1 == stat(b,&sb)) return -1;
		while ((0 < countout) && (sb.st_uid != getuid())) {
			if (-1 == stat(b,&sb)) return -1;
			usleep(100);
			countout--;
		}
		if (countout == 0) return -1;
#ifdef DEBUG_ON
/* Enable debugging to see how long the pin took to become available */
		printf("export time=%duS\n",((10000 - countout)*100));
#endif
	}
	return 0;
}

/******************************************************************************
 * Function 	: RP_GPIO_direction
 * Purpose		: Configure the direction of a GPIO pin.
 * Parameters	: (1) Integer value of pin number
 *				  (2) RP_GPIO_DIR - IN for input, OUT for output.
 * Returns		: 0 on success, -1 on failure.
 ******************************************************************************/
int RP_GPIO_direction(int pin, RP_GPIO_DIR dir) {
	char b[50];	
	int result;
	FILE *f;
	if (!pinValid(pin)) return -1;
	sprintf(b,"/sys/devices/virtual/gpio/gpio%d/direction",pin);
	f = fopen(b,"w");
	if (NULL == f) return -1;
	if (dir == IN) {
		result = fputs("in",f);
	} else {
		result = fputs("out",f);
	}
	if (0 > result) return -1;
	if (0 != fclose(f)) return -1;
	return 0;
}

/******************************************************************************
 * Function 	: RP_GPIO_write
 * Purpose		: Toggle the state of a GPIO pin that has been previously
 *				  configured as an output.
 * Parameters	: (1) Integer value of GPIO pin
 *				  (2) RP_GPIO_STATE - ON to set high, OFF to set low.
 * Returns		: 0 if success, -1 if failure.
 ******************************************************************************/
int RP_GPIO_write(int pin, RP_GPIO_STATE value) {
	char b[50];	
	int result;
	FILE *f;
	if (!pinValid(pin)) return -1;
	sprintf(b,"/sys/devices/virtual/gpio/gpio%d/value",pin);
	f = fopen(b,"w");
	if (NULL == f) return -1;
	if (value == ON) {
		result = fputs("1",f);
	} else {
		result = fputs("0",f);
	}
	if (0 > result) return -1;
	if (0 != fclose(f)) return -1;
	return 0;
}

/******************************************************************************
 * Function 	: RP_GPIO_read
 * Purpose		: Read the state of a GPIO pin that has been previously
 *				  configured as an input.
 * Parameters	: (1) Integer value of GPIO pin.
 *				  (2) Pointer to RP_GPIO_STATE - Will contain either ON or OFF
 *					  on function return.
 * Returns		: 0 if success, -1 if failre.
 ******************************************************************************/
int RP_GPIO_read(int pin, RP_GPIO_STATE* state) {
	char b[50];	
	char s[50];
	int result = 0;
	FILE *f;
	if (!pinValid(pin)) return -1;
	sprintf(b,"/sys/devices/virtual/gpio/gpio%d/value",pin);
	f = fopen(b,"r");
	if (NULL == f) return -1;
	if (fgets(s,5,f) == NULL) return -1;
#ifdef DEBUG_ON
printf("State = %s\n", s);
#endif	
	if (strcmp(s,"1\n") == 0) {
		*state = ON;
	} else if (strcmp(s,"0\n") == 0) {
		*state = OFF;
	} else {
		result = -1;
	}
	if (0 != fclose(f)) return -1;
	if (0 > result) return -1;	
	return 0;
}

