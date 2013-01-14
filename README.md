RP_GPIO
=======

Raspberry Pi GPIO control library in C

In order to use GPIO without the need to be running as root:

1. The following line must be added to /etc/rc.local

        chown -R pi:pi /sys/class/gpio
    
2. A text file called 99-gpio.rules must be created in /etc/udev/rules.d with the following contents

        SUBSYSTEM=="gpio", ACTION=="add", PROGRAM="/bin/sh -c 'chown -R pi:pi /sys%p'"

Usage:
1. Export the pin to allow use;
2. Configure pin direction;
3. Use pin;
4. Unexport after use.
