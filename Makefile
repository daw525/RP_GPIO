CC=GCC

RP_Blink: main.o RP_GPIO.o
	$(CC) -o RP_Blink main.o RP_GPIO.o
