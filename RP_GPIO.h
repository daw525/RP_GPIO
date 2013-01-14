/*******************************************************************************
 * RP_GPIO.h
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
#ifndef _RP_GPIO_H_
#define _RP_GPIO_H_

/* GPIO pin state */
typedef enum {
	OFF	= 0,
	ON	= 1
} RP_GPIO_STATE;

/* GPIO pin direction */
typedef enum {
	OUT	= 0,
	IN	= 1
} RP_GPIO_DIR;

/* Global function prototypes */
int RP_GPIO_export(int, RP_GPIO_STATE);
int RP_GPIO_direction(int, RP_GPIO_DIR);
int RP_GPIO_write(int, RP_GPIO_STATE);
int RP_GPIO_read(int, RP_GPIO_STATE*);

#endif /* _RP_GPIO_H_ */

