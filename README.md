RP_GPIO
=======

Raspberry Pi GPIO control library in C

The following line must be added to /etc/rc.local

    chown -R pi:pi /sys/class/gpio
    
This will allow the access of GPIO pins without the need to be running as root.
