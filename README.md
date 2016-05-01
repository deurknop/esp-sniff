Esp-sniff
=========

Simple ESP8266-based MAC address collector.

Installation
------------

### Hardware

You will need an ESP8266 hooked onto your computer.
I'm using a Witty cloud development board, but a NodeMCU should also do the trick.


### Software

- Install the Arduino IDE, and add the ESP8266 libs (https://github.com/esp8266/Arduino).
- Add the standard library (-lstdc++) to the compiler.c.elf.libs line in your ESP8266 platform.txt file (probably located in ~/Arduino/hardware/esp8266com/esp8266/platform.txt):
```compiler.c.elf.libs=-lm -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lcrypto -lmain -lwps -laxtls -lsmartconfig -lmesh -lwpa2 -lstdc++
```
- Open esp-sniff.ino
- Set Tools > Board to "Generic ESP8266 Module"
- Set Tools > Upload Speed to 115200
- Set Tools > Reset Method to "nodemcu" (Depending on your dev board)
- Select the USB port your dev board is using in Tools > Port
- Activate the serial monitor: Tools > Serial Monitor
- Click the "Upload" button and watch the collected MACs
