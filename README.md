# ESPLoraWeb

ESPLoraWeb is an example of LoRa working on the ESP8266 NodeMCU with SX1276 and RFM98W.

I share this example as I haven't been able to find one that work!

# What I expect to do?

* Connect with LoRa
* Send messages from a module and receive it from other modules

# Configuration

Please configure Settings.h to give access to your wireless network!
First copy Settings.h.sample to Settings.h (Settings.h is ignored by git.)

It's an example, the ip and wireless access is hardcoded.

You can then either connect on the WebSocket, port 81.
Or just use the web interface: http://{IpOfTheModule}/

# Needed libraries

pio lib list

* 124: RadioHead (1.74)
* 549: WebSockets (2.0.7)

The libraries should be installed automatically!

If it doesn't work, you can install them by typing in your terminal (the icon with a > on the left of PlatformIO).

platformio lib install {library id}

Example for the pixel library: platformio lib install 679


# What you need?

- PlatformIO
- An ESP8266
- Some LoRa modules

# Potentially supported modules

* RF22/24/26/27/69
* Si4
* 460/4461/4463/4464
* nRF24/nRF905
* SX1276/77/78
* RFM95/96/97/98

Tested:
* RFM98, SX1276

# Supported modules

You may be interested to buy:
Please note that other modules are supported. It's the one I've tested.

* SX1278 LoRa

https://www.aliexpress.com/item/433Mhz-Lora-SX1278-Long-Range-RF-Wireless-Module-SPI-DRF1278F-For-Arduino/32795077709.html

* RFM98W Wireless transceiver

https://www.aliexpress.com/item/RFM98W-RFM98W-Wireless-transceiver-modules-LoRa-spread-spectrum-communications-433M-SX1278-16-16mm/32707134262.html

* Holder for the LoRa Chips

https://www.aliexpress.com/item/10PCS-LOT-serial-WIFI-ESP8266-module-adapter-plate-ESP07-ESP12-ESP12E-excluding-module/32618522889.html

Remove the 3 resistances, connect the left pin of the left resistance to the bottom left pin (resistance to the bottom).

* Some Dupont cables

https://www.aliexpress.com/item/40pcs-10CM-Female-Female-Dupont-Line-for-Arduino-Breadboard-1Pin-Female-to-Female-Dupont-Cable-Jumper/32661309874.html

# How to deploy

All actions are done on the left panel.

To build: PlatformIO: Build

If you have any errors, verify that your libraries are well installed!

To deploy files (images, html, javascript),

click on : Run other target. PIO Upload SPIFFS image

Now click on PlatformIO: Upload

The leds will blink and you will see a new wireless access.
