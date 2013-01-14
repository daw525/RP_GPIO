/*******************************************************************************
 * RP_GPIO.c
 * GPIO library for Raspberry Pi, using the /sys/ file system.
 * 
 * Version 1 - 2012/12/22
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "RP_GPIO.h"

//#define DEBUG_ON (1)

static int pinValid(int);

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
		sprintf(b,"/sys/devices/virtual/gpio/gpio%d",pin);
		if (-1 == stat(b,&sb)) return -1;
		while ((0 < countout) && (sb.st_uid != getuid())) {
			if (-1 == stat(b,&sb)) return -1;
			usleep(100);
			countout--;
		}
		if (countout == 0) return -1;
#ifdef DEBUG_ON
		printf("export time=%duS\n",((10000 - countout)*100));
#endif
	}
	return 0;
}

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

