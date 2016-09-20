# RaspberryPi_GPIOBlinds
Rasberry pi gpio control.
Via IR remote (using wiring pi and lirc). 
Via Android socket client to connect to Raspberry pi gpio Android server. 
The raspberry has an IR diode on PIN GPIO02. Receiving RF signals fron a RF 
remote (Euskaltel brand, but you can use your own remote with proper lirc.conf file) 
you can control two motorized blinds conected via GPIO-SSRs.

You need to install and setup lirc and wiringpi (google about that if you need). 
In /boot/config.txt add this line at the end for the lirc IR imput: dtoverlay=lirc-rpi:gpio_in_pin=2. 
