/*******************************************************************************
 * RP_GPIO.h
 * GPIO library for Raspberry Pi, using the /sys/ file system.
 * 
 * To use this library, the directory /sys/class/gpio/ must have ownership
 * changed from root to the default user. For example:
 *
 * sudo chown pi /sys/class/gpio 
 *
 * Version 1 - 2012/12/22
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

int RP_GPIO_export(int, RP_GPIO_STATE);
int RP_GPIO_direction(int, RP_GPIO_DIR);
int RP_GPIO_write(int, RP_GPIO_STATE);
int RP_GPIO_read(int, RP_GPIO_STATE*);

#endif /* _RP_GPIO_H_ */

